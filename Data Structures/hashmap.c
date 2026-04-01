typedef struct oa_key_operations_s{
  uint32_t (*hash)(const void* data, void* arg);
  void* (*cp)(const void* data, void* arg);
  void* (*free)(void* data, void* arg);
  bool (*eq)(const void* data1, const void* data2, void* arg);
  void *arg;
} oa_key_operations;

typedef struct oa_value_operations_s{
  void* (*cp)(const void* data, void* arg);
  void* (*free)(void* data, void* arg);
  bool (*eq)(const void* data1, const void* data2, void* arg);
  void *arg;
} oa_value_operations;

typedef struct {
  uint32_t hash;
  void* key;
  void* val;
} oa_pair;

typedef struct oa_hash_s {
  size_t capacity;             // TOTAL NO. OF BUCKETS
  size_t size;                 // NO. OF ACTIVE ELEMENTS
  oa_pair **buckets;           // CONTIGIOUS ARRAY OF PAIR POINTERS
  void (*probing_fct)(struct oa_hash_s *htable, size_t *idx); // PROBING STRATEGY
  oa_key_operations key_ops;   // KEY SPECIFIC OPERATIONS 
  oa_value_operations val_ops; // VALUE SPECIFIC OPERATIONS
} oa_hash;

static inline void oa_hash_lp_index(oa_hash *table, size_t *idx) {
  (*idx)++;
  if((*idx) == htable->capacity) {
    (*idx) = 0; // WRAP AROUND
  }
}

#define OA_HASH_LOAD_FACTOR (0.75)   // Resize when 75% full
#define OA_HASH_GROWTH_FACTOR (1<<2) // 4x growth on resize
#define OA_HASH_INIT_CAPACITY (1<<12) // Initial capacity of 4096

// CREATING A NEW HASH TABLE 
oa_hash *oa_hash_new(
  oa_key_operations key_ops,
  oa_value_operations val_ops,
  void (*probing_fct)(struct oa_hash_s *htable, size_t *from_idx)
);

oa_hash *oa_hash_new_lp(
  oa_key_operations key_ops,
  oa_value_operations val_ops
);

// DESTRUCTOR LIKE MTHD FOR DESTROYING THE HASH TABLE
void oa_hash_free(oa_hash* htable);

// ADDING, READING, DELETING ENTRIES
void oa_hash_put(oa_hash* htable, const void* key, const void* val);
void* oa_hash_get(oa_hash* htable, const void* key);
void oa_hash_dalete(oa_hash* htable, const void* key);
void oa_hash_print(
  oa_hash *htable,
  void(*print_key)(const void *k),
  void(*print_val)(const void* v)
);

// PAIR RELATED
oa_pair* oa_pair_new(
  uint32_t hash,
  const void* key,
  const void* val
);

// STRING OPERATIONS
uint32_t oa_string_hash(const void* data, void* arg);
void* oa_string_cp(const void* data, void* arg);
bool oa_string_eq(const void* data1, const void* data2, void* arg);
void oa_string_free(void* data, void* arg);
void oa_string_print(const void* data);

oa_hash* oa_hash_new(

)
