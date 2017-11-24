#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <exception>

#include <utility> // std::declval
#include <type_traits>
#include <future>
// заголовок <future> уже подключён.
// заново подключать не нужно
// реализация функции mapreduce

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
	
	
	//std::vector<std::future<decltype(f2(f1(*p), f1(*p)))>> v_async;
	std::vector<decltype(f2(f1(*p), f1(*p)))> v;
	auto f = [f1, f2](It a, It b) {
		//std::cout << "a = " << *a << " b = " << *b << '\n';
		auto res = f1(*a);
		while (++a != b)
			res = f2(res, f1(*a));
		return res;
	};

	for (size_t i = 0; i <= threads - 1; ++i)
	{
		//std::cout << "v_t[i] = " << *v_threads[i] << "|| v_t[i+1] = " << *v_threads[i] << '\n';
		v.push_back(f(v_threads[i], v_threads[i + 1]));
		//v_async.push_back(std::async(std::launch::deferred, make_action<It, UnFunc, BiFunc>, (p + v_threads[i]), (p + v_threads[i + 1]), f1, f2));
		std::cout << "thread res = " << v[i] << '\n';
	}
	//std::cout << '\n';
	decltype(f2(f1(*p), f1(*p))) result = 0;
	if (threads % 2) {
		std::cout << "v[thr] = " << v[0] << '\n';
		result = f2(result, v[threads - 1]);
	}
	for (size_t i = 0; i < threads ; i+=2)
	{
		std::cout << "f2 = "  << '\n';
		result += f2(v[i], v[i + 1]);
		std::cout << "result = " << result << " i = " << i << '\n';
	}
	std::cout << "result = " << result << '\n';
	if (threads % 2) {
		std::cout << "v[thr] = " << v[threads - 1] << '\n';
		result = f2(result, v[threads - 1]);
	}
	return result;
}
int main()
{
	try {
		std::vector<int> l = { 1,2,3,4,5,6,7,8,9,10 };
		auto has_even = map_reduce(l.begin(), l.end(),
			[](int i) {return i; },
			std::plus<int>(), 5);
		std::cout << "final result = " << has_even << '\n';
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << '\n';
	}
	return 0;
}
