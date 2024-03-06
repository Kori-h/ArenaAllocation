#include "arena.hpp"

FixedArena::FixedArena(size_t size) : buffer(new char[size]), free(), padding(8), size(size), index(0), mem_deallocated(0), mem_reallocated(0)
{
	memset(buffer, 0, size);
}

FixedArena::~FixedArena()
{
	delete[] buffer;
}

void FixedArena::Flush()
{
	std::cout << "=== Memory Flushing Start ===" << std::endl;
	std::cout << "=== START : " << static_cast<void*>(buffer) << " ===" << std::endl;

	for (size_t i{}; i < index; ++i)
	{
		std::cout << "Memory[" << std::dec << i << "] : ";
		std::cout << std::hex << std::setw(8) << std::setfill('0');
		std::cout << static_cast<int>(*(buffer + i)) << std::endl;
	}

	std::cout << "=== END : " << static_cast<void*>(buffer + index) << " ===" << std::endl;

	Node* ptr = free.next;

	size_t i{};
	while (ptr)
	{
		std::cout << "Deallocated[" << std::dec << i << "] : ";
		std::cout << static_cast<void*>(ptr) << std::endl;
		ptr = ptr->next;
		++i;
	}

	std::cout << "=== Memory Deallocated [ " << std::dec << mem_deallocated << " ] ===" << std::endl;
	std::cout << "=== Memory Reallocated [ " << std::dec << mem_reallocated << " ] ===" << std::endl;
	std::cout << "=== Memory Allocated [ " << std::dec << index << "/" << size << " ] ===" << std::endl;
	std::cout << "=== Memory Flushing End ===" << std::endl;
}