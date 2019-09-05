# C++ Master Class Assignment 7: Containers (aka Just use std: :vector)

## Introduction
when working with containers one has to differentiate between ‘collections’ and ‘data structures’. A ‘collection’ is an
abstract data type that defines how its elements can be accessed (and other properties), while a ‘data structure’ defines
how a particular collection is *implemented*.

A C++ (std) container is a data structure.

### Collections
Collections are typically categorized along two dimensions:

- Ordering: {Ordered|Unordered} determines whether the elements retain the order in which they were inserted into a
collection or whether they are automatically re-ordered according to some predicate.

- Duplicates: {allowed|forbidden} determines whether the same element can be contained multiple times in a collection.
<table>
<tr>
<th></th><th>Duplicates allowed</th><th>Duplicates forbidden</th></tr>
<tr>
<th>ordered</th>
<td>
<a href="https://docs.oracle.com/javase/8/docs/api/java/util/List.html">List</a></td><td><a href="https://docs.oracle.com/
javase/8/docs/api/java/util/SortedSet.html">Sorted Set</a>,
<a href="https://docs.oracle.com/javase/8/docs/api/java/util/SortedMap.html">Sorted Map</a>
</td>
</tr>
<tr>
<th>Unordered</th>
<td>
<a href="https://docs.oracle.com/javase/8/docs/api/java/util/collection.html">Multi Set</a>,
Multi Map
</td>
<td>
<a href="https://docs.oracle.com/javase/8/docs/api/java/util/Set.html">Set</a>,
<a href="https://docs.oracle.com/javase/8/docs/api/java/util/Map.html">Map</a>
</td>
</tr>
</table>

There are other collections which place additional stipulation on their elements, such as Stack or Queue.

### Data Structures
The C++ standard library allows you to choose from a variety of implementations of each collection:

- List: [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std:: forward list](https://en.cppreference.com/w/cpp/container/forward_list), [std::list](https://en.cppreference.com/w/cpp/container/list)

- Set: [std::set](https://en.cppreference.com/w/cpp/container/set), [std::unordered set](https://en.cppreference.com/w/cpp/container/unordered_set)

- Map: [std::map](https://en.cppreference.com/w/cpp/container/map), [std::unordered map](https://en.cppreference.com/w/cpp/container/unordered_map)

- Multi Set: [std::multiset](https://en.cppreference.com/w/cpp/container/multiset), [std::unordered multiset](https://en.cppreference.com/w/cpp/container/unordered_multiset)

- Multi Map: [multimap](https://en.cppreference.com/w/cpp/container/multimap), [std::unordered multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)

- Sorted Set: [std::priority queue](https://en.cppreference.com/w/cpp/container/priority_queue)

Containers that implement the same collection will expose the same interface and exhibit the same behavior. They merely
differ in how they implement said interface and thus, in the time complexity they guarantee.

The family of `std::unordered_*` containers are named very poorly. Here, ‘unordered’ has nothing to do with unordered
collections as discussed above. In the C++ std library ‘unordered’ signifies that the container internally uses a [hash](https://en.wikipedia.org/wiki/Hash_table) (as opposed to a [tree](https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree)).

In practice, you want to first select a collection that *semantically* suits the needs of your application. For example you
would want to store an mp3 playlist inside a List, the members of a shooting club inside a Set, and students’ elected
courses inside a Multi Map. Afterwards, you choose an implementation of said collection that suits your needs: memory
consumption, time complexity, validity of iterators, etc.

## Assignment 7a
Describe the trade-offs between the various implementations of List and Set. (Solution follows in white, highlight text to
view ONLY after solving both 7a and 7b first!)

<ul style="color:#eee">
<li><p>Linked lists offer constant time manipulation (insertion/deletion of elements), promising to outperform vector
(arrays) for scenarios with many insertion/deletion operations relative to read operations. Vectors on the other hand
require to shift half the array by one slot. However, each manipulation requires a <code>new</code> or <code>delete</code>
operation which are very expensive. Additionally, in order to delete/insert a node, on average, we have to traverse half
the list first. This traversal is very inefficient due to nodes being scattered over memory rather than being laid out
consecutively (preventing the utilization of caches/memory prefetchers). Thus, in practice, lists never outperform arrays,
except for very specialized scenarios such as ‘always insert at the front’ at which point a more specialized data structure
such as ‘queue’ would be better suited.</p>

<p>One advantage of linked lists is that they never invalidate their iterators.</p></li>

<li>Tree sets promise better worst case complexity compared to hash sets. In practice, the worst case for hash sets is
virtually never hit. Tree sets suffer from the same disadvantages of linked lists requiring the allocation of a separate
node for each element. Similarly they don't invalidate iterators.</li>
</ul>

## Assignment 7b

Implement the linked list interface in assign7b.cpp, run the benchmarks, and interpret the results.