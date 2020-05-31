
#ifndef SKETCH_TIMER
#define SKETCH_TIMER

#include <iostream>
#include <chrono>
#include <iomanip>

namespace dmh {

struct Timer {
	std::string name;
	std::chrono::nanoseconds elapsed;
	std::chrono::high_resolution_clock::time_point start_time; 

	Timer() : Timer("Timer") {}
	Timer(std::string name) : name(name) {
		elapsed = std::chrono::nanoseconds(0);
	}

	void start() {
		start_time = std::chrono::high_resolution_clock::now();
	}

	void stop() {
		elapsed += std::chrono::high_resolution_clock::now() - start_time;
	}

	void reset() {
		elapsed = std::chrono::nanoseconds(0);
	}

	double elapsed_s() {
		return elapsed_ms()/1000;
	}

	double elapsed_ms() {
		return (double)elapsed.count()/1000000;
	}

	double elapsed_ns() {
		return elapsed.count();
	}

	void print_ms() {
		std::cout << std::fixed << std::setprecision(2);
		std::cout << name << ": " << elapsed_ms() << " (ms)" << std::endl;
	}

	void print_s() {
		std::cout << std::fixed << std::setprecision(2);
		std::cout << name << ": " << elapsed_s() << " (s)" << std::endl;
	}
};

} // namespace dmh

#endif
