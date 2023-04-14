import java.util.*;
import java.io.*;

/**
 * Sudi Assistant with POS tagging using Viterbi algorithm.
 *
 * @author Trung Nguyen '26 and Chip Nguyen '25, PS-5, Dartmouth CS10, Winter 2023.
 */

public class Sudi {
    private Scanner sc;
    private final String startTag = "#";
    private final double penaltyScore = -100D;          // unseen penalty score

    /**
     * Test our HMM from console input.
     * @param model         pre-trained Hidden Markov Model
     * @param unseenScore   current penalty score for unseen observations
     */
    public void consoleTest(HiddenMarkovModel model, double unseenScore) {
        System.out.println("Please type in your sentence. 'q' to exit, and 'file' to switch to fileTest.");
        while (sc.hasNext()) {
            String line = sc.nextLine();
            if (line.equals("q")) System.exit(1);
            else if (line.equals("file")) fileTest(model, unseenScore);
            else {
                List<String> tagList = Viterbi.getTags(line, model, unseenScore);
                System.out.println("Annotated sentence: " + model.getTagsCombined(tagList, line));
            }
            System.out.println("Please type in your sentence. 'q' to exit, and 'file' to switch to fileTest.");
        }
    }

    /**
     * Test our HMM based on a pair of test files.
     * @param model           current Hidden Markov Model
     * @param unseenScore     current penalty score
     */
    public void fileTest(HiddenMarkovModel model, double unseenScore) {
        System.out.println("Please choose your desired test: 'brown-test', 'simple-test', or 'example'."
                            + " 'q' to exit, and 'console' to switch to consoleTest.");
        while (sc.hasNext()) {
            String test = sc.nextLine();
            if (test.equals("q")) System.exit(1);
            else if (test.equals("console")) consoleTest(model, unseenScore);
            else {
                String dataName = "data\\" + test + "-sentences.txt";
                String tagName = "data\\" + test + "-tags.txt";
                BufferedReader inputData;
                BufferedReader inputTags;
                try {
                    inputData = new BufferedReader(new FileReader(dataName));
                    inputTags = new BufferedReader(new FileReader(tagName));
                    String lineSentences, lineTag;
                    int incorrect = 0, correct = 0;
                    while ((lineSentences = inputData.readLine()) != null && (lineTag = inputTags.readLine()) != null) {
                        String[] tags = lineTag.split(" ");
                        List<String> tagList = Viterbi.getTags(lineSentences, model, unseenScore);
                        for (int i = 0; i < tagList.size(); i++) {
                            if (i > tags.length - 1) {
                                System.err.println("Mismatch due to algorithm / test files");
                                inputData.close();
                                inputTags.close();
                                return;
                            } else {
                                if (tagList.get(i).equals((tags[i]))) correct++;
                                else incorrect++;
                            }
                        }
                    }
                    System.out.println("For our current model and test files, there are " + correct +
                            " correct tags and " + incorrect + " incorrect tags.");
                    inputData.close();
                    inputTags.close();
                    System.out.println("Please choose your desired test: 'brown-test', 'simple-test', or 'example'."
                            + " 'q' to exit, and 'console' to switch to consoleTest");
                } catch (IOException e) {
                    System.out.println(e.getMessage());
                }
            }
        }
    }

    /**
     * Hard-coded from programming drills
     */
    public void test0() {
        Map<String, Map<String, Double>> transitions = new HashMap<>();
        Map<String, Map<String, Double>> observations = new HashMap<>();
        transitions.put(startTag, new HashMap<>());
        transitions.put("NP", new HashMap<>());
        transitions.put("N", new HashMap<>());
        transitions.put("CNJ", new HashMap<>());
        transitions.put("V", new HashMap<>());
        observations.put("NP", new HashMap<>());
        observations.put("CNJ", new HashMap<>());
        observations.put("N", new HashMap<>());
        observations.put("V", new HashMap<>());
        transitions.get(startTag).put("NP", 3D);
        transitions.get(startTag).put("N", 7D);
        transitions.get("NP").put("V", 8D);
        transitions.get("NP").put("CNJ", 2D);
        transitions.get("N").put("V", 8D);
        transitions.get("N").put("CNJ", 2D);
        transitions.get("CNJ").put("NP", 2D);
        transitions.get("CNJ").put("N", 4D);
        transitions.get("CNJ").put("V", 4D);
        transitions.get("V").put("NP", 4D);
        transitions.get("V").put("CNJ", 2D);
        transitions.get("V").put("N", 4D);
        observations.get("NP").put("chase", 10D);
        observations.get("CNJ").put("and", 10D);
        observations.get("V").put("get", 1D);
        observations.get("V").put("chase", 3D);
        observations.get("V").put("watch", 6D);
        observations.get("N").put("cat", 4D);
        observations.get("N").put("dog", 4D);
        observations.get("N").put("watch", 2D);
        HiddenMarkovModel model = new HiddenMarkovModel(transitions, observations);
        consoleTest(model, -10D);
    }

    /**
     * Main interface for Sudi
     */
    public void run() {
        System.out.println("Please choose following files for training: 'example', 'brown-train', 'simple-train', or 'test0'");
        sc = new Scanner(System.in);
        while (sc.hasNext()) {
            String line = sc.nextLine();
            if (line.equals("test0")) this.test0();
            else if (line.equals("brown-train") || line.equals("simple-train") || line.equals("example")) {
                HiddenMarkovModel model = new HiddenMarkovModel(line);
                System.out.println("Please choose your desired test: 'console' or 'file'");
                while (sc.hasNext()) {
                    String nextL = sc.nextLine();
                    if (nextL.equals("console")) {
                        consoleTest(model, penaltyScore);
                    } else if (nextL.equals("file")) {
                        fileTest(model, penaltyScore);
                    } else {
                        System.err.println("Incorrect syntax.");
                    }
                }
            }
            else {
                System.err.println("Incorrect syntax.");
            }
        }
    }

    public static void main(String[] args) {
        new Sudi().run();
    }
}
