# C++ Master Class Assignment 6: Memory Management

## Introduction

In the old days, before C++'s capabilities were fully understood and before an idiomatic C++ style had emerged, that's how we used to write and teach code:

```
#include <iostream>

using namespace std;

int main()
{
    cout << "\nTesting dynamic storage allocation! " << endl;
    
    // To allocate storage:
    double width = 23.78;
    double* ptrWidth = &width;
    double* ptrLength = new double(32.54);
    double* ptrArea = new double;

    // To work with ptrWidth, ptrLength, and ptrArea:
    *ptrArea = *ptrWidth * *ptrLength;
    delete ptrLength; // Error: The object is still in use!
    cout << "\nWidth : " << *ptrWidth << "\nLength : " << *ptrLength << "\nArea : " << *ptrArea << endl;

    // To free storage:
    delete ptrWidth; // Error: The object has not

    // been dynamically reserved
    delete ptrLength; // ok
    delete ptrArea; // ok
    delete ptrLength; // Error: Pointer doesn't address any object.

    ptrLength = new double(19.45); // ok

    // To give a name to a dynamic object:
    double& length = *ptrLength; // Reference
    cout << "\nNew length : " << length << "\nCircumference : " << 2 * width * length << endl;

    return 0; // On terminating the program
}

// excerpt from "A Complete Guide to Programming in C++ by Ulla and Peter Prinz -- (not recommended)
```

Plastered with heap allocations (`new`), putting unnecessary stress on the memory allocator and the programmer who has to remember to free the resources again, preventing us from writing safe code (think of the un-avoidable memory leak in our `array` class (when the copy ctor fails)). Over time we learned how to use C++'s features (which were available from the inception of the language) more efficiently, resulting in the emergence of a few idioms:

- *Prefer stack allocation (`String name("dennis");`) over heap allocation (`String * name = new String("dennis");`)*, putting less stress on the memory allocator (speeding up our code) and letting the compiler manage object lifetimes for us as functions return and local variables go out of scope.
- *Use of RAII* (**R**esource **A**llocation **I**s **I**nitialization), aligning the lifetime of resources to the variables by which they are represented, and allowing us to use variables as synonyms for resources (memory, network connections, threads, etc.)
- *Default to value semantics, request reference semantics explicitly*. We thought we were writing efficient code by avoiding deep copies of objects (which btw were infinitely cheaper than anticipated by programmers) and in exchange produced code that was hell to manage as multiple pointers to the same object produced global dependencies and allowed modification of state in un-forseeable ways. Today we are smarter and default to deep copies (where `vec1 = vec2` creates an independent duplicate of `vec2`), we can still request references where this makes sense semantically.
- *Use of higher level abstractions*. Instead of allocating and freeing raw memory ourselves, we use `vectors`, `lists`, etc. that manage the memory for us.
- *Write (exception-)safe code*. C++ is one of the few languages that, because of how it handles object destruction, allows us to write safe code in the first place.

## std::unique_ptr

Sometimes we still find ourselves in need of managing our own memory, simply because containers such as `vector` are not applicable to the job. This does not mean, however, that we have to resort to `new` and `delete`. That's where `std::unique_ptr` and `std::shared_ptr` come into play. `unique_ptr` is a very thin, RAII wrapper around *single objects* of arbitrary type, the same way our `my::vector` is a very thin, RAII wrapper around *arrays* of arbitrary type. Its implementation is just as straight forward and goes along the lines of:

```
namespace std {

template<typename T>
class unique_ptr
{
public:
    unique_ptr(T * raw_ptr) : _data(raw_ptr) {}
    ~unique_ptr() { delete _data; }
    ...

private:
    T * _data;
};

} // namespace std


std::unique_ptr<String> name(new String("dennis"));
// as 'name' goes out of scope, the heap-allocated String it refers to gets deleted automatically
```

`unique_ptr` is not only a convenience, it allows us to write safer code, too. This is because of how C++ manages object lifetimes:

1. Before an object is (copy-)constructed, all its members have already been constructed in the order they are declared in the class, either with default parameters or with the parameters specified in the initializer list.
2. After an object is destructed, all its members are destructed in reverse order of their initialization.
3. When a scope ends, either normally (block `{}` ending, function returning ..) *or exceptionally* (hardware failure, raised exceptions, violated assertions), C++ **guarantees** that all objects living inside the scope are destructed deterministically and *without failure*, that inclueds:
    - local variables: `int x = 5;`
    - stack-allocated objects: `String name("dennis")`;
    - **class members** (in the case of an ending destructor, *regardless of whether it ended normally or exceptionally*): `private: my::array _data;`

Do you already see how `unique_ptr` can help us eradicate the memory leak from our `array`?

### Additional References

- [*Chapter 4. Smart Pointers* from Effective Modern C++ by Scott Meyers](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/ch04.html) (recommended!)

## Assignment 6

As you diagnosed in previous assignments our `my::array` class suffers from a memory leak in its copy constructor:

```
array(array const & other) : _size(other.size()), _data(new T[other.size()])
{
    std::copy(other.data(), other.data() + other.size(), _data.get());
}
```

If `std::copy` fails (for example because `T`'s copy assignment operator throws) we loose access to `_data` and have no way of freeing the already allocated memory. The application state is left un-modified but we also suffer from a memory leak. This is because `_data` is a primitive data type (a raw pointer) and no special action is performed by C++ for its destruction.

1. Eradicate the memory leak in our `my::array` class through the use of `std::unique_ptr`.
    - Download the reference solutions 'myarray.h' and 'assign05.cpp' from 'assign05_composition'.
    - Modify 'myarray.h' accordingly (this is the subject of this assignment).
    - Test your modifications by building and running 'assign05.cpp'.
2. Read 'Item 19: Use std::shared_ptr for shared-ownership resource management', Chapter 4. Smart Pointers, Effective Modern C++ by Scott Meyers'.