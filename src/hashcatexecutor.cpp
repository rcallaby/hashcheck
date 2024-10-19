#include "HashcatExecutor.h"
#include <iostream>
#include <cstdlib>  // For system() function

using json = nlohmann::json;

std::string HashcatExecutor::generateHashcatCommand(int hashcatMode, const json& config, const std::string& hash) {
    if (hashcatMode == -1) {
        return "No valid hashcat mode for detected hash type.";
    }

    // Extract wordlist, hashcat path, and other options from the config file
    std::string wordlist = config.contains("wordlist") ? config["wordlist"].get<std::string>() : "<default_wordlist>";
    std::string additionalOptions = config.contains("options") ? config["options"].get<std::string>() : "";
    std::string hashcatPath = config.contains("hashcat_path") ? config["hashcat_path"].get<std::string>() : "hashcat";

    // Construct the hashcat command with the actual hash
    return hashcatPath + " -m " + std::to_string(hashcatMode) + " '" + hash + "' " + wordlist + " " + additionalOptions;
}

void HashcatExecutor::executeHashcatCommand(const std::string& command) {
    std::cout << "Executing: " << command << std::endl;
    int result = system(command.c_str());  // Execute the hashcat command
    if (result != 0) {
        std::cerr << "Error: Hashcat command failed with exit code " << result << std::endl;
    }
}
