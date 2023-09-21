#include <catch.hpp>
#include <util.h>

#include <dither.h>
#include <cmath>

const auto kDir = GetFileDir(__FILE__) / "test";

TEST_CASE("Read image", "[image]") {
    Image small{kDir / "small.png"};
    REQUIRE(small.Height() == 512);
    REQUIRE(small.Width() == 512);
    REQUIRE(small.GetPixel(2, 2) == RGB{223, 137, 133});
    REQUIRE(small.GetPixel(0, 256) == RGB{208, 109, 108});
    REQUIRE(small.GetPixel(1, 511) == RGB{200, 99, 90});
}

TEST_CASE("Kd tree", "[tree]") {
    {
        KdTree tree{{{0, 0, 0}, {1, 2, 3}, {1, 1, 0}}};
        REQUIRE(tree.GetNearest({1, 1, 1}) == 2u);
    }
    {
        static constexpr auto kCount = 30'000;
        Points points;
        points.reserve(kCount);
        for (auto i = 0; i < kCount; ++i) {
            double val = i;
            points.push_back({val, val, 0.0});
        }
        KdTree tree{points};
        RandomGenerator rnd{47875656};
        for (auto i = 0; i < kCount; ++i) {
            auto pt = rnd.GenRealVector(3, 0, kCount);
            auto ind = tree.GetNearest(pt);
            auto b = pt[0] + pt[1];
            auto x = b / 2.0;
            auto expected = static_cast<int>(std::round(x));
            expected = std::min(expected, kCount - 1);
            REQUIRE(ind == static_cast<size_t>(expected));
        }
    }
}

TEST_CASE("Full (small)", "[full]") {
    static const std::filesystem::path kOutPath{"/tmp/small-result.png"};
    std::vector<RGB> pixels{{0, 0, 0}, {255, 255, 255}};
    Dither(kDir / "small.png", kOutPath, pixels);
    {
        Image test{kOutPath};
        std::vector<std::pair<int, int>> pos{
            {205, 441}, {400, 304}, {1, 52}, {123, 64}, {432, 328}};
        std::vector<RGB> expected{
            {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}, {255, 255, 255}};
        for (size_t i = 0; i < pos.size(); ++i) {
            REQUIRE(test.GetPixel(pos[i].first, pos[i].second) == expected[i]);
        }
    }
}

TEST_CASE("Full (middle)", "[full]") {
    static const std::filesystem::path kOutPath{"/tmp/small-result.png"};
    std::vector<RGB> pixels{{215, 198, 124}, {174, 159, 109}, {27, 24, 23},  {10, 33, 55},
                            {8, 9, 24},      {8, 8, 8},       {47, 40, 37},  {165, 176, 172},
                            {107, 89, 58},   {177, 139, 62},  {50, 73, 88},  {229, 223, 186},
                            {72, 60, 48},    {102, 120, 124}, {130, 115, 84}};
    Dither(kDir / "middle.png", kOutPath, pixels);
    {
        Image test{kOutPath};
        std::vector<std::pair<int, int>> pos{{318, 87},  {145, 263}, {6, 37},
                                             {446, 281}, {53, 366},  {465, 399}};
        std::vector<RGB> expected{{72, 60, 48}, {8, 8, 8},  {8, 9, 24},
                                  {8, 8, 8},    {8, 9, 24}, {8, 8, 8}};
        for (size_t i = 0; i < pos.size(); ++i) {
            REQUIRE(test.GetPixel(pos[i].first, pos[i].second) == expected[i]);
        }
    }
}
