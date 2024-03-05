#ifndef __ARENA__
#define __ARENA__

#include <iostream>
#include <iomanip>

template<size_t MEMORY_SIZE>
class FixedArena
{
	public:
		FixedArena() : memory_buffer(new char[MEMORY_SIZE]), memory_index(0)
		{

		}

		~FixedArena()
		{
			delete[] memory_buffer;
		}

		template<typename T>
		T* Allocate()
		{
			if (memory_index + sizeof(T) >= MEMORY_SIZE)
			{
				std::cerr << __func__ << " : Arena out of memory." << std::endl;
				return nullptr;
			}

			void* memory_address = memory_buffer + memory_index;
			memory_index += sizeof(T);
			return static_cast<T*>(memory_address);
		}

		template<typename T>
		void Deallocate(T* memory_address)
		{
			memory_address = nullptr;
		}

		void Flush()
		{
			std::cout << "=== Memory Flushing Start ===" << std::endl;

			for (size_t i{}; i < memory_index; ++i)
			{
				std::cout << "Memory[" << std::dec << i << "] : ";
				std::cout << std::hex << std::setw(8) << std::setfill('0');
				std::cout << static_cast<int>(*(memory_buffer + i)) << std::endl;
			}

			std::cout << "=== Memory Used [ " << std::dec << memory_index << "/" << MEMORY_SIZE << " ] ===" << std::endl;
			std::cout << "=== Memory Flushing End ===" << std::endl;
		}

	private:
		char* memory_buffer;
		size_t memory_index;
		size_t memory_deallocated;

		FixedArena(FixedArena const&) = delete;
		FixedArena& operator=(FixedArena const&) = delete;
		FixedArena(FixedArena&&) = delete;
		FixedArena& operator=(FixedArena&&) = delete;
};
#endif