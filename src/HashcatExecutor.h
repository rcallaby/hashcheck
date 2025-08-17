#ifndef HASHCAT_EXECUTOR_H
#define HASHCAT_EXECUTOR_H

#include <string>
#include <rapidjson/document.h>

class HashcatExecutor {
public:
    static std::string generateHashcatCommand(int hashcatMode, const rapidjson::Value& config);
    static void executeHashcatCommand(const std::string& command);
};

#endif
