#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <cstdio>

class FileHandler {
public:
    static std::string readHashFromFile(const std::string& filePath);
    static rapidjson::Document loadConfigFromFile(const std::string& configFilePath);
};

#endif

