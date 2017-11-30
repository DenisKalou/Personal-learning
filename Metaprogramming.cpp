#include "stdafx.h"
#include <iostream>
#include <tuple>

template<int... Args>
struct IntList;

template<int N, int... Args>
struct IntList<N, Args...> {
	static const int Head = N;
	using Tail = IntList<Args...>;
};

template<>
struct IntList<> { };

template<int N, class IL>
struct IntCons;

template<int N, int... Args>
struct IntCons<N, IntList<Args...>> {
	using type = IntList<N, Args...>;
};

template<int N, int K = 0>
struct Generate {
	typedef typename IntCons<K, typename Generate<N - 1, K + 1>::type>::type type;
};

template<int K>
struct Generate<0, K> {
	typedef IntList<> type;
};

template<class Fun, class... Args,
	class Index = typename Generate<sizeof... (Args)>::type>
	auto apply(Fun f, const std::tuple<Args...>& t)
	-> decltype(apply(f, t, Index()))
{
	return apply(f, t, Index());
}
template<class Fun, class... Args, int... Indices>
auto apply(Fun f, const std::tuple<Args...>& t, IntList<Indices...>)
-> decltype(f(std::get<Indices>(t)...))
{
	return f(std::get<Indices>(t)...);
}

template<class IL>
void printIntList(std::ostream& os) {
	os << (typename IL::Head) << " ";
	printIntList<typename IL::Tail>(os);
}
template<>
void printIntList<IntList<>>(std::ostream& os) {}

template<int a, int b>
struct Plus
{
	static int const value = a + b;
};

template<class L1, class L2, template<int...> class F>
struct Zip;

template<template<int...> class L1, int... Ints1, 
	template<int...> class L2, int... Ints2,
	template<int...> class F>
struct Zip<L1<Ints1...>, L2<Ints2...>, F>
{
	using type = L1<F<Ints1, Ints2>::value...>;
};

int main()
{
	// два списка одной длины
	using L1 = IntList<1, 2, 3, 4, 5>;
	using L2 = IntList<1, 3, 7, 7, 2>;

	// результат применения — список с поэлементными суммами
	using L3 = Zip<L1, L2, Plus>::type;  // IntList<2, 5, 10, 11, 7>
	printIntList<L3>(std::cout);
	//std::cout << L3 << '\n';
	return 0;
}
