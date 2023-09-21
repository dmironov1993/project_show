#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

#include <image.h>
#include <kd_tree.h>
#include <cmath>

struct RGBdouble {
    double r = 0., g = 0., b = 0.;
};

void Dither(const std::filesystem::path& input_file, const std::filesystem::path& output_file,
            const std::vector<RGB>& ok_pixels) {

    std::vector<std::vector<double>> points;
    for (auto pixels : ok_pixels) {
        double dim01 = static_cast<double>(pixels.r);
        double dim02 = static_cast<double>(pixels.g);
        double dim03 = static_cast<double>(pixels.b);
        points.push_back({dim01, dim02, dim03});
    }
    KdTree tree{points};

    Image image(input_file);
    int height = image.Height();
    int width = image.Width();

    std::vector<std::vector<RGBdouble>> matrix(height, std::vector<RGBdouble>(width, {0., 0., 0.}));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGB rgb = image.GetPixel(y, x);
            double dim01 = static_cast<double>(rgb.r);
            double dim02 = static_cast<double>(rgb.g);
            double dim03 = static_cast<double>(rgb.b);
            matrix[y][x] = {dim01, dim02, dim03};
        }
    }

    std::vector<RGBdouble> ok_pixels_double;
    for (auto& element : ok_pixels) {
        double red = static_cast<double>(element.r);
        double green = static_cast<double>(element.g);
        double blue = static_cast<double>(element.b);
        // ok_pixels_double.push_back({std::min(red, red / 255.),
        //                            std::min(green, green / 255.),
        //                            std::min(blue, blue / 255.)});
        ok_pixels_double.push_back({red, green, blue});
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGBdouble rgb = matrix[y][x];
            // std::vector<double> oldpixel = {std::min(rgb.r, rgb.r / 255.),
            //                                std::min(rgb.g, rgb.g / 255.),
            //                                std::min(rgb.b, rgb.b / 255.)};
            std::vector<double> oldpixel = {rgb.r, rgb.g, rgb.b};
            int index = tree.GetNearest(oldpixel);
            RGBdouble newpixel = ok_pixels_double[index];
            image.SetPixel(ok_pixels[index], y, x);
            matrix[y][x] = {newpixel.r, newpixel.g, newpixel.b};
            std::vector<double> quant_error = {oldpixel[0] - matrix[y][x].r,
                                               oldpixel[1] - matrix[y][x].g,
                                               oldpixel[2] - matrix[y][x].b};

            // (x + 1, y)
            if (x + 1 < width) {
                RGBdouble rgb01 = matrix[y][x + 1];
                rgb01.r = rgb01.r + quant_error[0] * 7.0 / 16.;
                rgb01.g = rgb01.g + quant_error[1] * 7.0 / 16.;
                rgb01.b = rgb01.b + quant_error[2] * 7.0 / 16.;
                matrix[y][x + 1] = {rgb01.r, rgb01.g, rgb01.b};
            }

            // (x + 1, y + 1)
            if (y + 1 < height && x + 1 < width) {
                RGBdouble rgb04 = matrix[y + 1][x + 1];
                rgb04.r = rgb04.r + quant_error[0] * 1.0 / 16.;
                rgb04.g = rgb04.g + quant_error[1] * 1.0 / 16.;
                rgb04.b = rgb04.b + quant_error[2] * 1.0 / 16.;
                matrix[y + 1][x + 1] = {rgb04.r, rgb04.g, rgb04.b};
            }

            // (x, y + 1)
            if (y + 1 < height) {
                RGBdouble rgb03 = matrix[y + 1][x];
                rgb03.r = rgb03.r + quant_error[0] * 5.0 / 16.;
                rgb03.g = rgb03.g + quant_error[1] * 5.0 / 16.;
                rgb03.b = rgb03.b + quant_error[2] * 5.0 / 16.;
                matrix[y + 1][x] = {rgb03.r, rgb03.g, rgb03.b};
            }

            // (x - 1, y + 1)
            if (y + 1 < height && x - 1 >= 0) {
                RGBdouble rgb02 = matrix[y + 1][x - 1];
                rgb02.r = rgb02.r + quant_error[0] * 3.0 / 16.;
                rgb02.g = rgb02.g + quant_error[1] * 3.0 / 16.;
                rgb02.b = rgb02.b + quant_error[2] * 3.0 / 16.;
                matrix[y + 1][x - 1] = {rgb02.r, rgb02.g, rgb02.b};
            }
        }
    }

    image.Write(output_file);
}
