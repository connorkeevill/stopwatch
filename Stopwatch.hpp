#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
using namespace chrono;

/**
 * @class Stopwatch
 * @brief A class for measuring the execution time of different sections of code.
 *
 * The Stopwatch class provides functionality to add measurements, track multiple timings,
 * and get the timing trace for each measurement. It uses the high_resolution_clock from the
 * chrono library to ensure accurate timing.
 */
class Stopwatch
{
public:
	Stopwatch();
	~Stopwatch() = default;

	void addMeasurement(const string label);
	void addMeasurement(string label, int samples);
	string getTimingTrace();
private:
	std::vector<pair<string, time_point<high_resolution_clock>>> measurements {};
	std::vector<pair<string, int>> samples {};
};

/**
 * The default Stopwatch constructor.
 * Initializes a new Stopwatch object and adds the first time measurement labeled "start".
 */
Stopwatch::Stopwatch() {
	time_point<high_resolution_clock> start = high_resolution_clock::now();
	this->measurements.emplace_back("start", start);
}

/**
 * Adds a new time measurement with provided label.
 *
 * @param label: The label for the new time measurement.
 */
void Stopwatch::addMeasurement(string label) {
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	this->measurements.emplace_back(label, now);
}

/**
 * Allows a time measurement to be added which spans multiple samples of something; for instance multiple iterations
 * of a loop. By providing the number of samples, the Stopwatch is able to average the timespan over each sample.
 *
 * @param label: The label for the new time measurement.
 * @param samples: The sample number for the new time measurement.
 */
void Stopwatch::addMeasurement(const string label, int samples) {
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	this->measurements.emplace_back(label, now);
	this->samples.emplace_back(label, samples);
}

/**
 * Constructs and returns a string that holds the time elapsed from the "start" to each measurement
 * and from each measurement to the next in the order they were added.
 *
 * @return Returns a string that holds the time trace.
 */
string Stopwatch::getTimingTrace()
{
	stringstream stream {};

	auto start = this->measurements[0].second;
	auto end = high_resolution_clock::now();

	auto total = duration_cast<microseconds>(end - start).count();

	stream << "Total; start -> now: " << total / 1000000.0 << "s" << endl;

	for (int measurement = 1; measurement < this->measurements.size(); measurement++) {
		auto previous = this->measurements[measurement - 1];
		auto current = this->measurements[measurement];

		auto duration = duration_cast<microseconds>(current.second - previous.second).count();

		stream << previous.first << " -> " << current.first << ": " << duration / 1000000.0 << "s" << endl;

		// If the label is in samples, we want to output the time divide by the number of samples:
		for (auto sample : this->samples) {
			if (sample.first == current.first) {
				stream << previous.first << " -> " << current.first << " per sample: " << duration / 1000000.0 / sample.second << "s" << endl;
			}
		}
	}

	return stream.str();
}
