#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <opencv2/opencv.hpp>
#include <string>

namespace ImageLoader {

    /**
     * Load an image from disk.
     *
     * Supports:
     * JPG, JPEG, PNG, BMP and other formats
     * supported by the installed OpenCV build.
     */
    cv::Mat loadImage(const std::string& imagePath);

    /**
     * Save an image to disk.
     */
    bool saveImage(
        const cv::Mat& image,
        const std::string& outputPath
    );

    /**
     * Validate whether a file exists and has a supported
     * image extension.
     */
    bool validateImagePath(const std::string& imagePath);

    /**
     * Print basic information about an image.
     */
    void printImageInfo(const cv::Mat& image);

    /**
     * Return true if the image is grayscale.
     */
    bool isGrayscale(const cv::Mat& image);

    /**
     * Convert an image into grayscale.
     *
     * Grayscale input is returned unchanged.
     * Color images are converted from BGR to grayscale.
     */
    cv::Mat toGrayscale(const cv::Mat& image);

}

#endif