#include <iostream>

void printHelp() {
    std::cout << "Usage: hashcheck [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help                  Show this help message and exit\n";
    std::cout << "  -f, --file <file>           Specify the file containing the hash\n";
    std::cout << "  -t, --type <type>           Specify the hash type (e.g., MD5, SHA256, etc.)\n";
    std::cout << "  -w, --wordlist <path>       Path to the wordlist file\n";
    std::cout << "  -c, --config <file>         Specify a config JSON file for custom settings\n";
    std::cout << "  -o, --output <file>         Save results to the specified file\n";
    std::cout << "  -v, --verbose               Enable verbose output\n";
    std::cout << "  --version                   Show the version of hashcheck\n\n";
    std::cout << "Examples:\n";
    std::cout << "  hashcheck -f hash.txt -w rockyou.txt\n";
    std::cout << "  hashcheck --file hash.txt --config config.json\n";
}
