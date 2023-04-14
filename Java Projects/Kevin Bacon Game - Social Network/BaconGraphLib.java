import java.util.*;

/**
 * A library to create and analyze graphs for Bacon Game
 * @author Trung Nguyen, Chip Nguyen, Dartmouth CS10, Winter 2023
 *
 * <V> Type for vertices
 * <E> Type for edge labels
 */
public class BaconGraphLib {

    /**
     * Do a bfs on the graph, create the shortest path tree.
     * @param g      our graph
     * @param source the center of the universe
     * @return       a graph that is the shortest path tree.
     */
    public static <V,E> Graph<V,E> bfs(Graph<V,E> g, V source){
        Set<V> visited = new HashSet<>();
        Queue<V> queue = new LinkedList<>();
        Graph<V,E> shortestPathTree = new AdjacencyMapGraph<>();
        queue.add(source);
        visited.add(source);
        shortestPathTree.insertVertex(source);
        while (!queue.isEmpty()){
            V u = queue.remove();
            for (V v: g.outNeighbors(u)){
                if (!visited.contains(v)){
                    queue.add(v);
                    visited.add(v);
                    shortestPathTree.insertVertex(v);
                    shortestPathTree.insertDirected(v, u, g.getLabel(v, u));
                }
            }
        }
        return shortestPathTree;
    }

    /**
     * Get a path from a vertex to the center of universe
     * @param tree  the Bacon tree created above
     * @param v     the vertex we are interested in
     * @return      List of vertices in the path of v to the center of universe
     */
    public static <V,E> List<V> getPath(Graph<V,E> tree, V v){
        List<V> path = new ArrayList<>();
        while (tree.outDegree(v) != 0){
            path.add(v);
            v = tree.outNeighbors(v).iterator().next();
        }
        path.add(v);
        return path;
    }

    /**
     * Find the set of vertices in the original graph but not the subgraph
     * @param graph the original graph
     * @param subgraph a subgraph of the original graph
     * @return set of vertices in the original, but not in the subgraph
     */
    public static <V,E> Set<V> missingVertices(Graph<V,E> graph, Graph<V,E> subgraph){
        Set<V> missing = new HashSet<>();
        for (V v: graph.vertices()){
            if (!subgraph.hasVertex(v)){
                missing.add(v);
            }
        }
        return missing;
    }

    /**
     * Calculate the separation from a root to every connected nodes.
     * @param tree our Bacon Tree, created with root as the center of universe
     * @param root the center of Universe
     * @return     a map that maps nodes to its separation from the root
     */

    public static <V,E> Map<V, Integer> getSeparation(Graph<V,E> tree, V root){
        Map<V, Integer> distanceFromRoot = new HashMap<>();
        distanceFromRoot.put(root, 0);

        for (V v : tree.vertices()){
            // We only consider leaves. Idea: Create a stack to backtrack from leaves to root. (recursive stack)
            // When pop elements out of stack, it will be in the right order of separation.
            if (tree.inDegree(v) == 0){
                Stack<V> stack = new Stack<>();
                V currentNode = v;
                while (!distanceFromRoot.containsKey(currentNode)){
                    stack.add(currentNode);
                    currentNode = tree.outNeighbors(currentNode).iterator().next();
                }

                int currentDistance = distanceFromRoot.get(currentNode);
                while (!stack.isEmpty()){
                    V nextNode = stack.pop();
                    distanceFromRoot.put(nextNode, currentDistance + 1);
                    currentDistance++;
                }
            }
        }
        return distanceFromRoot;
    }

    /**
     * Calculate the average separation from a root to all connected nodes.
     * @param tree  the Bacon Tree, initialized with root as its center of universe.
     * @param root  the center of universe
     * @return      average separation from all of root's connected nodes.
     */
    public static <V,E> double averageSeparation(Graph<V,E> tree, V root){
        Map<V, Integer> distanceFromRoot = getSeparation(tree, root);
        int totalSep = 0;
        for (V v: distanceFromRoot.keySet()){
            totalSep += distanceFromRoot.get(v);
        }
        return (double) totalSep/(distanceFromRoot.size() - 1);
    }

    /**
     * Hand-coded test
     * @param args
     */
    public static void main(String[] args) {
        Graph<String, Set<String>> baconGraph = new AdjacencyMapGraph<>();
        baconGraph.insertVertex("Dartmouth (Earl thereof)");
        baconGraph.insertVertex("Kevin Bacon");
        baconGraph.insertVertex("Alice");
        baconGraph.insertVertex("Bob");
        baconGraph.insertVertex("Charlie");
        baconGraph.insertVertex("Nobody");
        baconGraph.insertVertex("Nobody's Friend");
        Set<String> l1 = new HashSet<>();
        l1.add("A Movie");
        l1.add("E Movie");
        baconGraph.insertUndirected("Kevin Bacon", "Alice", l1);
        Set<String> l2 = new HashSet<>();
        l2.add("A Movie");
        baconGraph.insertUndirected("Kevin Bacon", "Bob", l2);
        Set<String> l3 = new HashSet<>();
        l3.add("A Movie");
        baconGraph.insertUndirected("Alice", "Bob", l3);
        Set<String> l4 = new HashSet<>();
        l4.add("D Movie");
        baconGraph.insertUndirected("Alice", "Charlie", l4);
        Set<String> l5 = new HashSet<>();
        l5.add("C Movie");
        baconGraph.insertUndirected("Charlie", "Bob", l5);
        Set<String> l6 = new HashSet<>();
        l6.add("B Movie");
        baconGraph.insertUndirected("Dartmouth (Earl thereof)", "Charlie", l6);
        Set<String> l7 = new HashSet<>();
        l7.add("F Movie");
        baconGraph.insertUndirected("Nobody", "Nobody's Friend", l7);
        Graph<String, Set<String>> baconTree = BaconGraphLib.bfs(baconGraph, "Kevin Bacon");
        System.out.println("Bacon Graph: \n" + baconGraph);
        System.out.println("Bacon Tree: \n" + baconTree);
    }
}
