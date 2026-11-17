#include "sequential.h"

#include "image_loader.h"
#include "histogram.h"

#include <chrono>
#include <stdexcept>

namespace Sequential {

    Result process(
        const cv::Mat& input
    ) {

        if (input.empty()) {

            throw std::runtime_error(
                "Sequential processing received an empty image."
            );
        }

        /*
         * Convert outside the timed algorithm section.
         *
         * This keeps image-format conversion separate from
         * the core Histogram Equalization measurement.
         */
        cv::Mat gray =
            ImageLoader::toGrayscale(input);


        Result result{};

        result.totalPixels =
            static_cast<long long>(
                gray.rows
            ) *
            static_cast<long long>(
                gray.cols
            );


        /*
         * Start timing only for the image-processing algorithm.
         */
        auto start =
            std::chrono::steady_clock::now();


        // Step 1: Histogram
        result.histogram =
            Histogram::calculateHistogram(
                gray
            );


        // Step 2: CDF
        result.cdf =
            Histogram::calculateCDF(
                result.histogram
            );


        // Step 3: Minimum non-zero CDF
        result.cdfMin =
            Histogram::findCDFMin(
                result.cdf
            );


        // Step 4: Intensity mapping
        result.mapping =
            Histogram::generateMapping(
                result.cdf,
                result.cdfMin,
                result.totalPixels
            );


        // Step 5: Transform pixels
        result.output =
            Histogram::applyMapping(
                gray,
                result.mapping
            );


        auto end =
            std::chrono::steady_clock::now();


        result.executionTimeMs =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();


        return result;
    }

}