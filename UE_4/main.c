#include <stdio.h>  
#include <stdlib.h>
#include "main.h"
#include "helpers.c"

#include "iueduma/duma.h"

int main()  
{
    // Duma Env Variable -> X% of memory allocations fail
    putenv("DUMA_MALLOC_ERROR=0");  

    heap_t* heap = heap_create(DEFAULT_SIZE);
    hash_t* hash = hash_create(DEFAULT_SIZE);

    while (1) {
        printf(
            "\n-------------------------------------------------\n"
            "0. Programm beenden\n"
            "1. Gesamten Heap loeschen\n"
            "2. Neuen Eintrag im Heap ablegen\n"
            "3. Heap ausgeben\n"
            "4. Eintrag im Heap suchen\n"
            "5. Ersten Eintrag aus dem Heap löschen\n"
            "6. Hash aus Heap erzeugen\n"
            "7. Gesamten Hash löschen\n"
            "8. Neuen Eintrag im Hash ablegen\n"
            "9. Hash ausgeben\n"
            "10. Eintrag im Hash suchen\n"
            "11. Eintrag aus dem Hash löschen\n"
            "12. Benchmark\n"
            "-------------------------------------------------\n"
            "\n"
        );

        long menu_point = read_value("Auswahl: ");

        switch(menu_point) {
            case 0: {
                if (heap) heap_free(heap);
                if (hash) hash_free(hash);
                return 0;
            };

            case 1: {
                if (heap) heap_free(heap);
                printf("The heap was deleted.\n");

                if (hash) hash_free(hash);
                printf("The hash was deleted.\n");

                heap = heap_create(DEFAULT_SIZE);
                hash = hash_create(DEFAULT_SIZE);
                break;
            }

            case 2: {
                if (!heap) {
                    printf("There is no heap.\n");
                }
                else {
                    char songtitle[MAX_LINE_LEN];
                    char interpreter[MAX_LINE_LEN];
                    read_string("Songtitle: ", songtitle);
                    read_string("Interpreter: ", interpreter);
                    
                    if (SUCCESS == heap_insert(heap, songtitle, interpreter)) {
                        printf("The element (%s, %s) could successfully be inserted.\n", songtitle, interpreter);
                    }
                    else {
                        printf("There was an issue inserting the new entry.\n");
                    }
                    break;
                }
                
                break;
            }

            case 3: {
                heap_print(heap);
                break;
            }

            case 4: {
                char songtitle[MAX_LINE_LEN];
                char interpreter[MAX_LINE_LEN];
                read_string("Songtitle: ", songtitle);
                read_string("Interpreter: ", interpreter);
                int index = heap_search(heap, songtitle, interpreter);
                
                if (index) {
                    printf("The element (%s, %s) is stored at index %i\n", songtitle, interpreter, index);
                }
                else {
                    printf("The element is not stored in the heap.\n");
                }
                
                break;
            }

            case 5: {
                printf("Deleting root of heap...\n");
                
                if (SUCCESS == heap_delete_first(heap)) {
                    printf("The root could be successfully deleted.\n");
                    heap_print(heap);
                }
                else {
                    printf("There was a problem deleting the root of the heap\n");
                }
                
                break;
            }

            case 6: {
                printf("Creating hash from heap...\n");
                hash_t *new_hash = convert_heap_to_hash(heap, DEFAULT_SIZE);

                if (new_hash) {
                    printf("The heap could be converted.\n");
                    hash_print(new_hash);
                    hash_free(hash);
                    hash = new_hash;
                }
                else {
                    printf("There was a problem with the conversion\n");
                }
                
                break;
            }

            case 7: {
                printf("Deleting hash...\n");
                hash_free(hash);
                printf("The hash was deleted.\n");
                break;
            }

            case 8: {
                char songtitle[MAX_LINE_LEN];
                char interpreter[MAX_LINE_LEN];
                read_string("Songtitle: ", songtitle);
                read_string("Interpreter: ", interpreter);

                hash_insert(hash, songtitle, interpreter, DEFAULT_SIZE);
                break;
            }

            case 9: {
                hash_print(hash);
                break;
            }

            case 10: {
                char songtitle[MAX_LINE_LEN];
                char interpreter[MAX_LINE_LEN];
                read_string("Songtitle: ", songtitle);
                read_string("Interpreter: ", interpreter);

                entry_t * found_entry = hash_search(hash, songtitle, interpreter, DEFAULT_SIZE);
                if (found_entry) {
                    printf("The entry could be found: %p (%s, %s)\n", found_entry, found_entry->songtitle, found_entry->interpreter);
                }
                else {
                    printf("The entry could not be found.\n");
                }
                
                break;
            }

            case 11: {
                char songtitle[MAX_LINE_LEN];
                char interpreter[MAX_LINE_LEN];
                read_string("Songtitle: ", songtitle);
                read_string("Interpreter: ", interpreter);

                if (hash_delete_entry(hash, songtitle, interpreter, DEFAULT_SIZE)) {
                    printf("The entry could be deleted.\n");
                }
                else {
                    printf("The entry could not be found.\n");
                }
                
                break;
            }

            case 12: {
                benchmark(DEFAULT_SIZE, DEFAULT_SIZE);
                
                break;
            }  
        }
    }
}   
