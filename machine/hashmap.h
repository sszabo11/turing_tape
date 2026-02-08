
// Hash table structure
// typedef struct ht ht;
//
// typedef struct {
//  const char *key;
//  void *value;
//
//  ht *_table;
//  size_t _index;
//} HashIterator;
//
//// Create hash table (allocate memory) and return pointer
// ht *ht_create(void);
//
//// Free memory for hash table
// void ht_destroy(ht *table);
//
//// Get item with key, return value
// void *ht_get(ht *table, const char *key);
//
//// Set item with given key and value, return address of copied key
// const char *ht_set(ht *table, const char *key, void *value);
//
//// Return number of items in table
// size_t ht_length(ht *table);
//
//// Return new hash table iterator
// HashIterator ht_irerator(ht *table);
//
//// Move itertor to next item in hash table
// bool ht_next(HashIterator *it);

#include <stddef.h>
#include <stdlib.h>
typedef struct {
  const char *key;
  void *value;
} Entry;

typedef struct {
  int len;
  Entry *entries;
  size_t capacity;
} HashTable;

HashTable *create_hashmap(int capacity);

const char *insert_value(HashTable *table, const char *key, void *value);
void *get_key_value(HashTable *table, const char *key);

void free_table(HashTable *table);
