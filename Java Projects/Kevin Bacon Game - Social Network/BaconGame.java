import java.io.*;
import java.util.Scanner;
import java.util.*;

/**
 * Handling the Bacon Game
 * @author Trung Nguye & Chip Nguyen, Dartmouth CS10, Winter 2023
 */
public class BaconGame {
    private Graph<String, Set<String>> baconGraph;   // our Bacon Graph
    private String center;                           // current center of universe
    private Graph<String, Set<String>> baconTree;    // tree containing shortest path from center to all connected nodes
    private int numConnectedActors;                  // number of actors connected with center of universe
    private double avgSeparation;                    // average separation of the current center
    private Set<String> missingActors;               // set of all actors that are not connected to center

    /**
     * Bacon Game Constructor, with Kevin Bacon as the default center of the universe.
     * @param actorsFilename        fileName containing id, actors
     * @param moviesFilename        fileName containing id, movies
     * @param movieActorsFilename   fileName containing actors, movies
     */
    public BaconGame(String actorsFilename, String moviesFilename, String movieActorsFilename){
        baconGraph = new AdjacencyMapGraph<>();
        Map<String, List<String>> actors = readData(actorsFilename);
        Map<String, List<String>> movies = readData(moviesFilename);
        Map<String, List<String>> movieActors = readData(movieActorsFilename);
        this.buildBaconGraph(actors, movies, movieActors);
        this.setCenter("Kevin Bacon");
        /*  Only for test files to check graphs
        System.out.println("Bacon Graph: \n" + baconGraph);
        System.out.println("Bacon Tree: \n" + baconTree);
         */
        this.handleInput();
    }

    /**
     * Create a map, described in return below. It maps String -> Set of Strings to allow the map
     * to work in all cases.
     * @param pathname Any filename (actors, movies, movie-actors)
     * @return A map that map either: id -> actors, id-> movies, or movies -> list of actors in that movie
     */
    private Map<String, List<String>> readData(String pathname) {
        BufferedReader input;
        try {
            input = new BufferedReader(new FileReader(pathname));
        } catch (IOException e) {
            System.err.println("Cannot open file.\n" + e.getMessage());
            return null;
        }
        Map<String, List<String>> data = new HashMap<>();

        try {
            String line;
            while ((line = input.readLine()) != null) {
                String[] str = line.split("\\|");
                if (data.containsKey(str[0])) {
                    data.get(str[0]).add(str[1]);
                } else {
                    List<String> dataSet = new ArrayList<>();
                    dataSet.add(str[1]);
                    data.put(str[0], new ArrayList<>(dataSet));
                }
            }
        } catch (IOException e) {
            System.err.println("Cannot read file.\n" + e.getMessage());
            return null;
        }
        return data;
    }

    /**
     * Create our BaconGraph, connecting any 2 actors (that has costarred in a movie) with their costarred movies
     * @param actors        Map created from actorsFile, from method above
     * @param movies        Map created from moviesFile, from method above
     * @param movieActors   Map created from movieActorsFile, from method above
     */
    private void buildBaconGraph(Map<String, List<String>> actors,
                                 Map<String, List<String>> movies, Map<String, List<String>> movieActors) {
        // Insert all actors as vertices
        for (String actor : actors.keySet()) {
            baconGraph.insertVertex(actors.get(actor).get(0));
        }
        for (String v : movieActors.keySet()) {
            String nameMovie = movies.get(v).get(0);

            // Loop over all pair of 2 actors
            for (int i = 0; i < movieActors.get(v).size() - 1; i++) {
                for (int j = i + 1; j < movieActors.get(v).size(); j++) {
                    String actor1 = actors.get(movieActors.get(v).get(i)).get(0);
                    String actor2 = actors.get(movieActors.get(v).get(j)).get(0);
                    if (!baconGraph.hasEdge(actor1, actor2)) {
                        baconGraph.insertUndirected(actor1, actor2, new HashSet<>());
                    }
                    baconGraph.getLabel(actor1, actor2).add(nameMovie);
                }
            }
        }
    }

    /**
     * Set our own center of the universe. Default: Kevin Bacon (see constructor above)
     * @param center    the new center of the universe
     */
    private void setCenter(String center){
        if (!baconGraph.hasVertex(center)) System.out.println("Data does not contains this actor.");
        else {
            this.center = center;
            baconTree = BaconGraphLib.bfs(baconGraph, center);
            numConnectedActors = baconTree.numVertices() - 1;
            avgSeparation = BaconGraphLib.averageSeparation(baconTree, center);
            missingActors = BaconGraphLib.missingVertices(baconGraph, baconTree);
            System.out.println(center + " is now the center of the acting universe, connected to "
                                + numConnectedActors + "/" + baconGraph.numVertices() +
                                " actors with average separation " + avgSeparation);
        }
    }

    /**
     * List top (positive number) or bottom (negative) centers of the universe,
     * sorted by average separation (increasing)
     * Print out found actors
     * @param number the number of actors listed, positive if from the top, negative if from the bottom.
     */
    private void sortedAverageSeparation(int number){
        Map<String, Double> mapSeparation = new HashMap<>();
        for (String actor: baconGraph.vertices()){
            Graph<String, Set<String>> aBaconTree = BaconGraphLib.bfs(baconGraph, actor);
            double averageSeparation = BaconGraphLib.averageSeparation(aBaconTree, actor);
            mapSeparation.put(actor, averageSeparation);
        }

        int anchorNumber = number;
        PriorityQueue<String> pq = new PriorityQueue<>((String a1, String a2) -> {
                /*
                 Decide the sign based on our input. MinPQ if positive, MaxPQ if negative
                 */
                double compare = (mapSeparation.get(a1) - mapSeparation.get(a2)) * anchorNumber;
                if (compare < 0) return -1;
                else if (compare > 0) return 1;
                else return 0;
            }
        );
        for (String actor: mapSeparation.keySet()){
            pq.add(actor);
        }
        System.out.println((number > 0 ? "Top " : "Bottom ") + Math.min(Math.abs(number), pq.size())
                + " centers of the universe, sorted by average separation:");
        number = Math.abs(number);
        while (!pq.isEmpty() && number > 0){
            number--;
            String actor = pq.poll();
            System.out.println(actor + ": " + mapSeparation.get(actor));
        }
    }

    /**
     * List actors sorted by degree (decreasing), showing only degree between low and high
     * @param low
     * @param high
     */
    private void sortedDegree(int low, int high){
        PriorityQueue<String> pq = new PriorityQueue<>((String a1, String a2) ->
                baconGraph.inDegree(a2) - baconGraph.inDegree(a1));
        for (String actor: baconGraph.vertices()){
            if (low <= baconGraph.inDegree(actor) && baconGraph.inDegree(actor) <= high) {
                pq.add(actor);
            }
        }
        System.out.println("The following are " + pq.size() + " actors sorted in decreasing order:");
        while (!pq.isEmpty()) {
            String actor = pq.poll();
            System.out.println(actor + ": " + baconGraph.outDegree(actor) + " costars" );
        }
    }

    /**
     * Find all actors that have infinite separation from the center of the acting universe
     * In other words, all actors that are not connected to our current center of the universe.
     */
    private void listInfiniteSeparation(){
        System.out.println("There are " + missingActors.size() + " actors with infinite separation " +
                            "from current center (" + center + "):");
        for (String actor: missingActors){
            System.out.println(actor);
        }
    }

    /**
     * Find path from an actor of interest to the center of universe, using static method from our lib
     * Print out the path
     * @param name  actor of interest
     */
    private void findPath(String name){
        if (!baconTree.hasVertex(name)) {
            System.out.println("This actor does not costar with the current center of the universe, " +
                    "a.k.a, his/her number is infinity");
        }
        else {
            List<String> currentPath = BaconGraphLib.getPath(baconTree, name);
            System.out.println(name + "'s number is " + (currentPath.size() - 1));
            for (int i = 0; i < currentPath.size() - 1; i++) {
                String firstActor = currentPath.get(i);
                String secondActor = currentPath.get(i + 1);
                Set<String> costarredMovies = baconGraph.getLabel(firstActor, secondActor);
                System.out.println(currentPath.get(i) + " appeared in " + costarredMovies + " with " + secondActor);
            }
        }
    }

    /**
     * List actors sorted by non-infinite separation from the current center,
     * with separation between low and high
     * @param low
     * @param high
     */
    private void listSortedSeparation(int low, int high){
        Map<String, Integer> currentSep = BaconGraphLib.getSeparation(baconTree, center);

        // Sort by increasing separation
        PriorityQueue<String> pq = new PriorityQueue<>(
                (String a1, String a2) -> currentSep.get(a1) - currentSep.get(a2));
        for (String actor: currentSep.keySet()){
            if (low <= currentSep.get(actor) && currentSep.get(actor) <= high){
                pq.add(actor);
            }
        }
        System.out.println("These are " + pq.size() + " valid actors, " +
                            "sorted in the order of increasing separation:");
        while(!pq.isEmpty()){
            String actor = pq.poll();
            System.out.println(actor + ": " +currentSep.get(actor));
        }
    }

    /**
     * Handle user input, using methods from above.
     * Letters used taken from PS4 webpage.
     */
    private void handleInput() {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNext()) {
            System.out.println("\n" + center + " game >");
            String line = sc.nextLine();
            char game = line.charAt(0);

            // quit game
            if (game == 'q') {
                System.out.println("Game ended.");
                System.exit(0);
            }

            // sorted avgSeparation . + to see the top, - to see the bottom
            else if (game == 'c'){
                String[] str = line.split(" ");
                int number = Integer.parseInt(str[1]);
                if (Math.abs(number) > baconGraph.numVertices()) {
                    System.err.println("The input number exceeded number of vertices.");
                } else this.sortedAverageSeparation(number);
            }

            // sorted by degree, with threshold
            else if (game == 'd'){
                String[] str = line.split(" ");
                int low = Integer.parseInt(str[1]);
                int high = Integer.parseInt(str[2]);
                if (low <= high){
                    this.sortedDegree(low, high);
                } else System.out.println("Invalid lower bounds and upper bounds!");
            }

            // list actors not connected (infinite separation)
            else if (game == 'i'){
                this.listInfiniteSeparation();
            }

            // find path to current center
            else if (game == 'p'){
                String name = line.split(" ", 2)[1];
                if (!baconGraph.hasVertex(name)) {
                    System.out.println("Invalid actor name!");
                }
                else {
                    this.findPath(name);
                }
            }

            // list actors in increasing order, based on their separation from center, with threshold
            else if (game == 's'){
                String[] str = line.split(" ");
                int low = Integer.parseInt(str[1]);
                int high = Integer.parseInt(str[2]);
                if (low <= high) {
                    this.listSortedSeparation(low, high);
                }
                else System.out.println("Invalid lower bounds and upper bounds!");
            }

            // set new center of universe
            else if (game == 'u'){
                String name = line.split(" ", 2)[1];
                if (baconGraph.hasVertex(name)) {
                    this.setCenter(name);
                } else {
                    System.out.println("Invalid actor name!");
                }
            }

            // wrong command.
            else System.out.println("Please enter the correct command.");
        }
    }

    /**
     * Main method, run to activate the Bacon Game.
     * @param args
     */
    public static void main(String[] args) {
        String actorsFilename = "inputs/actors.txt";
        String moviesFilename = "inputs/movies.txt";
        String moviesActorsFilename = "inputs/movie-actors.txt";
        System.out.println("Command:");
        System.out.println("c <#>: list top (positive number) or bottom (negative) <#> centers of" +
                " the universe, sorted by average separation");
        System.out.println("d <low> <high>: list actors sorted by degree, with degree between low and high");
        System.out.println("i: list actors with infinite separation from the current center");
        System.out.println("p <name>: find path from <name> to current center of the universe");
        System.out.println("s <low> <high>: list actors sorted by non-infinite separation from the current " +
                "center, with separation between low and high");
        System.out.println("u <name>: make <name> the center of the universe");
        System.out.println("q: quit game");
        new BaconGame(actorsFilename, moviesFilename, moviesActorsFilename);
    }
}
