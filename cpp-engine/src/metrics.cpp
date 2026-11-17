#include "metrics.h"

namespace Metrics {

    double calculateSpeedup(
        double sequentialTimeMs,
        double parallelTimeMs
    ) {

        if (sequentialTimeMs <= 0 ||
            parallelTimeMs <= 0) {

            return 0.0;
        }

        return sequentialTimeMs / parallelTimeMs;
    }


    double calculateEfficiency(
        double speedup,
        int threadCount
    ) {

        if (speedup <= 0 || threadCount <= 0) {
            return 0.0;
        }

        return (speedup / threadCount) * 100.0;
    }


    double calculateTimePerMegapixel(
        double executionTimeMs,
        long long totalPixels
    ) {

        if (executionTimeMs < 0 ||
            totalPixels <= 0) {

            return 0.0;
        }

        double megapixels =
            static_cast<double>(totalPixels) /
            1000000.0;

        return executionTimeMs / megapixels;
    }

}