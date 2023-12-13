#include <stdio.h>
#include <stdlib.h>
#include "graph_utils.h"

/********************************************************************************
 *
 * init_graph
 *
 * Initializes a graph so that it contains no edges
 *
 ********************************************************************************/
void init_graph(uint32_t graph[], int n_vertices)
{
	int i;

	for (i = 0; i < n_vertices; i++) {
		graph[i] = 0;
	}
}

/********************************************************************************
 *
 * read_graph
 *
 * Reads a graph from a file
 *
 * If the graph is successfully read, a value of 0 is returned, the value of
 * n_vertices is set to the number of vertices in the graph, and the graph array
 * has been allocated with n_vertices elements, with each element containing the
 * adjacency information for its corresponding vertex in the graph. Otherwise, a
 * value of 1 is returned
 *
 ********************************************************************************/
int read_graph(const char *filename, uint32_t *graph[], int *n_vertices)
{
	FILE *f;
	int i, j;

	if ((f = fopen(filename, "r")) == NULL) {
		return 1;
	}

	fscanf(f, "%d", n_vertices);
	*graph = malloc(sizeof(uint32_t) * (size_t)(*n_vertices));
	for (i = 0; i < *n_vertices; i++) {
		for (j = 0; j < 4; j++) {
			int b;
			fscanf(f, "%2x", &b);
			(*graph)[i] <<= 8;
			(*graph)[i] |= (uint32_t)b;
		}
	}

	fclose(f);

	return 0;
}

/********************************************************************************
 *
 * write_graph
 *
 * Writes a graph to a file
 *
 * The given graph is written in a compact format to the specified file. If
 * successful, a value of 0 is returned.
 *
 * The output file is plain text and contains the number of vertices in the
 * graph on a single line, followed by a hexadecimal representation -- in
 * big-endian byte order -- of the bit array adjacency lists for each of the
 * vertices in the graph.
 *
 ********************************************************************************/
int write_graph(const char *filename, uint32_t graph[], int n_vertices)
{
	FILE *f;
	int i, j;

	if ((f = fopen(filename, "w")) == NULL) {
		return 1;
	}

	fprintf(f, "%d\n", n_vertices);
	for (i = 0; i < n_vertices; i++) {
		if (i > 0) fprintf(f, " ");
		for (j = 24; j >= 0; j -= 8) {
			fprintf(f, "%02x", (graph[i] >> j) & 0xff);
		}
	}
	fprintf(f, "\n");

	fclose(f);

	return 0;
}

/********************************************************************************
 *
 * add_edge
 *
 * Adds the directed edge (a,b) to the given graph. If successful, a value of 0
 * is returne.
 *
 ********************************************************************************/
int add_edge(uint32_t graph[], int n_vertices, int a, int b)
{
	if (a < 0 || a >= n_vertices ||
	    b < 0 || b >= n_vertices) {
		return 1;	
	}

	graph[a] |= (1 << b);

	return 0;
}

/********************************************************************************
 *
 * remove_edge
 *
 * Removes the directed edge (a,b) from the given graph. If successful, a value
 * of 0 is returned.
 *
 ********************************************************************************/
int remove_edge(uint32_t graph[], int n_vertices, int a, int b)
{
	if (a < 0 || a >= n_vertices ||
	    b < 0 || b >= n_vertices) {
		return 1;	
	}

	graph[a] &= ~(1 << b);

	return 0;
}

/********************************************************************************
 *
 * print_graph
 *
 * Prints a human-readable representation of a graph
 *
 * Given a directed graph and the number of vertices in the graph, a
 * human-readable (and 'dot'-compatible) representation of the graph is printed
 * to standard output.
 *
 ********************************************************************************/
void print_graph(uint32_t graph[], int n_vertices)
{
	int i, j;

	printf("digraph {\n");
	for (i = 0; i < n_vertices; i++) {
		if (graph[i]) {
			int need_space = 0;
			printf("%d -> {", i);
			for (j = 0; j < n_vertices; j++) {
				if (graph[i] & (1 << j)) {
					if (need_space) printf(" ");
					printf("%d", j);
					need_space = 1;
				}
			}
			printf("}\n");
		}
	}
	printf("}\n");
}
