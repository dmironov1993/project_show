#include <catch.hpp>
#include <decoder.h>

#include <string>
#include <fstream>

std::string ReadText(const std::filesystem::path& path) {
    std::ifstream in{path};
    if (!in) {
        throw std::runtime_error{"Can't open file: " + path.string()};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

/* static auto dictionary = ReadNgrams("../decoder/english_quadgrams.txt"); */

TEST_CASE("Your test") {
}
