#include <cstddef>
#include <iostream>
#include <string>

// Helper macro to check conditions inside our test code
#define myassert(x)\
if (!(x))\
std::cout << "Assertion failed in line " << __LINE__ << std::endl;



/**
 * A very thin wrapper around char[] adding RAII semantics.
 * Represents a contiguous block of memory.
 */
class memory_block
{
public:
	/* Allocates a memory block of n bytes */
	explicit memory_block(std::size_t n) : _size_in_bytes(n)
	{
		_data = new char[n];
	}

	/* Frees the allocated memory */
	~memory_block() { delete[] _data; }

	/* @return The size in bytes of the memory block */
	std::size_t size() const { return _size_in_bytes; }

	/* @return Raw pointer to the data */
	char * data() {	return _data; }	
	char const * data() const { return _data; }

private:
	// std::size_t is the only type in the C++ language
	// guaranteed to be able to hold the size of the biggest
	// possible allocation on the target platform.
	std::size_t _size_in_bytes;

	// char is the only type in the C++ language
	// defined to hold a single byte, which is not
	// necessarily 8bits!
	char * _data;
};



int main()
{	
	{// Test (de)allocation
	 // We introduce a scope so any code we execute has no
	 // effect on surrounding code.
		memory_block name(256);
		std::string hello("Hello World!");

		std::strcpy(name.data(), hello.c_str());

		std::cout << name.data() << std::endl;
	}// name.~memory_block() is called here
	 // (this is easily verifiable by inserting a 'print' into the destructor).

	{// Test const
		memory_block const immutable(1);

		// Compiler error:
		// immutable.data()[0] = 'A';
	}

	{// Test size
		memory_block x(5);

		myassert(5 == x.size());
	}
	
	return 0;
}