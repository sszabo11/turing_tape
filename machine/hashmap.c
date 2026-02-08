#include "hashmap.h"
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char *key) {
  uint64_t hash = FNV_OFFSET;
  for (const char *p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}

HashTable *create_hashmap(int capacity) {
  HashTable *table = malloc(sizeof(HashTable));
  if (table == NULL) {
    return NULL;
  };

  table->capacity = capacity;
  table->len = 0;

  table->entries = calloc(capacity, sizeof(Entry));

  if (table->entries == NULL) {
    free(table);
    return NULL;
  }

  return table;
}

const char *insert_value(HashTable *table, const char *key, void *value) {
  assert(value != NULL);

  if (value == NULL) {
    return NULL;
  }

  uint64_t hash = hash_key(key);

  int index = hash % (table->capacity - 1);

  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {

      // DUPLIcates CAUSED THe ERROR< FREE PREV MEM
      free(table->entries[index].value);
      table->entries[index].value = value;
      return table->entries[index].key;
    }
    index++;

    if (index >= table->capacity) {
      index = 0;
    }
  };

  char *key_clone = strdup(key);
  // char *value_clone = strdup((char *)value);
  if (key_clone == NULL || value == NULL) {
    // free(key_clone);
    // free(value);
    return NULL;
  }

  table->len++;

  table->entries[index].key = (void *)key_clone;
  table->entries[index].value = value;

  return key;
}

void *get_key_value(HashTable *table, const char *key) {
  uint64_t hash = hash_key(key);

  int index = hash % (table->capacity - 1);

  while (table->entries[index].key != NULL) {
    // printf("E %s\n", table->entries[index].key);
    if (strcmp(key, table->entries[index].key) == 0) {
      // printf("asd: %s\n", (char *)table->entries[index].value);
      return table->entries[index].value;
    }

    index++;

    if (index >= table->capacity) {
      index = 0;
    }
  }

  return NULL;
}

void free_table(HashTable *table) {
  if (table == NULL)
    return;

  for (int i = 0; i < table->capacity; i++) {
    if (table->entries[i].key != NULL) {
      free((void *)table->entries[i].key);
      // free(table->entries[i].value);
    }
    if (table->entries[i].value != NULL) {
      free(table->entries[i].value);
    }
  }
  free(table->entries);
  free(table);
}
