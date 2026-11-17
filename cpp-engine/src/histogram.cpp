#include "histogram.h"

#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace Histogram {

    HistogramArray calculateHistogram(
        const cv::Mat& grayImage
    ) {

        if (grayImage.empty()) {
            throw std::runtime_error(
                "Cannot calculate histogram of an empty image."
            );
        }

        if (grayImage.channels() != 1) {
            throw std::runtime_error(
                "Histogram calculation requires a grayscale image."
            );
        }

        HistogramArray histogram{};
        histogram.fill(0);

        for (int row = 0; row < grayImage.rows; ++row) {

            const uchar* pixelRow =
                grayImage.ptr<uchar>(row);

            for (int col = 0; col < grayImage.cols; ++col) {

                const unsigned char intensity =
                    pixelRow[col];

                ++histogram[intensity];
            }
        }

        return histogram;
    }


    CDFArray calculateCDF(
        const HistogramArray& histogram
    ) {

        CDFArray cdf{};
        cdf.fill(0);

        cdf[0] = histogram[0];

        for (int i = 1; i < BIN_COUNT; ++i) {

            cdf[i] =
                cdf[i - 1] +
                histogram[i];
        }

        return cdf;
    }


    long long findCDFMin(
        const CDFArray& cdf
    ) {

        for (int i = 0; i < BIN_COUNT; ++i) {

            if (cdf[i] > 0) {
                return cdf[i];
            }
        }

        return 0;
    }


    MappingArray generateMapping(
        const CDFArray& cdf,
        long long cdfMin,
        long long totalPixels
    ) {

        MappingArray mapping{};
        mapping.fill(0);

        if (totalPixels <= 0) {
            return mapping;
        }

        const long long denominator =
            totalPixels - cdfMin;

        /*
         * Constant-intensity or single-value image.
         *
         * There is no meaningful range to stretch.
         */
        if (denominator <= 0) {

            for (int i = 0; i < BIN_COUNT; ++i) {

                mapping[i] =
                    static_cast<unsigned char>(i);
            }

            return mapping;
        }


        for (int i = 0; i < BIN_COUNT; ++i) {

            if (cdf[i] <= 0) {

                mapping[i] = 0;
                continue;
            }

            double normalized =
                static_cast<double>(
                    cdf[i] - cdfMin
                ) /
                static_cast<double>(
                    denominator
                );

            normalized =
                std::clamp(
                    normalized,
                    0.0,
                    1.0
                );

            double mappedValue =
                normalized * 255.0;

            /*
             * Explicit rounding policy:
             * nearest integer.
             */
            long long rounded =
                static_cast<long long>(
                    std::llround(mappedValue)
                );

            rounded =
                std::clamp(
                    rounded,
                    0LL,
                    255LL
                );

            mapping[i] =
                static_cast<unsigned char>(
                    rounded
                );
        }

        return mapping;
    }


    cv::Mat applyMapping(
        const cv::Mat& grayImage,
        const MappingArray& mapping
    ) {

        if (grayImage.empty()) {
            throw std::runtime_error(
                "Cannot apply mapping to empty image."
            );
        }

        if (grayImage.channels() != 1) {
            throw std::runtime_error(
                "Mapping requires a grayscale image."
            );
        }

        cv::Mat output(
            grayImage.size(),
            grayImage.type()
        );

        for (int row = 0; row < grayImage.rows; ++row) {

            const uchar* inputRow =
                grayImage.ptr<uchar>(row);

            uchar* outputRow =
                output.ptr<uchar>(row);

            for (int col = 0;
                 col < grayImage.cols;
                 ++col) {

                outputRow[col] =
                    mapping[inputRow[col]];
            }
        }

        return output;
    }


    bool validateHistogram(
        const HistogramArray& histogram,
        long long expectedPixels
    ) {

        if (expectedPixels < 0) {
            return false;
        }

        long long sum = 0;

        for (long long value : histogram) {

            if (value < 0) {
                return false;
            }

            sum += value;
        }

        return sum == expectedPixels;
    }


    bool validateCDF(
        const CDFArray& cdf
    ) {

        if (cdf[0] < 0) {
            return false;
        }

        for (int i = 1; i < BIN_COUNT; ++i) {

            if (cdf[i] < cdf[i - 1]) {
                return false;
            }
        }

        return true;
    }

}