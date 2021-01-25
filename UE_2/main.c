#include <stdio.h>  
#include <stdlib.h>
#include "main.h"
#include "helpers.c"

#include "iueduma/duma.h"

int main()  
{
    // Duma Env Variable -> X% of memory allocations fail
    putenv("DUMA_MALLOC_ERROR=0");  

    Element_t* list = NULL;

    while (1) {
        printf(
            "\n-------------------------------------------------\n"
            "0  - Programm beenden\n"
            "1  - Neuen Eintrag einlesen und am Ende einfügen\n"
            "2  - Liste ausgeben\n"
            "3  - Liste löschen\n"
            "4  - Anzahl der Elemente ausgeben\n"
            "5  - Element an der Stelle x ausgeben\n"
            "6  - Neuen Eintrag einlesen und an der Stelle x einfügen\n"
            "7  - Eintrag an der Stelle x löschen\n"
            "8  - Liste in umgekehrter Reihenfolge ausgeben\n"
            "9  - Liste in zufälliger Reihenfolge ausgeben\n"
            "10 - Liste aufsteigend nach songtitle sortieren\n"
            "11 - Liste absteigend nach songtitle sortieren\n"
            "12 - Duplikate aus Liste entfernen\n"
            "13 - Liste um x rotieren\n"
            "-------------------------------------------------\n"
            "\n"
        );

        long menu_point = read_value("Auswahl");

        switch(menu_point) {
            case 0: {
                if (!list) {
                    return 0;
                }
                free_list(&list);
                return 0;
            };
            case 1: {
                if (list) {
                    insert_last(list);
                }
                else {
                    list = allocate_element();
                }
                 break;
            }
            case 2: {
                if (list) {
                    print_entire_list(list); 
                }
                else {
                    printf("There are no elements in the list. \n");
                }
                break;
            }
            case 3: {
                if (list) {
                    free_list(&list); 
                    printf("Die Liste wurde gelöscht.\n");
                }
                else {
                    printf("There are no elements in the list. \n");
                }
                break;
            }
            case 4: {
                if (list) {
                     printf("The number of elements in the list: %ld\n", elements_count(list));
                }
                else {
                    printf("There are no elements in the list. \n");
                }
                break;
            }
            case 5: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }
                long index = read_value("Index");
                Element_t *current_node = element_at(list, index);

                if (current_node) {
                    print_single_element(current_node);
                }
                else {
                    printf("The index you gave was out of range\n");
                }
                
                break;
            }
            case 6: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }
                long index = read_value("Index");
                Element_t *current_node = insert_at(list, index);

                if (current_node) {
                    printf("The element was inserted at %ld\n", index);
                    list = current_node;
                }
                else {
                    printf("There was a problem inserting the node\n");
                }
                
                break;
            }
            case 7: {

                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }

                long index = read_value("Index");
                Element_t *current_node = delete_at(list, index);
                if (current_node) {
                    printf("The element at [%ld] was deleted.\n", index);
                    list = current_node;
                }
                else {
                    printf("The index you gave was out of range\n");
                }
                
                break;
            }
            case 8: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }

                print_list_reverse(list);
                
                break;
            }
            case 9: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }
                
                print_list_random(list);

                break;
            }
            case 10: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }

                sort_list(list, compare_songtitles_larger);
                break;
            }
            case 11: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }
                
                sort_list(list, compare_songtitles_smaller);
                break;
            }
            case 12: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }
                unique_list(list);
                break;
            }
            case 13: {
                if (!list) {
                    printf("There are no elements in the list \n");
                    break;
                }

                long shift = read_value("Shift");
                Element_t *current_node = rotate_list(list, shift);
                if (current_node) {
                    printf("The list was rotated by %ld.\n", shift);
                    list = current_node;
                }
                else {
                    printf("There was a problem rotating the list\n");
                }

                break;
            }

        }
    }
}   
