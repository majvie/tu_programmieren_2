#define MAX_INPUT_LEN 256
#define MAX_LINE_LEN 100
#define DEFAULT_SIZE 30

#define SUCCESS 1
#define FAILURE 0

#define START_TIMING clock_t timeStart = clock();
#define STOP_TIMING  (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC

typedef struct entry_s {
    char songtitle[256], interpreter[256];
} entry_t;

typedef struct heap_s {
    entry_t *entries;
    long size;
    long next_free;
} heap_t;

typedef struct hasharray_s {
    entry_t *entries;
    long num_entries;
} hasharray_t;

typedef struct hash_s {
    hasharray_t *hasharrays;
    long hashsize;
} hash_t;


// TEIL A
heap_t *heap_create(long size);
long heap_insert(heap_t *heap, char songtitle[], char interpreter[]);

// TEIL B
void heap_print(heap_t *heap);
void heap_free(heap_t *heap);

// TEIL C
long heap_search(heap_t *heap, char songtitle[], char interpreter[]);

// TEIL D
long heap_delete_first(heap_t *heap);

// TEIL E
hash_t *hash_create(long hashsize);
void hash_insert(hash_t *hash, char songtitle[], char interpreter[], long hashsize);

// TEIL F
void hash_print(hash_t *hash);
void hash_free(hash_t *hash);
hash_t *convert_heap_to_hash(heap_t *heap, long hashsize);

// TEIL G
entry_t *hash_search(hash_t *hash, char songtitle[], char interpreter[], long hashsize);
long hash_delete_entry(hash_t *hash, char songtitle[], char interpreter[], long hashsize);

// TEIL H
void benchmark(long hashsize, long heapsize);

