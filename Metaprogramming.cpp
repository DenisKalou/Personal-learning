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

template<int N, int K = 0>
struct Generate {
	using type = typename IntCons<K, typename Generate<N - 1, K + 1>::type>::type;
};

template<int N2>
struct Generate<0, N2> {
	using type = IntList<>;
};


int main()
{
	using L1 = IntList<2, 3, 4>;

	using L2 = IntCons<1, L1>::type;   // IntList<1,2,3,4>

	using L3 = Generate<5>::type;
	std::cout << L3::Head << L3::Tail::Head  << '\n';

	return 0;
}
