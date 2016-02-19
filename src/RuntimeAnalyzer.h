#ifndef RUNTIMEANALYZER_H_
#define	RUNTIMEANALYZER_H_

#define TIME_FUNCTION ExecutionTimer executionTimer(__func__);

#include <map>
#include <string>
#include "Timer.h"

namespace flat2d
{
	class RuntimeAnalyzer
	{
		public:
			using IntMap 	= std::map<std::string, int>;
			using FloatMap 	= std::map<std::string, float>;

		private:
			static IntMap 	callCount;
			static IntMap 	totalTime;
			static FloatMap avgTime;

		public:
			static void addCall(std::string func, int time);
			static const IntMap* getTotalTimes();
			static const FloatMap* getAvgTimes();
	};

	class ExecutionTimer
	{
		private:
			Timer timer;
			std::string func;

		public:
			explicit ExecutionTimer(std::string f) : func(f) {
				timer.start();
			}

			~ExecutionTimer() {
				RuntimeAnalyzer::addCall(func, timer.getTicks());
			}
	};
} // namespace flat2d

#endif // RUNTIMEANALYZER_H_
