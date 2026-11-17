#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <rapidjson/error/en.h>

std::string FileHandler::readHashFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open hash file: " + filePath);
    }
    std::string hash;
    if (!(file >> hash)) throw std::runtime_error("Hash file is empty: " + filePath);
    if (hash.size() > 16384) throw std::runtime_error("Hash exceeds maximum length");
    std::string extra;
    if (file >> extra) throw std::runtime_error("Hash file must contain exactly one hash");
    return hash;
}

rapidjson::Document FileHandler::loadConfigFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonStr = buffer.str();

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error(
            std::string("JSON parse error: ") +
            rapidjson::GetParseError_En(doc.GetParseError()) +
            " (offset " + std::to_string(doc.GetErrorOffset()) + ")"
        );
    }

    return doc;
}



