# C++ Master Class Assignment 2: The Rule of Three

## Introduction
In the last assignment we learned the value of RAII, how it allows us to implement automatic resource management and how, if we perform all initializations for a class inside its constructor, C++ can offer us very strong guarantees about object creation, namely that an object is either fully constructed and initialized or not constructed in the first place, aka RAII (=Resource Allocation Is Initialization). RAII is one of the building blocks for writing ACIDic code.

Another great feature of C++ is its value semantics. Be default, all objects are (deeply) copied by value. References have to be requested explicitly. This has great advantages: By taking a full copy we can modify both copies to our will without fear of affecting each other, localizing our changes, making our code more managable/readable. If we want a reference instead, either because we want to avoid deep copies for performance reasons, or because we DO want modifications to an object be reflected in our reference, that option is still there, but it has to be requested explicitly.

If we build classes by merely composing them of other, built-in types and classes (core language+standard library), C++ takes care of everything for us. It handles the construction/destruction of our classes' members (in the correct order) and provides default implementations for constructors, copy constructors, move constructors, etc. that simply do the right thing. For example:

```
struct Person {
    std::string name;
    int age;
}

Person p1("frank", 46);
Person p2 = p1;
p2.set_age(21);

print(p1.name() + p1.age()); // "Frank 46"
print(p2.name() + p2.age()); // "Frank 21"
```

However, if we do implement our own constructor (as we did for the memory_block class) the Rule of Three (aka "The Big Three") kicks in, stating: "If any of {constructor | copy constructor | copy assignment operator} is provided by the user, all three must be. The reasoning behind this is the following: If we provided a user-defined constructor, we did so because the default constructor (which default-initializes all members) didn't do what we wanted, we needed special initialization logic for our class. Chances are we will also need special logic to copy our class around correctly. The Rule of Three is a rule of thumb. There are 1% of cases where it does make sense to have a user-defined constructor but default copy constructors/assignment operators are fine. For 99% of cases however, including our memory_block class, the rule holds.

## Assignment 2
In this assignment we will add a user-defined copy constructor and copy-assignment operator to our class, the signatures for both methods are provided below. Please also extend your test code to verify the new functionality.

```
class memory_block {
    public:
    ...
    memory_block(memory_block const & other);
    memory_block & operator=(memory_block const & rhs);
    ...
}
// rhs = right-hand-side
```

### Hints
1. You'll find plenty of sample implementations for copy constructors and copy assignment operators online
2. 5 bonus points if you come up with a small optimization for the copy assignment operator that avoids copies in certain cases.
3. Please do not attempt to be clever and implement the "copy and swap idiom". This requires advanced knowledge of C++11 and exception safety which we will cover in dpeth in later assignments.