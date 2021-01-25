#include <stdio.h>  
#include <stdlib.h>
#include "main.h"
#include "helpers.c"

#include "iueduma/duma.h"

int main()  
{
    // Duma Env Variable -> X% of memory allocations fail
    //putenv("DUMA_MALLOC_ERROR=0");  

    node_t* rootnode = NULL;

    while (1) {
        printf(
            "\n-------------------------------------------------\n"
            "0. Programm beenden\n"
            "1. Gesamten Baum loeschen\n"
            "2. Neuen Knoten von der Tastatur einlesen und im Baum ablegen\n"
            "3. Baum ausgeben\n"
            "4. Anzahl der Knoten bestimmen\n"
            "5. Ersten Knoten mit Songtitle suchen\n"
            "6. Alle Knoten mit Interpreter suchen\n"
            "7. Maximale Tiefe des Baumes ausgeben\n"
            "8. Ebene x des Baumes ausgeben\n"
            "9. Bestimmten Knoten löschen\n"
            "10. Average Comparisons\n"
            "11. Check if order\n"
            "12. Baum ausbalancieren\n"
            "13. Baum ausbalancieren A bis Z \n"
            "14. Benchmark\n"
            "-------------------------------------------------\n"
            "\n"
        );

        long menu_point = read_value("Auswahl");

        switch(menu_point) {
            case 0: {
                 // Destroy Tree
                if (rootnode) {
                    destroy_tree(rootnode);
                }
                
                return 0;
            };

            case 1: {
                 // Destroy Tree
                if (!rootnode) {
                    printf("The tree is empty.\n");
                    break;
                }

                destroy_tree(rootnode);
                rootnode = NULL;

                printf("The tree was successfully destroyed. \n");
                break;
            };

            case 2: {
                // Add node
                char songtitle[MAX_INPUT_LEN];
                char interpreter[MAX_INPUT_LEN];
                read_string("Songtitle", songtitle);
                read_string("Interpreter", interpreter);
                
                if (!rootnode) {
                    rootnode = create_node(songtitle, interpreter);
                }
                else {
                    insert_node(rootnode, songtitle, interpreter);
                }

                printf("The node was successfully added. \n");
                break;
            };

            case 3: {
                // Print Tree
                if (!rootnode) {
                    printf("The tree is empty.\n");
                    break;
                }
                
                print_tree(rootnode);
                break;
            };

            case 4: {
                // Count Nodes
                long node_count = count_nodes(rootnode);
                printf("There are %ld nodes in this tree \n", node_count);
                break;
            };

            case 5: {
                // Search Nodes
                char songtitle[MAX_INPUT_LEN];
                read_string("Songtitle", songtitle);

                node_t *node = search_node(rootnode, songtitle);
                printf("The node %p : (%s, %s) could be found. \n", node, node->interpreter, node->songtitle);
                break;
            };

             case 6: {
                // Search all Nodes
                char interpreter[MAX_INPUT_LEN];
                read_string("Interpreter", interpreter);
                long found = 0;

                node_t **nodes = search_all(rootnode, interpreter, &found);

                if (!nodes) {
                    printf("The interpreter could not be found in the database.\n");
                    break;
                }

                node_t *node;
                printf("The following nodes were found: \n");
                for (int i = 0; i < found; i++) {
                    node = nodes[i];
                    printf("%p : (%s, %s) \n", node, node->interpreter, node->songtitle);
                    free(node);
                    node = NULL;
                }

                free(nodes);
                nodes = NULL;
                
                break;
            };

            case 7: {
                // Getting maximum depth
                long maximum_depth = max_depth(rootnode);
                printf("The maximum depth is %ld nodes\n", maximum_depth);
                break;
            };

            case 8: {
                // Printing the level
                long level = read_value("Level");
                print_level(rootnode, level);
                break;
            };

            case 9: {
                // Delete node
                char songtitle[MAX_INPUT_LEN];
                char interpreter[MAX_INPUT_LEN];
                read_string("Songtitle", songtitle);
                read_string("Interpreter", interpreter);
                
                if (!rootnode) {
                    printf("The tree is empty.\n");
                    break;
                }
                else {
                    node_t* result_node = delete_node(rootnode, songtitle, interpreter);
                    if (result_node) {
                        printf("The node was successfully deleted. \n");
                        rootnode = result_node;
                    }
                    else{
                        printf("The node could not be found.\n");
                    }
                }

                
                break;
            };

            case 10: {
                // Check if order OK
                if (!rootnode) {
                    printf("The tree is empty. \n");
                    break;
                }

                printf("Average Comparisons in current tree: %.2f\n", average_comparisons(rootnode));

                break;
            };
            
            case 11: {
                // Check if order OK
                if (!rootnode) {
                    printf("The tree is empty. \n");
                    break;
                }

                if (check_if_order_OK(rootnode)) {
                    printf("The order is ok.\n");
                }
                else {
                    printf("The order is not ok.\n");
                }

                break;
            };

            case 12: {
                // Check if order OK
                if (!rootnode) {
                    printf("The tree is empty. \n");
                    break;
                }
                rootnode = rebalance(rootnode);
                printf("The Binary Search Tree was rebalanced.\n");

                break;
            };

            case 13: {
                // Check if order OK                
                // Creating tree
                node_t *new_root = create_node("A", "A");
                char titles[25];
                for (int i = 0; i < 25; i++) {
                    titles[i] = 'B' + i;
                    insert_node(new_root, &titles[i], &titles[i]);
                }

                printf("Average comparisons unbalanced: %.2f\n", average_comparisons(new_root));

                new_root = rebalance(new_root);

                printf("Average comparisons balanced: %.2f\n", average_comparisons(new_root));
                
                destroy_tree(new_root);

                break;
            };

            case 14: {
                benchmark();
                break;
            };
        }
    }
}   
