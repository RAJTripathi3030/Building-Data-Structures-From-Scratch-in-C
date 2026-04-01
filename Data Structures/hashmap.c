// THIS FILE CONTAINS THE SCRATCH IMPLEMENTATION OF A HASHMAP IN C LANGUAGE
// This implementation uses Open Addressing with Linear Probing and Tombstones.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// We define a Node that will be stored in the hash table buckets.
// Since we are building a String-to-Int hash map, our key is char* and value is int.
struct hashNode {
  char* key;           // The string key
  int value;           // The integer value
  bool isTombstone;    // A flag to keep track of deleted items without breaking probing chains
};

// The main Hash Map container struct, matching your style from vector.c
struct tempMap {
  int size;                   // CURRENT NUMBER OF ACTIVE ELEMENTS
  int capacity;               // THE SPACE ALLOCATED (TOTAL BUCKETS) IN MEMORY
  struct hashNode* base_addr; // POINTS TO THE ARRAY OF BUCKETS
};

// The famous djb2 hash function for strings, as mentioned in the article.
// It generates a pseudo-random integer based on the characters in the string.
// We use `% capacity` later to ensure the final hash fits within our array bounds.
int hash_function(const char* key, int capacity) {
  unsigned long hash = 5381;
  int c;

  // Loop through each character until we hit the null-terminator '\0'
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash % capacity;
}

// Initializes a new hash map with a starting capacity (e.g., 8)
void hashmap_init(struct tempMap *m, int initial_capacity) {
  m->size = 0;
  m->capacity = initial_capacity;

  // Allocate memory for 'initial_capacity' number of hashNodes
  // calloc guarantees that all bits are initialized to 0 (so key will naturally be NULL)
  m->base_addr = calloc(m->capacity, sizeof(struct hashNode));
  if(m->base_addr == NULL) {
    printf("Memory allocation failed for hash map...\n");
    return;
  }
}

// Forward declaration for resizing function (so we can use it in put)
void hashmap_resize(struct tempMap *m);

// Inserts or updates a key-value pair in the hash map.
void hashmap_put(struct tempMap *m, const char* key, int value) {
  // 1. Check load factor. If >= 75% full, we double the capacity to avoid long chains and clustering.
  if ((float)m->size / m->capacity >= 0.75) {
    hashmap_resize(m);
  }

  // 2. Hash the key to find our starting bucket (index)
  int index = hash_function(key, m->capacity);
  int tombstone_idx = -1; // To remember the first tombstone we see

  // 3. Linear Probing Loop: Find an completely empty slot (NULL) OR the exact existing key
  while (m->base_addr[index].key != NULL) {
    // If we find our same key, this is an update!
    if (strcmp(m->base_addr[index].key, key) == 0) {
      if (m->base_addr[index].isTombstone) {
        // It was deleted, bring it back alive!
        m->base_addr[index].isTombstone = false;
        m->base_addr[index].value = value;
        m->size += 1; // It counts as an active item again
      } else {
        // Just standard update of the value
        m->base_addr[index].value = value;
      }
      return;
    }

    // If this slot is a tombstone, and we haven't remembered one yet, save its index 
    // so we can insert the new item here later!
    if (m->base_addr[index].isTombstone && tombstone_idx == -1) {
      tombstone_idx = index;
    }

    // Move to the next bucket (Linear Probing), wrap around to 0 if we hit the end.
    index = (index + 1) % m->capacity;
  }

  // 4. We hit an empty bucket (NULL), meaning the key does NOT exist yet.
  
  // If we found a tombstone along the way, we should reuse that spot!
  if (tombstone_idx != -1) {
    // Safe to reuse since we verified the key didn't already exist further down the collision chain.
    index = tombstone_idx;
    
    // The previous string was 'deleted' but still occupied the memory. We must free it over-writing.
    free(m->base_addr[index].key);
  }
  
  // Insert new key-value into the chosen index.
  // We use strdup() which automatically runs malloc and copies the char array.
  m->base_addr[index].key = strdup(key); 
  m->base_addr[index].value = value;
  m->base_addr[index].isTombstone = false;
  m->size += 1;
}

// Resizes the hashmap logic
void hashmap_resize(struct tempMap *m) {
  int old_capacity = m->capacity;
  struct hashNode* old_addr = m->base_addr;

  // Double capacity
  m->capacity = old_capacity * 2;
  
  // Create a whole new array of completely empty slots
  m->base_addr = calloc(m->capacity, sizeof(struct hashNode));
  if(m->base_addr == NULL) {
    printf("Memory allocation failed during resize...\n");
    return;
  }
  
  m->size = 0; // Reset size, we will recount as we move old valid nodes over

  // Re-hash and move all active elements perfectly into the newly allocated buckets.
  for (int i = 0; i < old_capacity; i++) {
    // Only copy over the bucket if it had a key AND wasn't deleted
    if (old_addr[i].key != NULL) {
      if (!old_addr[i].isTombstone) {
        // For performance, we find where it maps in the NEW larger capacity array
        int index = hash_function(old_addr[i].key, m->capacity);
        
        while (m->base_addr[index].key != NULL) {
            index = (index + 1) % m->capacity; // Probe collision
        }
        
        // Move the pointers directly. (No strdup mapping here to save memory allocation)
        m->base_addr[index].key = old_addr[i].key; 
        m->base_addr[index].value = old_addr[i].value;
        m->base_addr[index].isTombstone = false;
        m->size += 1;
      } else {
        // If it WAS a tombstone, it's effectively gone forever, so free its memory during this cleanup!
        free(old_addr[i].key);
      }
    }
  }

  // Destroy the old physical bucket array!
  free(old_addr);
}

// Retrieves the value for the given key. Returns -1 if not found.
// Note: This relies on the assumption we don't actively store -1 as valid data.
int hashmap_get(struct tempMap *m, const char* key) {
  int index = hash_function(key, m->capacity);

  // Keep probing as long as the slot isn't completely empty
  while (m->base_addr[index].key != NULL) {
    // Found the key AND it wasn't deleted!
    if (strcmp(m->base_addr[index].key, key) == 0) {
      if (m->base_addr[index].isTombstone) {
        return -1; // Exists physically, but it was "logically" deleted
      }
      return m->base_addr[index].value;
    }
    index = (index + 1) % m->capacity;
  }

  return -1; // Completely empty bucket
}

// Removes a key from the hash table.
void hashmap_delete(struct tempMap *m, const char* key) {
  int index = hash_function(key, m->capacity);

  while (m->base_addr[index].key != NULL) {
    if (strcmp(m->base_addr[index].key, key) == 0 && !m->base_addr[index].isTombstone) {
      // Found it! Mark as a tombstone instead of completely nulling the bucket.
      // Why? Because other keys might have collided and were pushed AFTER this bucket.
      // If we make this slot NULL, the probing chain is broken and future lookups would fail!
      m->base_addr[index].isTombstone = true;
      m->size -= 1;
      return;
    }
    index = (index + 1) % m->capacity;
  }
}

// Free absolutely all allocations (keys and array) 
void hashmap_free(struct tempMap *m) {
  for (int i = 0; i < m->capacity; i++) {
    if (m->base_addr[i].key != NULL) {
      free(m->base_addr[i].key); // Free the strdup'd key memory
    }
  }
  free(m->base_addr); // Destroy buckets array
  m->size = 0;
  m->capacity = 0;
}

// Small helper just to print internals debugging
void hashmap_print(struct tempMap *m) {
  printf("--- HashMap State (Size: %d, Capacity: %d) ---\n", m->size, m->capacity);
  for(int i = 0; i < m->capacity; i++) {
    if (m->base_addr[i].key == NULL) {
      printf("[%d] NULL\n", i);
    } else if (m->base_addr[i].isTombstone) {
      printf("[%d] TOMBSTONE (formerly '%s')\n", i, m->base_addr[i].key);
    } else {
      printf("[%d] Key: '%s' | Value: %d\n", i, m->base_addr[i].key, m->base_addr[i].value);
    }
  }
  printf("----------------------------------------------\n");
}


// MAIN() TESTING BLOCK
int main() {
  struct tempMap map;
  
  // 1. Initialize with an intentionally small capacity (e.g. 4) to force resize
  hashmap_init(&map, 4);
  
  // 2. Insert items
  printf("Inserting 3 items...\n");
  hashmap_put(&map, "Apple", 100);
  hashmap_put(&map, "Banana", 200);
  hashmap_put(&map, "Orange", 300);
  hashmap_print(&map); // Size 3, Capacity 4 => 3/4 = 0.75 So next insert triggers resize!

  // 3. Trigger Resize
  printf("\nInserting 'Grapes' (Triggers Resize!)...\n");
  hashmap_put(&map, "Grapes", 400); // Exceeds load factor, resizes to 8
  hashmap_print(&map);

  // 4. Update an existing item
  printf("\nUpdating 'Apple' to 150...\n");
  hashmap_put(&map, "Apple", 150);
  printf("Lookup 'Apple': %d\n", hashmap_get(&map, "Apple"));

  // 5. Delete an item and observe tombstone
  printf("\nDeleting 'Banana'...\n");
  hashmap_delete(&map, "Banana");
  hashmap_print(&map);
  
  printf("Lookup deleted 'Banana': %d\n", hashmap_get(&map, "Banana"));

  // 6. Free everything to prevent memory leaks
  printf("\nFreeing HashMap...\n");
  hashmap_free(&map);

  return 0;
}
