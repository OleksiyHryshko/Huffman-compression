/**
 * @file ConsoleApplication1.cpp
 * @brief Implementation of Huffman compression/decompression program.
 *
 * This program implements Huffman coding for file compression and decompression.
 * It supports two modes of operation:
 * - Compression mode ('k'): Compresses input file and generates dictionary.
 * - Decompression mode ('d'): Decompresses file using provided dictionary.
 *
 * The program expects the following command line arguments:
 * - Input file path
 * - Output file path
 * - Dictionary file path
 * - Mode of operation (either 'k' for compression or 'd' for decompression).
 */

#include "Header.h"

/**
 * @brief Main entry point of the program.
 *
 * This function processes command line arguments to determine the operation mode,
 * reads the input file, and calls the appropriate functions for compression or decompression.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return 0 on successful execution, 1 on error.
 */
int main(int argc, char* argv[]) {
    std::string output, input, dictionary;
    char mode;

    bool ok = GETPARAMS(argc, argv, output, input, dictionary, mode);
    if (!ok) {
        help();
        exit(1);
    }
    
    
    


    if (mode == 'k') {
     std::string data;
     bool rok = ReadFile(input, data);
     if (rok != 1) {
         std::cout << "Problems with reading file";
         exit(1);
     }
     compressing(data,  output, dictionary);
     std::cout << "Compression is done in file "<<output << std::endl;



    }
    else if (mode == 'd') {
        std::unordered_map <char, std::string> Huffmensdictionary;
        bool ok = Readfromdictionary(Huffmensdictionary,dictionary);
        
        if (!ok) {
            std::cout << "Problems with reading from dictionary: " << dictionary << std::endl;
            return 0;
        }
        decompressing(input, output, Huffmensdictionary);
        std::cout << "Decompression is done" << std::endl;
    }
    

    
        return 0; 
    
}