template <typename T>
class Array
{
public:
	explicit Array(size_t size = 0, const T& value = T()) : size_(size) {
		data_ = new T[size];
		for (size_t i = 0; i < size; ++i)
		{
			data_[i] = value;
		}
	}
	Array(const Array& other) {
		delete[] data_;
		size_ = other.size_;
		data_ = new T[size_];
		for (size_t i = 0; i < length; i++)
		{
			data_[i] = other.data_[i];
		}
	}
	~Array() {
		delete[] data_;
	}
	Array& operator=(Array other) {
		other.swap(*this);
		return *this;
	}
	void swap(Array &other) {
		swap(data_, other.data_);
		swap(size_, other.size_);
	}
	size_t size() const {
		return size_;
	}
	T& operator[](size_t idx) {
		return data_[idx];
	}
	const T& operator[](size_t idx) const {
		return data_[idx];
	}

private:
	size_t size_;
	T *data_;
};
