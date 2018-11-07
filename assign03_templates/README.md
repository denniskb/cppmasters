# C++ Master Class Assignment 3: Templates

## Introduction
C++ is a *statically typed* language. This means two things:

1. (typed) It is a *typed* language. Variables have types such as 'number', 'string', etc, with very strict rules as to which types can interoperate with one another in which ways. Furthermore, C++ allows user-defined types (such as our memory_block class).

2. (static) The types of all variables must be known at *compile-time*.

This brings a lot of advantages for code quality and robustness:

- It forces you to critically think about the interfaces of your classes (member types, method signatures) and plan them in advance, generally leading to better code.
- It catches many bugs automatically, at compile-time, that would otherwise either go completely unnoticed or go unnoticed until runtime, where, if they hadn't been caught in testing, could result in catastrophic failure. Static typing prevents non-sensical statements such as summing two postcodes for example.

It does require more typing and more time spent up-front, so for short prototypes (~100loc) a dynamic language such as Matlab or Python might be more suitable. Also, sometimes we simply can't know the type of an object in advance, like our `memory_block` class. We would like to be able to store all types in it: ints, strings, future user-defined types that we can't even perceive now. So we have to resolve to wildcards such as `void *`. We gain generic functionality at the cost of type safety. For this purpose, C++ has templates.

Templates enable both generic and type-safe code:

```
template <typename T>
class array
{
private:
    T * _data;
    ...
public:
    ...
};

// Array of integers:
array<int> exam_scores;
// Array of strings:
array<string> student_names;

```

```
template <typename T>
class matrix{...};

// single precision matrix:
matrix<float> camera;
// double precision matrix:
matrix<double> projection;
```

Internally, the C++ compiler simply scans your entire source code for all types a template class has been used with, and then writes a separate class definition for each one according to the 'template' you provided, hence the name. I.e. given

```
template <typename T>
class array{... T * _data; ...};
...
array<int> phone_book;
...
array<string> contacts;
```
 
the compiler generates the following code:

```
class array_int{... int * _data; ...};
class array_string{... string * _data; ...};
...
array_int phone_books;
array_string contacts;
```

## Assignment 3
In this assignment we want to 'templatize' our `memory_block` class. This will free us from constantly converting between `void *`/`char *` and the type we are actually storing inside, will resolve the problem of what `data()`'s return type should be, and will add type safety to our class. Furthermore, it will allow us to store complex, user-defined types and correctly `delete[]` them (invoking their destructors upon deletion).

We also want to overload the subscript operator `[]` to make our class more useable. Please add the following definitions to your class and test the new operator:

```
template <typename T>
class memory_block
{
private:
    ...
public:
    ...
    /* @return Element at index 'i' (starting from 0) */
    T & operator[](std::size_t i);
    T const & operator[](std::size_t i) const;
};
```

### Hints
- You will have to make more changes to your class than depicted above. Types of variables and method singatures will change, and comments should be updated accordingly to reflect the changed semantics.
- You will have to update all uses of `memory_block` (inside your test code) since the class definition has changed.
- Bonus points: Implement a simple "`dummy`" class that consists of nothing else but a single print statement inside its copy constructor, copy assignment operator, and destructor. Store it inside our `memory_block` and convince yourself that
    1. when `memory_block` is copied, all its `dummy` elements' copy constructors are invoked as well, and that
    2. when `memory_block` goes out of scope, not only its destructor is invoked, but so is the destructor of every single `dummy` inside it. 
- If you struggle with the previous bullet point, have a look at the commit *message* history of the reference implementation file (assign03.cpp) - do not peak at the code itself just yet!
