//
// Created by Sin Denis on 2019-11-10.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <memory>
#include <cstdint>

template <class T, class Allocator = std::allocator<T>>
class TQueue {
private:
	struct TNode;
	class TIterator;
public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = uint64_t;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using iterator = TIterator;
	using const_iterator = const TIterator;

	TQueue(): Size_(0)
	{
		FakeElem_ = NodeAllocate(value_type());
	}

	TQueue(const TQueue& other)
	{
		Allocator_ = std::allocator_traits<TAllocatorNode >
		::select_on_container_copy_construction(other.Allocator_);
		Size_ = other.Size_;

		FakeElem_ = NodeAllocate(other.FakeElem_->Value);
		std::shared_ptr<TNode> itNew = FakeElem_;
		std::shared_ptr<TNode> itOld = other.FakeElem_;
		for (int i = 0; i < Size_; ++i, itNew = itNew->Next, itNew = itOld->Next) {
			itNew->Next = NodeAllocate(itOld->Next->Value);
			itNew->Next->Prev = itNew;
		}
		if (Size_ >= 2) {
			itNew->Next = FakeElem_;
			FakeElem_->Prev = itNew;
		}
	}

	TQueue& operator= (const TQueue& other)
	{
		Allocator_ = std::allocator_traits<TAllocatorNode >
		::select_on_container_copy_construction(other.Allocator_);
		Size_ = other.Size_;

		FakeElem_->Next = nullptr;
		FakeElem_ = NodeAllocate(other.FakeElem_->Value);
		std::shared_ptr<TNode> itNew = FakeElem_;
		std::shared_ptr<TNode> itOld = other.FakeElem_;
		for (int i = 0; i < Size_; ++i, itNew = itNew->Next, itNew = itOld->Next) {
			itNew->Next = NodeAllocate(itOld->Next->Value);
			itNew->Next->Prev = itNew;
		}
		if (Size_ >= 2) {
			itNew->Next = FakeElem_;
			FakeElem_->Prev = itNew;
		}
		return *this;
	}

	~TQueue()
	{
		FakeElem_->Next = nullptr;
	}

	void Push(const value_type& value)
	{
		InsertImpl(FakeElem_, value);
	}

	void Pop()
	{
		if (Size_ == 0)
			throw std::logic_error("REMOVE_ERROR: List is empty");
		RemoveImpl(FakeElem_->Next);
	}

	const_reference Front() const
	{
		if (Size_ == 0)
			throw std::logic_error("ACCESS_ERROR: List is empty");
		return FakeElem_->Next->Value;
	}

	const_reference Back() const
	{
		if (Size_ == 0)
			throw std::logic_error("ACCESS_ERROR: List is empty");
		return FakeElem_->Prev.lock()->Value;
	}

	reference Front()
	{
		return const_cast<value_type &>(const_cast<const TQueue*>(this)->Front());
	}

	reference Back()
	{
		return const_cast<value_type &>(const_cast<const TQueue*>(this)->Back());
	}

	const_reference Top() const
	{
		return Back();
	}

	reference Top()
	{
		return Back();
	}

	size_type Size()
	{
		return Size_;
	}

	bool Empty()
	{
		return Size_ == 0;
	}

	const_iterator begin() const
	{
		if (Size_ == 0)
			return TIterator(FakeElem_, this);
		return TIterator(FakeElem_->Next, this);
	}

	const_iterator end() const
	{
		return TIterator(FakeElem_, this);
	}

	iterator begin()
	{
		if (Size_ == 0)
			return TIterator(FakeElem_, this);
		return TIterator(FakeElem_->Next, this);
	}

	iterator end()
	{
		return TIterator(FakeElem_, this);
	}

	iterator Insert(iterator pos, const value_type& value)
	{
		InsertImpl(pos.Item_.lock(), value);
		return TIterator(pos.Item_.lock()->Prev.lock(), this);
	}

	iterator Erase(iterator pos)
	{
		TIterator res = pos;
		++res;
		RemoveImpl(pos.Item_.lock());
		return res;
	}

private:
	struct TNode {
		std::shared_ptr<TNode> Next;
		std::weak_ptr<TNode> Prev;
		value_type Value;

		TNode(const value_type& val):
				Value(val), Next(nullptr)
		{
		}
	};

	class TIterator {
	public:

		using difference_type = uint32_t;
		using value_type = TQueue::value_type;
		using reference = TQueue::value_type&;
		using pointer = TQueue::value_type*;
		using iterator_category = std::bidirectional_iterator_tag;

		TIterator(std::shared_ptr<TNode> item, TQueue const * lst): Item_(item), Lst_(lst)
		{
		}

		~TIterator() = default;

		TIterator(const TIterator& other)
		{
			Item_ = other.Item_;
			Lst_ = other.Lst_;
		}

		TIterator& operator= (const TIterator& other)
		{
			Item_ = other.Item_;
			Lst_ = other.Lst_;
			return *this;
		}

		TIterator& operator++ ()
		{
			if (Lst_->Size_ == 0)
				return *this;
			if (Lst_->Size_ == 1 && Item_.lock() == Lst_->FakeElem_->Next) {
				Item_ = Lst_->FakeElem_;
				return *this;
			}
			Item_ = Item_.lock()->Next;
			return *this;
		}

		const TIterator operator++ (int)
		{
			if (Lst_->Size_ == 0)
				return *this;
			if (Lst_->Size_ == 1 && Item_.lock() == Lst_->FakeElem_->Next) {
				Item_ = Lst_->FakeElem_;
				return *this;
			}
			std::shared_ptr<TNode> res = Item_.lock();
			Item_ = Item_.lock()->Next;
			return TIterator(res);
		}

		TIterator& operator-- ()
		{
			if (Lst_->Size_ == 0)
				return *this;
			Item_ = Item_.lock()->Prev;
			return *this;
		}

		const TIterator operator-- (int)
		{
			if (Lst_->Size_ == 0)
				return *this;
			std::shared_ptr<TNode> res = Item_.lock();
			Item_ = Item_.lock()->Prev;
			return TIterator(res);
		}

		reference operator*(){
			return Item_.lock()->Value;
		}

		pointer operator->(){
			return &Item_.lock()->Value;
		}

		bool operator!= (const TIterator& other) const noexcept
		{
			return Item_.lock() != other.Item_.lock();
		}

		bool operator== (const TIterator& other) const noexcept
		{
			return Item_.lock() == other.Item_.lock();
		}


	private:
		std::weak_ptr<TNode> Item_;
		TQueue const *Lst_;
		friend class TQueue;
	};

	std::shared_ptr<TNode> FakeElem_;
	uint64_t Size_;
	using TAllocatorNode = typename allocator_type::template rebind<TNode>::other;
	TAllocatorNode Allocator_;

	std::shared_ptr<TNode> NodeAllocate(const value_type& value)
	{
		return std::allocate_shared<TNode>
				(Allocator_, value);
	}

	void EmptyInsertImpl(const value_type& value)
	{
		FakeElem_->Next = NodeAllocate(value);
		FakeElem_->Next->Prev = FakeElem_;
		FakeElem_->Prev = FakeElem_->Next;
	}

	void InsertImpl(std::shared_ptr<TNode> item, const value_type& value)
	{
		if (Size_ == 0) {
			EmptyInsertImpl(value);
			++Size_;
			return ;
		}
		std::shared_ptr<TNode> newElem = NodeAllocate(value);
		newElem->Next = item;
		newElem->Prev = item->Prev;
		item->Prev.lock()->Next = newElem;
		item->Prev = newElem;
		++Size_;
	}

	void RemoveImpl(std::shared_ptr<TNode> item)
	{
		if (Size_ == 1) {
			FakeElem_->Next = nullptr;
		} else {
			if (Size_ == 2) {
				if (item->Next == FakeElem_) {
					FakeElem_->Next->Next = nullptr;
					FakeElem_->Prev = FakeElem_->Next;
				} else {
					item->Next->Next = nullptr;
					item->Next->Prev = FakeElem_;
					FakeElem_->Next = item->Next;
				}
				return ;
			}
			item->Next->Prev = item->Prev;
			item->Prev.lock()->Next = item->Next;
		}
		--Size_;
	}
};

#endif
