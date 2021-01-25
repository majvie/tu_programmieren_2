#define MAX_LINE_LEN 256

typedef enum error_e
{
    SUCCESS=0,
    NULL_POINTER_ERROR,
    ALLOC_FAILURE_ERROR,
    OUT_OF_RANGE_ERROR,
    DIMENSION_MISMATCH_ERROR
} error_t;

typedef struct vector_s
{
    long *values;
    long dim;
} vector_t;

typedef struct vec_array_s
{
    vector_t **vectors;
    long length;
} vec_array_t;

// A
error_t vector_allocate(vector_t *vec, long dim);
error_t vector_clear(vector_t *vec);
error_t vector_fill(vector_t *vec, long index, long value);
error_t vector_print(vector_t *vec);

// B
error_t vector_change_dim(vector_t *vec, long new_dim);
error_t vector_add(vector_t *vec_a, vector_t *vec_b, vector_t *vec_sum);
error_t vector_slice(vector_t *vec, long index_a, long index_b);

// C
vector_t *vector_allocate_dynamic(long dim);
error_t vector_free(vector_t *vec);

// D
vec_array_t *vec_array_allocate(long length);
error_t vec_array_free(vec_array_t *array);
error_t vec_array_store(vec_array_t *array, vector_t *vec);
error_t vec_array_delete_at(vec_array_t *array, long index);

// E
error_t vec_array_save(vec_array_t *array, char *file_name);
vec_array_t *vec_array_load(char *file_name);

// F
double vector_norm(vector_t *vec);
typedef double (*measure_f) (vector_t *);
error_t vec_array_sort(vec_array_t *array, measure_f sort_key);

// My helpers
error_t vec_array_show(vec_array_t *array);
long get_number_of_lines(FILE* fp);