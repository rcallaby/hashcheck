#include "HashDetector.h"

HashDetector::HashDetector() {
    // Initialize hash types with characteristics and hashcat modes
        hashTypes = {
        {"Argon2", 96, std::regex("^\$argon2(id|i|d)?\$[A-Za-z0-9$=,]+$"), "$argon2", 34000},
        {"BLAKE2b-512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 600},
        {"BLAKE2s-256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 31000},
        {"bcrypt", 60, std::regex("^\$2[aby]\$[0-9]{2}\$[./A-Za-z0-9]{53}$"), "$2", 3200},
        {"CHAP", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 4800},
        {"CRC32", 8, std::regex("^[0-9A-Fa-f]{8}$"), "", 11500},
        {"DES crypt", 13, std::regex("^[./0-9A-Za-z]{13}$"), "", 1500},
        {"GOST R 34.11-94", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 6900},
        {"GOST R 34.11-2012 (Streebog-256)", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 11700},
        {"GOST R 34.11-2012 (Streebog-512)", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 11800},
        {"Kerberos 5 etype 23", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 7500},
        {"Kerberos 5 etype 18 (AES256-CTS-HMAC-SHA1-96)", 64, std::regex("^[0-9A-Fa-f]{64}$"), "", 19700},
        {"LM", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 3000},
        {"MD4", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 900},
        {"MD5", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 0},
        {"MD5 crypt", 34, std::regex("^\$1\$[./A-Za-z0-9]{1,8}\$[./A-Za-z0-9]{22}$"), "$1$", 500},
        {"MSCACHEv1", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 1100},
        {"MSCACHEv2", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 2100},
        {"NetNTLMv1", 48, std::regex("^[0-9A-Fa-f]{48}$"), "", 5500},
        {"NetNTLMv2", 64, std::regex("^[0-9A-Fa-f]{64}$"), "", 5600},
        {"NTLM", 32, std::regex("^[0-9A-Fa-f]{32}$"), "", 1000},
        {"PBKDF2-HMAC-SHA1", 160, std::regex("^\$pbkdf2-sha1\$[0-9]+\$[A-Za-z0-9+/=]+\$[A-Za-z0-9+/=]+$"), "$pbkdf2", 12000},
        {"PBKDF2-HMAC-SHA256", 256, std::regex("^\$pbkdf2-sha256\$[0-9]+\$[A-Za-z0-9+/=]+\$[A-Za-z0-9+/=]+$"), "$pbkdf2", 10900},
        {"PBKDF2-HMAC-SHA512", 512, std::regex("^\$pbkdf2-sha512\$[0-9]+\$[A-Za-z0-9+/=]+\$[A-Za-z0-9+/=]+$"), "$pbkdf2", 12100},
        {"RIPEMD-160", 40, std::regex("^[0-9A-Fa-f]{40}$"), "", 6000},
        {"scrypt", 64, std::regex("^\$scrypt\$[0-9]+\$[A-Za-z0-9+/=]+\$[A-Za-z0-9+/=]+$"), "$scrypt", 8900},
        {"SHA1", 40, std::regex("^[0-9a-fA-F]{40}$"), "", 100},
        {"SHA224", 56, std::regex("^[0-9a-fA-F]{56}$"), "", 1300},
        {"SHA256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 1400},
        {"SHA384", 96, std::regex("^[0-9a-fA-F]{96}$"), "", 10800},
        {"SHA512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 1700},
        {"SHA3-224", 56, std::regex("^[0-9a-fA-F]{56}$"), "", 17300},
        {"SHA3-256", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 17400},
        {"SHA3-384", 96, std::regex("^[0-9a-fA-F]{96}$"), "", 17500},
        {"SHA3-512", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 17600},
        {"Whirlpool", 128, std::regex("^[0-9a-fA-F]{128}$"), "", 6100},
        {"WPA-PMK", 64, std::regex("^[0-9a-fA-F]{64}$"), "", 2501},
        {"WPA-PMKID", 32, std::regex("^[0-9a-fA-F]{32}$"), "", 16801}
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
