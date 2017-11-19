#include "stdafx.h"
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include <array>

size_t factorial(size_t n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
size_t combination(size_t n, size_t k) {
	return factorial(n) / (factorial(n - k)*factorial(k));
}
template<class Iterator>
size_t count_permutations(Iterator p, Iterator q)
{
	using T = typename std::iterator_traits<Iterator>::value_type;
	std::vector<T> v_tmp;
	for (; p != q; ++p) {
		v_tmp.push_back(*p);
	}
	std::sort(v_tmp.begin(), v_tmp.end());
	std::vector<T> v2_tmp;
	T tmp = *(v_tmp.begin());
	size_t count = 1;
	for (auto it = ++v_tmp.begin(); it != v_tmp.end(); ++it) {
		if (tmp == *it) {
			count++;
		}
		else {
			v2_tmp.push_back(count);
			count = 1;
		}
		tmp = *it;
	}
	v2_tmp.push_back(count);
	for (auto& a : v2_tmp) {
		std::cout << a << " ";
	}
	std::cout << '\n';
	size_t num = v_tmp.size();
	int max_perm = 0;
	int denum = 1;
	count = 0;

	for (auto& a: v2_tmp){
		denum *= factorial(a);
	}
	max_perm = factorial(num) / denum;
	std::cout << max_perm << '\n';
	size_t h_count = 0;
	for (auto& a : v2_tmp) {
		count += a;
	} 
	for (auto& a : v2_tmp) {
		if (a > 1) {
			count += a - 1;
			std::cout << factorial(num - (a - 1)) / factorial(a) << '\n';
			denum *= factorial(a)
			std::cout << max_perm << '\n';
		}
	} 
	num -= count;
	if (count > 1) max_perm += factorial(num);
	std::cout << max_perm << '\n';

	return max_perm;
}

int main()
{
	std::array<int, 6> a1 = { 1,1,3,3,2,2 };
	size_t c1 = count_permutations(a1.begin(), a1.end()); // 6

														  //std::array<int, 5> a2 = { 1,2,3,4,4 };
														  //size_t c2 = count_permutations(a2.begin(), a2.end()); // 36
	std::cout << "result = " << c1 << '\n';
	return 0;
}
