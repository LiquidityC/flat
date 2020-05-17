#include "../src/UID.h"
#include "../src/SpinLock.h"
#include "catch.hpp"
#include <iostream>
#include <set>
#include <thread>

using namespace flat2d;

TEST_CASE("UID Generation sequence test", "[UID]")
{
	const int thread_count = 20;
	std::set<int> idSet;
	flat2d::SpinLock lock;

	auto call_from_thread = [&]() {
		lock.lock();
		size_t id = UID::generate();
		REQUIRE(idSet.find(id) == idSet.end());
		idSet.insert(id);
		lock.unlock();
	};

	REQUIRE(idSet.size() == 0);

	std::thread threads[thread_count];
	for (auto i = 0; i < thread_count; i++) {
		threads[i] = std::thread(call_from_thread);
	}

	for (auto i = 0; i < thread_count; i++) {
		threads[i].join();
	}

	REQUIRE(idSet.size() == thread_count);
}
