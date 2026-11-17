#include <cstdlib>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    const char* path = std::getenv("HASHCHECK_TEST_LOG");
    if (!path) return 90;
    std::ofstream out(path, std::ios::binary);
    if (!out) return 91;
    for (int i = 1; i < argc; ++i) out << std::string(argv[i]).size() << ':' << argv[i];
    out.close();
    const char* code = std::getenv("HASHCHECK_TEST_EXIT");
    return code ? std::atoi(code) : 0;
}
