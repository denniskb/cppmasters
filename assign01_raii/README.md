# C++ Master Class Assignment 1: RAII

## Introduction
One of the most important features of C++ is RAII (= Resource Allocation is Initialization). It guarantees a few things:
- Any object that is declared is also fully defined at the same time. Memory allocation and object initialization (running the constructor, setting default values for member variables, etc.) are two indistinguishable things. An object gets either allocated and fully initialized or never allocated in the first place. It can never exist in an intermediate, not well-defined state.
- Object destruction can never fail
- At the end of a scope, all temporary (stack-allocated) objects get destroyed in the reverse order of their declaration.

These are some incredible strong guarantees that allow one to write bullet-proof code in C++ (exception-free, adhering to the ACID principle famous from database design, etc.) Even with these strong guarantees it is still one of the fastest languages because you "only pay for what you (actually) use (in your code)".

Thus, the first lessons, we will focus on RAII, on the automatic management of resources (not only memory, but network connections, threads, etc.). We will start with memory and write building blocks for all data structures of the standard library and implement some of them.

## Assignment 1
C++ allows us to allocate and free memory via "new" and "delete". The problem is that these commands have to be called manually and explicitly. We would like to design a very thin wrapper around new and delete that automatically allocates memory upon its declaration and automatically frees it upon its deletion (when it goes out of scope). I.e., for starters, we are looking to build a "dumb" memory_block class that doesn't know anything about types or destructors, but merely automates memory (de)allocation, relieving the programmer from having (to remember to) manually call "delete". In the first iteration of the assignment the interface should look roughly like this (we will refine it in future iterations):

```
class memory_block {
public:
    // allocates a memory block of n bytes
    explicit memory_block(std::size_t n);
    // frees the allocated memory
    ~memory_block();

    // returns the size in bytes of the allocated memory block
    std::size_t size() const;

    // returns a (raw) pointer to the allocated memory block,
    // so we can read/write from/into it
    void * data();
    void const * data() const;
}
```

### Hints:
1. You will have to make use of "new", "delete", and "reinterpret_cast" for this assignment. You should read a bit about RAII and you'll have to research a little bit about const correctness. Just to get a basic idea, we will cover these in depth lateron.
2. Write some simple test code that checks the basic functionality of your class.
3. Would be nice if you created a github/gitlab/cvrl repo for the assignment solutions.