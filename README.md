SETUP
-----
- Ensure `python3` with matplotlib is available (example: `python3 -m venv .venv && source .venv/bin/activate && pip install matplotlib`).
- Run `make run` to compile all binaries, regenerate inputs/outputs, execute every QuickSort run, and refresh the timing plot.
- Use `make clean` to remove binaries, generated inputs/outputs, timing logs, and the saved figure while preserving the `input_*_run1.txt` / `output_*_run1.txt` samples.

GENERAL DESCRIPTION
-------------------
- `InputFileGenerator.cpp` writes random integer files for sizes 10, 100, and 1000 (25 runs each) under `inputs/`.
- `Pandala_Khushal_QuickSort.cpp` sorts one input into the matching output and records its time in `Pandala_Khushal_executionTime_singleRun.txt`.
- `Pandala_Khushal_RunExperiments.cpp` orchestrates every run, invoking the sorter and appending CSV rows to `Pandala_Khushal_executionTime.txt`.
- The Makefile wires these steps together so `make run` performs the complete experiment pipeline end-to-end.

PLOT
----
- `Pandala_Khushal_plotAverageExecutionTime.py` reads `Pandala_Khushal_executionTime.txt`, computes the mean time per input size, overlays the measured curve with a scaled `n log n` reference, and saves `Pandala_Khushal_plotAverageExecutionTime.jpg` (already in the repo).
