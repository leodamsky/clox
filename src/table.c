#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table *table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table *table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

static Entry *findEntry(Entry *entries, int capacity, Value key) {
    uint32_t index = hashValue(key) % capacity;
    Entry *tombstone = NULL;

    for (;;) {
        Entry *entry = &entries[index];
        if (entry->type == ENTRY_TOMBSTONE) {
            if (tombstone == NULL) tombstone = entry;
        } else if (entry->type == ENTRY_ABSENT) {
            return tombstone != NULL ? tombstone : entry;
        } else if (valuesEqual(entry->key, key)) {
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

static void adjustCapacity(Table *table, int capacity) {
    Entry *entries = ALLOCATE(Entry, capacity);
    for (int i = 0; i < capacity; ++i) {
        entries[i].type = ENTRY_ABSENT;
        entries[i].key = NIL_VAL;
        entries[i].value = NIL_VAL;
    }

    table->count = 0;
    for (int i = 0; i < table->capacity; ++i) {
        Entry *entry = &table->entries[i];
        if (entry->type != ENTRY_VALUE) continue;

        Entry *dest = findEntry(entries, capacity, entry->key);
        dest->type = entry->type;
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool tableGet(Table *table, Value key, Value *value) {
    if (table->count == 0) return false;

    Entry *entry = findEntry(table->entries, table->capacity, key);
    if (entry->type != ENTRY_VALUE) return false;

    *value = entry->value;
    return true;
}

bool tableSet(Table *table, Value key, Value value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    Entry *entry = findEntry(table->entries, table->capacity, key);
    bool isNewKey = entry->type != ENTRY_VALUE;
    if (isNewKey && entry->type == ENTRY_ABSENT) table->count++;

    entry->type = ENTRY_VALUE;
    entry->key = key;
    entry->value = value;
    return isNewKey;
}

bool tableDelete(Table *table, Value key) {
    if (table->count == 0) return false;

    Entry *entry = findEntry(table->entries, table->capacity, key);
    if (entry->type != ENTRY_VALUE) return false;

    entry->type = ENTRY_TOMBSTONE;
    entry->key = NIL_VAL;
    entry->value = NIL_VAL;
    return true;
}

void tableAddAll(Table *from, Table *to) {
    for (int i = 0; i < from->capacity; ++i) {
        Entry *entry = &from->entries[i];
        if (entry->type == ENTRY_VALUE) {
            tableSet(to, entry->key, entry->value);
        }
    }
}

ObjString *tableFindString(Table *table, const char *chars, int length, uint32_t hash) {
    if (table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;
    for (;;) {
        Entry *entry = &table->entries[index];
        // Stop if we found empty non-tombstone entry.
        if (entry->type == ENTRY_ABSENT) {
            return NULL;
        }

        if (!IS_STRING(entry->key)) continue;

        ObjString *keyStr = AS_STRING(entry->key);
        if (keyStr->length == length &&
            keyStr->hash == hash &&
            memcmp(keyStr->chars, chars, length) == 0) {
            return keyStr;
        }

        index = (index + 1) % table->capacity;
    }
}
