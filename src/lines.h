#ifndef clox_lines_h
#define clox_lines_h

#include "common.h"

/**
 * Lines is a dynamic array, were indexes represent line numbers and values - the last instruction on that line.
 */
typedef struct {
    int capacity;
    int count;
    int *values;
} Lines;

void initLines(Lines *lines);

void writeLine(Lines *lines, int line, int instructionId);

void freeLines(Lines *lines);

int getLine(Lines *lines, int instructionId);

#endif
