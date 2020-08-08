//
// Created by Sin Denis on 2019-11-12.
//

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "vector.h"
#include <cstdlib>

template <class T, size_t BLOCK_SIZE>
class TAllocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = std::size_t;

	TAllocator() noexcept : Buffer_(nullptr), FreeBlocks_(0)
	{
		static_assert(BLOCK_SIZE > 0, "Block size is lower then 0");
	}

	~TAllocator() noexcept
	{
		free(Buffer_);
	}

	template <class U>
	struct rebind {
		using other = TAllocator<U, BLOCK_SIZE>;
	};

	T* allocate(size_t n)
	{
		if (!Buffer_) {
			Buffer_ = (T*)malloc(BLOCK_SIZE * sizeof(T));
			FreeBlocks_.Resize(BLOCK_SIZE);
			FreeBlocksFill();
		}

		int i = SearchFreeSpace(n);
		FreeBlocks_.Erase(FreeBlocks_.begin() + i - n + 1, FreeBlocks_.begin() + i + 1);
		return Buffer_ + i;
	}

	void deallocate(T* ptr, size_t n)
	{
		for (int i = 0; i < n; ++i) {
			FreeBlocks_.PushBack(ptr + i);
		}
	}

	template <typename U, typename ...Args>
	void construct(U* p, Args&& ...args) {
		new (p) U(std::forward<Args>(args)...);
	}

	void destroy(pointer p) {
		p->~T();
	}


private:
	T* Buffer_;
	TVector<T*> FreeBlocks_;

	void FreeBlocksFill()
	{
		for (int i = 0; i < BLOCK_SIZE; ++i) {
			FreeBlocks_[i] = Buffer_ + i;
		}
	}

	int SearchFreeSpace(size_t n)
	{
		size_t total = 0;

		int i = FreeBlocks_.Size() - 1;
		for (; i >= 0; --i) {
			total = 1;
			for (int j = i; j > 0 && total < n; --j) {
				if (FreeBlocks_[j] - FreeBlocks_[j - 1] == 1)
					++total;
			}
			if (total >= n)
				break;
		}

		if (total < n)
			throw std::bad_alloc();

		return i;
	}
};

#endif
