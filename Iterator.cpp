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
		size_t index;
		const T* ptr;
		const ListT* data;
	public:
		operator bool() const
		{
			if (ptr)
				return true;
			else
				return false;
		}
		const_iterator(const T* i = nullptr) : ptr(i) {}
		const_iterator(const const_iterator &other) : index(other.index), ptr(other.ptr), data(other.data) {}
		const_iterator(const ListT* data, const T* i, bool begin = false) : ptr(i), data(data) {
			if (begin) {
				index = 0;
			}
			else {
				index = (*data).size() - 1;
			}
		}
		const_iterator& operator=(const const_iterator &other) = default;
		const_iterator& operator=(const T* i) { ptr = i; return *this; }
		~const_iterator() {}
		const T& operator*() { return *ptr; }
		const T& operator*() const { return *ptr; }
		const T* operator->() const { return ptr; }
		const_iterator& operator++() { ptr = realPtr('+', ptr); return *this; }
		const_iterator operator++(int) { auto tmp(*this);	ptr = realPtr('+', ptr);	return tmp; }
		const_iterator& operator--() { ptr = realPtr('-', ptr); return *this; }
		const_iterator operator--(int) { auto tmp(*this);	ptr = realPtr('-', ptr);	return tmp; }
		bool operator==(const const_iterator &other) const { return (ptr == other.ptr); }
		bool operator!=(const const_iterator &other) const { return !(*this == other); }
		const T* getPtr() const { return ptr; }
		const T* realPtr(const char& op, const T* p) {
			if (op == '+') {
				++p;
				auto it = (*data).begin();
				for (size_t i = 0; i < index; ++i)
				{
					++it;
				}
				std::cout << "ptr = " << p << " || iter = " << (&*((*it).end() - 1)) + 1 << std::endl;
				if ((p == (&*((*it).end() - 1)) + 1) && (p != &*((*data).back().end() - 1) + 1)) {
					++index;
					++it;
					p = &*(*(it)).begin();
				}
			}
			else {
				std::cout << "test: ";
				--p;
				auto it = (*data).begin();
				for (size_t i = 0; i < index; ++i)
				{
					++it;
				}
				for (size_t i = index; i > index; --i)
				{
					--it;
				}
				std::cout << "ptr = " << p << " || iter = " << &((*data).front()[0]) + 1 << std::endl;
				if ((p == (&*((*it).begin())) - 1) && (p != &((*data).front()[0]))) {
					--index;
					--it;
					p = &*((*(it)).end() - 1);
				}
			}
			return p;
		}
	};
	// определите методы begin / end
	const_iterator begin() const {
		if (this->size() == 0) {
			return const_iterator();
		}
		return const_iterator(&data_, &(data_.front()[0]), true);
	}
	const_iterator end()  const {
		if (this->size() == 0) {
			return this->begin();
		}
		return const_iterator(&data_, &*(data_.back().end() - 1) + 1);
	}
	// определите const_reverse_iterator
	class const_reverse_iterator
		: public const_iterator
	{
	public:
		const_reverse_iterator(const T* i = nullptr) : const_iterator(i) {}
		const_reverse_iterator(const const_iterator &other) { this->ptr = other.getPtr(); --this->ptr; }
		const_reverse_iterator& operator=(const const_reverse_iterator &other) = default;
		const_reverse_iterator& operator=(const T* i) { this->ptr = i; return *this; }
		const_reverse_iterator& operator=(const const_iterator &other) { this->ptr = other.getPtr(); --this->ptr; return *this; }
		~const_reverse_iterator() {}
		const T& operator*() { return *this->ptr; }
		const T& operator*() const { return *this->ptr; }
		const T* operator->() const { return this->ptr; }
		const_reverse_iterator& operator++() { --this->ptr; return *this; }
		const_reverse_iterator operator++(int) { auto tmp(*this);	--this->ptr;	return tmp; }
		const_reverse_iterator& operator--() { ++this->ptr; return *this; }
		const_reverse_iterator operator--(int) { auto tmp(*this);	++this->ptr;	return tmp; }
		bool operator==(const const_reverse_iterator &other) const { return (this->ptr == other.ptr); }
		bool operator!=(const const_reverse_iterator &other) const { return !(this->ptr == other.ptr); }
		const_iterator base() { const_iterator forwardIterator(this->ptr); ++forwardIterator; return forwardIterator; }
	};

	// определите методы rbegin / rend
	const_reverse_iterator rbegin() const {
		if (this->size() == 0) {
			return this->rend();
		}
		const_reverse_iterator it(&*(data_.back().end() - 1));
		return it;
	}
	const_reverse_iterator rend()   const {
		if (this->size() == 0) {
			return const_reverse_iterator();
		}
		const_reverse_iterator it(&(data_.front()[0]) - 1);
		return it;
	}
	//void data() {
	//	std::cout << &(data_.front()[0]) + this->size()) << std::endl;
	//	std::cout << &(data_.back().end() - 1) + 1 << std::endl;
	//}
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
