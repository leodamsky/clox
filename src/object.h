#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_CLOSURE(value) ((ObjClosure *) AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction *) AS_OBJ(value))
#define AS_NATIVE(value) (((ObjNative *) AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString *) AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*) AS_OBJ(value))->chars)

typedef enum {
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_UPVALUE,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj *next;
};

typedef struct {
    Obj obj;
    int arity;
    int upvalueCount;
    Chunk chunk;
    ObjString *name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value *args);

typedef struct {
    Obj obj;
    NativeFn function;
} ObjNative;

struct ObjString {
    struct Obj obj;
    int length;
    char *chars;
    uint32_t hash;
};

typedef struct ObjUpvalue {
    Obj obj;
    Value *location;
    Value closed;
    struct ObjUpvalue *next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    // GC:
    // Storing the upvalue count in the closure is redundant because the ObjFunction
    // that the ObjClosure references also keeps that count.
    // As usual, this weird code is to appease the GC.
    // The collector may need to know an ObjClosure’s upvalue array size
    // after the closure’s corresponding ObjFunction has already been freed.
    int upvalueCount;
} ObjClosure;

ObjClosure *newClosure(ObjFunction *function);

ObjFunction *newFunction();

ObjNative *newNative(NativeFn function);

struct ObjString *takeString(char *chars, int length);

struct ObjString *copyString(const char *chars, int length);

ObjUpvalue *newUpvalue(Value *slot);

void printObject(Value value);

void freeObject(Obj *object);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
