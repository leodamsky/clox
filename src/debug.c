#include <stdio.h>
#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static void printConstant(const char *name, int constant, Value value) {
    printf("%-16s %4d '", name, constant);
    printValue(value);
    printf("'\n");
}

static int longConstantInstruction(const char *name, Chunk *chunk, int offset) {
    int constant = chunk->code[offset + 1];
    constant += ((int) chunk->code[offset + 2]) << 8;
    constant += ((int) chunk->code[offset + 3]) << 16;
    printConstant(name, constant, chunk->constants.values[constant]);
    return offset + 4;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
    int constant = chunk->code[offset + 1];
    printConstant(name, constant, chunk->constants.values[constant]);
    return offset + 2;
}

static int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT_LONG:
            return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
