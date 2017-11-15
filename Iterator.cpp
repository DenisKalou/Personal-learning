#include "stdafx.h"
#include <cassert>
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
		: public std::iterator<std::bidirectional_iterator_tag, const T, std::ptrdiff_t,  const T*, const T&>
	{
	protected:
		const T* ptr;
	public:
		const_iterator(const T* i = nullptr) : ptr(i) {}
		const_iterator(const const_iterator &other) : ptr(other.ptr) {}
		const_iterator& operator=(const const_iterator &other) = default;
		const_iterator& operator=(const T* i) {	ptr = i; return *this;	}
		~const_iterator() {}
		const T& operator*() { return *ptr; }
		const T& operator*() const { return *ptr;}
		const T* operator->() { return ptr;}
		const_iterator& operator++() { ++ptr; return *this;	}
		const_iterator operator++(int) {auto tmp(*this);	++ptr;	return tmp;	}
		const_iterator& operator--() { --ptr; return *this;	}
		const_iterator operator--(int) { auto tmp(*this);	--ptr;	return tmp;	}
		bool operator==(const const_iterator &other) const { return (ptr == other.ptr);	}
		bool operator!=(const const_iterator &other) const { return !(*this == other); }

	};
	// определите методы begin / end
	const_iterator begin() const {
		return const_iterator(&(data_.front()[0]));
	}
	const_iterator end()  const {
		return const_iterator (&(data_.front()[0]) + this->size());
	}
	// определите const_reverse_iterator
	class const_reverse_iterator
		: public const_iterator
	{
	public:
		const_reverse_iterator(T* i = nullptr) : const_iterator(i) {}
		const_reverse_iterator(const const_iterator &other) { this->ptr = other.ptr; }
		const_reverse_iterator& operator=(const const_reverse_iterator &other) = default;
		const_reverse_iterator& operator=(const T* i) { this->ptr = i; return *this; }
		~const_reverse_iterator() {}
		const T& operator*() { return *this->ptr; }
		const T& operator*() const { return *this->ptr; }
		const T* operator->() { return this->ptr; }
		const_reverse_iterator& operator++() { ++this->ptr; return *this; }
		const_reverse_iterator operator++(int) { auto tmp(*this);	++this->ptr;	return tmp; }
		const_reverse_iterator& operator--() { --this->ptr; return *this; }
		const_reverse_iterator operator--(int) { auto tmp(*this);	--this->ptr;	return tmp; }
		bool operator==(const const_reverse_iterator &other) const { return (this->ptr == other.ptr); }
		bool operator!=(const const_reverse_iterator &other) const { return !(this->ptr == other.ptr); }
        const_iterator base(){const_iterator forwardIterator(this->ptr); ++forwardIterator; return forwardIterator;}
	};

	// определите методы rbegin / rend
	const_reverse_iterator rbegin() const {
		const_reverse_iterator it(&(data_.front()[0]) + this->size());
		return it;
	}
	const_reverse_iterator rend()   const {
		const_reverse_iterator it(&(data_.front()[0]));
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

	VectorList<int> v;
	assert(std::distance(v.begin(), v.end()) == 0);
	assert(std::distance(v.rbegin(), v.rend()) == 0);

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

	//std::vector<int>::iterator vit = v1.begin();
	//auto it = vlist.end();
	size_t size = vlist.size();
	for (auto it = vlist.begin(); it != vlist.end(); ++it) {
		std::cout << *it << " " << std::endl;
	//std::cout << *(vlist.begin()) << " " << std::endl;
	}

	//cout << typeid(vit).name() << endl;
	//auto a = vlist.begin();
	//vlist.data();
	return 0;
};
