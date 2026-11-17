#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>

#include "image_loader.h"
#include "sequential.h"
#include "parallel.h"
#include "metrics.h"

void printUsage() {

    std::cerr << R"(
Usage:

image_engine --input input.jpg --output output.jpg --mode sequential

image_engine --input input.jpg --output output.jpg --mode parallel --threads 4

image_engine --input input.jpg --output output.jpg --mode compare --threads 4

)";
}

bool getArgument(
    int argc,
    char* argv[],
    const std::string& name,
    std::string& value
) {

    for (int i = 1; i < argc - 1; ++i) {

        if (std::string(argv[i]) == name) {

            value = argv[i + 1];
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[]) {

    try {

        std::string inputPath;
        std::string outputPath;
        std::string mode;
        std::string threadString;

        if (!getArgument(argc, argv, "--input", inputPath) ||
            !getArgument(argc, argv, "--output", outputPath) ||
            !getArgument(argc, argv, "--mode", mode)) {

            printUsage();
            return 1;
        }

        int threads = 4;

        if (getArgument(argc, argv, "--threads", threadString)) {

            std::size_t parsed = 0;
            threads = std::stoi(threadString, &parsed);

            if (parsed != threadString.size() || threads < 1) {
                throw std::invalid_argument(
                    "Thread count must be a positive integer."
                );
            }
        }

        cv::Mat input =
            ImageLoader::loadImage(inputPath);

        const long long totalPixels =
            static_cast<long long>(input.rows) *
            input.cols;

        std::cout << std::fixed << std::setprecision(6);

        if (mode == "sequential") {

            auto result =
                Sequential::process(input);

            if (!ImageLoader::saveImage(
                    result.output,
                    outputPath)) {

                throw std::runtime_error(
                    "Unable to save output image."
                );
            }

            std::cout
                << "{"
                << "\"status\":\"success\","
                << "\"mode\":\"sequential\","
                << "\"width\":" << input.cols << ","
                << "\"height\":" << input.rows << ","
                << "\"totalPixels\":" << totalPixels << ","
                << "\"executionTimeMs\":"
                << result.executionTimeMs << ","
                << "\"outputPath\":\"" << outputPath << "\""
                << "}"
                << std::endl;
        }

        else if (mode == "parallel") {

            auto result =
                Parallel::process(input, threads);

            if (!ImageLoader::saveImage(
                    result.output,
                    outputPath)) {

                throw std::runtime_error(
                    "Unable to save output image."
                );
            }

            std::cout
                << "{"
                << "\"status\":\"success\","
                << "\"mode\":\"parallel\","
                << "\"width\":" << input.cols << ","
                << "\"height\":" << input.rows << ","
                << "\"totalPixels\":" << totalPixels << ","
                << "\"threadCount\":" << result.threadCount << ","
                << "\"executionTimeMs\":"
                << result.executionTimeMs << ","
                << "\"outputPath\":\"" << outputPath << "\""
                << "}"
                << std::endl;
        }

        else if (mode == "compare") {

            // Run sequential processing.
            auto sequentialResult =
                Sequential::process(input);

            // Run parallel processing on identical input.
            auto parallelResult =
                Parallel::process(input, threads);

            // Check pixel-by-pixel equivalence.
            double difference =
                cv::norm(
                    sequentialResult.output,
                    parallelResult.output,
                    cv::NORM_INF
                );

            bool equivalent = (difference == 0.0);

            if (!ImageLoader::saveImage(
                    parallelResult.output,
                    outputPath)) {

                throw std::runtime_error(
                    "Unable to save output image."
                );
            }

            double speedup =
                Metrics::calculateSpeedup(
                    sequentialResult.executionTimeMs,
                    parallelResult.executionTimeMs
                );

            double efficiency =
                Metrics::calculateEfficiency(
                    speedup,
                    threads
                );

            std::cout
                << "{"
                << "\"status\":\"success\","
                << "\"mode\":\"compare\","
                << "\"width\":" << input.cols << ","
                << "\"height\":" << input.rows << ","
                << "\"totalPixels\":" << totalPixels << ","
                << "\"threadCount\":" << threads << ","

                << "\"sequentialTimeMs\":"
                << sequentialResult.executionTimeMs << ","

                << "\"parallelTimeMs\":"
                << parallelResult.executionTimeMs << ","

                << "\"speedup\":"
                << speedup << ","

                << "\"efficiencyPercent\":"
                << efficiency << ","

                << "\"outputEquivalent\":"
                << (equivalent ? "true" : "false") << ","

                << "\"maxPixelDifference\":"
                << difference << ","

                << "\"outputPath\":\""
                << outputPath << "\""

                << "}"
                << std::endl;
        }

        else {

            throw std::invalid_argument(
                "Unsupported processing mode."
            );
        }

        return 0;

    }

    catch (const std::exception& e) {

        std::cerr
            << "Error: "
            << e.what()
            << std::endl;

        return 1;
    }
}