/*
Counter for OBS
Copyright (C) 2026 plugin-counter contributors

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <obs.h>

void counter_register_source();
void counter_auto_reset_all();

#ifdef __cplusplus
}
#endif
