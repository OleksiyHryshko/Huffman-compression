#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>

/**
 * @brief Reads Huffman encoding dictionary from a file.
 *
 * This function reads a dictionary file that contains mappings of characters to their Huffman codes.
 * It populates the provided map with these mappings.
 *
 * @param Huffmensdictionary Map to store character-to-code mappings.
 * @param dictionary Path to the dictionary file.
 * @return true if the dictionary was read successfully, false otherwise.
 */
bool Readfromdictionary(std::unordered_map <char, std::string>& Huffmensdictionary, std::string& dictionary);

/**
 * @brief Parses command line arguments to get input/output parameters.
 *
 * This function processes the command line arguments to extract input and output file paths,
 * as well as the mode of operation (compression or decompression).
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @param output Path to output file.
 * @param input Path to input file.
 * @param dictionary Path to dictionary file.
 * @param mode Operation mode ('k' for compression, 'd' for decompression).
 * @return true if parameters were parsed successfully, false otherwise.
 */
bool GETPARAMS(int argc, char* argv[], std::string& output, std::string& input, std::string& dictionary, char& mode);

/**
 * @brief Displays help information about program usage.
 *
 * This function prints the usage instructions for the program to the console.
 */
void help();

/**
 * @brief Reads content from an input file.
 *
 * This function reads the content of the specified input file and stores it in the provided string.
 *
 * @param input Path to the input file.
 * @param data String to store file content.
 * @return true if the file was read successfully, false otherwise.
 */
bool ReadFile(std::string& input, std::string& data);

/**
 * @brief Compresses input data using Huffman encoding.
 *
 * This function takes input data, compresses it using Huffman coding, and writes the compressed data
 * to the specified output file. It also generates a dictionary for the encoding.
 *
 * @param data Input data to compress.
 * @param output Path to the output file.
 * @param dictionary Path to the dictionary file where the encoding map will be saved.
 */
void compressing(std::string& data, std::string& output, std::string& dictionary);

/**
 * @brief Decompresses Huffman-encoded data.
 *
 * This function takes compressed data, decompresses it using the provided Huffman dictionary,
 * and writes the decompressed data to the specified output file.
 *
 * @param data Input compressed data.
 * @param output Path to the output file for decompressed data.
 * @param Huffmensdictionary Dictionary containing character-to-code mappings.
 */
void decompressing(std::string& data, std::string& output, std::unordered_map <char, std::string>& Huffmensdictionary);