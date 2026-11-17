# Adaptive Parallel Image Enhancement Using OpenMP

Local B.Tech Parallel Programming project: sequential and OpenMP Histogram Equalization, benchmark-guided adaptive thread selection, image metrics, Express API and React dashboard.

## Architecture
React/Vite → Express → safe child-process spawn → C++17/OpenCV/OpenMP. Benchmark data is stored in `experiments/results.csv`.

## Build (MSYS2 UCRT64)
Install packages in the UCRT64 terminal:
```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-opencv
cd /c/path/to/Adaptive-Image-Enhancement/cpp-engine
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```
On other platforms, install C++17, CMake, OpenMP and OpenCV development packages, then run `cmake -S cpp-engine -B cpp-engine/build && cmake --build cpp-engine/build -j`.

## CLI
```bash
image_engine --input input.jpg --output output.png --mode sequential
image_engine --input input.jpg --output output.png --mode parallel --threads 4
image_engine --input input.jpg --output output.png --mode adaptive
image_engine --input input.jpg --output output.png --mode compare --threads 4
```
Stdout is JSON; diagnostics go to stderr. Image decode and save are outside the timed algorithm region.

## Start local app
Terminal 1: `cd backend && npm install && npm start`
Terminal 2: `cd frontend && npm install && npm run dev`
Open http://localhost:5173. The engine is expected at `cpp-engine/build/image_engine.exe` on Windows or `cpp-engine/build/image_engine` elsewhere. Set `IMAGE_ENGINE_PATH` to override.

## API
`GET /api/health`, `GET /api/system/info`, `GET /api/benchmark`, `GET /api/benchmark/summary`, `POST /api/image/upload` (multipart field `image`), `POST /api/image/process` (JSON `{ "filename":"stored-file.png", "mode":"adaptive", "threads":4 }`), `POST /api/image/compare`.

## Algorithm
For histogram H(k), cumulative distribution CDF(k), first non-zero cumulative count CDF_min, and N pixels:
`T(k) = round(((CDF(k)-CDF_min)/(N-CDF_min))*255)`.
Constant images use identity mapping. Color images are converted BGR→YCrCb; only luminance Y is equalized. OpenMP uses per-thread histograms, deterministic merge and parallel row mapping.

## Adaptive mode
Reads actual parallel rows from CSV and selects the lowest recorded elapsed time for a tested thread count whose pixel count is within a factor of two of the current image. If no such row exists, a provisional CPU-based fallback is used and labeled in JSON. This initial implementation uses the best individual measurement; a more robust study should use repeated medians/means and account for system load.

## Metrics and caveats
Execution time, speedup, efficiency, mean brightness, grayscale contrast, PSNR and single-scale SSIM are computed. PSNR/SSIM are currently measured against the original input, which is not necessarily an ideal ground-truth reference. They express similarity, not proof of enhancement quality. Speedup/efficiency are emitted for compare mode; collect matching sequential baselines before making claims.

## Experiments
`python -m pip install -r experiments/requirements.txt`
`python experiments/benchmark.py --repeats 3`
`python experiments/generate_graphs.py`
Only actual engine timings are recorded. The benchmark uses deterministic synthetic test images. Speedup and efficiency charts are omitted unless matching sequential baseline data exists.

## Limitations
The dashboard shows genuine run history and basic performance charts. Histogram persistence and full per-run quality metric history are not wired into the API yet. Validate SSIM against a trusted implementation before academic publication. OpenCV color conversion may use its own internal threading. No experimental values are prefilled.
