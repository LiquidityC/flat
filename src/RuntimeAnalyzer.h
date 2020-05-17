#ifndef RUNTIMEANALYZER_H_
#define RUNTIMEANALYZER_H_

#define TIME_FUNCTION ExecutionTimer executionTimer(__func__);

#include "Timer.h"
#include <map>
#include <string>

namespace flat2d {
	/**
	 * Helper class to benchmark and time game execution.
	 * Put the macro TIME_FUNCTION at the top of any recurring function
	 * and it will have it's average execution time analyzed.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class RuntimeAnalyzer
	{
	  public:
		using IntMap = std::map<std::string, int>;
		using FloatMap = std::map<std::string, float>;

	  private:
		static IntMap callCount;
		static IntMap totalTime;
		static FloatMap avgTime;

	  public:
		static void addCall(std::string func, int time);
		static const IntMap* getTotalTimes();
		static const FloatMap* getAvgTimes();
	};

	/**
	 * Timer to monitor game performance
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class ExecutionTimer
	{
	  private:
		Timer timer;
		std::string func;

	  public:
		explicit ExecutionTimer(const std::string& f)
		  : func(f)
		{
			timer.start();
		}

		~ExecutionTimer() { RuntimeAnalyzer::addCall(func, timer.getTicks()); }
	};
} // namespace flat2d

#endif // RUNTIMEANALYZER_H_
