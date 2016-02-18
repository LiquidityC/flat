#include <string>
#include <iostream>
#include <iomanip>
#include "RuntimeAnalyzer.h"

namespace flat2d
{
	RuntimeAnalyzer::IntMap RuntimeAnalyzer::callCount =
		[] { RuntimeAnalyzer::IntMap ret; return ret; }();

	RuntimeAnalyzer::IntMap RuntimeAnalyzer::totalTime =
		[] { RuntimeAnalyzer::IntMap ret; return ret; }();

	RuntimeAnalyzer::FloatMap RuntimeAnalyzer::avgTime =
		[] { RuntimeAnalyzer::FloatMap ret; return ret; }();

	void RuntimeAnalyzer::addCall(std::string func, int time)
	{
		if (callCount.find(func) == callCount.end()) {
			callCount[func] = 0;
			totalTime[func] = time;
			avgTime[func] = 0;
		}

		callCount[func] = callCount[func] + 1;
		totalTime[func] = totalTime[func] + time;
		avgTime[func] = static_cast<float>(totalTime[func]) / static_cast<float>(callCount[func]);

		if (callCount[func] % 100 == 0) {
			std::cout << func << ": " << std::fixed << std::setprecision(5)
				<< avgTime[func]/1000 << std::endl;
		}
	}
} // namespace flat2d
