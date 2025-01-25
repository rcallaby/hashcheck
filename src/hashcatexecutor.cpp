#include "HashcatExecutor.h"
#include <iostream>
#include <cstdlib>  // For system() function
#include <rapidjson/document.h>

std::string HashcatExecutor::generateHashcatCommand(int hashcatMode, const rapidjson::Document& config, const std::string& hash) {
    if (hashcatMode == -1) {
        return "No valid hashcat mode for detected hash type.";
    }

    // Extract wordlist, hashcat path, and other options from the config file
    std::string wordlist = config.HasMember("wordlist") && config["wordlist"].IsString()
        ? config["wordlist"].GetString()
        : "<default_wordlist>";

    std::string additionalOptions = config.HasMember("options") && config["options"].IsString()
        ? config["options"].GetString()
        : "";

    std::string hashcatPath = config.HasMember("hashcat_path") && config["hashcat_path"].IsString()
        ? config["hashcat_path"].GetString()
        : "hashcat";

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
