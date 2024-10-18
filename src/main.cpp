#include "include\HashcatExecutor.h"
#include "include\FileHandler.h"
#include "include\HashDetector.h"
#include <iostream>

class HashcatCracker {
public:
    void run(int argc, char* argv[]) {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0] << " -f <file_path> | -i <hash_input> -c <config_file>" << std::endl;
            return;
        }

        std::string hashInput;
        int hashcatMode;
        std::string configFilePath;

        // Parse command line arguments
        std::string option = argv[1];
        try {
            if (option == "-f") {
                hashInput = FileHandler::readHashFromFile(argv[2]);
                configFilePath = argv[3];
            } else if (option == "-i") {
                hashInput = argv[2];
                configFilePath = argv[3];
            } else {
                std::cerr << "Invalid option. Use -f for file input or -i for direct input." << std::endl;
                return;
            }

            // Load configuration from JSON file
            json config = FileHandler::loadConfigFromFile(configFilePath);

            // Detect the hash type
            HashDetector detector;
            std::string hashType = detector.detectHashType(hashInput, hashcatMode);
            std::cout << "Detected Hash Type: " << hashType << std::endl;

            // Generate hashcat command based on config file
            std::string hashcatCommand = HashcatExecutor::generateHashcatCommand(hashcatMode, config);
            std::cout << "Generated Hashcat Command: " << hashcatCommand << std::endl;

            // Execute the hashcat command
            HashcatExecutor::executeHashcatCommand(hashcatCommand);

        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    HashcatCracker cracker;
    cracker.run(argc, argv);
    return 0;
}
