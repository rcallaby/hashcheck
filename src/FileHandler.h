#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <nlohmann/json.hpp>

class FileHandler {
public:
    static std::string readHashFromFile(const std::string& filePath);
    static nlohmann::json loadConfigFromFile(const std::string& configFilePath);
};

#endif
