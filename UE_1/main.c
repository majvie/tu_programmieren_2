#include <stdio.h>  
#include <stdlib.h> 
#include <math.h> 
#include <string.h>
#include "iueduma/duma.h"
#include "main.h"
#include <assert.h> 


int main()  
{
    // Duma Env Variable -> X% of memory allocations fail
    putenv("DUMA_MALLOC_ERROR=0");  

    vector_t vec1 = { NULL, 0 };
    vector_t vec2 = { NULL, 0 };
    vector_t vec_sum = { NULL, 0 };
    
    // Test A
    printf("---------- A ----------\n");
    printf("\nvector_allocate(): Allocating the vector...");
    assert(vector_allocate(&vec1, 10) == SUCCESS);  
    printf(" ok\n");

    printf("\nvector_fill(): Filling the vector...\n");
    assert(vector_fill(&vec1, 3, 2) == SUCCESS);  
    printf("Filled vector: ");
    vector_print(&vec1);

    printf("\n\nvector_clear(): Clearing the vector...");
    assert(vector_clear(&vec1) == SUCCESS);
    printf(" ok\n");

    // Test B
    printf("\n---------- B ----------\n");
    assert(vector_allocate(&vec1, 10) == SUCCESS); 

    printf("\nvector_change_dim(): Changing the vectors dimension from 10 to 5...\n");
    printf("From : ");
    vector_print(&vec1);
    assert(vector_change_dim(&vec1, 5) == SUCCESS);
    printf("\nTo: ");
    vector_print(&vec1);
    vector_clear(&vec1);
    
    printf("\n\nvector_add(): Adding two vectors...\n");
    vector_allocate(&vec1, 4);  
    vector_fill(&vec1, 3, 5);
    vector_print(&vec1);
    printf("\n+\n");

    vector_allocate(&vec2, 4); 
    vector_fill(&vec2, 2, 2);
    vector_fill(&vec2, 3, 3);
    vector_print(&vec2);
    printf("\n=\n");

    vector_allocate(&vec_sum, 4);
    assert(vector_add(&vec1, &vec2, &vec_sum) == SUCCESS);
    vector_print(&vec_sum);

    printf("\n\nvector_add(): Adding two vectors of different dimensions...");
    vector_change_dim(&vec1, 5);
    assert(vector_add(&vec1, &vec2, &vec_sum) == DIMENSION_MISMATCH_ERROR);
    printf(" ok\n");

    printf("\n\nvector_slice(): Slicing a vector\n");
    printf("Slicing from 1:3 (incl, excl) from: ");
    vector_print(&vec_sum);
    assert(vector_slice(&vec_sum, 1, 3) == SUCCESS);
    printf("\nTo: ");
    vector_print(&vec_sum);
    
    vector_clear(&vec1);
    vector_clear(&vec2);
    vector_clear(&vec_sum);

    // Test C
    printf("\n\n---------- C ----------\n");
    printf("\nvector_allocate_dynamic(): Allocating the vector of size 4 dynamically...\n");
    vector_t *new_vector = vector_allocate_dynamic(4);
    printf("Vector: ");
    vector_print(new_vector);

    printf("\n\nvector_free(): Free the dynamically allocated vector...");
    assert(vector_free(new_vector) == SUCCESS);
    printf(" ok\n");
    
    // Test D
    printf("\n---------- D ----------\n");
    vector_t *d_vector_1 = vector_allocate_dynamic(4);
    vector_t *d_vector_2 = vector_allocate_dynamic(2);
    vector_fill(d_vector_1, 0, 12);
    vector_fill(d_vector_2, 1, 4);
    printf("D_Vector_1: ");
    vector_print(d_vector_1);
    printf("\nD_Vector_2: ");
    vector_print(d_vector_2);

    printf("\n\nvec_array_allocate(): Allocating the vector array dynamically...");
    vec_array_t *vector_array = vec_array_allocate(0);
    assert(vector_array);
    printf(" ok\n");

    printf("\nvec_array_store(): Storing above vectors in array...\n");
    assert(vec_array_store(vector_array, d_vector_1) == SUCCESS);
    assert(vec_array_store(vector_array, d_vector_2) == SUCCESS);
    vec_array_show(vector_array);

    /*
    printf("\nvec_array_delete_at(): Storing above vectors in array...");
    assert(vec_array_delete_at(vector_array, 0) == SUCCESS);
    printf(" ok\n");
    vec_array_show(vector_array);
    */
    

    printf("\nvec_array_free(): Freeing the dynamically allocated vector array...");
    assert(vec_array_free(vector_array) == SUCCESS);
    printf(" ok\n");
    
    // Test E
    printf("---------- E ----------\n");
    vec_array_t *vector_array_e = vec_array_allocate(0);
    vector_t *e_vector = vector_allocate_dynamic(4);
    vec_array_store(vector_array_e, e_vector);

    printf("\nvec_array_save(): Saving vector array...\n");
    assert(vec_array_save(vector_array_e, "testfile.txt") == SUCCESS);

    printf("\nvec_array_load(): Loading vector array...\n");
    vec_array_t *loaded_vector_array = vec_array_load("testfile.txt");
    assert(loaded_vector_array);
    vec_array_show(loaded_vector_array);

    // Test F
    printf("\n---------- F ----------\n\n");
    vec_array_t *vector_array_f = vec_array_load("array_to_sort.txt");
    vec_array_show(vector_array_f);

    printf("\nvector_norm(): Getting euclidian norm...\n");
    double norm_of_vector = vector_norm(vector_array_f->vectors[0]);
    printf("Vector norm of [2, 3, 4] = %g\n", norm_of_vector);

    printf("\nvec_array_sort(): Sorting vector array...\n");
    assert(vec_array_sort(vector_array_f, vector_norm) == SUCCESS);
    vec_array_show(vector_array_f);


    // Freeing all the vector arrays
    vec_array_free(vector_array_e);
    vec_array_free(loaded_vector_array);
    vec_array_free(vector_array_f);
    
    printf("\nEND\n");
    return 0;
}   

error_t vector_allocate(vector_t *vec, long dim) {
    if (!vec) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    if (dim < 1) {
        fprintf(stderr, "The dimension needs to be > 0\n");
        return DIMENSION_MISMATCH_ERROR;
    }

    long *values = calloc(dim, sizeof(long));
    if (!values) {
        fprintf(stderr, "There was a problem allocating memory.\n");
        return ALLOC_FAILURE_ERROR;
    }

    vec->values = values;
    vec->dim = dim;
    return SUCCESS;
}

error_t vector_clear(vector_t *vec) {
    vec->dim = 0;
    free(vec->values);
    vec->values = NULL;
    return SUCCESS;
}

error_t vector_fill(vector_t *vec, long index, long value) {
    if (!vec) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    if (index >= 0 && index > vec->dim - 1) {
        fprintf(stderr, "Index out of range\n");
        return OUT_OF_RANGE_ERROR;
    }
    vec->values[index] = value;
    return SUCCESS;
}

error_t vector_print(vector_t *vec) {
    if (!vec || !vec->values || vec->dim < 1) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    printf("[ ");
    for (int i = 0; i < vec->dim; i++) {
        printf("%ld, ", vec->values[i]);
    }
    printf("\b\b ]");

    return SUCCESS;
}

error_t vector_change_dim(vector_t *vec, long new_dim) {
    // Changes the dimension of the vector to new_dim, cuts off values or initialises new ones with 0
    long *new_values = calloc(new_dim, sizeof(long));
    if (!new_values) {
        fprintf(stderr, "There was a problem allocating memory. \n");
        return ALLOC_FAILURE_ERROR;
    }

    if (!vec) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    long smaller_dim = vec->dim > new_dim ? new_dim : vec->dim;
    for (int i = 0; i < smaller_dim; i++) {
        new_values[i] = vec->values[i];
    }

    free(vec->values);
    vec->dim = new_dim;
    vec->values = new_values;
    
    return SUCCESS;
}

error_t vector_add(vector_t *vec_a, vector_t *vec_b, vector_t *vec_sum) {
    // Can only add vectors if they have the same dimension
    if (!vec_a || !vec_b || !vec_sum) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }
    if (vec_a->dim != vec_b->dim || vec_a->dim != vec_sum->dim) {
        fprintf(stderr, "The dimensions of the vectors do not match.\n");
        return DIMENSION_MISMATCH_ERROR;
    }

    for (int i = 0; i < vec_a->dim; i++) {
        vec_sum->values[i] = vec_a->values[i] + vec_b->values[i];
    }

    return SUCCESS;
}

error_t vector_slice(vector_t *vec, long index_a, long index_b) {
    long new_field_length = index_b - index_a;

    if (index_a < 0 || index_b > vec->dim) {
        fprintf(stderr, "The indices need to be in range\n");
        return OUT_OF_RANGE_ERROR;
    }

    if (!vec) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    long *new_field = calloc(new_field_length, sizeof(long));
    if (!new_field) {
        fprintf(stderr, "There was a problem allocating memory.\n");
        return ALLOC_FAILURE_ERROR;
    }

    for (int i = 0; i < new_field_length; i++) {
        new_field[i] = vec->values[index_a + i];
    }

    free(vec->values);
    vec->values = new_field;
    vec->dim = new_field_length;

    return SUCCESS;
}

vector_t *vector_allocate_dynamic(long dim) {
    vector_t *new_vec = malloc(sizeof(vector_t));
    long *values = calloc(dim, sizeof(long));
    if (!values || !new_vec) {
        fprintf(stderr, "There was a problem allocating memory.\n");
        return NULL;
    }

    if (dim == 0) {
        new_vec->values = NULL;
    }
    else {
        new_vec->values = values;
    }

    new_vec->dim = dim;
    return new_vec;
}

error_t vector_free(vector_t *vec) {
    free(vec->values);
    vec->values = NULL;
    free(vec);
    return SUCCESS;
}

vec_array_t *vec_array_allocate(long length) {
    vec_array_t *new_vec_array = malloc(sizeof(vec_array_t));
    vector_t **vectors = (vector_t **) malloc(length * sizeof(vector_t*));
    if (!new_vec_array || !vectors) {
        fprintf(stderr, "There was a problem allocating memory.\n");
        return NULL;
    }
    
    // Setting all the vector value pointers to null as initial value
    for (int i = 0; i < length; i++) {
        vectors[i] = vector_allocate_dynamic(1);
        vectors[i]->values = NULL;
        vectors[i]->dim = 0;
    }

    new_vec_array->vectors = vectors;
    new_vec_array->length = length;
    return new_vec_array;
}

error_t vec_array_free(vec_array_t *array) {
    if (!array) {
        fprintf(stderr, "This array does not exist\n");
        return NULL_POINTER_ERROR;
    }

    for (int i = 0; i < array->length; i++) {
        if (array->vectors[i]) {
            vector_free(array->vectors[i]);
        }
    }
    free(array->vectors);
    free(array);
    return SUCCESS;
}

error_t vec_array_store(vec_array_t *array, vector_t *vec) {
    // Reallocating memory
    vector_t** new_vectors = realloc(array->vectors, (array->length+1)*sizeof(vector_t*));
    if (!new_vectors) {
        fprintf(stderr, "The field could not be resized.\n");
        return ALLOC_FAILURE_ERROR;
    }

    if (!vec) {
        fprintf(stderr, "vec_array_store: The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    // Adding new vector and freeing old memory
    //free(array->vectors); // No need to free because realloc already does it
    new_vectors[array->length] = vec;
    array->vectors = new_vectors;
    array->length += 1;

    return SUCCESS;
}

error_t vec_array_delete_at(vec_array_t *array, long index) {
    if (!array) {
        fprintf(stderr, "The array does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    long new_array_length = array->length-1;
    if (index > new_array_length) {
        fprintf(stderr, "The index is out of range.\n");
        return OUT_OF_RANGE_ERROR;
    }

    vector_t** new_field = malloc(sizeof(vector_t)*new_array_length);
    if (!new_field) {
        fprintf(stderr, "The field could not be resized.\n");
        return ALLOC_FAILURE_ERROR;
    }

    // Copy the data
    int j = 0;
    for (int i = 0; i < new_array_length; i++, j++) {
        // Skip specified index
        if (i == index) {
            j++;
        }
        new_field[i] = array->vectors[j];
        free(array->vectors[i]);
    }

    free(array->vectors);
    array->vectors = new_field;
    array->length -= 1;

    return SUCCESS;
}

error_t vec_array_show(vec_array_t *array) {
    if (!array || array->length < 1) {
        fprintf(stderr, "The array does not exist. \n");
        return NULL_POINTER_ERROR;
    }
    
    printf("{\n");

    for (int i = 0; i < array->length; i++) {
        if (array->vectors[i]->dim > 0) {
            printf("  ");
            vector_print(array->vectors[i]);
            printf(",\n");
        }
        else {
            printf("Uninitialized vector: %p,\n", array->vectors[i]);
        }
    }

    printf("}\n");

    return SUCCESS;
}

error_t vec_array_save(vec_array_t *array, char *file_name) {
    // Save vector array as csv
    if (!array) {
        fprintf(stderr, "The array does not exist.\n");
        return NULL_POINTER_ERROR;
    }

    FILE* fp = fopen(file_name, "w");
    if (!fp) {
        fprintf(stderr, "The file could not be opened.\n");
        fclose(fp);
        return NULL_POINTER_ERROR;
    }
    
    vector_t *current_vector;
    for (int i = 0; i < array->length; i++) {
        current_vector = array->vectors[i];

        // Uninitialised vector
        if (current_vector->dim == 0) {
            fprintf(fp, "NULL,");
        }
        else {
            for (int j = 0; j < current_vector->dim; j++) {
                fprintf(fp, "%ld, ", current_vector->values[j]);
            }
        }
        fprintf(fp, "\n");
    }

    printf("The %ld vectors were saved in %s\n", array->length, file_name);
    fclose(fp);
    return SUCCESS;
}

long get_number_of_lines(FILE* fp) {
    long lines = 0;
    char ch;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch == '\n') {
            lines++;
        }
    }
    fseek(fp, 0, SEEK_SET);
    return lines;
}

vec_array_t *vec_array_load(char *file_name) {
    // Read from csv, returns array vector pointer
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "The file could not be opened\n");
        fclose(fp);
        return NULL;
    }

    long number_of_lines = get_number_of_lines(fp);
    vec_array_t *vector_array = vec_array_allocate(0);
    vector_t *current_vector;
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Check if uninitialised
        if (strcmp(line, "NULL,\n") == 0) {
            current_vector = vector_allocate_dynamic(0);
            vec_array_store(vector_array, current_vector);
            continue;
        }

        // Dimensions of current vector
        int dim = 0;
        // Getting elements of line
        for (int i = 0; line[i]; i++) {
            if (',' == line[i]) {
                dim++;
            }
        }

        current_vector = vector_allocate_dynamic(dim);

        // parse line with ',' delimiter
        long number;
        char* token = strtok(line, ",");
        char* end;
        for (int i = 0; i < dim; i++) {
            number = strtol(token, &end, 10);
            vector_fill(current_vector, i, number);
            token = strtok(NULL, ",");
        }
        vec_array_store(vector_array, current_vector);
    }
    
    printf("The vectors were read from %s\n", file_name);
    fclose(fp);
    return vector_array;
}

double vector_norm(vector_t *vec) {
    // Returns euclidian norm of vector
    if (!vec) {
        fprintf(stderr, "The vector does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    long norm_of_vector = 0;
    for (int i = 0; i < vec->dim; i++) {
        norm_of_vector += pow(vec->values[i], 2);
    }
    return pow(norm_of_vector, 0.5);
}

error_t vec_array_sort(vec_array_t *array, measure_f sort_key) {
    if (!array) {
        fprintf(stderr, "The array does not exist. \n");
        return NULL_POINTER_ERROR;
    }

    vector_t** smallest, *temp;
    for (int i = 0; i < array->length-1; i++) {
        smallest = &array->vectors[i];
        for (int j = i+1; j < array->length; j++)
            if (sort_key(array->vectors[j]) < sort_key(*smallest))
                smallest = &array->vectors[j];
        // Swap
        temp = array->vectors[i];
        array->vectors[i] = *smallest;
        *smallest = temp;
    }
    return SUCCESS;
}
