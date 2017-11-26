#include "stdafx.h"
#include <iostream>

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

template<int N, class IC>
struct Generate;

template<int N, class IC = IntCons<N-2, IntList<N-1>>>
struct Generate {
	using type = typename IC::type;
};

template<int N>
struct Generate<N, class IC> {
	using type = typename Generate<N - 1>::type;
};

template<>
struct Generate<1> {
	using type = typename IntCons<0, IntList<1>>::type;
};


int main()
{
	using L1 = IntList<2, 3, 4>;

	using L2 = IntCons<1, L1>::type;   // IntList<1,2,3,4>

	using L3 = Generate<5>::type;
	std::cout << L3::Head << L3::Tail::Head  << '\n';

	return 0;
}
