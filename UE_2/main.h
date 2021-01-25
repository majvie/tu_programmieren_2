#define MAX_LINE_LEN 256
#define MAX_INPUT_LEN 256


typedef struct Element_s {
    char songtitle[MAX_INPUT_LEN], interpreter[MAX_INPUT_LEN];
    struct Element_s *next, *prev;
} Element_t;

// A
void read_from_keyboard(Element_t *item);
Element_t *allocate_element();
Element_t *insert_last(Element_t *list);
void free_list(Element_t **list);
void print_single_element(Element_t *element);
void print_entire_list(Element_t *list);


// B
long elements_count(Element_t *list);
Element_t *element_at(Element_t *list, long index);

// C
Element_t *insert_at(Element_t *list, long index);

// D
Element_t *delete_at(Element_t *list, long index);

// E
void print_list_reverse(Element_t *list);
void print_list_random(Element_t *list);

// F
typedef long (*element_compare) (Element_t *, Element_t *);
long compare_songtitles_larger(Element_t *A, Element_t *B);
long compare_songtitles_smaller(Element_t *A, Element_t *B);
Element_t *sort_list(Element_t *list, element_compare fkt);

// G
Element_t *unique_list(Element_t *list);

// H
Element_t *rotate_list(Element_t *list, long shift);