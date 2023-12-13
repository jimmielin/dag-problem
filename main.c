#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "graph_utils.h"

/*
 * Forward declarations of is_dag
 * See function definition further down for documentation
 */
int is_dag(uint32_t digraph[], int n_vertices);

uint32_t bfs_explore_node(uint32_t digraph[], int n_vertices,
					  int current_node,
	                  uint32_t *gray_vertices, uint32_t *black_vertices,
	                  uint32_t current_map);

int main(int argc, const char **argv)
{
	int i;

	if (argc <= 1) {
		fprintf(stderr, "\nUsage: is_dag <digraph file>+\n\n");
		return 1;
	}

	for (i = 1; i < argc; i++) {
		uint32_t *digraph;
		int n_vertices;

		if (read_graph(argv[i], &digraph, &n_vertices)) {
			fprintf(stderr, "Error reading digraph from file %s\n", argv[i]);
			return 1;
		}

		/* hplin debug 12/12/23
		print_graph(digraph, n_vertices); */

		printf("%s is a dag? %u\n", argv[i], is_dag(digraph, n_vertices));

		free(digraph);
	}

	return 0;
}


/********************************************************************************
 *
 * is_dag
 *
 * Determines whether the given digraph is a dag
 *
 * Given a directed graph and the number of vertices in the graph, returns 1 if
 * the digraph is a dag and 0 otherwise.
 *
 * The digraph is represented as an array of adjacency lists, and each adjacency
 * list is stored as a uint32_t that may be interpreted as a bit-array
 * representation of an adjacency list, with a 1 in bit position i indicates
 * that there is a directed edge to vertex i. Since each adjacency list (i.e.,
 * each uint32_t value of the array) can only represent out edges to vertices 0
 * through 31, the digraph can have at most 32 vertices.
 *
 ********************************************************************************/
int is_dag(uint32_t digraph[], int n_vertices)
{
	int v;
	uint32_t gray_vertices, black_vertices; /* unexplored nodes */
	uint32_t map_dest[n_vertices]; /* possible destination nodes from source node */

	for(v = 0; v < n_vertices; v++) {
		map_dest[v] = digraph[v];
		gray_vertices = 0;
		black_vertices = 0;

		/* issue recursive lookup */
		map_dest[v] = bfs_explore_node(digraph, n_vertices, v, &gray_vertices, &black_vertices, map_dest[v]);

		/* printf("after recursion: node %u has targets %u\n", v, map_dest[v]); */

		/* loop onto itself? if yes, is not a dag */
		if(map_dest[v] & (1 << v)) return 0;
	}

	return 1;
}

/********************************************************************************
 *
 * bfs_explore_node
 * Explores further node in digraph
 * Arguments:
 * 	   digraph					uint32_t[]
 *     n_vertices			    int
 *     current_node 			int
 *     gray_vertices 			uint32_t 			queue of unexplored nodes (inout)
 * 	   black_vertices 			uint32_t	 		list of explored nodes (inout)
 *     current_map				uint32_t	 		map of top-level node's connected nodes (out)
 *	Return: current_map 		uint32_t		... to continue recursion
 */
uint32_t bfs_explore_node(uint32_t digraph[], int n_vertices,
					  int current_node,
	                  uint32_t *gray_vertices, uint32_t *black_vertices,
	                  uint32_t current_map) {

	/** breadth-first: map through queue and issue recursive bfs_explore_node **/
	int vv;

	/* mark this node as explored */
	*black_vertices = *black_vertices | (1 << current_node);

	/* add current map to queue, removing the current_node from queue */
	*gray_vertices = (*gray_vertices | digraph[current_node]) & ~(1 << current_node);

	/* nodes that this node can lead to are nodes that current_map should contain. */
	current_map = current_map | digraph[current_node];

	/* printf("bfs_explore_node: current_node = %u (gray = %u, black = %u, cmap = %u)\n", current_node, *gray_vertices, *black_vertices, current_map); */

	/** recurse deeper in the queue. **/
	for(vv = 0; vv < n_vertices; vv++) {
		if(*gray_vertices & (1 << vv)) { /* in queue */
			/* if already explored, do not explore this node and add to current_map */
			if(*black_vertices & (1 << vv)) {
				current_map = current_map | (1 << vv);
				*gray_vertices = *gray_vertices & ~(1 << current_node);
			}
			else {
				current_map = bfs_explore_node(digraph, n_vertices, vv, gray_vertices, black_vertices, current_map);
			}
		}
	}

	return current_map;
}