#include "parallel.h"
#include "image_loader.h"
#include "histogram.h"

#include <omp.h>

#include <chrono>
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace Parallel {

    Result process(
        const cv::Mat& input,
        int threadCount
    ) {

        if (input.empty()) {
            throw std::runtime_error(
                "Parallel processing received an empty image."
            );
        }

        if (threadCount < 1 ||
            threadCount > omp_get_num_procs()) {

            throw std::invalid_argument(
                "Invalid thread count."
            );
        }

        // Convert input to grayscale before timing.
        cv::Mat gray =
            ImageLoader::toGrayscale(input);

        const int rows = gray.rows;
        const int cols = gray.cols;

        const long long totalPixels =
            static_cast<long long>(rows) * cols;

        Result result{};

        result.totalPixels = totalPixels;
        result.threadCount = threadCount;

        Histogram::HistogramArray globalHistogram{};
        globalHistogram.fill(0);

        // Disable dynamic adjustment for reproducibility.
        omp_set_dynamic(0);

        // Start algorithm timing.
        auto start =
            std::chrono::steady_clock::now();

        // -----------------------------------------
        // Stage 1: Parallel histogram calculation
        // -----------------------------------------

        std::vector<Histogram::HistogramArray>
            localHistograms(threadCount);

        for (auto& histogram : localHistograms) {
            histogram.fill(0);
        }

        #pragma omp parallel num_threads(threadCount)
        {
            const int tid = omp_get_thread_num();

            auto& localHistogram =
                localHistograms[tid];

            #pragma omp for schedule(static)
            for (int row = 0; row < rows; ++row) {

                const uchar* pixels =
                    gray.ptr<uchar>(row);

                for (int col = 0; col < cols; ++col) {

                    ++localHistogram[pixels[col]];
                }
            }
        }

        // -----------------------------------------
        // Merge thread-local histograms
        // -----------------------------------------

        for (int thread = 0;
             thread < threadCount;
             ++thread) {

            for (int bin = 0;
                 bin < Histogram::BIN_COUNT;
                 ++bin) {

                globalHistogram[bin] +=
                    localHistograms[thread][bin];
            }
        }

        result.histogram = globalHistogram;

        // -----------------------------------------
        // Stage 2: CDF and intensity mapping
        // -----------------------------------------

        result.cdf =
            Histogram::calculateCDF(
                result.histogram
            );

        result.cdfMin =
            Histogram::findCDFMin(
                result.cdf
            );

        result.mapping =
            Histogram::generateMapping(
                result.cdf,
                result.cdfMin,
                totalPixels
            );

        // -----------------------------------------
        // Stage 3: Parallel pixel transformation
        // -----------------------------------------

        result.output = cv::Mat(
            gray.size(),
            gray.type()
        );

        #pragma omp parallel for \
            num_threads(threadCount) \
            schedule(static)

        for (int row = 0; row < rows; ++row) {

            const uchar* inputRow =
                gray.ptr<uchar>(row);

            uchar* outputRow =
                result.output.ptr<uchar>(row);

            for (int col = 0; col < cols; ++col) {

                outputRow[col] =
                    result.mapping[inputRow[col]];
            }
        }

        // End algorithm timing.
        auto end =
            std::chrono::steady_clock::now();

        result.executionTimeMs =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        return result;
    }

}