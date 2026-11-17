#ifndef ADAPTIVE_H
#define ADAPTIVE_H

#include <opencv2/opencv.hpp>

namespace Adaptive {

    struct SelectionResult {

        int selectedThreads;

        bool benchmarkFound;

        std::string selectionReason;
    };


    SelectionResult selectThreads(
        int width,
        int height
    );

}

#endif