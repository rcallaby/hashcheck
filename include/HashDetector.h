#ifndef HASH_DETECTOR_H
#define HASH_DETECTOR_H

#include <string>
#include <regex>
#include <vector>

struct HashInfo {
    std::string name;
    int length;
    std::regex pattern;
    std::string prefix;
    int hashcatMode;  // Hashcat mode for the specific hash type
};

class HashDetector {
public:
    HashDetector();
    std::string detectHashType(const std::string& hash, int &hashcatMode);

private:
    std::vector<HashInfo> hashTypes;
};

#endif
