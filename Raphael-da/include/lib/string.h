/*
* Copyright (c) 2008 Travis Geiselbrecht
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files
* (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __LIB_STRING_H
#define __LIB_STRING_H

#include <sys/types.h>
#include <compiler.h>

void *memset(void *s, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
int memcmp(const void *cs, const void *ct, size_t count);

int strnicmp(char const *, char const *, size_t) __PURE;
int strncmp(char const *, char const *, size_t) ;
char *strncat(char *, char const *, size_t);
char *strncpy(char *, char const *, size_t);
char *strstr(char const *s1, char const *s2);
size_t strlen(char const *s);

#endif
