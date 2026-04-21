#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdlib.h>

typedef long unsigned size_t;

#define NULL (void *)0

char *strncat(char *dest, const char *src, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);

size_t strlen(const char *str);
void *memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memchr(const void *str, int c, size_t n);

int memcmp(const void *str1, const void *str2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
int strncmp(const char *str1, const char *str2, size_t n);

char *strtok(char *str, const char *delim);
int is_delim(char c, const char *delim);
char *strerror(int errnum);

size_t strcspn(const char *str1, const char *str2);
char *strpbrk(const char *str1, const char *str2);
char *strstr(const char *haystack, const char *needle);

void *to_upper(const char *str);
void *to_lower(const char *str);
void *insert(const char *src, const char *str, size_t start_index);
void *trim(const char *src, const char *trim_chars);

int sprintf(char *str, const char *format, ...);

#endif