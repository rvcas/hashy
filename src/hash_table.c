#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static int ht_hash(const char* s, const int a, const int m) {
  long hash = 0;

  const int len_s = strlen(s);

  for (int i = 0; i < len_s; i++) {
    hash += (long)pow(a, len_s - (i+1)) * s[i];
    hash = hash % m;
  }

  return (int)hash;
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);

  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

static ht_item* ht_new_item(const char* k, const char* v) {
  ht_item* i = malloc(sizeof(ht_item));

  i->key = strdup(k);
  i->value = strdup(v);

  return i;
}

static void ht_del_item(ht_item* i) {
  free(i->key);
  free(i->value);
  free(i);
}

ht_hash_table* ht_new() {
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  ht->size = 53;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

  return ht;
}

void ht_del_hash_table(ht_hash_table* ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];

    if (item != NULL) {
      ht_del_item(item);
    }

    free(ht->items);
    free(ht);
  }
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
  int index;
  ht_item* cur_item;

  ht_item* item = ht_new_item(key, value);

  for (int i = 0; cur_item != NULL && cur_item != &HT_DELETED_ITEM; i++ ) {
    index = ht_get_hash(item->key, ht->size, i);
    cur_item = ht->items[index];

    if (cur_item != &HT_DELETED_ITEM && strcmp(cur_item->key, key) == 0) {
      ht_del_item(cur_item);
      ht->items[index] = item;

      return;
    }
  }

  ht->items[index] = item;
  ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
  int index;
  ht_item* item;

  for (int i = 0; item != NULL; i++) {
    index = ht_get_hash(key, ht->size, 0);
    item = ht->items[index];

    if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
      return item->value;
    }
  }

  return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key) {
  int index;
  ht_item* item;

  for (int i = 0; item != NULL; i++) {
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];

    if (item != &HT_DELETED_ITEM) {
      ht_del_item(item);
      ht->items[index] = &HT_DELETED_ITEM;
    }
  }

  ht->count--;
}






