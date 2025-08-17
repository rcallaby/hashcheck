#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <rapidjson/document.h>

class FileHandler {
public:
    static std::string readHashFromFile(const std::string& filePath);
    static rapidjson::Document loadConfigFromFile(const std::string& filePath);
};

#endif


