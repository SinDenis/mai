#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>

template <typename T>
class TVector {
public:
	using value_type = T;
	using iterator = value_type*;
	using const_iterator = const value_type*;

	TVector(std::initializer_list<T> initList):
		AlreadyUsed_(initList.size()), StorageSize_(initList.size() * 2)
	{
		Storage_ = new T[StorageSize_];
		std::copy(initList.begin(), initList.end(),
				  Storage_);
	}

	TVector(): AlreadyUsed_(0), StorageSize_(0), Storage_(nullptr) {
	}

	TVector(int size, const value_type& default_value = value_type()): TVector() {
		assert(size >= 0);

		if (size == 0) {
			return;
		}

		AlreadyUsed_ = size;
		StorageSize_ = size;
		Storage_ = new value_type[size];

		std::fill(Storage_, Storage_ + AlreadyUsed_, default_value);
	}

	TVector(TVector&& v) noexcept {
		Storage_ = v.Storage_;
		AlreadyUsed_ = v.AlreadyUsed_;
		StorageSize_ = v.StorageSize_;
		v.Storage_ = nullptr;
		v.AlreadyUsed_ = 0;
		v.StorageSize_ = 0;
	}

	TVector& operator= (TVector&& v) noexcept {
		Storage_ = v.Storage_;
		AlreadyUsed_ = v.AlreadyUsed_;
		StorageSize_ = v.StorageSize_;
		v.Storage_ = nullptr;
		v.AlreadyUsed_ = 0;
		v.StorageSize_ = 0;
		return *this;
	}

	int Size() const {
		return AlreadyUsed_;
	}

	bool Empty() const {
		return Size() == 0;
	}

	iterator begin() const {
		return Storage_;
	}

	iterator end() const {
		if (Storage_) {
			return Storage_ + AlreadyUsed_;
		}

		return nullptr;
	}

	friend void swap(TVector& lhs, TVector& rhs) {
		using std::swap;

		swap(lhs.AlreadyUsed_, rhs.AlreadyUsed_);
		swap(lhs.StorageSize_, rhs.StorageSize_);
		swap(lhs.Storage_, rhs.Storage_);
	}

	TVector& operator=(TVector other) {
		swap(*this, other);
		return *this;
	}

	TVector(const TVector& other): TVector() {
		TVector next(other.StorageSize_);
		next.AlreadyUsed_ = other.AlreadyUsed_;

		if (other.Storage_ ) {
			std::copy(other.Storage_, other.Storage_ + other.StorageSize_,
						next.Storage_);
		}

		swap(*this, next);
	}

	~TVector() {
		delete[] Storage_;

		StorageSize_ = 0;
		AlreadyUsed_ = 0;
		Storage_ = nullptr;
	}

	void Resize(int count) {
		if (AlreadyUsed_ == count)
			return ;

		if (AlreadyUsed_ > count || count <= StorageSize_) {
			AlreadyUsed_ = count;
			return;
		}

		int next_size = count * 2;

		TVector next(next_size);
		next.AlreadyUsed_ = count;

		if (Storage_ ) {
			std::copy(Storage_, Storage_ + StorageSize_, next.Storage_);
		}
		swap(*this, next);
	}

	void PushBack(const value_type& value) {
		if (AlreadyUsed_ < StorageSize_) {
			Storage_[AlreadyUsed_] = value;
			++AlreadyUsed_;
			return;
		}

		int next_size = 1;
		if (StorageSize_) {
			next_size = StorageSize_ * 2;
		}

		TVector next(next_size);
		next.AlreadyUsed_ = AlreadyUsed_;

		if (Storage_ ) {
			std::copy(Storage_, Storage_ + StorageSize_, next.Storage_);
		}

		next.PushBack(value);
		swap(*this, next);
	}

	void PopBack() {
		if (AlreadyUsed_ == 0) {
			throw std::logic_error("Vector is emtpy");
		}
		--AlreadyUsed_;
	}

	void Insert(const_iterator pos, const T& value) {
		int numElems = end() - pos;
		Resize(AlreadyUsed_ + 1);
		iterator it = end() - 1;
		while (numElems > 0) {
			--numElems;
			*it = *(it - 1);
			--it;
		}
		*it = value;
	}

	iterator Erase(const_iterator pos) {
		auto res = (iterator)(pos + 1);
		auto start = (iterator)pos;
		const_iterator finish = end();
		while (start != finish) {
			*start = *(start + 1);
			++start;
		}
		return res;
	}

	iterator Erase(iterator first, iterator last) {
		auto res = last + 1;
		auto finish = end();

		while (first != finish) {
			*first = *last;
			++first;
			++last;
		}
		AlreadyUsed_ -= last - first;
		return res;
	}

	const value_type& At(int index) const {
		if (index < 0 || index > AlreadyUsed_) {
			throw std::out_of_range("You are doing this wrong!");
		}

		return Storage_[index];
	}

	value_type& At(int index) {
		const value_type& elem = const_cast<const TVector*>(this)->At(index);
		return const_cast<value_type&>(elem);
	}

	const value_type& operator[](int index) const {
		return At(index);
	}

	value_type& operator[](int index) {
		return At(index);
	}

	void Clean() {
		for (int i = 0; i < Size(); ++i) {
			Storage_[i] = {};
		}
	}

private:
	int AlreadyUsed_;
	int StorageSize_;
	value_type* Storage_;
};

#endif
