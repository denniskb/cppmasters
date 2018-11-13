# C++ Master Class Assignment 4: Exception Safety

## Introduction

### Error Handling Mechanisms

C++ offers two mechanisms to handle errors inside your application: exceptions and assertions. Exceptions are used to signal *exceptional* circumstances that resulted in a program error, while assertions are used to enforce preconditions and invariants. Some operations are guaranteed to always succeed, like assigning Plain Old Data (POD) types to each other (primitive types (char, int, float, bool, ...) and aggregates thereof), destroying an object, or deallocating memory. Some operations may fail and raise an exception as a result, these are mostly operations involving resources like allocating memory, spawning threads, or opening network connections. The programmer can choose to handle exceptions by catching them, ignore exceptions in which case they are propagated up the call stack until they are either caught or cause program termination, or she can even raise their own exceptions if an unrecoverable error occurs in her own code.

Let's have a look at some examples. Since memory (RAM) is a limited resource, allocations such as `new int[N]` might fail if there isn't enough available memory in the system. You can blindly allocate memory and hope that everything goes well (or that the caller of your code takes care of error handling) or you can catch and handle errors yourself:

```
int * buffer;
try
{
    buffer = new int[N]; // might fail
}
catch (std::exception const &)
{
    /* Error handling options:
     * - Print error message and apologize to user.
     * - Pause program execution for a brief moment and hope that memory
     *   becomes available in the meantime -> try again.
     * - Try again with a smaller N.
     * - ... */
}
```

You can also raise your own exceptions:

```
void train_neural_network(...)
{
    ...
    if (...)
    {
        throw std::exception("Training diverged :(");
    }
}
```

If an exception is raised, normal program flow is aborted at that point, and the exception is propagated up the call chain until it is either caught (and handled) or until it results in program termination (which happens when the exception propagates all the way to `main()` without being caught). The advantage of using exceptions for error handling is that they don't clutter normal program flow: The implementation of an algorithm remains unchanged -- we do not abuse return types to signal error codes (on the callee side) or clutter our code to check said error codes (on the caller side). Furthermore, exceptions are optional to handle -- sometimes we simply don't have the means to handle an exception well and it would make more sense to pass it on to the caller, or we are prototyping and don't care.

Assertions on the other hand are used to enforce preconditions and invariants (postconditions should be enforced by testing). In contrast to exceptions, assertions are not graceful. If an assertion evaluates to `false`, the program is immediately aborted and an error message printed to `stdout` with details about the assertion (condition, line number, etc.). This is because, as opposed to exceptions which are used to handle unforseeable, exceptional, but *recoverable* circumstances, assertions are used to enforce rules over which the programmer has *complete control*. They are a tool to catch bugs early during development time. **Thus they only function in the debug version** and are compiled to no-ops in the release version. Assertions come in tow falvors: `static_assert` is used to check assertions during compile time and is built into the language, while `assert` is used to check assertions during runtime and can be found in the `<cassert>` header. Let's have a look at some examples:

```
#include <cassert>

// @pre T must be a number
template <typename T>
T sum(T a, T b)
{
    static_assert
    (
        std::is_same<T, short>::value ||
        std::is_same<T, int>  ::value ||
        std::is_same<T, float>::value,
        "T sum(T, T) precondition failed: T must be a number type."
    );
    return a + b;
}

// @pre x must be >= 0
double square_root(double x)
{
    assert(x >= 0.0);
    // Alternative form with error message:
    // assert(x >= 0.0 && "double square_root(double x) precondition failed: x >= 0");
    ... 
}

// @invariant capacity() - size() >= 0
template <typename T>
class vector
{
...
public:
    ...
    void push_back(T x)
    {
        if (size() == capacity())
        {
            try
            {
                // re-allocate internal buffer and copy over current elements
            }
            catch (std::exception const &)
            {
                throw std::exception("vector reached its size limit, failed to grow the vector :(");
            }
        }

        _data[size()] = x;
        _size++;

        // At the end of every non-const function (i.e. every function that modifies the state
        // of the object) we ensure that all invariants still hold:
        assert(capacity() - size() >= 0 && "Invariant broken: capacity() - size() >= 0");
    }
};
```

To summarize:

<table>
<tr><th rowspan="2">Exceptions</th><th colspan="2">Assertions</th></tr>
<tr><th>Runtime Assertions</th><th>Static Assertions</th></tr>
<tr><td><code>#include <exception></code></td><td><code>#include <cassert></code></td><td>[built-in]</td></tr>
<tr><td><code>try {...} catch(...) {...}</code><br/><code>throw ...;</code></td><td><code>assert(...);</code><br/><code>assert(... && "msg");</code></td><td><code>static_assert(..., "msg")</code></td></tr>
<tr><td colspan="2" align="center">runtime</td><td>compile time</td></tr>
<tr><td>release & debug version</td><td>debug version only</td><td>release & debug version</td></tr>
<tr><td>exceptional and unforseeable errors</td><td colspan="2">preconditions and invariants (completely avoidable)</td></tr>
</table>

### Levels of Exception Safety

In programming languages we do not only differentiate between operations that are guaranteed to succeed and ones that might fail, we also differentiate different levels of failure, referred to as (levels 0 to 3) of "exception safety". The following hierarchy was formalized by [David Abrahams](https://www.boost.org/community/exception_safety.html) and classifies all potential error conditions according to their severity and recoverability (similarly to how Noam Chomsky classified formal languages into a hierarchy of increasing expressiveness):

- **Level 0 "no exception safety"**: No guarantees are made. The operation might succeed or the computer might catch fire. I.e. after the operation the application might be in an undefined/invalid state.
- **Level 1 "*basic* exception safety"**: If an error occurs the application is left in a well-defined, valid state and all invariants are preserved. However, the operation might have caused side effects so that the application finds itself in a different (though valid) state from before the operation was invoked.
- **Level 2 "*strong* exception safety"**: Like level 1, but additionally no side effects are produced. I.e. if an operation that offers a strong exception safety fails, the application's state remains unmodified, as if the operation had never been invoked in the first place.
- **Level 3 "*no-throw" exception safety"**: The operation is guaranteed to succeed. If an error occurs it will be handled internally and not be observable by clients (either directly (return value/program execution) or indirectly (side effects)).

Built-in functions and classes offer different levels of exception safety. For example destruction, de-allocation, POD assignment are no-throw. Arithmetic division offers no safety guarantees (since one might divide by 0). The exception safety level of all built-in and library functions is well documented in all popular [API references](http://www.cplusplus.com/). The exception safety level of custom code has to be determined through careful, holistic analysis -- it is not a simple minimum of the exception safeties of invoked functions for example. Futhermore, when analyzing functions according to their exception safety we confine ourselves to the scope of the C++ language and its runtime. Otherwise any analysis would be worthless as one could argue that any function can fail due to an imminent meteor impact. I.e. we do not consider outside forces during the analysis (operating system, hardware failures, etc.).

A high level of exception safety is very desireable as it results in robust and dependable code that is crucial to implement critical applications such as planetary scale databases or space ship flight controllers. The higher up we go the hierarchy the more difficult (if not impossible) and *costly* it becomes to offer a certain level of exception safety.

**Further Reading**<br/>
- [Tutorial on using C++ Exceptions](https://www.tutorialspoint.com/cplusplus/cpp_exceptions_handling.htm)
- [Overview of Exception Safety (Wikipedia)](https://en.wikipedia.org/wiki/Exception_safety)
- [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library (whie paper by David Abrahams)](https://www.boost.org/community/exception_safety.html)

## Assignment 4a

Perform an exception safety analysis of your `memory_block`'s *constructor*, *copy constructor*, *copy assignment operator*, and *destructor*. Extend your comments of each of these methods with an '@exception' clause and not only *identify* the exception safety level they provide, but also justify your choice in-depth.

### Hints

- Assignment of primitve data types (ints, pointers, PODs) can never fail (level 3).
- Freeing of memory (`delete`/`delete[]`/`free()`) and destruction of objects can never fail (level 3).
- Allocation of memory (`new`/`new[]`/`malloc()`) might fail but leaves no memory leaks behind if it does (level 2).
- If an exception occurs inside a constructor, the object construction is aborted alltogether. Objects are either fully constructed and initialized or not constructed at all.
- For each operation inside a method, ask yourself: "Can this operation fail?", if the answer is "yes", follow up with "What state is the application left in, if it does?". This is how you determine exception safety.
- What is the exception safety of `std::copy`?
- Traditionally, basic exception safety (level 1) includes leak freedom (referring to resources such as memory, file handles, etc.) as one of its requirements. We are going to ignore this to simplify the assignment. Trying to incorporate leak freedom into our analysis would require knowledge of advanced C++11 features which are subject of later assignments. For now, if an operation adheres to the definition provided above, it can be considered level 1, even if it leaks memory.

## Assignment 4b

*After receiving your feedback*, try to improve the exception safety of the methods analyzed in 4a as much as you can. Modify the code accordingly, and identify as well as justify the new exception safety level.