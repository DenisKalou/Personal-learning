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
		//bool flag = false;
	public:
		const_iterator() {}
		const_iterator(const ListT* data, 
			typename std::list<std::vector<T>>::const_iterator lptr, 
			typename std::vector<T>::const_iterator ptr) 
			: data(data), lptr(lptr), ptr(ptr) {}
		const_iterator(const const_iterator &other) : data(other.data), lptr(other.lptr), ptr(other.ptr) {}
		const_iterator& operator=(const const_iterator &other) = default;
		~const_iterator() {}
		const T& operator*() { return *ptr; }
		const T& operator*() const { return *ptr; }
		const T* operator->() const { return &*ptr; }
		const_iterator& operator++() { 
			if (ptr < (*lptr).end()) {
				++ptr;
			}
			if (ptr == (*lptr).end()) {
				++lptr;
				if (lptr != (*data).end()) {
					ptr = (*lptr).begin();
				}
			}
			return *this; 
		}
		const_iterator operator++(int) { 
			auto tmp(*this); 
			++*this;
			return tmp; }
		const_iterator& operator--() { 
			if (lptr == (*data).end()) --lptr;
			if (ptr > (*lptr).begin()) {
				--ptr;
			}
			else if (ptr == (*lptr).begin()) {
				--lptr;
				ptr = --(*lptr).end();
			}
			return *this; }
		const_iterator operator--(int) { 
			auto tmp(*this); 
			--*this;
			return tmp; }
		bool operator==(const const_iterator &other) const {

			return (lptr == other.lptr) ? (ptr == other.ptr) : false;
		}
		bool operator!=(const const_iterator &other) const {
			return !(*this == other);
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
		return const_iterator(&data_, data_.begin(), data_.front().begin());
	}
	const_iterator end()  const {
		if (this->size() == 0) {
			return this->begin();
		}
		return const_iterator(&data_, data_.end(), data_.back().end());
	}
	// определите const_reverse_iterator
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	// определите методы rbegin / rend
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}
	const_reverse_iterator rend()   const {
		return const_reverse_iterator(begin());
	}

private:
	ListT data_;
};
int main() {
	return 0;
};
