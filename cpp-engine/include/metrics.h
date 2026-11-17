#ifndef METRICS_H
#define METRICS_H

namespace Metrics {

    double calculateSpeedup(
        double sequentialTimeMs,
        double parallelTimeMs
    );

    double calculateEfficiency(
        double speedup,
        int threadCount
    );

    double calculateTimePerMegapixel(
        double executionTimeMs,
        long long totalPixels
    );

}

#endif