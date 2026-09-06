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

/* Load persisted slot values for a counter identified by uuid.
   Returns true if an entry existed, false if defaults were used. */
bool counter_persist_load(const char *uuid, long long *slot_a, long long *slot_b);

/* Atomically write the slot values for a counter identified by uuid. */
void counter_persist_save(const char *uuid, long long slot_a, long long slot_b);

#ifdef __cplusplus
}
#endif
