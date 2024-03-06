#include "arena.hpp"

FixedArena::FixedArena(size_t size) : m_buffer(new char[size]), m_buffer_index(0), m_free_node(), m_padding(8), m_size(size), m_deallocated(0), m_reallocated(0)
{
	memset(m_buffer, 0, size);
}

FixedArena::~FixedArena()
{
	delete[] m_buffer;
}

void FixedArena::flush()
{
	std::cout << "=== Memory Flushing Start ===" << std::endl;
	std::cout << "=== START : " << static_cast<void*>(m_buffer) << " ===" << std::endl;

	for (size_t i{}; i < m_buffer_index; ++i)
	{
		std::cout << "Memory[" << std::dec << i << "] : ";
		std::cout << std::hex << std::setw(8) << std::setfill('0');
		std::cout << static_cast<int>(*(m_buffer + i)) << std::endl;
	}

	std::cout << "=== END : " << static_cast<void*>(m_buffer + m_buffer_index) << " ===" << std::endl;

	Node* ptr = m_free_node.p_next;

	size_t i{};
	while (ptr)
	{
		std::cout << "Deallocated[" << std::dec << i << "] : ";
		std::cout << static_cast<void*>(ptr) << std::endl;
		ptr = ptr->p_next;
		++i;
	}

	std::cout << "=== Memory Deallocated [ " << std::dec << m_deallocated << " ] ===" << std::endl;
	std::cout << "=== Memory Reallocated [ " << std::dec << m_reallocated << " ] ===" << std::endl;
	std::cout << "=== Memory Allocated [ " << std::dec << m_buffer_index << "/" << m_size << " ] ===" << std::endl;
	std::cout << "=== Memory Flushing End ===" << std::endl;
}