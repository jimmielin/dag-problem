#include <stdint.h>

void init_graph(uint32_t graph[], int n_vertices);
int read_graph(const char *filename, uint32_t *graph[], int *n_vertices);
int write_graph(const char *filename, uint32_t graph[], int n_vertices);
int add_edge(uint32_t graph[], int n_vertices, int a, int b);
int remove_edge(uint32_t graph[], int n_vertices, int a, int b);
void print_graph(uint32_t graph[], int n_vertices);
