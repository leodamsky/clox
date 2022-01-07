#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type *) allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj *) reallocate(NULL, 0, size);
    object->type = type;

    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static ObjString *allocateString(char *chars, int length) {
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString *takeString(char *chars, int length) {
    ObjString *string = allocateString(chars, length);
    string->constant = false;
    return string;
}

ObjString *constantString(const char *chars, int length) {
    // we don't take ownership here
    // we set a flag that it's a constant
    // based on which, we will decide whether to free or not
    ObjString *string = allocateString(chars, length);
    string->constant = true;
    return string;
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: {
            ObjString *string = AS_STRING(value);
            printf("%.*s", string->length, string->chars);
        }
    }
}
