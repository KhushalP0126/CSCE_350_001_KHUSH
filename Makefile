CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra
PYTHON = python3

run:
	$(CXX) $(CXXFLAGS) Pandala_Khushal_QuickSort.cpp -o Pandala_Khushal_QuickSort
	$(CXX) $(CXXFLAGS) InputFileGenerator.cpp -o InputFileGenerator
	$(CXX) $(CXXFLAGS) Pandala_Khushal_RunExperiments.cpp -o Pandala_Khushal_RunExperiments
	rm -rf inputs outputs
	./InputFileGenerator
	./Pandala_Khushal_RunExperiments
	$(PYTHON) Pandala_Khushal_plotAverageExecutionTime.py

clean:
	rm -f Pandala_Khushal_QuickSort InputFileGenerator Pandala_Khushal_RunExperiments
	rm -f Pandala_Khushal_executionTime.txt Pandala_Khushal_averageExecutionTime.txt Pandala_Khushal_executionTime_singleRun.txt Pandala_Khushal_timingReport.txt
	mkdir -p inputs outputs
	find inputs -maxdepth 1 -type f ! -name 'input_10_run1.txt' ! -name 'input_100_run1.txt' ! -name 'input_1000_run1.txt' -delete
	find outputs -maxdepth 1 -type f ! -name 'output_10_run1.txt' ! -name 'output_100_run1.txt' ! -name 'output_1000_run1.txt' -delete
	find inputs -mindepth 1 -type d -exec rm -rf {} +
	find outputs -mindepth 1 -type d -exec rm -rf {} +
	touch inputs/input_10_run1.txt inputs/input_100_run1.txt inputs/input_1000_run1.txt
	touch outputs/output_10_run1.txt outputs/output_100_run1.txt outputs/output_1000_run1.txt
	rm -f Pandala_Khushal_plotAverageExecutionTime.jpg

.PHONY: run clean
