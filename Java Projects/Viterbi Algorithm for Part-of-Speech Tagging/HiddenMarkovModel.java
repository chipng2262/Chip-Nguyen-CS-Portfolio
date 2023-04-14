import java.util.*;
import java.io.*;

/**
 * Train Hidden Markov Model from input data.
 *
 * @author Trung Nguyen '26 and Chip Nguyen '25, PS-5, Dartmouth CS10, Winter 2023.
 */

public class HiddenMarkovModel {
    private final String startTag = "#";
    private Map<String, Map<String, Double>> transitions;       // tag -> (next tag -> counts/probability)
    private Map<String, Map<String, Double>> observations;      // tag -> (word -> counts/probability)
    private final boolean DEBUG = false;                         // debugging purposes
    /**
     * Default constructor for our class, calling main method to train model
     * Also initiate HashMap for startTag, denoted "#" as PS-5 webpage.
     *
     * @param data training file name
     */
    public HiddenMarkovModel(String data) {
        transitions = new HashMap<>();
        observations = new HashMap<>();
        this.processData(data);
    }

    /**
     * A constructor for hard-coded test. Manually create transitions and observations map
     * and pass those parameters in.
     * @param transitions       Map describing transitions
     * @param observations      Map describing observations
     */
    public HiddenMarkovModel(Map<String, Map<String, Double>> transitions,
                             Map<String, Map<String, Double>> observations) {
        this.transitions = transitions;
        this.observations = observations;
    }

    /**
     * Get the next states (tags) of a given state (tag)
     * @param tag   the state of interest
     * @return      map of all possible next states
     */
    public Map<String, Double> getTransitions(String tag) {
        return transitions.get(tag);
    }

    /**
     * Get the words associated with a given state (tag)
     * @param tag   the state of interest
     * @return      map of all possible words
     */
    public Map<String, Double> getObservations(String tag) {
        return observations.get(tag);
    }

    /**
     * Process data file and train our HiddenMarkovModel by calling helper method (down below).
     * @param data training file name
     */
    private void processData(String data) {
        String sentencesFile = "data\\" + data + "-sentences.txt";
        String tagFile = "data\\" + data + "-tags.txt";
        BufferedReader inputSentences;
        BufferedReader inputTags;
        try {
            inputSentences = new BufferedReader(new FileReader(sentencesFile));
            inputTags = new BufferedReader(new FileReader(tagFile));

            String lineSentences, lineTag;
            while ((lineSentences = inputSentences.readLine()) != null && (lineTag = inputTags.readLine()) != null) {
                String[] words = lineSentences.split(" ");
                String[] tags = lineTag.split(" ");
                if (words.length == 0 || tags.length == 0 || words.length != tags.length) {
                    System.err.println("Line is blank // Input file do not match!");
                    transitions = new HashMap<>();
                    observations = new HashMap<>();
                    return;
                }
                // Build up our maps
                this.buildTransitions(tags);
                this.buildObservations(words, tags);
            }
            // Logarithm-ize our maps
            this.logarithmize(transitions);
            this.logarithmize(observations);

            // Only for debug
            if (DEBUG == true) {
                System.out.println(transitions);
                System.out.println(observations);
            }

            inputSentences.close();
            inputTags.close();
        } catch (IOException e) {
            System.err.println(e.getMessage());
        }
    }

    /**
     * Helper method to build Transitions map (from tag to the next tag) for each line our inputTag has read.
     * @param tags an array of tags in a sentence.
     */
    private void buildTransitions(String[] tags) {
        for (int i = -1; i < tags.length - 1; i++) {
            String currTag;

            // i = -1 is the case for transitions from startTag -> first tag of the sentence.
            currTag = ((i == -1) ? startTag : tags[i]);
            String nextTag = tags[i + 1];
            if (!transitions.containsKey(currTag)) {
                transitions.put(currTag, new HashMap<>());
            }
            if (!transitions.get(currTag).containsKey(nextTag)) {
                transitions.get(currTag).put(nextTag, 0.0);
            }
            transitions.get(currTag).put(nextTag, transitions.get(currTag).get(nextTag) + 1);
        }
    }

    /**
     * Helper method to build Observations map (from tag to word) for each 2 lines
     * our inputSentences and inputTag has read.
     * Two params point to the same sentence, one is words of sentence, the other is tag of such words.
     * @param words an array of words of a sentence.
     * @param tags  an array of tags of the same sentence.
     */
    private void buildObservations(String[] words, String[] tags) {
        for (int i = 0; i < words.length; i++) {
            String currWord = words[i];
            String currTag = tags[i];
            if (!observations.containsKey(currTag)) observations.put(currTag, new HashMap<>());
            if (!observations.get(currTag).containsKey(currWord)) {
                observations.get(currTag).put(currWord, 0D);
            }
            observations.get(currTag).put(currWord, observations.get(currTag).get(currWord) + 1);
        }
    }

    /**
     * Change total counts in transitions map and observation maps to logarithm of probability,
     * by logarithm-ize total counts.
     * @param map either Transitions map or Observations map
     */
    private void logarithmize(Map<String, Map<String, Double>> map) {
        for (String tag : map.keySet()) {
            double totalCount = 0.0;

            // "data" is either next tag or the word the tag refers to. Count the total for each of the tag.
            for (String data: map.get(tag).keySet()) {
                totalCount += map.get(tag).get(data);
            }

            // After calculating the total, loop through again to update counts to log.
            for (String data: map.get(tag).keySet()) {
                double log = Math.log(map.get(tag).get(data) / totalCount);
                map.get(tag).put(data, log);
            }
        }
    }

    /**
     * Create an annotated sentence given the tag list (created from Viterbi algorithm) and original sentence
     * @param sentence      original sentence
     * @param tagsList      tags of that sentence
     * @return  a String representing the sentence with appended tags
     */
    public String getTagsCombined(List<String> tagsList, String sentence) {
        String result = "";
        String[] words = sentence.split(" ");
        for (int i = 0; i < tagsList.size(); i++) {
            if (i > 0) result += " ";
            result += words[i] + "/" + tagsList.get(i);
        }
        return result;
    }
}