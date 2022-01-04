#include <stdio.h>

#include "lines.h"
#include "memory.h"

void initLines(Lines *lines) {
    lines->count = 0;
    lines->capacity = 0;
    lines->values = NULL;
}

void writeLine(Lines *lines, int line, int instructionId) {
    // last line matches the input
    // so we should mutate its last instruction
    // we don't need new allocations
    if (lines->count > 0) {
        int prevLine = lines->values[lines->count - 2];
        if (prevLine == line) {
            lines->values[lines->count - 1] = instructionId;
            return;
        }
    }

    if (lines->capacity < lines->count + 1) {
        int oldCapacity = lines->capacity;
        lines->capacity = GROW_CAPACITY(oldCapacity);
        lines->values = GROW_ARRAY(int, lines->values, oldCapacity, lines->capacity);
    }

    // we don't need to check capacity twice because we always do even number of writes
    // capacity is always even too
    lines->values[lines->count] = line;
    lines->count++;
    lines->values[lines->count] = instructionId;
    lines->count++;
}

void freeLines(Lines *lines) {
    FREE_ARRAY(int, lines->values, lines->capacity);
    initLines(lines);
}

int getLine(Lines *lines, int instructionId) {
    for (int i = 0; i < lines->count; i = i + 2) {
        int line = lines->values[i];
        int lastInstruction = lines->values[i + 1];
        if (lastInstruction >= instructionId) {
            return line;
        }
    }
    return -1;
}
