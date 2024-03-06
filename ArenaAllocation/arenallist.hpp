#ifndef __ARENA_LIST__
#define __ARENA_LIST__

#include "arena.hpp"

template<typename T>
struct ArenaListItem
{
	ArenaListItem() : p_next{ nullptr }, T{} {};
	ArenaListItem* p_next;
	T m_item;
};

template<typename T>
class ArenaList
{
	public:
		ArenaList();
		~ArenaList();

		void setArena(FixedArena& arena);
		void pushBack(T const& item);
		T* operator[](size_t index);
		void clear();

	private:
		ArenaListItem<T>* mp_head;
		FixedArena* p_arena;

		ArenaListItem<T>* allocateNode();
		void deallocateNode(ArenaListItem<T>* p_node);
};

template<typename T>
ArenaList<T>::ArenaList() : mp_head(nullptr), p_arena(nullptr)
{

}

template<typename T>
ArenaList<T>::~ArenaList()
{
	clear();
}

template<typename T>
void ArenaList<T>::setArena(FixedArena& arena)
{
	p_arena = &arena;
}

template<typename T>
void ArenaList<T>::pushBack(T const& item)
{
	if (p_arena == nullptr)
	{
		std::cerr << __func__ << " : No Arena is set." << std::endl;
		return;
	}

	ArenaListItem<T>* p_new_node = allocateNode();
	p_new_node->m_item = item;
	p_new_node->p_next = nullptr;

	if (mp_head == nullptr)
	{
		mp_head = p_new_node;
		mp_head->p_next = nullptr;
		return;
	}

	ArenaListItem<T>* p_current = mp_head;

	while (p_current->p_next)
	{
		p_current = p_current->p_next;
	}

	p_current->p_next = p_new_node;
}

template<typename T>
T* ArenaList<T>::operator[](size_t index)
{
	if (p_arena == nullptr) 
	{
		std::cerr << __func__ << " : No Arena is set." << std::endl;
		return nullptr;
	}

	ArenaListItem<T>* p_current = mp_head;
	size_t count = 0;

	while (p_current)
	{
		if (count == index)
		{
			return &p_current->m_item;
		}

		p_current = p_current->p_next;
		++count;
	}

	return nullptr;
}

template<typename T>
void ArenaList<T>::clear()
{
	if (p_arena == nullptr)
	{
		std::cerr << __func__ << " : No Arena is set." << std::endl;
	}

	ArenaListItem<T>* p_current = mp_head;
	ArenaListItem<T>* p_deallocate = nullptr;

	while (p_current)
	{		
		p_deallocate = p_current;
		p_current = p_current->p_next;
		deallocateNode(p_deallocate);
	}

	mp_head = nullptr;
}

template<typename T>
ArenaListItem<T>* ArenaList<T>::allocateNode()
{
	return p_arena->allocate<ArenaListItem<T>>();
}

template<typename T>
void ArenaList<T>::deallocateNode(ArenaListItem<T>* p_node)
{
	p_arena->deallocate<ArenaListItem<T>>(p_node);
}
#endif