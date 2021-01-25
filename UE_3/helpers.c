#include <string.h>
#include "time.h"
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

node_t *create_node(char songtitle[], char interpreter[]) {
    node_t* new_node = malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "create_node(): There was an issue allocating the memory for a new node.\n");
        return NULL;
    }

    strcpy(new_node->songtitle, songtitle);
    strcpy(new_node->interpreter, interpreter);

    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

node_t *insert_node(node_t *rootnode, char songtitle[], char interpreter[]) {
    node_t *new_node = create_node(songtitle, interpreter);
    node_t *last_node = rootnode;

    if (!new_node) {
        fprintf(stderr, "insert_node(): There was an issue creating a new node \n");
        return NULL;
    }
    else if (!rootnode) {
        fprintf(stderr, "insert_node(): The rootnode cannot be NULL \n");
        return NULL;
    }

    while (last_node) {        
        if (strcmp(last_node->songtitle, songtitle) > -1) {
            // if already leaf we insert the node
            if (!last_node->left) {
                last_node->left = new_node;
                new_node->parent = last_node;
                return new_node;
            }

            last_node = last_node->left;
        }
        else {
            if (!last_node->right) {
                last_node->right = new_node;
                new_node->parent = last_node;
                return new_node;
            }
            last_node = last_node->right;
        }
    }

    return NULL;
}

void print_tree(node_t *rootnode) {
    // Not really performant, but for small trees ok
    
    long depth = max_depth(rootnode);
    for (int i = 0; i < depth; i++) {
        // Indentation (maybe logarithmic spacing so better looking later)
        for (int j = 0; j < depth-i; j++){
            printf("     ");
        }
         
        print_level(rootnode, i);
        printf("\n");
    }
}

void destroy_tree(node_t *rootnode) {
    if (rootnode){
        destroy_tree(rootnode->left);
        destroy_tree(rootnode->right);

        free(rootnode);
    }
}

long count_nodes(node_t *rootnode) {
    long count = 1; 

    if (rootnode->left) {
        count += count_nodes(rootnode->left);
    }
    if (rootnode->right) {
        count += count_nodes(rootnode->right);
    }
    return count;
}

node_t *search_node(node_t *rootnode, char songtitle[]) {
    if (!rootnode) {
        fprintf(stderr, "search_node(): The rootnode cannot be NULL \n");
        return NULL;
    }

    node_t *last_node = rootnode;
    while (last_node) {
        if (strcmp(last_node->songtitle, songtitle) == 0) {
            return last_node;
        }     
        if (strcmp(last_node->songtitle, songtitle) > -1) {
            last_node = last_node->left;
        }
        else {
            last_node = last_node->right;
        }
    }

    return NULL;
}

void search_all_recursively(node_t *rootnode, char interpreter[], node_t **found_nodes, long *found) {
    if (!rootnode) {
        return;
    }

    if (strcmp(rootnode->interpreter, interpreter) == 0) {
        found_nodes[*found] = rootnode;
        *found += 1;
    }

    search_all_recursively(rootnode->left, interpreter, found_nodes, found);
    search_all_recursively(rootnode->right, interpreter, found_nodes, found);

}

node_t **search_all(node_t *rootnode, char interpreter[], long *found) {
    node_t **found_nodes = malloc(sizeof(node_t) * MAX_ELEMENTS);
    if (!found_nodes) {
        fprintf(stderr, "search_all(): There was an issue creating the node array \n");
        return NULL;
    }
    
    search_all_recursively(rootnode, interpreter, found_nodes, found);

    return found_nodes;
}

long max_depth(node_t *rootnode) {
    if (!rootnode) {
        return 0;
    }
    else if (!rootnode->left && !rootnode->right) {
        return 1;
    }
    else {
        long left_depth = max_depth(rootnode->left);
        long right_depth = max_depth(rootnode->right);
        
        return (left_depth > right_depth) ? left_depth + 1 : right_depth + 1;
    }
}

void print_level(node_t *rootnode, long level) {
    if (!rootnode) {
        for (int i = 0; i < level + 1; i++) {
            printf(" (NULL) ");
        }
        return;
    }
    if (level == 0) {
        printf(" (%s, %s) ", rootnode->interpreter, rootnode->songtitle);
    }
    else {
        print_level(rootnode->left, level-1);
        print_level(rootnode->right, level-1);
    }
}

node_t *get_minimum_key_node(node_t *rootnode) {
    // getting minimum value by going left because of tree structure
    while (rootnode->left) {
        rootnode = rootnode->left;
    }
    return rootnode;
}

node_t *delete_node(node_t *rootnode, char songtitle[], char interpreter[]) {
    if (!rootnode) {
        fprintf(stderr, "delete_node(): The rootnode cannot be NULL \n");
        return NULL;
    }

    long found = 0;
    node_t **nodes_of_interpreter = search_all(rootnode, interpreter, &found);

    node_t *current_node, *parent_node;
    for (int i = 0; i < found; i++) {
        current_node = nodes_of_interpreter[i];
        printf("current   %p (%s, %s) \n", current_node, current_node->interpreter, current_node->songtitle);
        
        if (strcmp(current_node->songtitle, songtitle) == 0) {
            // Delete tree with only one entry
            if (current_node == rootnode) {
                break;
            }

            // node has no children
            if (!current_node->left && !current_node->right) {
                printf("delete node with no children:   %p (%s, %s) \n", current_node, current_node->interpreter, current_node->songtitle);
                printf("parent: %p  \n", current_node->parent);

                // Set child of parent to NULL
                if (current_node->parent->left == current_node) {
                    printf("first if");

                    current_node->parent->left = NULL;
                }
                else {
                    printf("second if");
                    current_node->parent->right = NULL;
                }

                printf("before free ");
                free(current_node);
                current_node = NULL;
            }

            // node has two children
            else if (current_node->left && current_node->right) {
                // replacing node with smallest value above it (leftmost leaf in right tree)
                node_t *successor = get_minimum_key_node(current_node->right);
                printf("successor %p (%s, %s) ", successor, successor->interpreter, successor->songtitle);
                
                // Putting successor data in node
                strcpy(current_node->songtitle, successor->songtitle);
                strcpy(current_node->interpreter, successor->interpreter);

                delete_node(successor, successor->songtitle, successor->interpreter);
            }
            
            // node has one child
            else {
                printf("delete node with one child %p (%s, %s) \n", current_node, current_node->interpreter, current_node->songtitle);
                node_t *child_node = (current_node->left) ? current_node->left : current_node->right;
                child_node->parent = current_node->parent;

                if (current_node->parent->left == current_node) {
                    current_node->parent->left = child_node;
                }
                else {
                    current_node->parent->right = child_node;
                }
                
                free(current_node);
                current_node = NULL;
            }

            // Free the rest of the nodes
            for (int j = i+1; j < found; j++) {
                if (nodes_of_interpreter[j]) {
                    free(nodes_of_interpreter[j]);
                }
            }
            free(nodes_of_interpreter);
            return rootnode;
        }
        else {
            if (current_node) {
                free(current_node);
            }
        }
    }

    free(nodes_of_interpreter);
    return NULL;
}

double get_weighted_depth(node_t *rootnode, long depth) {
    // To reach a level N we need N comparisons
    // We add together these N comparisons for every node in the graph
    if (!rootnode) {
        return 0;
    }
    else if (!rootnode->left && !rootnode->right) {
        // for every leaf we add the current depth
        return depth;
    }
    else {
        double left_comparisons = get_weighted_depth(rootnode->left, depth + 1);
        double right_comparisons = get_weighted_depth(rootnode->right, depth + 1);
        
        // we add current depth plus depths of underlying nodes
        return depth + left_comparisons + right_comparisons;
    }
}

double average_comparisons(node_t *rootnode) {
    long start_depth = 1;

    return get_weighted_depth(rootnode, start_depth) / count_nodes(rootnode);
}

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

void benchmark() {
    node_t* rootnode = create_node("ROOT", "ROOT");
    long N = 1000;
    long times_of_tries = 5; // N = (100, 200, ... 1000)
    long string_length = 10;

    char *list_of_songs[N * times_of_tries];
    char *list_of_interpreters[N * times_of_tries];

    clock_t timeStart;
    double passed_time;

    for (int i = 0; i < N * times_of_tries; i++) {
        list_of_songs[i] = get_random_string(string_length);
        list_of_interpreters[i] = get_random_string(string_length);
    }

    for (int try = 1; try < times_of_tries+1; try++) {
        printf("Inserting into a tree of size \t\t%ld: \n", try * N);

        timeStart = clock();
        for (int i = 0; i < try * N; i++) {
            insert_node(rootnode, list_of_songs[i], list_of_interpreters[i]);
        }
        passed_time = (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC;

        printf("Time for insert of %ld nodes:\t\t\t\t\t %.2f \n", try * N, passed_time);


        timeStart = clock();
        for (int i = 0; i < try * N; i++) {
            search_node(rootnode, list_of_songs[i]);
        }
        passed_time = (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC;

        double comparisons = average_comparisons(rootnode);
        
        printf("Average time for search of %ld nodes:\t\t\t\t %.2f \n", try * N, passed_time);
        printf("Average time for comparison during search of %ld nodes:\t %.2f \n", try * N, passed_time/comparisons);

        // Freeing memory
        destroy_tree(rootnode);
        rootnode = create_node("ROOT", "ROOT");

        printf("\n");
    }

    // Freeing data
    for (int i = 0; i < times_of_tries * N; i++) {
        free(list_of_songs[i]);
        free(list_of_interpreters[i]);
    }
    free(rootnode);

    // Ca log(N) verlauf => skaliert mit der Hoehe des Baumes die log(2) verlauf ist
    
}

long check_if_order_OK(node_t *rootnode) {
    // Compares songtitles because same logic in insert() function
    if (rootnode->left) {
        if (strcmp(rootnode->left->songtitle, rootnode->songtitle) < 1) {
            return check_if_order_OK(rootnode->left);
        }
        else{
            return 0;
        }
    }
    else if (rootnode->right) {
        if (strcmp(rootnode->right->songtitle, rootnode->songtitle) > 0) {
            return check_if_order_OK(rootnode->right);
        }
        else{
            return 0;
        }
    }

    return 1;
}

node_t *build_tree(node_t **array_of_nodes, node_t* parent_node, int lower, int upper) {
    // Rootnode is middle of array
    // Then slices array into halves while recursively adding them to the 
    // rootnodes left and right branches

    long size = upper - lower + 1;
    long middle = lower + size / 2;

    if (size <= 0) {
        return NULL;
    }

    node_t *new_node = create_node(array_of_nodes[middle]->songtitle, array_of_nodes[middle]->interpreter);
    new_node->parent = parent_node;
    new_node->left = build_tree(array_of_nodes, new_node, lower, middle - 1);
    new_node->right = build_tree(array_of_nodes, new_node, middle + 1, upper);

    free(array_of_nodes[middle]);

    return new_node;
}

void save_tree_to_ordered_list(node_t* rootnode, node_t **array_of_nodes, long *index) {
    // saves nodes of BST with root rootnode in order into array_of_nodes
    if (!rootnode) {
        return;
    }

    save_tree_to_ordered_list(rootnode->left, array_of_nodes, index);

    array_of_nodes[*index] = rootnode;
    *index = *index +1;

    save_tree_to_ordered_list(rootnode->right, array_of_nodes, index);
}

node_t *rebalance(node_t *rootnode) {
    long number_of_nodes = count_nodes(rootnode);
    node_t *array_of_nodes[number_of_nodes];

    // Makes array out of BST in order to build the balanced tree
    long index = 0;
    save_tree_to_ordered_list(rootnode, array_of_nodes, &index);

    // Recursively adds node to root in a sorted manner
    return build_tree(array_of_nodes, rootnode, 0, number_of_nodes-1);
}
