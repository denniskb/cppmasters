#include <cstddef>
#include <iostream>
#include <string>

// Helper macro to check conditions inside our test code
#define myassert(x)\
if (!(x))\
std::cout << "Assertion failed in line " << __LINE__ << std::endl;



/**
 * A very thin wrapper around built-in arrays (T[]) adding RAII semantics.
 */
template <typename T>
class memory_block
{
public:
	/* Allocates a memory block of n elements */
	explicit memory_block(std::size_t n) : _size(n)
	{
		_data = new T[n];
	}

	/* Creates a (deep) copy of other */
	memory_block(memory_block const & other) : _size(other.size())
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

		_size = rhs.size();
		_data = new T[rhs.size()];
		std::memcpy(_data, rhs.data(), rhs.size());

		return * this;
	}

	/* @return The number of elements in the memory block */
	std::size_t size() const { return _size; }

	/* @return Raw pointer to the data */
	T * data() {	return _data; }	
	T const * data() const { return _data; }

	/* @return Element at index i */
	T & operator[](std::size_t i) { return _data[i]; }
	T const & operator[](std::size_t i) const { return _data[i]; }

private:
	std::size_t _size;
	T * _data;
};



int main()
{	
	{// Test (de)allocation
		memory_block<char> x(1);
		x.data()[0] = 'x';

		myassert('x' == x.data()[0]);
	}

	{// Test const data()
		memory_block<char> const immutable(1);

		// Compiler error:
		// immutable.data()[0] = 'A';
	}

	{// Test size
		memory_block<char> x(5);

		myassert(5 == x.size());
	}

	{// Test copy
		memory_block<char> a(1);
		a.data()[0] = 'a';

		// Check b is copy of a
		memory_block<char> b(a);
		myassert(1 == b.size());
		myassert(a.data()[0] == b.data()[0]);

		// Check changing a doesn't affect b
		a.data()[0] = 'x';
		myassert('a' == b.data()[0]);
	}

	{// Test assignment
		memory_block<char> a(1), b(2);
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

		// Check b doesn't change after self-assignment.
		b = b;
		myassert(1 == b.size());
		myassert('a' == b.data()[0]);
	}

	{// Test subscript operator
		memory_block<int> x(1);

		x[0] = 27;
		myassert(27 == x[0]);
		myassert(27 == x.data()[0]);

		x.data()[0] = 13;
		myassert(13 == x[0]);
	}

	{// Test const subscript operator
		memory_block<int> const x(1);

		// Compiler error:
		//x[0] = -1;
	}
	
	return 0;
}