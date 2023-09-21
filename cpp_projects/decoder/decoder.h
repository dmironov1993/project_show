#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cctype>
#include <random>

using Map = std::unordered_map<std::string, uint64_t>;

inline Map ReadNgrams(const std::filesystem::path& path) {
    Map map;
    std::ifstream file(path);
    std::string str;
    while (std::getline(file, str)) {
        size_t index = str.find(' ');
        std::string key;
        for (size_t i = 0; i < index; ++i) {
            key += str[i];
        }
        std::string count;
        for (size_t i = index + 1; i < str.size(); ++i) {
            if (std::isdigit(str[i])) {
                count += str[i];
            }
        }
        map[key] = std::stoi(count);
    }
    return map;
}

inline uint64_t MapCount(const Map& dict) {
    uint64_t n = 0;
    for (auto& [key, count] : dict) {
        n += count;
    }
    return n;
}

inline double Score(const std::string& text, const Map& dict, uint64_t n) {
    double score = 0.0;
    for (size_t i = 0; i < text.size(); i++) {
        std::string token = text.substr(i, 4);
        if (dict.find(token) != dict.end()) {
            score += std::log10(static_cast<double>(dict.at(token)) / static_cast<double>(n));
        } else {
            score += std::log10(static_cast<double>(0.01) / static_cast<double>(n));
        }
    }
    return score;
}

inline std::string Decode(const std::string& original_phrase, const Map& dict,
                          const std::filesystem::path& path_letters) {

    Map map;
    std::ifstream file(path_letters);
    std::string str;
    while (std::getline(file, str)) {
        size_t index = str.find(' ');
        std::string key;
        for (size_t i = 0; i < index; ++i) {
            key += str[i];
        }
        std::string count;
        for (size_t i = index + 1; i < str.size(); ++i) {
            if (std::isdigit(str[i])) {
                count += str[i];
            }
        }
        map[key] = std::stoi(count);
    }
    (void)map;

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::mt19937 gen(23);
    std::uniform_int_distribution<int> dist(0, 25);

    std::string phrase = original_phrase;
    for (size_t i = 0; i < original_phrase.size(); ++i) {
        phrase[i] = static_cast<char>(toupper(phrase[i]));
    }
    uint64_t n = MapCount(dict);
    double best_score = Score(phrase, dict, n);

    std::string initial = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::unordered_map<char, char> mapper;
    for (size_t i = 0; i < initial.size(); ++i) {
        mapper[initial[i]] = initial[i];
    }

    std::string text;
    for (size_t i = 0; i < phrase.size(); ++i) {
        if (mapper.find(phrase[i]) != mapper.end()) {
            text += mapper[phrase[i]];
        } else {
            text += phrase[i];
        }
    }

    for (size_t i = 0; i < 11000; ++i) {
        int a = dist(gen);
        int b = dist(gen);

        char original_a = mapper[initial[a]];
        char original_b = mapper[initial[b]];

        mapper[initial[a]] = original_b;
        mapper[initial[b]] = original_a;

        std::string new_string;
        for (size_t j = 0; j < text.size(); ++j) {
            if (mapper.find(text[j]) != mapper.end()) {
                new_string += mapper[text[j]];
            } else if (text[j] != ' ') {
                new_string += text[j];
            }
        }

        double new_score = Score(new_string, dict, n);
        if (new_score > best_score) {
            best_score = new_score;
        } else {
            mapper[initial[a]] = original_a;
            mapper[initial[b]] = original_b;
        }
    }

    std::string answer = original_phrase;
    for (size_t i = 0; i < phrase.size(); i++) {
        if (isalpha(original_phrase[i])) {
            if (mapper.find(phrase[i]) != mapper.end()) {
                if (islower(original_phrase[i])) {
                    answer[i] = static_cast<char>(tolower(mapper[phrase[i]]));
                } else {
                    answer[i] = static_cast<char>(mapper[phrase[i]]);
                }
            } else {
                answer[i] = static_cast<char>(original_phrase[i]);
            }
        } else {
            answer[i] = static_cast<char>(original_phrase[i]);
        }
    }
    return answer;
}
