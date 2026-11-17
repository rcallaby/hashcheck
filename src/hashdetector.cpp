#include "HashDetector.h"

HashDetector::HashDetector() {
    hashTypes = {
        {"bcrypt", 60, std::regex(R"(^\$2[aby]\$[0-9]{2}\$[./A-Za-z0-9]{53}$)"), "", 3200},
        {"MD5 crypt", 0, std::regex(R"(^\$1\$[./A-Za-z0-9]{1,8}\$[./A-Za-z0-9]{22}$)"), "", 500},
        {"SHA256 crypt", 0, std::regex(R"(^\$5\$(rounds=[0-9]+\$)?[./A-Za-z0-9]{1,16}\$[./A-Za-z0-9]{43}$)"), "", 7400},
        {"SHA512 crypt", 0, std::regex(R"(^\$6\$(rounds=[0-9]+\$)?[./A-Za-z0-9]{1,16}\$[./A-Za-z0-9]{86}$)"), "", 1800}
    };
}

std::string HashDetector::detectHashType(const std::string& hash, int& hashcatMode) {
    hashcatMode = -1;
    for (const auto& type : hashTypes) {
        if ((type.length == 0 || hash.size() == static_cast<size_t>(type.length)) &&
            std::regex_match(hash, type.pattern)) {
            hashcatMode = type.hashcatMode;
            return type.name;
        }
    }
    if (std::regex_match(hash, std::regex(R"(^[0-9a-fA-F]{32}$)"))) return "Ambiguous 32-character hexadecimal digest";
    if (std::regex_match(hash, std::regex(R"(^[0-9a-fA-F]{40}$)"))) return "Ambiguous 40-character hexadecimal digest";
    if (std::regex_match(hash, std::regex(R"(^[0-9a-fA-F]{56}$)"))) return "Ambiguous 56-character hexadecimal digest";
    if (std::regex_match(hash, std::regex(R"(^[0-9a-fA-F]{64}$)"))) return "Ambiguous 64-character hexadecimal digest";
    if (std::regex_match(hash, std::regex(R"(^[0-9a-fA-F]{96}$)"))) return "Ambiguous 96-character hexadecimal digest";
    if (std::regex_match(hash, std::regex(R"(^[0-9a-fA-F]{128}$)"))) return "Ambiguous 128-character hexadecimal digest";
    return "Unknown";
}
