#ifndef __ARENA__
#define __ARENA__

#include <iostream>
#include <iomanip>

struct Node 
{
	Node* p_next;
};

class FixedArena
{
	public:
		FixedArena(size_t size);
		FixedArena(char* buffer, size_t size);
		~FixedArena();
		template<typename T> T* allocate();
		template<typename T> void deallocate(T*& memory);
		void flush();

	private:
		bool m_extern_flag;
		char* m_buffer;
		size_t m_buffer_index;

		Node m_free_node;

		size_t m_padding;
		size_t m_size;
		
		size_t m_deallocated;
		size_t m_reallocated;

		FixedArena(FixedArena const&) = delete;
		FixedArena& operator=(FixedArena const&) = delete;
		FixedArena(FixedArena&&) = delete;
		FixedArena& operator=(FixedArena&&) = delete;
};

template<typename T>
T* FixedArena::allocate()
{
	size_t blocks = (sizeof(T) + m_padding - 1) / m_padding;
	void* p_address = nullptr;

	Node* p_prev_node = nullptr;
	Node* p_curr_node = m_free_node.p_next;

	size_t i{};
	while (p_curr_node)
	{
		++i;

		if (blocks == i)
		{
			if (p_prev_node)
			{
				p_prev_node->p_next = p_curr_node->p_next;
				p_address = static_cast<void*>(p_prev_node);
			}
			else
			{
				p_address = m_free_node.p_next;
				m_free_node.p_next = p_curr_node->p_next;
			}

			size_t allocated = blocks * m_padding;
			m_reallocated += allocated;	
			std::cout << __func__ << "[used] " << typeid(T).name() << " : " << p_address << std::endl;

			// allocate this block
			memset(p_address, 0, allocated);
			return static_cast<T*>(p_address);
		}

		if (p_curr_node->p_next)
		{
			// check if the next ptr is only 1 block away
			// else reset i since the data cannot fit here
			if (p_curr_node->p_next - p_curr_node != 1)
			{
				p_prev_node = p_curr_node->p_next;
				i = 0;
			}

			p_curr_node = p_curr_node->p_next;
		}
		else
		{
			break;
		}
	}

	if (m_buffer_index + sizeof(T) >= m_size)
	{
		std::cerr << __func__ << " : Arena out of memory." << std::endl;
		return nullptr;
	}

	p_address = m_buffer + m_buffer_index;
	size_t allocated = blocks * m_padding;
	m_buffer_index += allocated;
	std::cout << __func__ << "[new] " << typeid(T).name() << " : " << p_address << std::endl;
	memset(p_address, 0, allocated);
	return static_cast<T*>(p_address);
}

template<typename T>
void FixedArena::deallocate(T*& p_memory)
{
	if (p_memory == nullptr) 
	{
		std::cerr << __func__ << " : Invalid deallocation of null pointer." << std::endl;
		return;
	}

	void* p_address = static_cast<void*>(p_memory);
	std::cout << __func__ << " " << typeid(T).name() << " : " << p_address << std::endl;

	size_t deallocated = sizeof(T);
	memset(p_address, 0, deallocated);

	size_t blocks = (sizeof(T) + m_padding - 1) / m_padding;

	Node* ptr = &m_free_node;

	while (ptr->p_next) 
	{
		ptr = ptr->p_next;
	}

	for (size_t i{}; i < blocks; ++i)
	{
		p_address = reinterpret_cast<char*>(p_memory) + i * m_padding;
		ptr->p_next = static_cast<Node*>(p_address);
		ptr = ptr->p_next;
	}

	m_deallocated += blocks * m_padding;
	p_memory = nullptr;
}
#endif