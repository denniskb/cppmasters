#include "myarray.h"

#include <algorithm>
#include <cassert>
#include <cstddef>



namespace my {
/**
 * Growable, type-safe, memory-managed list of elements (simplified version of std::vector)
 * @invariant size() <= capacity()
 */
template <typename T>
class vector
{
public:
	/**
	 * Constructor, creates an empty vector.
	 * @exception no-throw
	 * @post size() == capacity() == 0
	 * @post data() == nullptr
	 */
	vector() : _size(0) {}
	/**
	 * Constructor, creates a vector with n elements.
	 * @exception might throw if not enough memory is available or if
	 * T's constructor throws. Provides strong exception saftey
	 * @post size() == capacity() == n
	 * @post data() != nullptr
	 */
	explicit vector(std::size_t n) : _data(n), _size(n) {}

	/**
	 * @return number of elements currently stored in the vector
	 * @exception no-throw
	 */
	std::size_t size() const { return _size; }
	/**
	 * @return total number of elements that can be stored in
	 * the vector without growing it
	 * @exception no-throw
	 */
	std::size_t capacity() const { return _data.size(); }

	/**
	 * @return raw pointer to underlying data
	 * @exception no-throw
	 */
	T * data() { return _data.data(); }
	T const * data() const { return _data.data(); }

	/**
	 * @return Element at index i
	 * @pre i < size()
	 * @exception no-throw
	 */
	T & operator[](std::size_t i)
	{
		assert(i < size());
		return _data[i];
	}
	T const & operator[](std::size_t i) const
	{
		assert(i < size());
		return _data[i];
	}

	/**
	 * Appends the element 'val' to the end of this vector,
	 * grows the vector if necessary.
	 * @exception might throw if not enough memory is available to grow the vecotr or
	 * if T's copy assignment operator throws. Provides strong exception saftey.
	 * @post size() grows by 1
	 * @post (*this)[size()-1] == val
	 */
	void push_back(T const & val)
	{
		if (size() == capacity()) // grow vector
		{
			array<T> tmp(capacity() + (capacity() / 2) + 1); // enlarge vector by 1.5x but at least 1
			std::copy(_data.data(), _data.data() + _data.size(), tmp.data());
			_data = tmp;
		}

		_data[size()] = val;
		_size++;

		assert(size() <= capacity());
	}

private:
	array<T> _data;
	std::size_t _size;
};
} // namespace my



int main()
{
	// TODO: Comments, asserts, exceptions, tests
	// clear (needs manual delete/dtor) (maybe later../maybe a/b...)
	using namespace my;

	{// Test vector()
		vector<int> x;

		assert(x.size() == 0);
		assert(x.capacity() == 0);
		assert(x.data() == nullptr);
	}

	{// Test vector(n)
		vector<int> x(10);

		assert(x.size() == 10);
		assert(x.capacity() == 10);
		assert(x.data() != nullptr);
	}

	{// Test copy
		vector<int> x(10);

		for (int i = 0; i < 10; i++)
			x[i] = i;

		vector<int> y(x);

		assert(y.size() == x.size());
		assert(y.capacity() == x.capacity());
		assert(y.data() != x.data());
		for (int i = 0; i < 10; i++)
			assert(y[i] == x[i]);

		y[1] = 23;
		
		assert(x[1] == 1);

		vector<int> z;
		z = x;

		assert(z.size() == x.size());
		assert(z.capacity() == x.capacity());
		assert(z.data() != x.data());
		for (int i = 0; i < 10; i++)
			assert(z[i] == i);

		z[5] = 99;
		
		assert(x[5] == 5);
	}

	{// Test []
		vector<int> x(10);

		for (int i = 0; i < 10; i++)
			x[i] = i;

		for (int i = 0; i < 10; i++)
			assert(x[i] == i);
	}

	{// Test push_back()
		vector<int> x;

		for (int i = 0; i < 100; i++)
		{			
			x.push_back(i);

			assert(x[i] == i);

			int const n = i + 1;

			assert(x.size() == n);
			assert(x.capacity() >= n);
		}
	}

	return 0;
}