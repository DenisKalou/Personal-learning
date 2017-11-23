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
template <class It, class UnFunc, class BiFunc>
auto make_action(It p, It q, UnFunc f1, BiFunc f2) -> decltype(f2(f1(*p), f1(*p))) {
	if (p == q) return 0;
	auto res = f1(*p);
	while (++p != q)
		res = f2(res, f1(*p));
	return res;
}
template <class It, class UnFunc, class BiFunc>
auto map_reduce(It p, It q, UnFunc f1, BiFunc f2, size_t threads) 
					-> decltype(f2(f1(*p), f1(*p)))
{
	auto range = std::distance(p, q);
	std::cout << range << '\n';
	std::vector<size_t> v_threads;
	v_threads.push_back(0);
	for (size_t i = 0; i < threads - 1; ++i)
	{
		v_threads.push_back(range / (threads - 1));
	}
	v_threads.push_back(range % (threads - 1));
	for (auto& a : v_threads) {
		std::cout << a << " ";
	}
	std::cout << '\n';
	std::vector<std::future<decltype(f2(f1(*p), f1(*p)))>> v_async;
	for (size_t i = 0; i < threads; ++i)
	{
		v_async.push_back(std::async(std::launch::deferred, make_action<It, UnFunc, BiFunc>, (p + v_threads[i]), (p + v_threads[i + 1]), f1, f2));
	}
	decltype(f2(f1(*p), f1(*p))) result;
	for (size_t i = 0; i < threads - 1; ++i)
	{
		result = f2(v_async[i].get(), v_async[i + 1].get());
	}
	return result;
}
int main()
{
	try {
		std::vector<int> l = { 1,2,3,4,5,6,7,8,9,10 };
		auto has_even = map_reduce(l.begin(), l.end(),
			[](int i) {return i % 2 == 0; },
			std::plus<int>(), 5);
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << '\n';
	}
	return 0;
}
