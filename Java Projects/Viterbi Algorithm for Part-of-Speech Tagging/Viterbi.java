import java.util.*;

/**
 * Given a Hidden Markov Model, implement Viterbi algorithm to give tags to sentences.
 *
 * @author Trung Nguyen '26 and Chip Nguyen '25, PS-5, Dartmouth CS10, Winter 2023.
 */
public class Viterbi {
    private static final String startTag = "#";             // Start tag of HMM
    private static List<Map<String, String>> backtrace;     // Used to backtrack from end to start (curr -> nextTag)
    private static List<String> tagsList;                   // The tag sequence for the sentence.
    private static final double minInf = -1000000D;         // self-defined negative infinity

    /**
     * Compute the score using Viterbi algorithm
     * @param sentence      a sentence of interest
     * @param model         pre-trained Hidden Markov Model
     * @param unseenScore   penalty for unseen observations
     * @return              a map that map final possible states -> current scores
     */
    private static Map<String, Double> computeScore(String sentence, HiddenMarkovModel model, double unseenScore) {
        backtrace = new ArrayList<>();
        Map<String, Double> currScores = new HashMap<>();
        Set<String> currStates = new HashSet<>();
        String[] words = sentence.split(" ");
        currScores.put(startTag, 0D);
        currStates.add(startTag);

        for (int i = 0; i< words.length; i++) {
            backtrace.add(new HashMap<>());
            String currWord = words[i].toLowerCase();
            Map<String, Double> nextScores = new HashMap<>();
            Set<String> nextStates = new HashSet<>();

            for (String currState: currStates) {
                double currentScore = currScores.get(currState);
                Map<String, Double> possibleTrans = model.getTransitions(currState);

                // Check transition maps
                if (possibleTrans == null) continue;

                // If transition map is not null, calculate score.
                for (String nextState: possibleTrans.keySet()) {
                    nextStates.add(nextState);
                    double nextScore = currentScore + possibleTrans.get(nextState);
                    Map<String, Double> possibleObs = model.getObservations(nextState);

                    /*
                    If we have seen the word with this possible next state before, add the observation score.
                    Else, add unseenScore.
                     */
                    nextScore += (possibleObs.containsKey(currWord) ? possibleObs.get(currWord) : unseenScore);

                    // Record if it is the best so far.
                    if (!nextScores.containsKey(nextState) || nextScores.get(nextState) < nextScore) {
                        nextScores.put(nextState, nextScore);
                        backtrace.get(i).put(nextState, currState);
                    }
                }
            }
            currScores = nextScores;
            currStates = nextStates;
        }
        return currScores;
    }

    /**
     * Backtrack to get tag list, using map created from computeScore and backtrack arraylist.
     * @param finalScores       Map final possible states -> current scores
     */
    private static void backtrack(Map<String, Double> finalScores) {
        double bestFinalScore = minInf;
        String currFinalState = null;
        tagsList = new ArrayList<>();

        // Find the best final states (the highest score)
        for (String finalState: finalScores.keySet()) {
            if (bestFinalScore < finalScores.get(finalState)) {
                currFinalState = finalState;
                bestFinalScore = finalScores.get(finalState);
            }
        }

        // After finding the final state, using ArrayList backtrace to retrieve the path there
        for (int i = backtrace.size() - 1; i >= 0; i--) {
            tagsList.add(currFinalState);
            currFinalState = backtrace.get(i).get(currFinalState);
        }

        // The tagList is in reverse order, so reverse it.
        Collections.reverse(tagsList);
    }

    /**
     * The main static method to give list of tags to sentence.
     * @param sentence          Sentence we want to give tags to
     * @param model             Pre-trained Hidden Markov Model
     * @param unseenScore       UnseenScore penalty
     * @return                  List of tags, given by HMM and Viterbi for our sentence of interest.
     */
    public static List<String> getTags(String sentence, HiddenMarkovModel model, double unseenScore) {
        Map<String, Double> finalScores = computeScore(sentence, model, unseenScore);
        backtrack(finalScores);
        return tagsList;
    }
}

