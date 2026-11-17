#include "HashcatExecutor.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#ifdef _WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#endif

static std::string requiredString(const rapidjson::Document& config, const char* key) {
    if (!config.IsObject() || !config.HasMember(key) || !config[key].IsString() || !*config[key].GetString())
        throw std::runtime_error(std::string("Missing or invalid configuration key: ") + key);
    return config[key].GetString();
}

static std::vector<std::string> arguments(int mode, const rapidjson::Document& config, const std::string& hash) {
    if (mode < 0 || hash.empty() || hash.size() > 16384 || hash.find('\n') != std::string::npos || hash.find('\r') != std::string::npos || hash.find('\0') != std::string::npos)
        throw std::runtime_error("Invalid hash or Hashcat mode");
    auto executable = requiredString(config, "hashcat_path");
    auto wordlist = requiredString(config, "wordlist");
    if (executable.find('\0') != std::string::npos || wordlist.find('\0') != std::string::npos)
        throw std::runtime_error("NUL byte in configuration path");
    if (!std::filesystem::is_regular_file(wordlist))
        throw std::runtime_error("Wordlist is not a readable regular file: " + wordlist);
    // -- ends option parsing; use a temporary file for hashes starting with '-'.
    // Hashcat accepts a hash as a positional argument; reject option-like values.
    if (hash[0] == '-') throw std::runtime_error("Hash must not begin with '-'");
    return {executable, "-m", std::to_string(mode), "-a", "0", hash, wordlist};
}

std::string HashcatExecutor::generateHashcatCommand(int mode, const rapidjson::Document& config, const std::string& hash) {
    auto args = arguments(mode, config, hash);
    std::string result;
    for (const auto& arg : args) {
        if (!result.empty()) result += " ";
        result += "[" + arg + "]";
    }
    return result; // Display only; never feed this representation to a shell.
}

void HashcatExecutor::executeHashcatCommand(const std::string&) {
    throw std::runtime_error("Shell command execution disabled; use executeHashcat instead");
}

void HashcatExecutor::executeHashcat(int mode, const rapidjson::Document& config, const std::string& hash) {
    auto args = arguments(mode, config, hash);
    std::vector<char*> argv;
    for (auto& s : args) argv.push_back(s.data());
    argv.push_back(nullptr);
#ifdef _WIN32
    int result = _spawnvp(_P_WAIT, argv[0], argv.data());
    if (result == -1) throw std::runtime_error("Unable to launch hashcat");
    // Hashcat: 0=cracked, 1=exhausted, 2=aborted; other codes indicate errors.
    if (result != 0) throw std::runtime_error("hashcat exited with status " + std::to_string(result));
#else
    pid_t pid = fork();
    if (pid < 0) throw std::runtime_error("fork failed");
    if (pid == 0) { execvp(argv[0], argv.data()); _exit(127); }
    int status = 0;
    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) throw std::runtime_error("waitpid failed");
    }
    if (WIFSIGNALED(status))
        throw std::runtime_error("hashcat terminated by signal " + std::to_string(WTERMSIG(status)));
    if (!WIFEXITED(status)) throw std::runtime_error("hashcat did not exit normally");
    if (WEXITSTATUS(status) != 0)
        throw std::runtime_error("hashcat exited with status " + std::to_string(WEXITSTATUS(status)));
#endif
}
