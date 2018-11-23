# C++ Master Class Assignment 5: Composition

## Introduction

In this assignment we want to practice what we learned so far by re-implementing a simplified `std::vector` (a growable array), utilizing our `memory_block`. In order for us all to be at the same page I've refactored the `memory_block` we built so far, it is now called `array` (a more suitable name given its functionality) and resides in the namespace `my` so it doesn't conflict with similarly named standard classes:

```
// myarray.h

#pragma once

namespace my
{
    template <typename T>
    class array
    {
        ...
    };
}
```

In order to use it, simply copy 'myarray.h' into your working directory and include it:

```
#include "myarray.h"

int main()
{
    using namespace my;
    
    array<int> student_scores(223);
    ...
}
```

If you've followed the assignments so far you'll notice that `my::array` is 99%, if not completely identical with the `memory_block` you've built so far. Nevertheless, please use `my::array` in order to implement your `vector` so we are all on the same page.

## Assignment 5

Please implement the following interface:

```
#include "myarray.h"

#include <algorithm>
#include <cmath>
#include <cstddef>



namespace my {

/**
 * Growable, type-safe, memory-managed list of elements (simplified version of std::vector)
 */
template <typename T>
class vector
{
public:
	/**
	 * Constructor, creates an empty vector.
	 * @post size() == capacity() == 0
	 * @post data() == nullptr
	 */
	vector();
	/**
	 * Constructor, creates a vector with n elements.
	 * @post size() == capacity() == n
	 * @post data() != nullptr
	 */
	explicit vector(std::size_t n);

	/**
	 * @return number of elements currently stored in the vector
	 */
	std::size_t size() const;
	/**
	 * @return total number of elements that can be stored in
	 * the vector without growing it
	 */
	std::size_t capacity() const;

	/**
	 * @return raw pointer to underlying data
	 */
	T * data() { return _data.data(); }
	T const * data() const { return _data.data(); }

	/**
	 * @return Element at index i
	 */
	T & operator[](std::size_t i);
	T const & operator[](std::size_t i) const;

	/**
	 * Appends the element 'val' to the end of this vector,
	 * grows the vector if necessary.
	 */
	void push_back(T const & val);

private:
	...
};

} // namespace my
```

### TODO

1. Implement the interface provided above
2. Complete comments with @invariant, @pre, @post, @exception clauses where applicable (you can draw inspiration from `my::array`).
3. **Test** your `vector` class!
4. Showcase everything you've learned so far, from RAII semantics, over testing and documentation, to writing robust and exception-safe code.

(As always, the reference implementation doesn't claim to be perfect.)