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
	 * as if the constructor had never been called in the first place.
	 *
	 * b) We can't do any better than this: we could re-try to allocate multiple times
	 * in the case of failure, but there is no guarantee that 'new T[n]' will EVER succeed. */
	explicit memory_block(std::size_t n) : _size(n), _data(new T[n]) {}

	/* Creates a (deep) copy of other
	 * @exception Strong exception safety: If any of the operations in a constructor fail
	 * the construction of the object is aborted.
	 * MEMORY LEAK: If std::copy fails the constructor is aborted. However, we've already allocated
	 * memory in the initializer list which is now lost forever because we no longer have access to _data
	 * and thus no way of freeing it. To fix this requires C++11 features.
	 *
	 * b) Using the same arugmentation as in the ctor we can't do any better than this.
	 * (We will be able to resolve the memory leak in the future.) */
	memory_block(memory_block const & other) : _size(other.size()), _data(new T[other.size()])
	{
		std::copy(other.data(), other.data() + other.size(), _data);
	}

	/* Frees the allocated memory
	 * @exception No-throw safety: delete[] never fails.
	 *
	 * b) No-throw is already the highest conceivable exception safety. */
	~memory_block() { delete[] _data; }

	/* Creates a (deep) copy of rhs
	 * @exception No exception safety: 'delete[]' and the assignment '_size = rhs.size()'
	 * always succeed. However, if then the allocation fails, the method exits and the
	 * object is left in an invalid state (_data points to invalid memory, _size contains an
	 * invalid number of elements).
	 *
	 * b) We could provide a basic exception safety by immediately setting _data and _size to null/0 after
	 * the delete[]:
	 * 
	 * ...
	 * delete[] _data;
	 * _data = nullptr; // (new line)
	 * _size = 0;       // (new line)
	 * ...
	 *
	 * Since these three operations are guaranteed to succeed, we bring our object into a valid state:
	 * The data has been successfully deleted and this is reflected in the _data pointer (null) and the _size (0).
	 * If then, subsequently, the allocation fails, the method is aborted. We have produced side effects (we have
	 * deleted our elements), but at least we left our object in a well-defined, valid state, and it
	 * continues to be usable. Thus we could provide a basic exception safety this way.
	 *
	 * However, we can do even better (see code). Through a simple re-arrangment of statements we are able
	 * to provide a strong exception safety:
	 * 
	 * 1. We allocate the new memory. If this fails, the method is aborted and our object is left untouched.
	 * 2. We perform the potentially dangerous std::copy which depends on T's copy assignment operator's
	 *    exception safety. If this fails, again our object is left untouched -- as if we never invoked operator=().
	 * 3. Finally, we safely delete the existing data and reassign the correct data pointer and size.
	 *
	 * Note that '_data = tmp' is a simple pointer assignment (a pointer is nothing else than an int variable
	 * holding a memory address instead of a number (which is nothing else than a specially formatted number))
	 * and thus does not invoke a copy of any kind.
	 * 
	 * Please also note that we temporarily potentially more than double our memory consumption,
	 * because for a short period of time we replicate rhs's data in two different places.
	 * This is the tradeoff we accept for safer code. For most use cases this tradeoff is
	 * preferrable (as a high exception safety is worth striving for) unless we are dealing
	 * with such large arrays (in the gigabyes) that we simply cannot afford to create temporary copies
	 * of our data but are forced to work 'in-place' (because they simply wouldn't fit into RAM).
	 *
	 * Finally and most importantly, observe how we are able to compose a method that offers a strong
	 * exception safety out of methods with lower or no exception safeties! The exception safety
	 * of a method is not a simple minimum of that of the methods it calls, but requries holistic analysis and design.
	 *
	 * MEMORY LEAK: Technically we are not achieving strong exception safety as we are not guaranteeing
	 * leak freedom. If std::copy fails the method is aborted and we loose access to tmp and thus
	 * can never free it. The memory is forever leaked (until program termination). We will learn how
	 * to fix this in the future. */
	memory_block & operator=(memory_block const & rhs)
	{
		if (this != & rhs)
		{
			T * tmp = new T[rhs.size()];
			std::copy(rhs.data(), rhs.data() + rhs.size(), tmp); // might fail => memory leak (tmp)

			delete[] _data;
			_data = tmp;
			_size = rhs.size();

			/* OLD CODE -- no exception safety
			 * delete[] _data;
			 *
			 * _size = rhs.size();
			 * _data = new T[rhs.size()];
			 * std::copy(rhs.data(), rhs.data() + rhs.size(), _data);
			 */

			/* OLD CODE with basic exception safety only (avoids temporary copies/leaks though)
			 * delete[] _data;                                        // can't fail
			 * _data = nullptr;                                       // restore valid state
			 * _size = 0;                                             // restore valid state
			 * 
			 * _data = new T[rhs.size()];                             // might fail
			 * _size = rhs.size();                                    // restore valid state
			 * 
			 * std::copy(rhs.data(), rhs.data() + rhs.size(), _data); // might fail
			 */
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
