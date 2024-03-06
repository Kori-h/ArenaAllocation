#ifndef __ARENA__
#define __ARENA__

#include <iostream>
#include <iomanip>

struct Node 
{
	Node* next;
};

class FixedArena
{
	public:
		FixedArena(size_t size);
		~FixedArena();
		template<typename T> T* Allocate();
		template<typename T> void Deallocate(T* memory);
		void Flush();

	private:
		char* buffer;
		Node free;
		
		size_t padding;
		size_t size;
		size_t index;

		size_t mem_deallocated;
		size_t mem_reallocated;

		FixedArena(FixedArena const&) = delete;
		FixedArena& operator=(FixedArena const&) = delete;
		FixedArena(FixedArena&&) = delete;
		FixedArena& operator=(FixedArena&&) = delete;
};

template<typename T>
T* FixedArena::Allocate()
{
	size_t blocks = (sizeof(T) + padding - 1) / padding;
	void* address = nullptr;

	Node* prev_ptr = nullptr;
	Node* ptr = free.next;

	size_t i{};
	while (ptr)
	{
		++i;

		if (blocks == i)
		{
			if (prev_ptr)
			{
				prev_ptr->next = ptr->next;
				address = static_cast<void*>(prev_ptr);
			}
			else
			{
				address = free.next;
				free.next = ptr->next;
			}

			size_t allocated = blocks * padding;
			mem_reallocated += allocated;
			std::cout << "Allocate[used] " << typeid(T).name() << " : " << address << std::endl;

			// allocate this block
			return static_cast<T*>(address);
		}

		if (ptr->next)
		{
			// check if the next ptr is only 1 block away
			// else reset i since the data cannot fit here
			if (ptr->next - ptr != 1)
			{
				prev_ptr = ptr->next;
				i = 0;
			}

			ptr = ptr->next;
		}
		else
		{
			break;
		}
	}

	if (index + sizeof(T) >= size)
	{
		std::cerr << __func__ << " : Arena out of memory." << std::endl;
		return nullptr;
	}

	address = buffer + index;
	size_t allocated = blocks * padding;
	index += allocated;
	std::cout << "Allocate[new] " << typeid(T).name() << " : " << address << std::endl;
	return static_cast<T*>(address);
}

template<typename T>
void FixedArena::Deallocate(T* memory)
{
	void* address = static_cast<void*>(memory);
	size_t deallocated = sizeof(T);
	memset(address, 0, deallocated);

	size_t blocks = (sizeof(T) + padding - 1) / padding;;

	Node* ptr = &free;
	while (ptr->next) ptr = ptr->next;

	for (size_t i{}; i < blocks; ++i)
	{
		address = reinterpret_cast<char*>(memory) + i * padding;
		ptr->next = static_cast<Node*>(address);
		ptr = ptr->next;
	}

	std::cout << "Deallocate " << typeid(T).name() << " : " << address << std::endl;
	mem_deallocated += blocks * padding;
	memory = nullptr;
}
#endif