#include "stdafx.h"
#include <cassert>
#include <string>
#include <iterator>
#include <iostream>
#include <list>
#include <vector>
//using namespace std;

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
		std::size_t size = 0;
		for (auto& a : data_) {
			for (auto& b : a) {
				size++;
			}
		}
		return size;
	}

	// определите const_iterator
	class const_iterator
		: public std::iterator<std::bidirectional_iterator_tag, const T, std::ptrdiff_t, const T*, const T&>
	{
	protected:
		const ListT* data;
		typename std::list<std::vector<T>>::const_iterator lptr;
		typename std::vector<T>::const_iterator ptr;
		bool flag = false;
	public:
		const_iterator() {}
		const_iterator(const ListT* data, int begin) : data(data) {
			if (begin == 1) {
				lptr = (*data).begin();
				ptr = (*lptr).begin();
			}
			else if (begin == 2) {
				lptr = (*data).end();
				ptr = (*data).back().end();
			}
			if (begin == 3) {
				lptr = --(*data).end();
				ptr = --(*lptr).end();
			}
			else if (begin == 4) {
				lptr = (*data).begin();
				ptr = (*data).back().begin();
			}
		}
		const_iterator(const const_iterator &other) : data(other.data), lptr(other.lptr), ptr(other.ptr) {}
		const_iterator(const typename VectorList::const_reverse_iterator &other) : data(other.data), lptr(other.lptr), ptr(other.ptr) {}
		const_iterator& operator=(const const_iterator &other) = default;
		~const_iterator() {}
		const T& operator*() { return *ptr; }
		const T& operator*() const { return *ptr; }
		const T* operator->() const { return &*ptr; }
		const_iterator& operator++() { ++ptr; leap(); return *this; }
		const_iterator operator++(int) { auto tmp(*this);	++ptr; leap();	return tmp; }
		const_iterator& operator--() { leapBack(); return *this; }
		const_iterator operator--(int) { auto tmp(*this); leapBack();	return tmp; }
		bool operator==(const const_iterator &other) const {
			return (lptr == other.lptr) ? (ptr == other.ptr) : false;
		}
		bool operator!=(const const_iterator &other) const {
			return !(*this == other);
		}
		void leap() {
			if (ptr == (*lptr).end()) {
				++lptr;
				if (lptr != (*data).end()) {
					ptr = (*lptr).begin();
				}
			}
		}
		void leapBack() {
			if (lptr == (*data).end()) {
				--lptr;
			}
			if (flag) {
				--lptr;
				ptr = --(*lptr).end();
				flag = false;
			}
			else {
				--ptr;
			}
			if (ptr == (*lptr).begin()) {
				flag = true;
			}
		}
		const typename std::vector<T>::const_iterator getPtr() const { return ptr; }
		const typename std::list<std::vector<T>>::const_iterator getLptr() const { return lptr; }
		const ListT* getData() const { return data; }

	};

	// определите методы begin / end
	const_iterator begin() const {
		if (this->size() == 0) {
			return const_iterator();
		}
		return const_iterator(&data_, 1);
	}
	const_iterator end()  const {
		if (this->size() == 0) {
			return this->begin();
		}
		return const_iterator(&data_, 2);
	}
	// определите const_reverse_iterator
	class const_reverse_iterator
		: public const_iterator
	{
	public:
		const_reverse_iterator() : const_iterator() {}
		const_reverse_iterator(const ListT* data, int begin) : const_iterator(data, begin) {

		}
		const_reverse_iterator(const const_iterator &other) {
			this->ptr = other.getPtr(); --this->ptr;
			this->data = other.getData();
			this->lptr = other.getLptr();
		}
		const_reverse_iterator& operator=(const const_reverse_iterator &other) = default;
		const_reverse_iterator& operator=(const const_iterator &other) {
			this->ptr = other.getPtr(); --this->ptr;
			this->data = other.getData();
			this->lptr = other.getLptr();
			return *this;
		}
		~const_reverse_iterator() {}
		const T& operator*() { return *this->ptr; }
		const T& operator*() const { return *this->ptr; }
		const T* operator->() const { return &*this->ptr; }
		const_reverse_iterator& operator++() { this->leapBack(); return *this; }
		const_reverse_iterator operator++(int) { auto tmp(*this);	this->leapBack();	return tmp; }
		const_reverse_iterator& operator--() { ++this->ptr; return *this; }
		const_reverse_iterator operator--(int) { auto tmp(*this); ++this->ptr; return tmp; }
		bool operator==(const const_reverse_iterator &other) const {
			return (this->lptr == other.lptr) ? (this->ptr == other.ptr) : false;
		}
		bool operator!=(const const_reverse_iterator &other) const { return !(*this == other); }
		void leap() {
			if (this->ptr == (*this->lptr).end()) {
				++this->lptr;
				if (this->lptr != (*this->data).end()) {
					this->ptr = (*this->lptr).begin();
				}
			}
		}
		void leapBack() {
			if (this->flag) {
				--this->lptr;
				this->ptr = --(*this->lptr).end();
				this->flag = false;
			}
			else {
				--this->ptr;
			}
			if (this->ptr == (*this->lptr).begin()) {
				this->flag = true;
			}
		}
		const_iterator base() { const_iterator forwardIterator(*this); ++forwardIterator; return forwardIterator; }
	};

	// определите методы rbegin / rend
	const_reverse_iterator rbegin() const {
		if (this->size() == 0) {
			return this->rend();
		}
		return const_reverse_iterator(&data_, 3);
	}
	const_reverse_iterator rend()   const {
		if (this->size() == 0) {
			return const_reverse_iterator();
		}
		return const_reverse_iterator(&data_, 4);
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

	std::vector<std::string> v1 = { "one", "two", "three" };
	std::vector<std::string> v2 = { "four", "five", "six", "seven", "eight" };
	std::vector<std::string> v3 = { "nine", "ten", "eleven", "twelve" };
	std::vector<std::string> v4 = {};
	for (int k = 13; k <= 30; ++k) {
		v4.push_back(std::to_string(k) + "-th");
	}
	VectorList<std::string> vl;
	std::cout << "empty distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;

	vl.append(v1.begin(), v1.end());
	vl.append(v2.begin(), v2.end());
	vl.append(v3.begin(), v3.end());
	vl.append(v4.begin(), v4.end());
	std::cout << "size = " << vl.size() << std::endl;

	for (auto i = --vl.end(); i != vl.begin() - 1; --i)
	{
		std::cout << *i << " ";
	}
	//for (auto& a : vl) {
		//std::string a = *it;
		//std::cout << a << " ";
	//}
	/*std::cout << std::endl;
	std::cout << "distance = " << std::distance(vl.begin(), vl.end()) << std::endl;
	VectorList<std::string>::const_iterator eit = vl.end();
	for (; eit != vl.begin(); ++eit) {

	std::cout << *eit << " ";
	}
	std::cout << std::endl;
	VectorList<std::string>::const_reverse_iterator rit = vl.end();
	for (; rit != vl.rend(); ++rit) {
	std::string a = *rit;
	std::cout << a << " ";
	}
	std::cout << std::endl;
	std::cout << "reverse distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;
	VectorList<std::string>::const_reverse_iterator erit = vl.rend();
	for (; erit != vl.rbegin();) {
	std::string a = *(--erit);
	std::cout << a << " ";
	}
	std::cout << std::endl;
	VectorList<std::string>::const_iterator i = vl.rend().base();
	for (; i != vl.end(); ++i) {
	std::string a = *i;
	std::cout << a << " ";
	}*/
	//vl.data();

	std::cout << std::endl;
	return 0;
};
