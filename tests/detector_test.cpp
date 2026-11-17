#include "HashDetector.h"
#include <cassert>
#include <string>

int main() {
    HashDetector detector;
    int mode = 123;
    assert(detector.detectHashType("$2b$12$" + std::string(53, 'a'), mode) == "bcrypt" && mode == 3200);
    assert(detector.detectHashType("$2b$12$short", mode) == "Unknown" && mode == -1);
    assert(detector.detectHashType(std::string(32, 'a'), mode).find("Ambiguous") == 0 && mode == -1);
    assert(detector.detectHashType(std::string(64, 'b'), mode).find("Ambiguous") == 0 && mode == -1);
    assert(detector.detectHashType("$1$salt1234$abcdefghijklmnopqrstuv", mode) == "MD5 crypt" && mode == 500);
    assert(detector.detectHashType("invalid", mode) == "Unknown" && mode == -1);
}
