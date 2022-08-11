#pragma once

#include <stdio.h>

#ifdef LOGGING_ON
#define LOG(M, ...) {fprintf(stderr, "%s (%s:%d) " M "\n", __func__, __FILE__, __LINE__, __VA_ARGS__);}
#define NOTE(...) {fprintf(stdout, __VA_ARGS__);}
#else
#define LOG(M, ...) do {} while(0)
#define NOTE(M, ...) do {} while(0)
#endif
