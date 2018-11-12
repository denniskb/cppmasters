#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>



/**
 * A very thin wrapper around built-in arrays (T[]) adding RAII semantics.
 */
template <typename T>
class memory_block
{
public:
	/* Allocates a memory block of n elements
	 * @exception Strong exception safety: If the allocation (new T[n]) fails,
	 * the construction of memory_block is aborted and the application is left in a state
	 * as if the constructor had never been called in the first place. */
	explicit memory_block(std::size_t n) : _size(n), _data(new T[n]) {}

	/* Creates a (deep) copy of other
	 * @exception Strong exception safety: If any of the operations in a constructor fail
	 * the construction of the object is aborted. */
	memory_block(memory_block const & other) : _size(other.size()), _data(new T[other.size()])
	{
		std::copy(other.data(), other.data() + other.size(), _data);
	}

	/* Frees the allocated memory
	 * @exception No-throw safety: delete[] never fails. */
	~memory_block() { delete[] _data; }

	/* Creates a (deep) copy of rhs
	 * @exception No exception safety: 'delete[]' and the assignment '_size = rhs.size()'
	 * always succeed. However, if then the allocation fails, the method exits and the
	 * object is left in an invalid state (_data points to invalid memory, _size contains an
	 * invalid number of elements). */
	memory_block & operator=(memory_block const & rhs)
	{
		if (this != & rhs)
		{
			delete[] _data;

			_size = rhs.size();
			_data = new T[rhs.size()];
			std::copy(rhs.data(), rhs.data() + rhs.size(), _data);
		}
		
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

		assert('x' == x.data()[0]);
	}

	{// Test const data()
		memory_block<char> const immutable(1);

		// Compiler error:
		// immutable.data()[0] = 'A';
	}

	{// Test size
		memory_block<char> x(5);

		assert(5 == x.size());
	}

	{// Test copy
		memory_block<int> a(1);
		a.data()[0] = 17;

		// Check b is copy of a
		memory_block<int> b(a);
		assert(1 == b.size());
		assert(a.data()[0] == b.data()[0]);

		// Check changing a doesn't affect b
		a.data()[0] = 23;
		assert(17 == b.data()[0]);
	}

	{// Test assignment
		memory_block<char> a(1), b(2);
		a.data()[0] = 'a';
		b.data()[0] = 'b';
		b.data()[0] = 'b';

		// Check b is copy of a
		b = a;
		assert(1 == b.size());
		assert(a.data()[0] == b.data()[0]);		

		// Check changing a doesn't affect b
		a.data()[0] = 'x';
		assert('a' == b.data()[0]);

		// Check b doesn't change after self-assignment.
		b = b;
		assert(1 == b.size());
		assert('a' == b.data()[0]);
	}

	{// Test subscript operator
		memory_block<int> x(1);

		x[0] = 27;
		assert(27 == x[0]);
		assert(27 == x.data()[0]);

		x.data()[0] = 13;
		assert(13 == x[0]);
	}

	{// Test const subscript operator
		memory_block<int> const x(1);

		// Compiler error:
		//x[0] = -1;
	}
	
	return 0;
}
