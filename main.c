#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "graph_utils.h"

/*
 * Forward declarations of is_dag
 * See function definition further down for documentation
 */
int is_dag(uint32_t digraph[], int n_vertices);

/*
 * Forward declaration of explore_node
 */
void explore_node(uint32_t digraph[], int n_vertices,
                      int current_node,
                      uint32_t *gray_vertices, uint32_t *black_vertices,
                      uint32_t map_dest[]);

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
    /* int vd; */
    uint32_t gray_vertices, black_vertices; /* unexplored nodes */
    uint32_t map_dest[n_vertices+1]; /* possible destination nodes from source node with flag for fully traversed */
    for(v = 0; v < n_vertices; v++) {
        map_dest[v] = digraph[v];
    }
    map_dest[n_vertices] = 0;

    for(v = 0; v < n_vertices; v++) {
        gray_vertices = 0;
        black_vertices = 0;

        /* loop onto itself? if yes, is not a dag */
        if(map_dest[v] & (1 << v)) return 0;

        /*
         * issue recursive lookup.
         * even though gray/black vertices appear to be reset at each top-level iteration,
         * the intent is (1) explicitly map through all top-level nodes to find one that loops onto itself,
         * and (2) leave future expansion for map_dest to record possible destinations from each node.
         * if the intent is to obtain map_dest, then after one top-level iteration, map_dest[vv] could be
         * populated using deeper recursive calls in a lazy manner. note that the check for loop-onto-itself
         * will short-circuit this top-level loop as soon as not-a-dag, so map_dest is left incomplete.
         * if the intent is to obtain the looped path, then another reference could be carried to record
         * the paths corresponding to each map_dest[v] (or just locate the loop and backtrack, in which case
         * the top level node # can be always passed in.)
         */
        explore_node(digraph, n_vertices, v, &gray_vertices, &black_vertices, map_dest);

        /* printf("after recursion: node %u has targets %u\n", v, map_dest[v]);
        for(vd = 0; vd < n_vertices; vd++) {
            printf("=> dbg: node %u has targets %u\n", vd, map_dest[vd]);
        } */

        /* loop onto itself? if yes, is not a dag */
        if(map_dest[v] & (1 << v)) return 0;
    }

    return 1;
}

/********************************************************************************
 *
 * explore_node
 * Recursive explore further node in digraph, tweaked to create a map of destinations from
 * given top-level root node. Subsequent recursive depth-first calls will also update
 * all map_dest of the previously traversed (black) nodes using the result.
 *
 * Arguments:
 *     digraph                  uint32_t[]
 *     n_vertices               int
 *     current_node             int
 *     gray_vertices            uint32_t       queue of unexplored nodes (inout)
 *     black_vertices           uint32_t       list of explored nodes (inout)
 *     map_dest                 uint32_t[]     matrix of all connected nodes from given node (inout) with flag for fully
 */
void explore_node(uint32_t digraph[], int n_vertices,
                      int current_node,
                      uint32_t *gray_vertices, uint32_t *black_vertices,
                      uint32_t map_dest[]) {

    /** map through queue and issue recursive explore_node **/
    int vv;

    /* if already populated return */
    if(map_dest[n_vertices] & (1 << current_node)) {
        /* printf("===> skip %u (trav = %u)\n", current_node, map_dest[n_vertices]); */ return;
    }

    /* mark this node as explored */
    *black_vertices = *black_vertices | (1 << current_node);

    /* add current map to queue, removing the current_node from queue */
    *gray_vertices = (*gray_vertices | digraph[current_node]) & ~(1 << current_node);

    /* nodes that this node can lead to are nodes that map_dest should contain. */
    map_dest[current_node] = map_dest[current_node] | digraph[current_node];

    /* printf("explore_node: current_node = %u (gray = %u, black = %u, cmap = %u)\n", current_node, *gray_vertices, *black_vertices, map_dest); */

    /** recurse deeper in the queue. **/
    for(vv = 0; vv < n_vertices; vv++) {
        if(*gray_vertices & (1 << vv)) { /* in queue */
            /*
             * if already explored, do not explore this node and add to map_dest
             * this is also a sign of a loop so if map_dest is not desired, then the execution
             * of is_dag can be terminated here.
             */
            if(*black_vertices & (1 << vv)) {
                map_dest[current_node] = map_dest[current_node] | (1 << vv);
                *gray_vertices = *gray_vertices & ~(1 << current_node);
            }
            else {
                explore_node(digraph, n_vertices, vv, gray_vertices, black_vertices, map_dest);
            }
            map_dest[n_vertices] = map_dest[n_vertices] | (1 << vv);

            /** if current_node can reach vv, then update curr with vv. **/
            if(map_dest[current_node] & (1 << vv)) {
                /* printf("...updating %u with map_dest of %u (%u)\n", current_node, vv, map_dest[vv]); */
                map_dest[current_node] = map_dest[current_node] | map_dest[vv];
                map_dest[n_vertices] = map_dest[n_vertices] | (1 << current_node);
            }
        }
    }
}