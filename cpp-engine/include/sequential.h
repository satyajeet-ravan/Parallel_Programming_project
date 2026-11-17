#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include <opencv2/opencv.hpp>
#include <array>

#include "histogram.h"

namespace Sequential {

    struct Result {

        cv::Mat output;

        Histogram::HistogramArray histogram;

        Histogram::CDFArray cdf;

        Histogram::MappingArray mapping;

        long long cdfMin;

        long long totalPixels;

        double executionTimeMs;
    };


    /**
     * Perform sequential grayscale Histogram Equalization.
     *
     * Only the actual processing algorithm is timed.
     * Image loading and saving happen outside this function.
     */
    Result process(
        const cv::Mat& input
    );

}

#endif