#include "Header.h"


struct Node {
    char symbol;                 ///< Symbol stored in the node
    unsigned int fr;             ///< Frequency of the symbol
    Node* left;                  ///< Left child of the node
    Node* right;                 ///< Right child of the node

    /**
     * @brief Constructor for the Node struct
     *
     * Initializes a new node with the given symbol, frequency, and child nodes.
     *
     * @param symbol The symbol to be stored in the node
     * @param fr The frequency of the symbol
     * @param left The left child of the node (default: nullptr)
     * @param right The right child of the node (default: nullptr)
     */
    Node(char symbol, unsigned int fr, Node* left = nullptr, Node* right = nullptr)
        : symbol(symbol), fr(fr), left(left), right(right) {
    }
};


/**
 * @brief Creates a new node with the given symbol, frequency, and child nodes.
 *
 * @param symbol The symbol to be stored in the node
 * @param fr The frequency of the symbol
 * @param left The left child of the node
 * @param right The right child of the node
 * @return A pointer to the newly created node
 */
Node* newNode(char symbol, unsigned int fr, Node* left, Node* right) {
    return new Node(symbol, fr, left, right);
}


struct comp {
    /**
     * @brief Comparison function for the priority queue
     *
     * Compares two nodes based on their frequencies.
     *
     * @param a The first node to compare
     * @param b The second node to compare
     * @return True if the frequency of node a is greater than the frequency of node b
     */
    bool operator()(Node* a, Node* b) {
        return a->fr > b->fr;
    }
};


/**
 * @brief Encodes the Huffman tree and generates the Huffman codes
 *
 * Recursively traverses the Huffman tree and generates the Huffman codes for each symbol.
 *
 * @param root The root of the Huffman tree
 * @param str The current Huffman code
 * @param huffmanCode A map to store the Huffman codes for each symbol
 */
void encode(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (root == nullptr)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->symbol] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

/**
 * @brief Deletes the Huffman tree
 *
 * Recursively deletes the nodes in the Huffman tree.
 *
 * @param root The root of the Huffman tree
 */
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}




/**
 * @brief Decodes the Huffman code and prints the original text
 *
 * Recursively traverses the Huffman tree and prints the original text based on the Huffman code.
 *
 * @param root The root of the Huffman tree
 * @param index The current index in the Huffman code
 * @param str The Huffman code
 */
void decode(Node* root, int& index, const std::string& str) {
    if (root == nullptr) {
        return;
    }

    if (!root->left && !root->right) {
        std::cout << root->symbol;
        return;
    }

    index++;

    if (str[index] == '0')
        decode(root->left, index, str);
    else
        decode(root->right, index, str);
}




/**
 * @brief Prints the usage and modes of the program
 */
void help() {
    std::cout << "Usage: -i <input file> -o <output file> -t <k/d> -s <dictionary file>\n";
    std::cout << "Modes:\n";
    std::cout << "  k - compression (default)\n";
    std::cout << "  d - decompression\n";
}





/**
 * @brief Compresses a file using Huffman coding
 *
 * Reads the input file, generates the Huffman codes, and writes the compressed data to the output file.
 *
 * @param data The input data to be compressed
 * @param output The path to the output file
 * @param dictionary The path to the dictionary file
 */
void compressing(std::string& data,  std::string& output,  std::string& dictionary) {
    std::unordered_map<char, unsigned int> frequency;
    for (char c : data) {
        frequency[c]++;
    }

    std::priority_queue<Node*, std::vector<Node*>, comp> pq;
    for (const auto& pair : frequency) {
        pq.push(newNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); 
        // makes the most important element the left child
        pq.pop();
        //removes from the queue
        Node* right = pq.top(); pq.pop();
        unsigned int sum = left->fr + right->fr;
        pq.push(newNode('\0', sum, left, right));
    }

    Node* root = pq.top();
    std::unordered_map<char, std::string> huffmanCode;
    
    encode(root,"", huffmanCode);
    //writing pairs to dictionary
    std::ofstream dict(dictionary, std::ios::binary);
    for (const auto& pair : huffmanCode) {
        dict << pair.first <<pair.second << '\n';
    }
    dict.close();
    //writing compressed text
    std::ofstream file(output, std::ios::binary);
    std::string str;
    for (char ch : data) {
        str += huffmanCode[ch];
    }

    if (!file.is_open()) {
        std::cerr << "Error opening file " << output << std::endl;
        exit(0);
    }

    
    std::vector<unsigned char> bites;

    
    for (int i = 0; i < str.size(); i += 8) {
        
        std::bitset<8> byte(str.substr(i, 8));
        
        bites.push_back(static_cast<unsigned char>(byte.to_ulong()));
    }

    
    file.write(reinterpret_cast<const char*>(bites.data()), bites.size());

    file.close();
    deleteTree(root);
}








/**
 * @brief Reads the dictionary file and populates the Huffman dictionary
 *
 * Reads the dictionary file and populates the Huffman dictionary with the symbol-Huffman code pairs.
 *
 * @param Huffmensdictionary The Huffman dictionary to be populated
 * @param dictionary The path to the dictionary file
 * @return True if the dictionary file is read successfully, false otherwise
 */
bool Readfromdictionary(std::unordered_map <char, std::string>& Huffmensdictionary, std::string& dictionary) {
    std::ifstream file(dictionary);
    if (!file.is_open()) {
        
        return 0;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            
            if (std::getline(file, line)) {
                Huffmensdictionary['\n'] = line;
            }
            else {
                
                break;
            }
        }
        else {
            
            char symbol = line[0];
            std::string code = line.substr(1);
            Huffmensdictionary[symbol] = code;
        }
    }
    
    file.close();
    

    return true;
}




/**
 * @brief Decompresses a file using Huffman coding
 *
 * Reads the compressed data from the input file, reconstructs the original text using the Huffman dictionary, and writes the decompressed text to the output file.
 *
 * @param input The path to the input file containing the compressed data
 * @param output The path where the decompressed output will be saved
 * @param Huffmensdictionary The Huffman dictionary used for decompression
 */
void decompressing(std::string& input, std::string& output, std::unordered_map <char, std::string> &Huffmensdictionary) {
    std::ifstream inputfile(input, std::ios::binary);
    std::string data = "";
    std::vector<unsigned char> buffer;
    inputfile.seekg(0, std::ifstream::end);
    std::streamsize fileSize = inputfile.tellg();
    inputfile.seekg(0, std::ifstream::beg);
    buffer.resize(fileSize);

    if (!inputfile.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cout << "Input file cannot be read" << std::endl;
        return;
    }
    for (unsigned char bite : buffer) {
        data += std::bitset<8>(bite).to_string();
    }
    
    std::string recreatedText = "";
    
    std::string bit = "";
    for (const char c : data) {
        bit += c;
        for (const auto& [key, value] : Huffmensdictionary) {
            if (value == bit) {
                recreatedText += key;
                bit = "";
                break;
            }
        }
    }
   
    
   
    std::ofstream outFile(output);
    if (outFile.is_open()) {
        outFile << recreatedText;
        outFile.close();
    }
    else {
        std::cerr << "Error opening output file: " << output << std::endl;
    }
    
    
    
}










/**
 * @brief Reads a file and returns its contents as a string
 *
 * Reads the input file and returns its contents as a string.
 *
 * @param input The path to the input file
 * @param data The string to store the file contents
 * @return True if the file is read successfully, false otherwise
 */
bool ReadFile(std::string& input, std::string& data) {

    std::ifstream file(input, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << input << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    std::size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(fileSize);
    file.read(&data[0], fileSize);
    file.close();

    return true;
}
/**
 * @brief Parses the command-line arguments and returns the input, output, dictionary, and mode
 *
 * Parses the command-line arguments and returns the input, output, dictionary, and mode.
 *
 * @param argc The number of command-line arguments
 * @param argv The array of command-line arguments
 * @param output The path to the output file
 * @param input The path to the input file
 * @param dictionary The path to the dictionary file
 * @param mode The mode of operation (compression or decompression)
 * @return True if the command-line arguments are parsed successfully, false otherwise
 */
bool GETPARAMS(int argc, char* argv[], std::string& output, std::string& input, std::string& dictionary, char& mode) {
    bool Linput = false, Loutput = false, Lmode = false, Ldictionary = false;
    
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-i" && i + 1 < argc) {
            input = argv[++i];
            
            Linput = true;
        }
        else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            output = argv[++i];
            Loutput = true;
        }
        else if (std::string(argv[i]) == "-t" && i + 1 < argc) {
            mode = argv[++i][0];
            if (mode == 'k' || 'd') {
                Lmode = true;
            }
            else {
                Lmode = false;
            }
            
        }
        else if (std::string(argv[i]) == "-s" && i + 1 < argc) {
            dictionary = argv[++i];
            Ldictionary = true;
        }
    }

    if (Linput && Loutput && Lmode && Ldictionary) {
        return true;
    }
    else {
        help();
        return false;
    }
}