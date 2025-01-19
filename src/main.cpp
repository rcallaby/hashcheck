#include "HashDetector.h"
#include "FileHandler.h"
#include "HashcatExecutor.h"
#include <iostream>

// Helper function to display the menu
void displayMenu() {
    
    std::cout << " _     _            _     _______ _                 _     " << std::endl;
    std::cout << "(_)   (_)          | |   (_______) |               | |    " << std::endl;
    std::cout << " _______ _____  ___| |__  _      | |__  _____  ____| |  _ " << std::endl;
    std::cout << "|  ___  (____ |/___)  _ \\| |     |  _ \\| ___ |/ ___) |_/ )" << std::endl;
    std::cout << "| |   | / ___ |___ | | | | |_____| | | | ____( (___|  _ ( " << std::endl;
    std::cout << "|_|   |_\\_____(___/|_| |_|\\______)_| |_|_____\\____)_| \\_) " << std::endl;
    std::cout << "                                                          " << std::endl;

                                                    
    std::cout << "\n--- Hashcat Cracker Menu ---" << std::endl;
    std::cout << "1. Load hash from a file" << std::endl;
    std::cout << "2. Manually enter a hash" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Please choose an option: ";
}

// Function to display help message
void printHelp() {
    std::cout << "Usage: hashcheck [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h,                         Show this help message and exit\n";
    std::cout << "  -f,                         Specify the file containing the hash\n";
    std::cout << "  -t,                         Specify the hash type (e.g., MD5, SHA256, etc.)\n";
    std::cout << "  -w,                         Path to the wordlist file\n";
    std::cout << "  -c,                         Specify a config JSON file for custom settings\n";
    std::cout << "  -o,                         Save results to the specified file\n";
    std::cout << "Examples:\n";
    std::cout << "  hashcheck -f hash.txt -w rockyou.txt\n";
    std::cout << "  hashcheck --f hash.txt --c config.json\n";
}

// Function to get user input for hash manually
std::string getUserInputHash() {
    std::string hash;
    std::cout << "Enter the hash: ";
    std::cin >> hash;
    return hash;
}

// Function to handle the menu-based interaction
std::string getHashInputFromMenu() {
    int choice;
    std::string hashInput, filePath;

    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter file path: ";
                std::cin >> filePath;
                try {
                    hashInput = FileHandler::readHashFromFile(filePath);
                    return hashInput;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            case 2:
                hashInput = getUserInputHash();
                return hashInput;
            case 0:
                std::cout << "Exiting..." << std::endl;
                exit(0);
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    }
}

// Main class for the overall flow of detecting hash and executing hashcat
class HashcatCracker {
public:
    void run(int argc, char* argv[]) {
        std::string hashInput;
        int hashcatMode;
        std::string configFilePath;

        // Check command-line arguments for input and config file
        if (argc >= 2) {
            std::string option = argv[1];
            if (option == "-h") {
                printHelp();
                exit(0);
            } else if (option == "-f" && argc >= 4) {
                try {
                    hashInput = FileHandler::readHashFromFile(argv[2]);
                    configFilePath = argv[3];
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return;
                }
            } else if (option == "-i" && argc >= 4) {
                hashInput = argv[2];
                configFilePath = argv[3];
            } else {
                std::cerr << "Invalid option. Use -h for usage information." << std::endl;
                return;
            }
        } else {
            // If no command-line input is provided, use menu system
            std::cout << "No input provided. Launching menu..." << std::endl;
            hashInput = getHashInputFromMenu();

            // Prompt for the config file path
            std::cout << "Enter path to JSON configuration file: ";
            std::cin >> configFilePath;
        }

        // Load configuration from JSON file
        json config;
        try {
            config = FileHandler::loadConfigFromFile(configFilePath);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return;
        }

        // Detect the hash type
        HashDetector detector;
        std::string hashType = detector.detectHashType(hashInput, hashcatMode);
        std::cout << "Detected Hash Type: " << hashType << std::endl;

        if (hashcatMode == -1) {
            std::cerr << "Unsupported hash type. Exiting..." << std::endl;
            return;
        }

        // Generate hashcat command based on config file
        std::string hashcatCommand = HashcatExecutor::generateHashcatCommand(hashcatMode, config, hashInput);
        std::cout << "Generated Hashcat Command: " << hashcatCommand << std::endl;

        // Execute the hashcat command
        HashcatExecutor::executeHashcatCommand(hashcatCommand);
    }
};

int main(int argc, char* argv[]) {
    HashcatCracker cracker;
    cracker.run(argc, argv);
    return 0;
}
