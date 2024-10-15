#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

// Struct to represent a hash type with its name, length, and regex pattern
struct HashInfo {
    string name;
    int length;
    regex pattern;
    string prefix;
};

// Extended list of hash types with their unique characteristics
vector<HashInfo> hashTypes = {
    {"MD5", 32, regex("^[0-9a-fA-F]{32}$"), ""},
    {"MD4", 32, regex("^[0-9a-fA-F]{32}$"), ""},
    {"NTLM", 32, regex("^[0-9A-Fa-f]{32}$"), ""},
    {"SHA1", 40, regex("^[0-9a-fA-F]{40}$"), ""},
    {"SHA224", 56, regex("^[0-9a-fA-F]{56}$"), ""},
    {"SHA256", 64, regex("^[0-9a-fA-F]{64}$"), ""},
    {"SHA384", 96, regex("^[0-9a-fA-F]{96}$"), ""},
    {"SHA512", 128, regex("^[0-9a-fA-F]{128}$"), ""},
    {"SHA3-256", 64, regex("^[0-9a-fA-F]{64}$"), ""},
    {"SHA3-512", 128, regex("^[0-9a-fA-F]{128}$"), ""},
    {"NTLMv2", 64, regex("^[0-9A-Fa-f]{64}$"), ""},
    {"bcrypt", 60, regex("^[./A-Za-z0-9]{53}$"), "$2"},
    {"scrypt", 64, regex("^[a-zA-Z0-9+/=]+$"), ""},
    {"Argon2", 96, regex("^[a-zA-Z0-9+/=]{96}$"), "$argon2"},
    {"CRC32", 8, regex("^[0-9A-Fa-f]{8}$"), ""},
};

// Function to detect hash type based on length, pattern, and prefix
string detectHashType(const string& hash) {
    for (const auto& hashType : hashTypes) {
        if (hash.length() == hashType.length) {
            if (!hashType.prefix.empty() && hash.rfind(hashType.prefix, 0) == 0) {
                return hashType.name;  // Prefix match found
            }
            if (regex_match(hash, hashType.pattern)) {
                return hashType.name;  // Regex pattern match found
            }
        }
    }
    return "Unknown";  // No match found
}

// Function to read hash from a file
string readHashFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filePath);
    }
    string hash;
    getline(file, hash);
    file.close();
    return hash;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " -f <file_path> | -i <hash_input>" << endl;
        return 1;
    }

    string hashInput;

    // Parse command line arguments
    string option = argv[1];
    if (option == "-f") {
        try {
            hashInput = readHashFromFile(argv[2]);
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
            return 1;
        }
    } else if (option == "-i") {
        hashInput = argv[2];
    } else {
        cerr << "Invalid option. Use -f for file input or -i for direct input." << endl;
        return 1;
    }

    // Detect the hash type
    string hashType = detectHashType(hashInput);
    cout << "Detected Hash Type: " << hashType << endl;

    return 0;
}
