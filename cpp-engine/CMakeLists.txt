cmake_minimum_required(VERSION 3.16)

project(
    AdaptiveImageEnhancement
    VERSION 1.0
    LANGUAGES CXX
)


# ---------------------------------------------------------
# C++ Standard
# ---------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ---------------------------------------------------------
# Find OpenCV
# ---------------------------------------------------------

find_package(OpenCV REQUIRED)


# ---------------------------------------------------------
# Find OpenMP
# ---------------------------------------------------------

find_package(OpenMP REQUIRED)


# ---------------------------------------------------------
# Include directories
# ---------------------------------------------------------

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)


# ---------------------------------------------------------
# Source files
# ---------------------------------------------------------

set(SOURCES

    src/main.cpp

    src/image_loader.cpp

    src/histogram.cpp

    src/sequential.cpp

    src/parallel.cpp

    src/adaptive.cpp

    src/metrics.cpp
)


# ---------------------------------------------------------
# Executable
# ---------------------------------------------------------

add_executable(
    image_engine
    ${SOURCES}
)


# ---------------------------------------------------------
# Link OpenCV
# ---------------------------------------------------------

target_link_libraries(
    image_engine
    PRIVATE
    ${OpenCV_LIBS}
)


# ---------------------------------------------------------
# Link OpenMP
# ---------------------------------------------------------

if(OpenMP_CXX_FOUND)

    target_link_libraries(
        image_engine
        PRIVATE
        OpenMP::OpenMP_CXX
    )

else()

    message(
        FATAL_ERROR
        "OpenMP was not found. Please install/configure OpenMP."
    )

endif()


# ---------------------------------------------------------
# Compiler warnings
# ---------------------------------------------------------

if(MSVC)

    target_compile_options(
        image_engine
        PRIVATE
        /W4
    )

else()

    target_compile_options(
        image_engine
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()


# ---------------------------------------------------------
# Build information
# ---------------------------------------------------------

message(STATUS "======================================")
message(STATUS "Adaptive Image Enhancement")
message(STATUS "C++ Standard : C++17")
message(STATUS "OpenCV       : ${OpenCV_VERSION}")
message(STATUS "OpenMP       : ENABLED")
message(STATUS "======================================")