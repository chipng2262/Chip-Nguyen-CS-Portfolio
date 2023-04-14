import java.util.*;
import java.io.*;

/**
 * A class to implement Huffman algorithms to create compressed and decompressed file, implementing an
 * interface created by Tim Pierson & Henry Kim (Dartmouth CS 10, Winter 2023).
 * @author Trung Nguyen & Chip Nguyen, Dartmouth CS 10, Winter 2023.
 */

public class HuffmanCompress implements Huffman {

    /**
     * Read characters from a text file, construct a frequency table.
     * @param pathName - path to a file to read
     * @return frequency table as a map
     * @throws IOException
     */
    public Map<Character, Long> countFrequencies(String pathName) throws IOException {
        BufferedReader input = new BufferedReader(new FileReader(pathName));
        Map<Character, Long> frequencies = new HashMap<Character, Long>();
        int c;
        // now iterate over all characters
        while ((c = input.read()) != -1) {
            char current = (char) c;
            if (!frequencies.containsKey(current)) {
                frequencies.put(current, 1L);
            }
            else {
                frequencies.put(current, frequencies.get(current) + 1);
            }
        }

        // Close input files
        try {
            input.close();
        } catch (IOException e) {
            throw e;
        }
        return frequencies;
    }

    /**
     * Construct a code tree from frequency table. Handling the case of empty files (return empty tree).
     * @param frequencies a map of Characters with their frequency counts from countFrequencies
     * @return the code tree.
     */
    public BinaryTree<CodeTreeElement> makeCodeTree(Map<Character, Long> frequencies) {
        // If the file is empty, frequency table is empty. Check and return null if needed.
        if (frequencies.isEmpty()) return new BinaryTree<CodeTreeElement>(null);
        else {
            // Comparator for PriorityQueue
            class TreeComparator implements Comparator<BinaryTree<CodeTreeElement>> {
                public int compare(BinaryTree<CodeTreeElement> t1, BinaryTree<CodeTreeElement> t2) {
                    if (t1.getData().getFrequency() > t2.getData().getFrequency()) return 1;
                    else if (t1.getData().getFrequency() < t2.getData().getFrequency()) return -1;
                    else return 0;
                }
            }

            PriorityQueue<BinaryTree<CodeTreeElement>> pq = new PriorityQueue<>(new TreeComparator());

            // Create new tree for all elements
            for (Character c: frequencies.keySet()) {
                CodeTreeElement data = new CodeTreeElement(frequencies.get(c), c);
                pq.add(new BinaryTree<CodeTreeElement>(data));
            }

            // Huffman algorithm!
            while (pq.size() > 1) {
                BinaryTree<CodeTreeElement> T1 = pq.poll();
                BinaryTree<CodeTreeElement> T2 = pq.poll();
                CodeTreeElement totalData = new CodeTreeElement(
                        T1.getData().getFrequency() + T2.getData().getFrequency(), null);
                pq.add(new BinaryTree<CodeTreeElement>(totalData, T1, T2));
            }
            return pq.poll();
        }
    }

    /**
     * Computes the code for all characters in the tree and enters them
     * into a map where the key is a character and the value is the code of 1's and 0's representing
     * that character. If the file is blank, return empty map.
     * @param codeTree the tree for encoding characters produced by makeCodeTree
     * @return the map from characters to codes
     */
    public Map<Character, String> computeCodes(BinaryTree<CodeTreeElement> codeTree) {
        // If file is blank, the tree is null, so check and return null if needed.
        if (codeTree.getData() == null) return new HashMap<Character, String>();
        else {
            Map<Character, String> codeMap = new TreeMap<Character, String>();

            // If codeTree has 1 character, give its code "1"
            if (codeTree.isLeaf()) {
                codeMap.put(codeTree.getData().getChar(), "1");
            }
            else {
                String pathSoFar = "";
                retrieveCode(codeTree, pathSoFar, codeMap);
            }
            return codeMap;
        }
    }

    /**
     * Helper method to compute code for characters & add them to a map, keeping track of "path-so-far"
     * during traversal.
     * @param codeTree: the tree for encoding characters produced by makeCodeTree
     * @param pathSoFar: ArrayList containing characters encountered so far
     * @param codeMap: the map from characters to codes
     */
    private void retrieveCode (BinaryTree<CodeTreeElement> codeTree, String pathSoFar,
                               Map<Character, String> codeMap){
        // If current node is a leaf, we have our char.
        if (codeTree.isLeaf()) {
            codeMap.put(codeTree.getData().getChar(), pathSoFar);
        }
        if (codeTree.hasLeft()) {
            // Add codes to the end of pathSoFar, removing the end of it when done.
            pathSoFar = pathSoFar + '0';
            retrieveCode(codeTree.getLeft(), pathSoFar, codeMap);
            pathSoFar = pathSoFar.substring(0, pathSoFar.length() - 1);
        }
        if (codeTree.hasRight()) {
            pathSoFar = pathSoFar + '1';
            retrieveCode(codeTree.getRight(), pathSoFar, codeMap);
            pathSoFar = pathSoFar.substring(0, pathSoFar.length() - 1);
        }
    }

    /**
     * Compress the file pathName and store compressed representation in compressedPathName.
     * @param codeMap - Map of characters to codes produced by computeCodes
     * @param pathName - File to compress
     * @param compressedPathName - Store the compressed data in this file
     * @throws IOException
     */
    public void compressFile(Map<Character, String> codeMap, String pathName, String compressedPathName)
            throws IOException {
        BufferedReader input = new BufferedReader(new FileReader(pathName));
        BufferedBitWriter bitOutput = new BufferedBitWriter(compressedPathName);

        // Create an arraylist to hold all the characters.
        ArrayList<Character> allData = new ArrayList<>();
        int current;
        while ((current = input.read()) != -1) {
            char character = (char) current;
            allData.add(character);
        }

        // Iterate over the arraylist (and then codeMap), AFTER checking if the codeMap is empty or not.
        if (!codeMap.isEmpty()) {
            for (Character c : allData) {
                for (Character bit : codeMap.get(c).toCharArray()) {
                    // Write bits!
                    bitOutput.writeBit(bit == '0' ? false : true);
                }
            }

            // Close output file.
            try {
                bitOutput.close();
            } catch (IOException e) {
                throw e;
            }
        }
    }

    /**
     * Decompress file compressedPathName and store plain text in decompressedPathName.
     * @param compressedPathName - file created by compressFile
     * @param decompressedPathName - store the decompressed text in this file, contents should match the original file before compressFile
     * @param codeTree - Tree mapping compressed data to characters
     * @throws IOException
     */
    public void decompressFile(String compressedPathName, String decompressedPathName,
                               BinaryTree<CodeTreeElement> codeTree) throws IOException {
        BufferedWriter output = new BufferedWriter(new FileWriter(decompressedPathName));

        // Check if the codeTree is empty (which means file is blank) before trying to read bits
        if (codeTree.getData() != null) {
            BufferedBitReader bitInput = new BufferedBitReader(compressedPathName);
            BinaryTree<CodeTreeElement> currTree = codeTree;

            while (bitInput.hasNext()) {
                if (currTree.isLeaf()) {
                    output.write(currTree.getData().getChar());
                    currTree = codeTree;
                }
                boolean currBit = bitInput.readBit();

                if (!currBit) {
                    currTree = currTree.getLeft();
                }
                /* Before, in our codeTree, if the file has only 1 char, we give that char code "1".
                   We need to confirm that this is not the case before traversing to the right.
                 */
                else if (currBit && !codeTree.isLeaf()) {
                    currTree = currTree.getRight();
                }
            }
            //Add one last bit to our file
            if (currTree.isLeaf() && !codeTree.isLeaf()) {
                output.write(currTree.getData().getChar());
                currTree = codeTree;
            }

            // Close reader.
            try {
                bitInput.close();
            } catch (IOException e) {
                throw e;
            }
        }

        // Close writer.
        try {
            output.close();
        } catch(IOException e) {
            throw e;
        }
    }
}
