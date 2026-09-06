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

#include <obs-module.h>
#include <util/platform.h>
#include <mutex>
#include "counter-persist.hpp"

namespace {

std::mutex g_mutex;

char *persist_path()
{
	char *dir = obs_module_config_path(nullptr);
	if (dir) {
		os_mkdirs(dir);
		bfree(dir);
	}
	return obs_module_config_path("counters.json");
}

obs_data_t *load_doc()
{
	char *path = persist_path();
	obs_data_t *doc = obs_data_create_from_json_file(path);
	bfree(path);
	return doc ? doc : obs_data_create();
}

void save_doc(obs_data_t *doc)
{
	char *path = persist_path();
	obs_data_save_json_safe(doc, path, "tmp", "bak");
	bfree(path);
}

} // namespace

bool counter_persist_load(const char *uuid, long long *slot_a, long long *slot_b)
{
	std::lock_guard<std::mutex> lock(g_mutex);

	obs_data_t *doc = load_doc();
	obs_data_t *counters = obs_data_get_obj(doc, "counters");
	obs_data_t *entry = counters ? obs_data_get_obj(counters, uuid) : nullptr;

	const bool found = entry != nullptr;
	if (found) {
		*slot_a = obs_data_get_int(entry, "A");
		*slot_b = obs_data_get_int(entry, "B");
	}

	obs_data_release(entry);
	obs_data_release(counters);
	obs_data_release(doc);
	return found;
}

void counter_persist_save(const char *uuid, long long slot_a, long long slot_b)
{
	std::lock_guard<std::mutex> lock(g_mutex);

	obs_data_t *doc = load_doc();

	obs_data_t *counters = obs_data_get_obj(doc, "counters");
	if (!counters) {
		counters = obs_data_create();
		obs_data_set_obj(doc, "counters", counters);
	}

	obs_data_t *entry = obs_data_create();
	obs_data_set_int(entry, "A", slot_a);
	obs_data_set_int(entry, "B", slot_b);
	obs_data_set_obj(counters, uuid, entry);

	save_doc(doc);

	obs_data_release(entry);
	obs_data_release(counters);
	obs_data_release(doc);
}
