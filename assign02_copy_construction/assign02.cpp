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

	/* Creates a (deep) copy of other */
	memory_block(memory_block const & other) : _size_in_bytes(other.size())
	{
		_data = new char[other.size()];
		std::memcpy(_data, other.data(), other.size());
	}

	/* Frees the allocated memory */
	~memory_block() { delete[] _data; }

	/* Creates a (deep) copy of rhs */
	memory_block & operator=(memory_block const & rhs)
	{
		// Avoid self-assignment
		if (this == & rhs)
			return * this;

		delete[] _data;

		_size_in_bytes = rhs.size();
		_data = new char[rhs.size()];
		std::memcpy(_data, rhs.data(), rhs.size());

		return * this;
	}

	/* @return The size in bytes of the memory block */
	std::size_t size() const { return _size_in_bytes; }

	/* @return Raw pointer to the data */
	char * data() {	return _data; }	
	char const * data() const { return _data; }

private:
	std::size_t _size_in_bytes;
	char * _data;
};



int main()
{	
	{// Test (de)allocation
		memory_block x(1);
		x.data()[0] = 'x';

		myassert('x' == x.data()[0]);
	}

	{// Test const
		memory_block const immutable(1);

		// Compiler error:
		// immutable.data()[0] = 'A';
	}

	{// Test size
		memory_block x(5);

		myassert(5 == x.size());
	}

	{// Test copy
		memory_block a(1);
		a.data()[0] = 'a';

		// Check b is copy of a
		memory_block b(a);
		myassert(1 == b.size());
		myassert(a.data()[0] == b.data()[0]);

		// Check changing a doesn't affect b
		a.data()[0] = 'x';
		myassert('a' == b.data()[0]);
	}

	{// Test assignment
		memory_block a(1), b(2);
		a.data()[0] = 'a';
		b.data()[0] = 'b';
		b.data()[0] = 'b';

		// Check b is copy of a
		b = a;
		myassert(1 == b.size());
		myassert(a.data()[0] == b.data()[0]);		

		// Check changing a doesn't affect b
		a.data()[0] = 'x';
		myassert('a' == b.data()[0]);

		// Check b didn't change after self-assignment.
		b = b;
		myassert(1 == b.size());
		myassert('a' == b.data()[0]);
	}
	
	return 0;
}