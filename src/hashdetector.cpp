#include "HashDetector.h"

HashDetector::HashDetector() {
    // Initialize hash types with characteristics and hashcat modes
    hashTypes = {
        {"Adler32", 8, std::regex("^[0-9A-Fa-f]{8}$"), "", 14100},
        {"Argon2", 96, std::regex("^\\$argon2[di]?[A-Za-z0-9$=,]+$"), "$argon2", 13300},
        {"bcrypt", 60, std::regex("^\\$2[aby]\\$[0-9]{2}\\$[./A-Za-z0-9]{53}$"), "$2", 3200},
        {"CRC32", 8, std::regex("^[0-9A-Fa-f]{8}$"), "", 11500},
        {"LM", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 3000},
        {"MD2", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 800},
        {"MD4", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 900},
        {"MD5", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 0},
        {"MSCACHEv1", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 1100},
        {"MSCACHEv2", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 2100},
        {"NetNTLMv1", 48, std::regex("^[0-9A-Fa-f]{48}$"), "", 5500},
        {"NetNTLMv2", 64, std::regex("^[0-9A-Fa-f]{64}$"), "", 5600},
        {"NTLM", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 1000},
        {"PBKDF2-HMAC-SHA1", 160, std::regex("^\\$pbkdf2-sha1\\$[0-9]+\\$[A-Za-z0-9+/=]+\\$[A-Za-z0-9+/=]+$"), "$pbkdf2", 12001},
        {"PBKDF2-HMAC-SHA256", 256, std::regex("^\\$pbkdf2-sha256\\$[0-9]+\\$[A-Za-z0-9+/=]+\\$[A-Za-z0-9+/=]+$"), "$pbkdf2", 10900},
        {"PBKDF2-HMAC-SHA512", 512, std::regex("^\\$pbkdf2-sha512\\$[0-9]+\\$[A-Za-z0-9+/=]+\\$[A-Za-z0-9+/=]+$"), "$pbkdf2", 12100},
        {"RIPEMD-160", 40, std::regex("^[0-9A-Fa-f]{40}$"), "", 6000},
        {"scrypt", 64, std::regex("^\\$scrypt\\$[0-9]+\\$[A-Za-z0-9+/=]+\\$[A-Za-z0-9+/=]+$"), "$scrypt", 8900},
        {"SHA1", 40, std::regex("^[0-9a-fA-F]{40}$"), "", 100},
        {"SHA224", 56, std::regex("^[0-9a-fA-F]{56}$"), "", 1300},
        {"SHA256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 1400},
        {"SHA384", 96, std::regex("^[0-9a-fA-F]{96}$"), "", 10800},
        {"SHA512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 1700},
        {"SHA3-224", 56, std::regex("^[0-9a-fA-F]{56}$"), "", 17300},
        {"SHA3-256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 17600},
        {"SHA3-384", 96, std::regex("^[0-9a-fA-F]{96}$"), "", 17400},
        {"SHA3-512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 17800},
        {"WPA-PMK", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 2500},
        {"WPA-PMKID", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 16800},
    };
}


std::string HashDetector::detectHashType(const std::string& hash, int &hashcatMode) {
    // Loop through all known hash types
    for (const auto& hashType : hashTypes) {
        if (hash.length() == hashType.length) {
            // Check prefix if defined
            if (!hashType.prefix.empty() && hash.rfind(hashType.prefix, 0) == 0) {
                hashcatMode = hashType.hashcatMode;
                return hashType.name;
            }
            // Check regex pattern match
            if (std::regex_match(hash, hashType.pattern)) {
                hashcatMode = hashType.hashcatMode;
                return hashType.name;
            }
        }
    }
    hashcatMode = -1;
    return "Unknown";
}
