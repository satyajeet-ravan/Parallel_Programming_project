#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <array>
#include <vector>
#include <opencv2/opencv.hpp>

namespace Histogram {

    constexpr int BIN_COUNT = 256;

    using HistogramArray = std::array<long long, BIN_COUNT>;
    using CDFArray = std::array<long long, BIN_COUNT>;
    using MappingArray = std::array<unsigned char, BIN_COUNT>;

    /**
     * Calculate a 256-bin grayscale histogram.
     */
    HistogramArray calculateHistogram(
        const cv::Mat& grayImage
    );

    /**
     * Calculate cumulative distribution function.
     */
    CDFArray calculateCDF(
        const HistogramArray& histogram
    );

    /**
     * Find the first non-zero CDF value.
     */
    long long findCDFMin(
        const CDFArray& cdf
    );

    /**
     * Generate the intensity transformation mapping.
     *
     * T(k) =
     * round(((CDF(k) - CDF_min) /
     *       (N - CDF_min)) * 255)
     */
    MappingArray generateMapping(
        const CDFArray& cdf,
        long long cdfMin,
        long long totalPixels
    );

    /**
     * Apply an intensity mapping to a grayscale image.
     */
    cv::Mat applyMapping(
        const cv::Mat& grayImage,
        const MappingArray& mapping
    );

    /**
     * Check whether a histogram is valid.
     */
    bool validateHistogram(
        const HistogramArray& histogram,
        long long expectedPixels
    );

    /**
     * Check whether a CDF is non-decreasing.
     */
    bool validateCDF(
        const CDFArray& cdf
    );

}

#endif