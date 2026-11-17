#include "HashDetector.h"
#include "FileHandler.h"
#include "HashcatExecutor.h"
#include <iostream>
#include <stdexcept>
#include <string>

static void help() {
    std::cout << "Usage: hashcheck-rc (-i HASH | -f FILE) -c CONFIG [--mode NUMBER] [--dry-run]\n"
                 "       hashcheck-rc --help\n"
                 "Unambiguous structured hashes can be detected automatically.\n"
                 "Raw hexadecimal digests require --mode because their algorithms are ambiguous.\n";
}

int main(int argc, char** argv) {
    try {
        std::string hash, file, configPath;
        int mode = -1;
        bool explicitMode = false, dryRun = false;
        if (argc == 1) { help(); return 2; }
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") { help(); return 0; }
            if (arg == "--dry-run") { dryRun = true; continue; }
            if (arg != "-i" && arg != "-f" && arg != "-c" && arg != "--mode")
                throw std::runtime_error("Unknown argument: " + arg);
            if (++i >= argc) throw std::runtime_error("Missing value for " + arg);
            std::string value = argv[i];
            if (arg == "-i") { if (!file.empty() || !hash.empty()) throw std::runtime_error("Only one input allowed"); hash = value; }
            if (arg == "-f") { if (!file.empty() || !hash.empty()) throw std::runtime_error("Only one input allowed"); file = value; }
            if (arg == "-c") configPath = value;
            if (arg == "--mode") {
                size_t consumed = 0;
                mode = std::stoi(value, &consumed);
                if (consumed != value.size() || mode < 0) throw std::runtime_error("Invalid mode");
                explicitMode = true;
            }
        }
        if (!file.empty()) hash = FileHandler::readHashFromFile(file);
        if (hash.empty() || configPath.empty()) throw std::runtime_error("Input and -c CONFIG are required");
        auto config = FileHandler::loadConfigFromFile(configPath);
        HashDetector detector;
        int detected = -1;
        auto type = detector.detectHashType(hash, detected);
        std::cout << "Hash identification: " << type << "\n";
        if (!explicitMode) mode = detected;
        if (mode < 0) throw std::runtime_error("Cannot determine a unique mode; specify --mode explicitly");
        std::cout << "Selected Hashcat mode: " << mode << "\n";
        auto command = HashcatExecutor::generateHashcatCommand(mode, config, hash);
        if (dryRun) { std::cout << "Dry run: " << command << "\n"; return 0; }
        HashcatExecutor::executeHashcat(mode, config, hash);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
