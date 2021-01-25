#include <string.h>
#include <math.h>
#include "time.h"
#include "iueduma/duma.h"

long read_value(char *prompt) {
  long number;
  char text_b[MAX_INPUT_LEN];

  printf("%s", prompt);
  if (fgets(text_b, sizeof(text_b), stdin) != NULL) {
    if (sscanf(text_b, "%ld", &number) == 0) {
      fprintf(stderr, "Der Input konnte nicht gelesen werden.\n");
      number = read_value(prompt);
    }
  }
  return number;
}

void read_string(char *prompt, char *text) {
  char text_b[MAX_INPUT_LEN];

  while (1) {
    printf("%s", prompt);
    if (fgets(text_b, sizeof(text_b), stdin) != NULL) {
      if (sscanf(text_b, "%s", text) == 0) {
        fprintf(stderr, "read_string(): The input could not be read.\n");
        continue;
      }
      // Buffer Overflow == no newLine written
      else if (!strchr(text_b, '\n')) {
        fprintf(stderr, "read_string(): The input was too long.\n");
        while ((fgetc(stdin)) != '\n'); // Flush rest of input buffer
        continue;
      }
    }
    break;
  }
}

// TEIL A
heap_t *heap_create(long size) {
    heap_t* new_heap = malloc(sizeof(heap_t));
    if (!new_heap) {
      fprintf(stderr, "heap_create(): There was a problem allocating memory for the new heap.\n");
      return FAILURE;
    }

    entry_t* entries = malloc(sizeof(entry_t) * size);
    if (!entries) {
      fprintf(stderr, "heap_create(): There was a problem allocating memory for the entries.\n");
      return FAILURE;
    }

    new_heap->size = size;
    new_heap->entries = entries;
    new_heap->next_free = 1;

    return new_heap;
}

void upheap(heap_t *heap, long index) {
    entry_t element = heap->entries[index];

    while(index >= 2 && strcmp(heap->entries[index / 2].songtitle, element.songtitle) < 0) {
        heap->entries[index] = heap->entries[index / 2];
        index = index / 2;
    }
    
    heap->entries[index] = element;
}

long heap_insert(heap_t *heap, char songtitle[], char interpreter[]) {
    if (!heap || !heap->size) {
      fprintf(stderr, "heap_insert(): Heap must not be NULL.\n");
      return FAILURE;
    }

    entry_t new_element;
    strcpy(new_element.songtitle, songtitle);
    strcpy(new_element.interpreter, interpreter);

    heap->entries[heap->next_free] = new_element;
    upheap(heap, heap->next_free);
    heap->next_free++;

    return SUCCESS;
}

// TEIL B
void heap_print(heap_t *heap) {
    for(int i = 1; i < heap->next_free; i++) {
      if (i % 2 == 0) {
        printf("\n");
      }
      printf("(%s, %s) ", heap->entries[i].songtitle, heap->entries[i].interpreter);
    }
    printf("\n");
}

void heap_free(heap_t *heap) {
  free(heap->entries);
  free(heap);
}

// TEIL C
long heap_search(heap_t *heap, char songtitle[], char interpreter[]) {
    int level = 0;

    // Going down the levels from top to bottom
    // TODO: Add condition to stop if all elements of one level are "smaller" than searched one
    // Here not necessary, but better for bigger data structure
    while (pow(2, level) < heap->next_free) {
      // Searching through one level
      for (int i = 0; i < pow(2, level); i++) {
        int current_index  = pow(2, level) + i;
        if (strcmp(heap->entries[current_index].songtitle, songtitle) == 0) {
          if (strcmp(heap->entries[current_index].interpreter, interpreter) == 0) {
            return current_index;
          }
        }
      }
      level++;
    }
    
    return FAILURE;
}

// TEIL D
void swap_entries(entry_t *a, entry_t *b) {
    entry_t tmp = *a;
    *a = *b;
    *b = tmp;
}

// Heap reparieren nach dem entfernen der Wurzel
void downHeap(heap_t *heap, long index) {
    long largest = index;
    long left = 2 * index;
    long right = left + 1;

    if(left < heap->next_free && strcmp(heap->entries[largest].songtitle, heap->entries[left].songtitle) < 0) {
        largest = left; 
    }
    if(right < heap->next_free && strcmp(heap->entries[largest].songtitle, heap->entries[right].songtitle) < 0) {
        largest = right;
    }
    if(largest != index) {
        swap_entries(&(heap->entries[largest]), &(heap->entries[index]));
        downHeap(heap, largest);
    }
}

long heap_delete_first(heap_t *heap) {
  long first_index = 1;

  if(heap->next_free > first_index) {
    entry_t heap_root = heap->entries[first_index];
    printf("The root is: (%s, %s).\n", heap_root.songtitle, heap_root.interpreter);

    heap->next_free--;
    heap->entries[first_index] = heap->entries[heap->next_free];
    downHeap(heap, first_index);

    return SUCCESS;
  } else {
      fprintf(stderr, "heap_delete_first(): Heap is empty!");
      return FAILURE;
  }

}

// TEIL E
long hash_key(char songtitle[], char interpreter[], long hashsize)
{
  unsigned long index = 0, i;
  for (i = 0; i < strlen(songtitle); ++i)
  index = 64 * index + (long)(songtitle[i]);
  for (i = 0; i < strlen(interpreter); ++i)
  index = 64 * index + (long)(interpreter[i]);
  return index % hashsize;
}

hash_t *hash_create(long hashsize) {
  hash_t *new_hash = malloc(sizeof(hash_t));
  if (!new_hash) {
    fprintf(stderr, "hash_create(): There was a problem allocating memory for the hash.\n");
    return FAILURE;
  }

  hasharray_t *new_hash_arrays = malloc(sizeof(hasharray_t)*hashsize);
  if (!new_hash_arrays) {
    fprintf(stderr, "hash_create(): There was a problem allocating memory for the hasharrays.\n");
    return FAILURE;
  }

  for (int i = 0; i < hashsize; i++) {
    new_hash_arrays[i].num_entries = 0;
    new_hash_arrays[i].entries = NULL;
  }

  new_hash->hashsize = hashsize;
  new_hash->hasharrays = new_hash_arrays;

  return new_hash;
}

void hash_insert(hash_t *hash, char songtitle[], char interpreter[], long hashsize) {
  entry_t new_element;
  strcpy(new_element.songtitle, songtitle);
  strcpy(new_element.interpreter, interpreter);

  int index = hash_key(songtitle, interpreter, hashsize);
  hasharray_t *hasharray = &hash->hasharrays[index];

  // First time writing to bucket -> create entry list of lenghth 1
  if (!hasharray->entries) {
    entry_t *new_entries = malloc(sizeof(entry_t));
    if (!new_entries) {
      fprintf(stderr, "hash_insert(): Memory for new entries could not be allocated.\n");
      return;
    }

    new_entries[0] = new_element;
    hasharray->entries = new_entries;
    hasharray->num_entries = 1;
  }

  // Collision: Append new element to the entry array
  else {
    entry_t *updated_entries = realloc(hasharray->entries, sizeof(entry_t) * (hasharray->num_entries+1));
    if (!updated_entries) {
      fprintf(stderr, "hash_insert(): Memory for new entries could not be reallocated.\n");
      return;
    }
    else {
      hasharray->entries = updated_entries;
    }

    hasharray->entries[hasharray->num_entries] = new_element;
    hasharray->num_entries++;
  }
}

// TEIL F
void hash_print(hash_t *hash) {
  for (int i = 0; i < hash->hashsize; i++) {
    if (hash->hasharrays[i].entries) {
      printf("%i: ", i);
      for (int j = 0; j < hash->hasharrays[i].num_entries; j++) {
        entry_t current_entry = hash->hasharrays[i].entries[j];
        printf("(%s, %s) ", current_entry.songtitle, current_entry.interpreter);
      }
      printf("\n");
    }
  }
}

void hash_free(hash_t *hash) {
  if (hash) {
    for (int i = 0; i < hash->hashsize; i++) {
      free(hash->hasharrays[i].entries);
    }

    free(hash->hasharrays);
    free(hash);
  }
}

hash_t *convert_heap_to_hash(heap_t *heap, long hashsize) {
  hash_t *new_hash = hash_create(hashsize);
  if (FAILURE == new_hash) {
    fprintf(stderr, "convert_heap_to_hash(): There was an issue creating the new hash.\n");
    return FAILURE;
  }

  for (int i = 1; i < heap->next_free; i++) {
    entry_t current_entry = heap->entries[i];
    hash_insert(new_hash, current_entry.songtitle, current_entry.interpreter, hashsize);
  }

  return new_hash;
}

// TEIL G
entry_t *hash_search(hash_t *hash, char songtitle[], char interpreter[], long hashsize) {
  long index = hash_key(songtitle, interpreter, hashsize);
  for (int i = 0; i < hash->hasharrays[index].num_entries; i++) {
    entry_t *current_entry = &hash->hasharrays[index].entries[i];
    if (strcmp(songtitle, current_entry->songtitle) == 0 && strcmp(interpreter, current_entry->interpreter) == 0) {
      return current_entry;
    }
  }
  return FAILURE;
}

long hash_delete_entry(hash_t *hash, char songtitle[], char interpreter[], long hashsize) {
  long index = hash_key(songtitle, interpreter, hashsize);
  long bucket_size = hash->hasharrays[index].num_entries;
  for (int i = 0; i < bucket_size; i++) {
    entry_t *current_entry = &hash->hasharrays[index].entries[i];
    if (strcmp(songtitle, current_entry->songtitle) == 0 && strcmp(interpreter, current_entry->interpreter) == 0) {
      // If only one element remove the whole array
      if (hash->hasharrays[index].num_entries == 1) {
        free(hash->hasharrays[index].entries);
        hash->hasharrays[index].num_entries = 0;
        hash->hasharrays[index].entries = NULL;
      }
      
      // Delete entry at index, shift next elements to the left, realloc the block
      else {
        for(int j = i; j < bucket_size - 1; j++)
        {
            hash->hasharrays[index].entries[j] = hash->hasharrays[index].entries[j+1];
        }

        hash->hasharrays[index].num_entries--;
        entry_t *updated_entries = realloc(hash->hasharrays[index].entries, sizeof(entry_t)*hash->hasharrays[index].num_entries);
        if (!updated_entries) {
          fprintf(stderr, "hash_delete_entry(): Memory for new entries could not be reallocated.\n");
          return FAILURE;
        }
        else {
          hash->hasharrays[index].entries = updated_entries;
        }
      }

      return SUCCESS;
      
    }
  }
  return FAILURE;
}

// TEIL H

// from benchmark.zip
/* create a random alphanumeric string */
char *get_random_string(size_t length)
{
  if (!length) { return 0; }

  size_t key, i;
  static char charset[] = "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGIJKLMNOPQRSTUVWXYZ"
                          "0123456789";

  size_t l = (int)(sizeof(charset)/sizeof(charset[0]) - 1);

  char *rnd_string = malloc((length+1)*sizeof(char));

  if (rnd_string)
  {
    for (i=0; i<length; i++)
    {
      key = rand() % l;
      rnd_string[i] = charset[key];
    }
    rnd_string[length] = '\0';
    return rnd_string;
  }
  else
    return 0;
}

/* creates an array of strings with length string_length */
char **generate_random_string_array(size_t array_size, size_t string_length)
{
  if (!array_size || !string_length) { return 0; }

  int i;
  char **rnd_string_array = malloc(array_size*sizeof(char *));

  if (!rnd_string_array) { return 0; }

  for (i=0; i<array_size; i++)
    rnd_string_array[i] = get_random_string(string_length);

  return rnd_string_array;
}
/////

void benchmark(long hashsize, long heapsize) {
    hash_t *new_hash = hash_create(hashsize);
    heap_t *new_heap = heap_create(heapsize);
    long N = 100;
    long times_of_tries = 5; // N = (100, 200, ... 1000)
    long string_length = 10;

    char *list_of_songs[N * times_of_tries];
    char *list_of_interpreters[N * times_of_tries];

    double times_heap_insert[times_of_tries];
    double times_heap_search[times_of_tries];
    double times_hash_insert[times_of_tries];
    double times_hash_search[times_of_tries];

    clock_t timeStart;
    double passed_time;

    for (int i = 0; i < N * times_of_tries; i++) {
        list_of_songs[i] = get_random_string(string_length);
        list_of_interpreters[i] = get_random_string(string_length);
    }

    for (int try = 1; try < times_of_tries+1; try++) {
        printf("Inserting into a heap of size \t%ld... \n", try * N);
        hash_t *new_hash = hash_create(try*N+1);
        heap_t *new_heap = heap_create(try*N+1);


        // TODO: put timing into function with function pointers as arguments

        // HEAP
        timeStart = clock();
        for (int i = 0; i < try * N; i++) {
            heap_insert(new_heap, list_of_songs[i], list_of_interpreters[i]);
        }
        passed_time = (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC;
        times_heap_insert[try-1] = passed_time;

        timeStart = clock();
        for (int i = 0; i < try * N; i++) {
            heap_search(new_heap, list_of_songs[i], list_of_interpreters[i]);
        }
        passed_time = (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC;
        times_heap_search[try-1] = passed_time;
        // 

        // HASH
        timeStart = clock();
        for (int i = 0; i < try * N; i++) {
            hash_insert(new_hash, list_of_songs[i], list_of_interpreters[i], try*N);
        }
        passed_time = (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC;
        times_hash_insert[try-1] = passed_time;

        timeStart = clock();
        for (int i = 0; i < try * N; i++) {
            hash_search(new_hash, list_of_songs[i], list_of_interpreters[i], try*N);
        }
        passed_time = (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC;
        times_hash_search[try-1] = passed_time;
        // 

        // Freeing memory
        heap_free(new_heap);
        hash_free(new_hash);

        printf("\n");
    }

    printf("\n---------- HEAP ---------\n");
    for (int try = 0; try < times_of_tries; try++) {
        // Funktionaler Zusammenhang:
        // Insert O(1), Search O(N)
        printf("Time for heap insert of %ld nodes:\t %.2f \n", (try+1) * N, times_heap_insert[try]);
        printf("Time for heap search of %ld nodes:\t %.2f \n", (try+1) * N, times_heap_search[try]);
        printf("\n");
    }

    printf("\n---------- HASH ---------\n");
    for (int try = 0; try < times_of_tries; try++) {
        // Funktionaler Zusammenhang:
        // Insert O(1), Search O(1)
        printf("Time for hash insert of %ld nodes:\t\t %.2f \n", (try+1) * N, times_hash_insert[try]);
        printf("Time for hash search of %ld nodes:\t\t %.2f \n", (try+1) * N, times_hash_search[try]);
        printf("\n");
    }
    

    // Freeing data
    for (int i = 0; i < times_of_tries * N; i++) {
        free(list_of_songs[i]);
        free(list_of_interpreters[i]);
    }
    heap_free(new_heap);
    hash_free(new_hash);
}
