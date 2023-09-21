#include <random>
#include <iostream>
#include <stdlib.h>
#include <numeric>
#include <limits>
#include <vector>
#include <cstdlib>
#include <fstream>

#define MAX 344
#define POWER 3225807

const std::string kAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const unsigned kMAX = std::numeric_limits<unsigned>::max();
const std::string kPassword = "NLXGI4NoAp";

void GenString(std::string* s) {
    for (auto& c : *s) {
        c = kAlphabet[std::rand() % kAlphabet.size()];
    }
}

bool ReadString(std::string* line, size_t max_length) {
    line->clear();
    while (std::cin && (line->size() <= max_length)) {
        if (auto symbol = std::cin.get(); symbol != '\n') {
            line->push_back(symbol);
        } else {
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "RUNNING ..." << '\n';

    std::vector<std::vector<int>> matrix = {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                                             0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                             0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                             1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                                             0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                             0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                             1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                                             0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                             0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                             1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                                             0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                             0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0},
                                            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                             1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0},
                                            {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                                             0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1},
                                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                             0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1}};

    std::vector<std::vector<int>> matrix_pow = matrix;
    for (int p = 1; p < POWER; ++p) {
        std::vector<std::vector<int>> temp(31, std::vector<int>(31, 0));
        for (int i = 0; i < 31; ++i) {
            for (int j = 0; j < 31; ++j) {
                int number = 0;
                for (int k = 0; k < 31; ++k) {
                    number += matrix_pow[i][k] * matrix[k][j];
                }
                temp[i][j] = number;
            }
        }
        matrix_pow = temp;
        std::cout << "POWER: " << p << " DONE" << '\n';
    }

    std::cout << '\n';
    std::cout << '\n';
    std::cout << "MATRIX HAS BEEN CALCULATED" << '\n';
    std::cout << '\n';
    std::cout << '\n';

    unsigned seed;
    std::string res;
    for (seed = 1; seed < kMAX; ++seed) {

        int r[MAX];
        r[0] = seed;
        for (int i = 1; i < 31; i++) {
            r[i] = (16807LL * r[i - 1]) % 2147483647;
            if (r[i] < 0) {
                r[i] += 2147483647;
            }
        }
        for (int i = 31; i < 34; i++) {
            r[i] = r[i - 31];
        }
        for (int i = 34; i < 344; i++) {
            r[i] = r[i - 31] + r[i - 3];
        }

        std::vector<int64_t> output(31, 0);
        for (int i = 0; i < 31; ++i) {
            int number = 0;
            for (int j = 0; j < 31; ++j) {
                number += matrix_pow[i][j] * r[313 + j];
            }
            output[i] = static_cast<unsigned int>(number) >> 1;
        }

        std::cout << '\n';
        std::cout << '\n';

        std::string ans;
        for (int i = 4; i < 14; ++i) {
            ans += kAlphabet[output[i] % kAlphabet.size()];
        }
        if (ans == kPassword) {
            res = ans;
            std::cout << "FOUND!!!!" << '\n';
            std::cout << "SEED = " << seed << '\n';
            std::cout << "ANSWER = " << ans << " PASSWORD = " << kPassword << '\n';
            break;
        }
        std::cout << "SEED = " << seed << " is done" << '\n';
    }

    std::srand(seed);

    std::string password(10, 'a');
    for (auto i = 0; i < 10'000'000; ++i) {
        GenString(&password);
    }

    if (password != "NLXGI4NoAp") {
        std::cout << "Wrong seed\n";
        return 0;
    }

    GenString(&password);
    std::string guess = password;

    std::cout << guess << '\n';
    std::ofstream myfile;
    myfile.open("/home/denis/Desktop/denis.mironov7/bad-rand/input.txt");
    myfile << guess << '\n';
    myfile.close();

    return 0;
}
