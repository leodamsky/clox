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
    // sizeof(ObjString) behaves like "chars" (flexible array member) is empty
    // so, we need to compute the whole size
    int totalStructSize = sizeof(ObjString) * length * sizeof(char);
    ObjString *string = (ObjString *) allocateObject(totalStructSize, OBJ_STRING);
    string->length = length;
    memcpy(string->chars, chars, length);
    FREE_ARRAY(char, chars, length);
    return string;
}

ObjString *takeString(char *chars, int length) {
    return allocateString(chars, length);
}

ObjString *copyString(const char *chars, int length) {
    char *heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
    }
}
