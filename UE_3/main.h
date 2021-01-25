#include <time.h>

#define MAX_LINE_LEN 256
#define MAX_INPUT_LEN 256
#define MAX_ELEMENTS 30

#define START_TIMING clock_t timeStart = clock();
#define STOP_TIMING  (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC


typedef struct node_s {
    char songtitle[256], interpreter[256];
    struct node_s *parent, *left, *right;
} node_t;

// Teil A
node_t *create_node(char songtitle[], char interpreter[]);
node_t *insert_node(node_t *rootnode, char songtitle[], char interpreter[]);

// Teil B
void print_tree(node_t *rootnode);
void destroy_tree(node_t *rootnode);

// Teil C
long count_nodes(node_t *rootnode);
node_t *search_node(node_t *rootnode, char songtitle[]);

// Teil D
node_t **search_all(node_t *rootnode, char interpreter[], long *found);

// Teil E
long max_depth(node_t *rootnode);
void print_level(node_t *rootnode, long level);

// Teil F
node_t *delete_node(node_t *rootnode, char songtitle[], char interpreter[]);

// Teil G
double average_comparisons(node_t *rootnode);
void benchmark();

// Teil H
long check_if_order_OK(node_t *rootnode);
node_t *rebalance(node_t *rootnode);
