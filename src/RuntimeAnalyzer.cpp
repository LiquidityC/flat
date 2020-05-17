#include "RuntimeAnalyzer.h"
#include <iostream>
#include <string>

namespace flat2d {
	RuntimeAnalyzer::IntMap RuntimeAnalyzer::callCount = [] {
		RuntimeAnalyzer::IntMap ret;
		return ret;
	}();

	RuntimeAnalyzer::IntMap RuntimeAnalyzer::totalTime = [] {
		RuntimeAnalyzer::IntMap ret;
		return ret;
	}();

	RuntimeAnalyzer::FloatMap RuntimeAnalyzer::avgTime = [] {
		RuntimeAnalyzer::FloatMap ret;
		return ret;
	}();

	void RuntimeAnalyzer::addCall(std::string func, int time)
	{
		if (callCount.find(func) == callCount.end()) {
			callCount[func] = 0;
			totalTime[func] = 0;
			avgTime[func] = 0;
		}

		callCount[func] = callCount[func] + 1;
		totalTime[func] = totalTime[func] + time;
		avgTime[func] = (static_cast<float>(totalTime[func]) /
		                 static_cast<float>(callCount[func])) /
		                1000;
	}

	const RuntimeAnalyzer::IntMap* RuntimeAnalyzer::getTotalTimes()
	{
		return &totalTime;
	}

	const RuntimeAnalyzer::FloatMap* RuntimeAnalyzer::getAvgTimes()
	{
		return &avgTime;
	}
} // namespace flat2d
