#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

// TODO: Complete!
class list
{
private:
	struct node
	{
		node * next = nullptr;
		int value = 0;
	};

	// HINT: Don't store any data inside _head itself but treat it
	// as a *pointer to the FIRST element* (rather than the first element itself)
	// makes coding easier.. (no if/else required)
	node * _head = new node;

	// OPTIONAL: Implement an explicit tail-pointer and size, see if it changes anything!
	//node * _tail = nullptr;
	// int size = 0;

public:
	// Delete all allocated nodes properly
	~list()
	{
		// TODO
	}

	// Return number of elements inside list
	int size() const
	{
		// TODO
		return 0;
	}

	// Append element at the end of list
	void append(int x)
	{
		// TODO
	}

	// Insert element 'x' at specific posiiton 'i' (i = 0 means insert at the front)
	void insert(int i, int x)
	{
		// TODO
	}

	// Return element at specific position 'i' (i = 0 means return from the front)
	int at(int i)
	{
		// TODO
		return 0;
	}

	// Delete element at specific position 'i' (i = 0 means delete very first element)
	void remove(int i)
	{
		// TODO
	}
};

int main()
{
	{	// Test-code (run in DEBUG mode!!!)
		list l;
		assert(l.size() == 0);

		l.append(1);
		assert(l.size() == 1);
		assert(l.at(0) == 1);

		l.append(2);
		assert(l.size() == 2);
		assert(l.at(1) == 2);

		l.append(3);
		assert(l.size() == 3);
		assert(l.at(2) == 3);

		l.insert(1, 5);
		assert(l.size() == 4);
		assert(l.at(0) == 1);
		assert(l.at(1) == 5);
		assert(l.at(2) == 2);
		assert(l.at(3) == 3);

		l.remove(3);
		assert(l.size() == 3);
		assert(l.at(2) == 2);
	}

	// Benchmark (run in RELEASE mode!!!)
	int const ITER = 20'000; // might need to adjust slightly for your machine

	{// Append (at the end)
		std::vector<int> v;
		list l;
		std::chrono::high_resolution_clock c;

		auto t1 = c.now();
		for (int i = 0; i < ITER; i++)
			v.push_back(i);
		auto t2 = c.now();
		std::cout << "tAppend (vector): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;

		t1 = c.now();
		for (int i = 0; i < ITER; i++)
			l.append(i);
		t2 = c.now();
		std::cout << "tAppend (list): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	}
	std::cout << std::endl;
	{// Prepend (at the front)
		std::vector<int> v;
		list l;
		std::chrono::high_resolution_clock c;

		auto t1 = c.now();
		for (int i = 0; i < ITER; i++)
			v.insert(v.begin(), i);
		auto t2 = c.now();
		std::cout << "tPrepend (vector): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;

		t1 = c.now();
		for (int i = 0; i < ITER; i++)
			l.insert(0, i);
		t2 = c.now();
		std::cout << "tPrepend (list): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	}
	std::cout << std::endl;
	{// Insert (in the middle)
		std::vector<int> v;
		list l;
		std::chrono::high_resolution_clock c;

		auto t1 = c.now();
		for (int i = 0; i < ITER; i++)
			v.insert(v.begin() + v.size() / 2, i);
		auto t2 = c.now();
		std::cout << "tPrepend (vector): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;

		t1 = c.now();
		for (int i = 0; i < ITER; i++)
			l.insert(i / 2, i);
		t2 = c.now();
		std::cout << "tPrepend (list): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	}
}
