#include "FileHandler.h"
#include <fstream>
#include <sstream>

std::string FileHandler::readHashFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

rapidjson::Document FileHandler::loadConfigFromFile(const std::string& configFilePath) {
    rapidjson::Document doc;
    FILE* fp = fopen(configFilePath.c_str(), "rb");
    if (!fp) return doc;
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(is);
    fclose(fp);
    return doc;
}


