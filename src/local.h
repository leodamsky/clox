#ifndef clox_local_h
#define clox_local_h

#include "common.h"
#include "scanner.h"

typedef struct {
    Token name;
    int depth;
} Local;

typedef struct {
    int capacity;
    int count;
    Local *values;
} LocalArray;

void initLocalArray(LocalArray *array);

void writeLocalArray(LocalArray *array, Local value);

void freeLocalArray(LocalArray *array);

#endif
