#include "local.h"
#include "memory.h"

void initLocalArray(LocalArray *array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeLocalArray(LocalArray *array, Local value) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Local, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void freeLocalArray(LocalArray *array) {
    FREE_ARRAY(Local, array->values, array->capacity);
    initLocalArray(array);
}
