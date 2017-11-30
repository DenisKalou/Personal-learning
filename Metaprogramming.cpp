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
template<int a, int b>
struct Minus
{
	static int const value = a - b;
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

template<class... L>
struct Quantity;

template<int... Ints>
struct Quantity<IntList<Ints...>> {
private:
	
	double val_;
public:
	using type = IntList<Ints...>;
	explicit Quantity(const double& val)
		: val_(val) {};
	~Quantity() {};
	double& value() const{
		return val_;
	}
	double& operator/(const Quantity& other) {
		val_ *= other.val_;
		using type = typename Zip<type, typename other::type, Minus>::type;
	}
};

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

using NumberQ = Quantity<Dimension<>>;          // число без размерности
using LengthQ = Quantity<Dimension<1>>;          // метры
using MassQ = Quantity<Dimension<0, 1>>;       // килограммы
using TimeQ = Quantity<Dimension<0, 0, 1>>;    // секунды
using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
using AccelQ = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
using ForceQ = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах

int main()
{
	LengthQ   l{ 30000 };      // 30 км
	TimeQ     t{ 10 * 60 };    // 10 минут
							   // вычисление скорости
	//VelocityQ v = l / t;     // результат типа VelocityQ, 50 м/с

	//AccelQ    a{ 9.8 };        // ускорение свободного падения
	//MassQ     m{ 80 };         // 80 кг
							   // сила притяжения, которая действует на тело массой 80 кг
	//ForceQ    f = m * a;     // результат типа ForceQ
	return 0;
}
