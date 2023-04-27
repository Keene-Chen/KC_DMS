/**
 * Copyright (c) 2016 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `ini.c` for details.
 */

#ifndef __RXI_INI_H__
#define __RXI_INI_H__

#define INI_VERSION "0.1.1"

typedef struct ini_t ini_t;

ini_t* ini_load(const char* filename);
void ini_free(ini_t* ini);
const char* ini_get(ini_t* ini, const char* section, const char* key);
int ini_sget(ini_t* ini, const char* section, const char* key, const char* scanfmt, void* dst);

#endif // __RXI_INI_H__
