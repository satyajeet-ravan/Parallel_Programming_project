#ifndef PARALLEL_H
#define PARALLEL_H

#include <opencv2/opencv.hpp>
#include <array>

#include "histogram.h"

namespace Parallel {

    struct Result {

        cv::Mat output;

        Histogram::HistogramArray histogram;

        Histogram::CDFArray cdf;

        Histogram::MappingArray mapping;

        long long cdfMin;

        long long totalPixels;

        int threadCount;

        double executionTimeMs;
    };


    Result process(
        const cv::Mat& input,
        int threadCount
    );

}

#endif