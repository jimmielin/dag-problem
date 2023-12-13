# dag-problem
An exercise in detecting cycles in digraphs

-----

Let $G = (V,E)$ be a directed graph (*digraph*) with vertex set $V$ and edge set $E$. Each
directed edge $e \in E$ is an ordered pair of vertices $(v_1,v_2)$, with $v_1, v_2 \in V$. Note that a
directed edge from a vertex $v_i$ to itself, i.e., an edge $(v_i,v_i)$, is permissible. Fig. 1 shows
a simple directed graph with $V = \lbrace 0, 1, 2, 3 \rbrace$ and $E = \lbrace (0,1), (0,3), (1,3), (3,2), (2,0) \rbrace$.

<p align="center">
<img src="/figures/digraph.svg"/>
<br>
<em>Fig. 1: A simple directed graph.</em>
</p>

A directed path is a sequence of vertices $\langle v_0, \ldots, v_m \rangle$ such that
$(v_i, v_{i+1}) \in E$ for $i = 0, \ldots, m-1$. A cycle in a digraph is a path with $v_0 \equiv v_m$.
The digraph in Fig. 1 contains two cycles: $\langle 0, 1, 3, 2, 0 \rangle$ and $\langle 3, 2, 0, 3 \rangle$.
A directed acyclic graph (*dag*) is a digraph that contains no cycles. Fig. 2 shows a simple
dag; note that the edge $(0,2)$ replaces the edge $(2,0)$ from the digraph in Fig. 1.

<p align="center">
<img src="/figures/dag.svg"/>
<br>
<em>Fig. 2: A simple dag.</em>
</p>

A digraph $G = (V,E)$ may be respresented as a set of adjacency
lists, one list for each vertex $v \in V$. The adjacency list for $v$ contains the vertices
$u_i$ for all $(v,u_i) \in E$, i.e, all vertices to which $v$ contains an outgoing edge.
The adjacency list for vertex $0$ in Fig. 1 contains the vertices $1$ and $3$.

If we assume that the vertices are numbered $0, \ldots, |V|-1$, then the adjacency list
for each vertex $v_i$ may be represented as a bit-array, where a bit value of 1 in
array position $j$ implies an edge $(v_i, v_j)$. When $|V| \le 32$, a 32-bit integer
is sufficient to represent the adjacency list for a vertex. Fig. 3 shows a bit-array
representation of the adjacency list for vertex $0$ of the digraph in Fig. 1.

<p align="center">
<img src="/figures/adj_list.svg"/>
<br>
<em>Fig. 3: A bit-array representation of an adjacency list.</em>
</p>

In this exercise, our objective is to write a function in C/C++ that will
determine whether a digraph is a dag. The main driver program, which is
provided in the file `main.c`, takes as a command-line argument the name of a file containing a
representation of a digraph and reads the graph into an array of `uint32_t`
values. The size of the array is equal to the number of vertices in the digraph,
and each element $i$ of the array stores the adjacency list for vertex $v_i$.

The function to determine whether a digraph is a dag has the following
prototype:
```c
int is_dag(uint32_t digraph[], int n_vertices);
```
This function takes as input an adjacency list representation of a digraph
and the number of vertices in the graph. The size of the `digraph` array is
`n_vertices`, with `n_vertices` at most 32. The function returns 1 if the given
graph is a dag and 0 otherwise.

After implementing the `is_dag` function, the program should be run for each of
the digraphs in the `test_graphs/` sub-directory of this repository.
