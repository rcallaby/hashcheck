#ifndef HASHCAT_EXECUTOR_H
#define HASHCAT_EXECUTOR_H

#include <string>
#include <nlohmann/json.hpp>

class HashcatExecutor {
public:
    static std::string generateHashcatCommand(int hashcatMode, const nlohmann::json& config);
    static void executeHashcatCommand(const std::string& command);
};

#endif
