#include <string.h>
#include "iueduma/duma.h"

long read_value(char *prompt) {
  long number;
  char text_b[MAX_INPUT_LEN];

  printf("%s: ", prompt);
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
    printf("%s: ", prompt);
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

void read_from_keyboard(Element_t *item) {
    if (!item) {
        fprintf(stderr, "read_from_keyboard(): Item vector was not defined. \n");
        return;
    }

    read_string("songtitle: ", item->songtitle);
    read_string("interpreter: ", item->interpreter);

    return;
}

Element_t *allocate_element() {
    Element_t* new_element = malloc(sizeof(Element_t));
    if (!new_element) {
        fprintf(stderr, "allocate_element(): The new element could not be initialised.\n");
        return NULL;
    }
    read_from_keyboard(new_element);
    new_element->prev = NULL;
    new_element->next = NULL;

    return new_element;
}

Element_t *insert_last(Element_t *list) {
    if (!list) {
        fprintf(stderr, "insert_last(): The argument was NULL.\n");
        return NULL;
    }

    // Jumping to last element
    while (list->next) { list = list->next; }

    Element_t *last_item = allocate_element();

    if (!last_item) {
        fprintf(stderr, "insert_last(): The new item could not be initialised.\n");
        return NULL;
    }

    last_item->prev = list;
    list->next = last_item;

    // Going back to first element
    while (list->prev) { list = list->prev; }

    return list;
}

void free_list(Element_t **list) {
    if (!*list) {
        fprintf(stderr, "free_list(): The argument cannot be NULL.\n");
        return;
    }

    // Go back to beginning
    while ((*list)->prev) { *list = (*list)->prev; }

    while ((*list)->next) {
        *list = (*list)->next;
        free((*list)->prev);
    }
    free(*list);
    *list = NULL;
}

void print_single_element(Element_t *element) {
    if (!element) {
        fprintf(stderr, "print_single_element(): Argument need not be NULL.\n");
        return;
    }
    printf("Title: %s \t\t Artist: %s", element->songtitle, element->interpreter);
}

void print_entire_list(Element_t *list) {
    if (!list) {
        fprintf(stderr, "print_entire_list(): Argument need not be NULL.\n");
        return;
    }

    while (list->prev) { list = list->prev; } // Go back to first element

    int counter = 1;
    while (list) {
        printf("%i. ", counter);
        print_single_element(list);
        printf("\n");
        list = list->next;
        counter++;
    }
}

long elements_count(Element_t *list) {
    if (!list) {
        fprintf(stderr, "elements_count(): The argument cannot be NULL\n");
        return 1;
    }

    int counter = 0;

    // Go back to beginning
    while (list->prev) { list = list->prev; }

     // Jumping to last element
    do { 
        counter++;
        list = list->next; 
    } while (list);

    return counter;
}

Element_t *element_at(Element_t *list, long index) {
    // Go back to beginning
    while (list->prev) { list = list->prev; }

    if (index < 0) {
        fprintf(stderr, "element_at(): The index needs to be bigger than 0, negative indices are not supported yet\n");
        return NULL;
    }
    for (int i = 0; i < index; i++) {
        list = list->next;

        // Reaching end of list
        if (!list) {
            fprintf(stderr, "element_at(): Index out of range\n");
            return NULL;
        }
    }
    return list;
}

Element_t *insert_at(Element_t *list, long index) {
    Element_t *element_to_replace = element_at(list, index);
    if (!element_to_replace) {
        fprintf(stderr, "insert_at(): There was a problem inserting the new node\n");
        return NULL;
    }

    Element_t *new_element = allocate_element();

    if (!new_element) {
        fprintf(stderr, "insert_at(): The new item could not be initialised.\n");
        return NULL;
    }

    new_element->next = element_to_replace;
    new_element->prev = element_to_replace->prev;

    if (element_to_replace->prev) {
        element_to_replace->prev->next = new_element;
        element_to_replace->prev = new_element; 
    }
    
    // Going back to first element
    while (new_element->prev) { new_element = new_element->prev; }

    return new_element;
}

Element_t *delete_at(Element_t *list, long index) {
    Element_t *element_to_delete = element_at(list, index);
    if (!element_to_delete) {
        fprintf(stderr, "delete_at(): The element at [%ld] does not exist\n", index);
        return NULL;
    }

    Element_t *list_element;
    
    if (element_to_delete->prev) {
        element_to_delete->prev->next = element_to_delete->next;
        list_element = element_to_delete->prev;
    }
    if (element_to_delete->next) {
        element_to_delete->next->prev = element_to_delete->prev;
        list_element = element_to_delete->next;
    }
    
    free(element_to_delete);
   
    // Going back to first element
    while (list_element->prev) { list_element = list_element->prev; }
    return list_element;
}

void print_list_reverse(Element_t *list) {
     if (!list) {
        fprintf(stderr, "print_list_reverse(): Argument need not be NULL.\n");
        return;
    }

    // Going to the end
    while (list->next) { list = list->next; }

    int counter = 1;
    while (list) {
        printf("%i. ", counter);
        print_single_element(list);
        printf("\n");
        list = list->prev;
        counter++;
    }
}

void print_list_random(Element_t *list){
    long list_len = elements_count(list);

    // Initialise Digits
    long random_array[list_len];
    for (int i = 0; i < list_len; i++) {
        random_array[i] = i;
    } 

    // Own implementation of shuffle
    long first_index, second_index, placeholder;
    for (int i = 0; i < list_len; i++) { // shuffle dependant on listlength
        // Indices to swap
        first_index = rand() % list_len;
        second_index = rand() % list_len;
        
        // Swap
        placeholder = random_array[first_index];
        random_array[first_index] = random_array[second_index];
        random_array[second_index] = placeholder;
    }
    
    // Printing elements
    Element_t *current_element;
    long counter = 1;
    for (int i = 0; i < list_len; i++) {
        printf("%ld. ", counter);
        current_element = element_at(list, random_array[i]);
        print_single_element(current_element);
        printf("\n");
        counter++;
    } 
}

long compare_songtitles_larger(Element_t *A, Element_t *B) {
    // 1 if larger, 0 if not
    if (strcmp(A->songtitle, B->songtitle) > 0) {
        return 1;
    }
    return 0;
}

long compare_songtitles_smaller(Element_t *A, Element_t *B) {
    // 1 if smaller, 0 if not
    if (strcmp(A->songtitle, B->songtitle) < 0) {
        return 1;
    }
    return 0;
}

Element_t *sort_list(Element_t *list, element_compare fkt) {
    if (!list) {
        fprintf(stderr, "sort_list(): The list does not exist. \n");
        return NULL;
    }

    int changed; 
    Element_t *last_ptr = NULL;
    char buffer[MAX_INPUT_LEN];
    do {
        changed = 0; 
        while (list->prev) { list = list->prev; } // Go back to first element
        printf("#####\n");
        while (list->next != last_ptr) { 
            if (fkt(list, list->next)) {  
                // Only swapping values because swapping pointers too complicated
                strcpy(buffer, list->songtitle);
                strcpy(list->songtitle, list->next->songtitle);
                strcpy(list->next->songtitle, buffer);

                strcpy(buffer, list->interpreter);
                strcpy(list->interpreter, list->next->interpreter);
                strcpy(list->next->interpreter, buffer);
                printf("-------\n");
                print_entire_list(list);
                changed = 1;
            } 
            list = list->next; 
        }
        last_ptr = list;
    } while (changed);
    
    return list;
}

Element_t *unique_list(Element_t *list) {
    if (!list) {
        fprintf(stderr, "unique_list(): The list cannot be NULL\n");
        return NULL;
    }
    
    // Not really using linked lists as they should be used with
    // performance issues, but with small list lengths alright
    long list_length = elements_count(list);
    Element_t *original_element, *next_element;
    for (int i = 0; i < list_length - 1; i++) {
        original_element = element_at(list, i);
        for (int j = i + 1; j < list_length; j++) {
            next_element = element_at(list, j);
            if (strcmp(original_element->songtitle, next_element->songtitle) == 0) {
                if (strcmp(original_element->interpreter, next_element->interpreter) == 0) {
                    delete_at(list, j);
                    list_length--;
                }
            }
        }
    }
    return list;
}

Element_t *rotate_list(Element_t *list, long shift) {
    if (shift < 0) {
        fprintf(stderr, "rotate_list(): The shift must be > 0\n");
        return NULL;
    }
    Element_t *start = list;
    while (list->next) { list = list->next; } // Go to the last element
    
    // Linking last and first node
    start->prev = list;
    list->next = start;

    for (int i = 0; i < shift; i++)
    {
        start = start->next;
        list = list->next;
    }

    // Unlink the list
    start->prev = NULL;
    list->next = NULL;
    return start;
}
