import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Map;

/**
 * A class to test the HuffmanCompress class.
 * @author Trung Nguyen & Chip Nguyen, Dartmouth CS 10, Winter 2023.
 */
public class HuffmanCompressor {
    private static final boolean CHECK_IDENTICAL = true;    //boolean var, set to true to compare results.

    /**
     * Main method to test. Generate compressed and decompressed filenames; call methods from HuffmanCompress
     * as well as handle IOException. Result: Create compressed & decompressed files.
     * @param pathName
     */
    public static void test (String pathName) {
        HuffmanCompress compressor = new HuffmanCompress();
        try {
            String compressedPathName = pathName.substring(0, pathName.length() - 4) + "_compressed.txt";
            String decompressedPathName = pathName.substring(0, pathName.length() - 4) + "_decompressed.txt";
            Map<Character, Long> frequencies = compressor.countFrequencies(pathName);
            BinaryTree<CodeTreeElement> codeTree = compressor.makeCodeTree(frequencies);
            Map<Character, String> codeMap = compressor.computeCodes(codeTree);
            compressor.compressFile(codeMap, pathName, compressedPathName);
            compressor.decompressFile(compressedPathName, decompressedPathName, codeTree);
            if (CHECK_IDENTICAL) {
                if (checkIdenticalFiles(pathName, decompressedPathName)) {
                    System.out.println("Successful.");
                } else {
                    System.out.println("Error.");
                }
            }
        }
        catch (IOException e) {
            System.err.println("Problem opening/closing/reading/writing files. " + e.getMessage());
        }
    }

    /**
     * A method to check if the original and decompressed file are identical or not.
     * @param firstPathName
     * @param secondPathName
     * @return true if two files are identical; false if they are not.
     */
    private static boolean checkIdenticalFiles(String firstPathName, String secondPathName) {
        try {
            BufferedReader firstReader = new BufferedReader(new FileReader(firstPathName));
            BufferedReader secondReader = new BufferedReader(new FileReader(secondPathName));
            int firstChar, secondChar;
            while ((firstChar = firstReader.read()) != -1 && (secondChar = secondReader.read()) != -1) {
                if (firstChar != secondChar) {
                    return false;
                }
            }
            if ((firstReader.read() != -1) || (secondReader.read() != -1)) { // in case different file size
                return false;
            }
            return true;
        } catch (IOException e) {
            System.err.println("Cannot check identical files.\n" + e.getMessage());
        }
        return false;
    }

    public static void main(String[] args) {
        test("inputs/Simple2.txt");
        test("inputs/Simple.txt");
        test("inputs/Single.txt");
        test("inputs/Blank.txt");
        test("inputs/Repeated.txt");
        test("inputs/WarAndPeace.txt");
    }
}
