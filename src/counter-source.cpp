/*
Counter for OBS
Copyright (C) 2026 plugin-counter contributors

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "counter-source.hpp"
#include "counter-persist.hpp"
#include "plugin-support.h"

#include <obs-module.h>
#include <climits>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <vector>

#define SOURCE_ID "counter_source"
#define TEXT_SOURCE_ID "text_ft2_source"

struct counter_source {
	obs_source_t *self;
	obs_source_t *text; /* internal text source we composite and update */
	long long slot_a = 0;
	long long slot_b = 0;
	char *tmpl = nullptr; /* format string with {A} / {B} placeholders */
	bool auto_reset = false;
	char uuid[64] = {0};
	obs_hotkey_id hotkey_inc_a = OBS_INVALID_HOTKEY_ID;
	obs_hotkey_id hotkey_dec_a = OBS_INVALID_HOTKEY_ID;
	obs_hotkey_id hotkey_inc_b = OBS_INVALID_HOTKEY_ID;
	obs_hotkey_id hotkey_dec_b = OBS_INVALID_HOTKEY_ID;
	obs_hotkey_id hotkey_reset = OBS_INVALID_HOTKEY_ID;
};

namespace {

/* Live instances, used by auto-reset and to keep persist writes scoped. */
std::mutex g_mutex;
std::vector<counter_source *> g_counters;

void counter_source_update(void *data, obs_data_t *settings);
void counter_reset(counter_source *s);

void counter_refresh_text(counter_source *s)
{
	if (!s->text || !s->tmpl)
		return;

	char buf[512];
	size_t out = 0;
	buf[0] = '\0';

	for (const char *p = s->tmpl; *p && out < sizeof(buf) - 32; ++p) {
		if (p[0] == '{' && (p[1] == 'A' || p[1] == 'B') && p[2] == '}') {
			long long value = p[1] == 'A' ? s->slot_a : s->slot_b;
			int written = snprintf(buf + out, sizeof(buf) - out, "%lld", value);
			if (written > 0)
				out += (size_t)written;
			p += 2;
		} else {
			buf[out++] = *p;
		}
	}
	buf[out] = '\0';

	obs_data_t *ts = obs_data_create();
	obs_data_set_string(ts, "text", buf);
	obs_source_update(s->text, ts);
	obs_data_release(ts);
}

void counter_persist(counter_source *s)
{
	counter_persist_save(s->uuid, s->slot_a, s->slot_b);
}

void hotkey_inc_a(void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed)
{
	if (!pressed)
		return;
	auto *s = (counter_source *)data;
	if (s->slot_a < LLONG_MAX)
		s->slot_a++;
	counter_persist(s);
	counter_refresh_text(s);
}

void hotkey_dec_a(void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed)
{
	if (!pressed)
		return;
	auto *s = (counter_source *)data;
	if (s->slot_a > 0)
		s->slot_a--;
	counter_persist(s);
	counter_refresh_text(s);
}

void hotkey_inc_b(void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed)
{
	if (!pressed)
		return;
	auto *s = (counter_source *)data;
	if (s->slot_b < LLONG_MAX)
		s->slot_b++;
	counter_persist(s);
	counter_refresh_text(s);
}

void hotkey_dec_b(void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed)
{
	if (!pressed)
		return;
	auto *s = (counter_source *)data;
	if (s->slot_b > 0)
		s->slot_b--;
	counter_persist(s);
	counter_refresh_text(s);
}

void hotkey_reset(void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed)
{
	if (!pressed)
		return;
	auto *s = (counter_source *)data;
	s->slot_a = 0;
	s->slot_b = 0;
	counter_persist(s);
	counter_refresh_text(s);
}

void counter_reset(counter_source *s)
{
	s->slot_a = 0;
	s->slot_b = 0;
	counter_persist(s);
	counter_refresh_text(s);
}

/* ---------------- obs_source_info callbacks ---------------- */

const char *counter_get_name(void *)
{
	return obs_module_text("CounterName");
}

void *counter_create(obs_data_t *settings, obs_source_t *source)
{
	auto *s = (counter_source *)bzalloc(sizeof(counter_source));
	s->self = source;

	const char *uuid = obs_source_get_uuid(source);
	if (uuid)
		strncpy(s->uuid, uuid, sizeof(s->uuid) - 1);
	else
		strncpy(s->uuid, obs_source_get_name(source), sizeof(s->uuid) - 1);

	s->text = obs_source_create(TEXT_SOURCE_ID, "counter-internal-text", nullptr, nullptr);
	obs_source_add_active_child(source, s->text);

	if (!counter_persist_load(s->uuid, &s->slot_a, &s->slot_b))
		counter_persist(s);

	s->hotkey_inc_a = obs_hotkey_register_source(source, "Counter.IncrementA", obs_module_text("Hotkey.IncrementA"),
						     hotkey_inc_a, s);
	s->hotkey_dec_a = obs_hotkey_register_source(source, "Counter.DecrementA", obs_module_text("Hotkey.DecrementA"),
						     hotkey_dec_a, s);
	s->hotkey_inc_b = obs_hotkey_register_source(source, "Counter.IncrementB", obs_module_text("Hotkey.IncrementB"),
						     hotkey_inc_b, s);
	s->hotkey_dec_b = obs_hotkey_register_source(source, "Counter.DecrementB", obs_module_text("Hotkey.DecrementB"),
						     hotkey_dec_b, s);
	s->hotkey_reset =
		obs_hotkey_register_source(source, "Counter.Reset", obs_module_text("Hotkey.Reset"), hotkey_reset, s);

	{
		std::lock_guard<std::mutex> lock(g_mutex);
		g_counters.push_back(s);
	}

	counter_source_update(s, settings);

	return s;
}

void counter_destroy(void *data)
{
	auto *s = (counter_source *)data;

	{
		std::lock_guard<std::mutex> lock(g_mutex);
		g_counters.erase(std::remove(g_counters.begin(), g_counters.end(), s), g_counters.end());
	}

	obs_hotkey_unregister(s->hotkey_inc_a);
	obs_hotkey_unregister(s->hotkey_dec_a);
	obs_hotkey_unregister(s->hotkey_inc_b);
	obs_hotkey_unregister(s->hotkey_dec_b);
	obs_hotkey_unregister(s->hotkey_reset);

	if (s->text) {
		obs_source_remove_active_child(s->self, s->text);
		obs_source_release(s->text);
	}
	bfree(s->tmpl);
	bfree(s);
}

void counter_source_update(void *data, obs_data_t *settings)
{
	auto *s = (counter_source *)data;

	bfree(s->tmpl);
	s->tmpl = bstrdup(obs_data_get_string(settings, "template"));
	s->auto_reset = obs_data_get_bool(settings, "auto_reset");

	obs_data_t *ts = obs_data_create();
	obs_data_t *font = obs_data_get_obj(settings, "font");
	if (font) {
		obs_data_set_obj(ts, "font", font);
		obs_data_release(font);
	}
	obs_data_set_int(ts, "color", obs_data_get_int(settings, "color"));
	obs_source_update(s->text, ts);
	obs_data_release(ts);

	counter_refresh_text(s);
}

void counter_video_render(void *data, gs_effect_t *)
{
	auto *s = (counter_source *)data;
	obs_source_video_render(s->text);
}

uint32_t counter_get_width(void *data)
{
	auto *s = (counter_source *)data;
	return (uint32_t)obs_source_get_width(s->text);
}

uint32_t counter_get_height(void *data)
{
	auto *s = (counter_source *)data;
	return (uint32_t)obs_source_get_height(s->text);
}

/* Required for OBS_SOURCE_COMPOSITE sources; the text child produces no audio. */
bool counter_audio_render(void *, uint64_t *, obs_source_audio_mix *, uint32_t, size_t, size_t)
{
	return false;
}

bool reset_button_clicked(obs_properties_t *, obs_property_t *, void *data)
{
	counter_reset((counter_source *)data);
	return false;
}

obs_properties_t *counter_get_properties(void *data)
{
	obs_properties_t *props = obs_properties_create();

	obs_property_t *tmpl =
		obs_properties_add_text(props, "template", obs_module_text("Template"), OBS_TEXT_DEFAULT);
	obs_property_set_long_description(tmpl, obs_module_text("Template.Desc"));

	obs_properties_add_font(props, "font", obs_module_text("Font"));
	obs_properties_add_color(props, "color", obs_module_text("Color"));
	obs_properties_add_bool(props, "auto_reset", obs_module_text("AutoReset"));
	obs_properties_add_button2(props, "reset_now", obs_module_text("ResetNow"), reset_button_clicked, data);

	return props;
}

void counter_get_defaults(obs_data_t *settings)
{
	obs_data_t *font = obs_data_create();
	obs_data_set_string(font, "face", "Arial");
	obs_data_set_string(font, "style", "Regular");
	obs_data_set_int(font, "size", 96);
	obs_data_set_default_obj(settings, "font", font);
	obs_data_release(font);

	obs_data_set_default_int(settings, "color", 0xFFFFFFFF);
	obs_data_set_default_string(settings, "template", "Death Count: {A}");
	obs_data_set_default_bool(settings, "auto_reset", false);
}

} // namespace

void counter_register_source()
{
	obs_source_info info = {};
	info.id = SOURCE_ID;
	info.type = OBS_SOURCE_TYPE_INPUT;
	info.output_flags = OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_COMPOSITE | OBS_SOURCE_VIDEO;
	info.get_name = counter_get_name;
	info.create = counter_create;
	info.destroy = counter_destroy;
	info.update = counter_source_update;
	info.video_render = counter_video_render;
	info.get_width = counter_get_width;
	info.audio_render = counter_audio_render;
	info.get_height = counter_get_height;
	info.get_properties = counter_get_properties;
	info.get_defaults = counter_get_defaults;
	info.icon_type = OBS_ICON_TYPE_TEXT;
	obs_register_source(&info);
}

void counter_auto_reset_all()
{
	std::lock_guard<std::mutex> lock(g_mutex);
	for (counter_source *s : g_counters) {
		if (s->auto_reset)
			counter_reset(s);
	}
}
