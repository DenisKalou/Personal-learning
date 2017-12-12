#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <exception>

#include <utility> // std::declval
#include <type_traits>
#include <future>

template<class It>
std::vector<It> divideByThreads(It p, It q, size_t threads) {
	auto range = std::distance(p, q);
	std::vector<It> v_threads;
	It beg = p;
	It end = q;
	v_threads.push_back(beg);
	for (size_t i = 0; i < threads - 1; ++i)
	{
		std::advance(beg, range / (threads));
		v_threads.push_back(beg);
	}
	std::advance(beg, (range / (threads)) + range % (threads));
	v_threads.push_back(beg);
	v_threads.push_back(end);
	return v_threads;
}

template <class It, class UnFunc, class BiFunc>
auto map_reduce(It p, It q, UnFunc f1, BiFunc f2, size_t threads)
-> decltype(f2(f1(*p), f1(*p)))
{
	std::vector<It> v_threads = divideByThreads(p, q, threads);
	std::vector<std::future<decltype(f2(f1(*p), f1(*p)))>> v_async(threads);
	auto f = [f1, f2](It a, It b) {
		auto res = f1(*a);
		while (++a != b)
			res = f2(res, f1(*a));
		return res;
	};

	for (size_t i = 0; i <= threads - 1; ++i)
	{
		v_async[i] = std::async(std::launch::async, f, v_threads[i], v_threads[i + 1]);
	}
	decltype(f2(f1(*p), f1(*p))) result;
	for (size_t i = 0; i < threads; ++i)
	{
		auto tmp = v_async[i].get();
		result = f2(result, tmp);
	}
	return result;
}
int main()
{
	
	return 0;
}
