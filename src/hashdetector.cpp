#include "include\HashcatExecutor.h"

HashDetector::HashDetector() {
    // Initialize hash types with characteristics and hashcat modes
    hashTypes = {
        {"MD5", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 0},
        {"MD4", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 900},
        {"NTLM", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 1000},
        {"SHA1", 40, std::regex("^[0-9a-fA-F]{40}$"), "", 100},
        {"SHA224", 56, std::regex("^[0-9a-fA-F]{56}$"), "", 1300},
        {"SHA256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 1400},
        {"SHA384", 96, std::regex("^[0-9a-fA-F]{96}$"), "", 10800},
        {"SHA512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 1700},
        {"SHA3-256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 17600},
        {"SHA3-512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 17800},
        {"NTLMv2", 64, std::regex("^[0-9A-Fa-f]{64}$"), "", 5600},
        {"bcrypt", 60, std::regex("^[./A-Za-z0-9]{53}$"), "$2", 3200},
        {"scrypt", 64, std::regex("^[a-zA-Z0-9+/=]+$"), "", 8900},
        {"Argon2", 96, std::regex("^[a-zA-Z0-9+/=]{96}$"), "$argon2", 13300},
        {"CRC32", 8, std::regex("^[0-9A-Fa-f]{8}$"), "", 11500}
    };
}

std::string HashDetector::detectHashType(const std::string& hash, int &hashcatMode) {
    for (const auto& hashType : hashTypes) {
        if (hash.length() == hashType.length) {
            if (!hashType.prefix.empty() && hash.rfind(hashType.prefix, 0) == 0) {
                hashcatMode = hashType.hashcatMode;
                return hashType.name;  // Prefix match found
            }
            if (std::regex_match(hash, hashType.pattern)) {
                hashcatMode = hashType.hashcatMode;
                return hashType.name;  // Regex pattern match found
            }
        }
    }
    hashcatMode = -1;
    return "Unknown";  // No match found
}
