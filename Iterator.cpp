// try_it.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iterator>
#include <iostream>
#include <list>
#include <vector>
using namespace std;


template<class T>
class VectorList
{
private:
	using VectT = std::vector<T>;
	using ListT = std::list<VectT>;

public:
	using value_type = T;

	VectorList() = default;
	VectorList(VectorList const &) = default;
	VectorList(VectorList &&) = default;

	VectorList & operator=(VectorList &&) = default;
	VectorList & operator=(VectorList const &) = default;

	// метод, который будет использоваться для заполнения VectorList
	// гарантирует, что в списке не будет пустых массивов
	template<class It>
	void append(It p, It q); // определена снаружи
							 /*  {
							 if (p != q)
							 data_.push_back(VectT(p,q));
							 }
							 */

	bool empty() const { return size() == 0; }

	// определите метод size
	size_t size() const
	{
		size_t size = 0;
		for (auto& a : data_) {
			for (auto& b : a) {
				size++;
			}
		}
		return size;
	}

	// определите const_iterator
	class const_iterator 
		: public std::iterator<forward_iterator_tag, VectorList> 
	{
	public:
		const_iterator(const T* p)
			: it_data_(p) {}
		const_iterator(const const_iterator& it) 
			: it_data_(it.it_data_){}
		~const_iterator() {}
		T operator*() const { return *it_data_; }
		bool operator==(const const_iterator& it) { return it_data_ == it.it_data_; }
		bool operator!=(const const_iterator& it) {	return !(it_data_ == it.it_data_); }
		const T* operator++() { return ++it_data_; }
		const T* operator++(int) {	return it_data_++; }
	private:
		const T* it_data_;
	};
	
		// определите методы begin / end
	const_iterator begin() const { 
		const_iterator it(&*(data_.begin())->begin());
		return it;
	}
	const_iterator end()   const { 
		const_iterator it(&*(data_.begin())->begin() + this->size());
		return it;
	}
	
	// определите const_reverse_iterator
	struct const_reverse_iterator 
		: std::iterator<forward_iterator_tag, VectorList> 
	{
	public:
		const_reverse_iterator(const T* p)
			: it_data_(p) {}
		const_reverse_iterator(const const_reverse_iterator& it)
			: it_data_(it.it_data_) {}
		~const_reverse_iterator() {}
		T operator*() const { return *it_data_; }
		bool operator==(const const_reverse_iterator& it) { return it_data_ == it.it_data_; }
		bool operator!=(const const_reverse_iterator& it) { return !(it_data_ == it.it_data_); }
		const_reverse_iterator& operator++() { return --it_data_; }
		const_reverse_iterator& operator++(int) { return it_data_--; }
	private:
		const T* it_data_;
	};

		// определите методы rbegin / rend
	const_reverse_iterator rbegin() const {
		const_reverse_iterator it(&*(data_.end())->end());
		return it;
	}
	const_reverse_iterator rend()   const {
		const_reverse_iterator it(&*(data_.begin())->begin());
		return it;
	}

private:
	ListT data_;
};

template<class T>
template<class It>
void VectorList<T>::append(It p, It q)
{
	if (p != q)
		data_.push_back(VectT(p, q));
}

int main() {
	VectorList<int> vlist;

	std::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	std::vector<int> v2;
	v1.push_back(4);
	v1.push_back(5);
	v1.push_back(6);
	v1.push_back(7);
	vlist.append(v1.begin(), v1.end());
	vlist.append(v2.begin(), v2.end());

	//cout << vlist.size() << endl;
	auto a = vlist.begin();
	//vlist.data();
	return 0;
};
