#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>



namespace my {
/**
 * A very thin wrapper around built-in arrays (T[]) adding RAII and value semantics.
 */
template <typename T>
class array
{
public:
	/**
	 * Constructor, creates an empty array.
	 * @exception no-throw
	 * @post size() == 0
	 * @post data() == nullptr
	 */
	array() : _size(0), _data(nullptr) {}

	/**
	 * Constructor, allocates an array of n elements.
	 * @param n Size in elements of array
	 * @exception An exception is thrown if not enough memory is available.
	 * Provides strong exception safety
	 * @post size() == n
	 * @post data() != nullptr
     */
	explicit array(std::size_t n) : _size(n), _data(new T[n]) {}

	/**
	 * Copy constructor, creates a deep copy of 'other'.
	 *
	 * @exception Might throw depending on T's copy assignment operator
	 * exception specification. Provides strong exception safety.
	 * @post *this == other
	 */
	array(array const & other) : _size(other.size()), _data(new T[other.size()])
	{
		std::copy(other.data(), other.data() + other.size(), _data);
	}

	/**
	 * Destructor, de-allocates the array.
	 *
	 * @exception no-throw
	 */
	~array() { delete[] _data; }

	/**
	 * Copy assignment operator, creates a deep copy of 'rhs'.
	 *
	 * @exception might throw if there isn't enough memory available, or if T's
	 * copy assignment operator throws. Provides strong exception safety.
	 * @post *this == rhs
	 */
	array & operator=(array const & rhs)
	{
		if (this != & rhs)
		{
			T * tmp = new T[rhs.size()];
			std::copy(rhs.data(), rhs.data() + rhs.size(), tmp);

			delete[] _data;
			_data = tmp;
			_size = rhs.size();
		}
		
		return * this;
	}

	/**
	 * @return The number of elements in the memory block
	 * @exception no-throw
	 */
	std::size_t size() const { return _size; }

	/**
	 * @return Raw pointer to the data
	 * @exception no-throw
	 */
	T * data() { return _data; }	
	T const * data() const { return _data; }

	/**
	 * @return (Reference to) element at index i
	 * @pre i < size()
	 * @exception no-throw
	 */
	T & operator[](std::size_t i)
	{
		assert(i < _size);
		return _data[i];
	}
	T const & operator[](std::size_t i) const
	{
		assert(i < _size);
		return _data[i];
	}

private:
	std::size_t _size;
	T * _data;
};
} // namespace my