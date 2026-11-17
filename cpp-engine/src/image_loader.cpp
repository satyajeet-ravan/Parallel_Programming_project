#include "image_loader.h"

#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace fs = std::filesystem;

namespace ImageLoader {

    bool validateImagePath(const std::string& imagePath) {

        if (imagePath.empty()) {
            return false;
        }

        fs::path path(imagePath);

        if (!fs::exists(path)) {
            return false;
        }

        if (!fs::is_regular_file(path)) {
            return false;
        }

        std::string extension = path.extension().string();

        std::transform(
            extension.begin(),
            extension.end(),
            extension.begin(),
            [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            }
        );

        return (
            extension == ".jpg" ||
            extension == ".jpeg" ||
            extension == ".png" ||
            extension == ".bmp"
        );
    }


    cv::Mat loadImage(const std::string& imagePath) {

        if (!validateImagePath(imagePath)) {
            throw std::runtime_error(
                "Invalid or unsupported image path: " + imagePath
            );
        }

        cv::Mat image = cv::imread(
            imagePath,
            cv::IMREAD_UNCHANGED
        );

        if (image.empty()) {
            throw std::runtime_error(
                "Failed to load image: " + imagePath
            );
        }

        return image;
    }


    bool saveImage(
        const cv::Mat& image,
        const std::string& outputPath
    ) {

        if (image.empty()) {
            return false;
        }

        try {

            fs::path outputFile(outputPath);

            if (outputFile.has_parent_path()) {
                fs::create_directories(
                    outputFile.parent_path()
                );
            }

            return cv::imwrite(
                outputPath,
                image
            );

        } catch (const cv::Exception&) {

            return false;

        } catch (const fs::filesystem_error&) {

            return false;
        }
    }


    void printImageInfo(const cv::Mat& image) {

        if (image.empty()) {
            std::cerr << "Image is empty.\n";
            return;
        }

        std::cout
            << "Width      : " << image.cols << '\n'
            << "Height     : " << image.rows << '\n'
            << "Channels   : " << image.channels() << '\n'
            << "Data type  : " << image.type() << '\n'
            << "Total pixels: "
            << static_cast<long long>(image.rows) *
               static_cast<long long>(image.cols)
            << '\n';
    }


    bool isGrayscale(const cv::Mat& image) {

        if (image.empty()) {
            return false;
        }

        return image.channels() == 1;
    }


    cv::Mat toGrayscale(const cv::Mat& image) {

        if (image.empty()) {
            throw std::runtime_error(
                "Cannot convert an empty image to grayscale."
            );
        }

        if (image.channels() == 1) {
            return image.clone();
        }

        cv::Mat gray;

        if (image.channels() == 3) {

            cv::cvtColor(
                image,
                gray,
                cv::COLOR_BGR2GRAY
            );

        } else if (image.channels() == 4) {

            cv::cvtColor(
                image,
                gray,
                cv::COLOR_BGRA2GRAY
            );

        } else {

            throw std::runtime_error(
                "Unsupported number of image channels."
            );
        }

        return gray;
    }

}