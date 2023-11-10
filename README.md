# FastList

Are you in a hurry? Do you need to push and pop very very VERY FAST? Do you want quick and easy access to your data, almost as quick as access can possibly be? Look no further, my hasty friend, cause you stumbled on the right library for you! <br>
This is FastList, the list implementation you were dreaming of. This list uses a dynamic array in its backend that shrinks as widens as data is pushed and pop from the structure, aiming to guarantee the fastest possible push and pop times. All of this wrapped in a very comfortable interface of functions that can be called on FastLists to make your programming easy and your code fast. <br>
To start your super-fast programming session copy the **fastlist.h** file in your projects's folder and include the code with **#include "fastlist.h"**. But be careful: are you sure you can handle such speeds?

## FastList interface

So you downloaded the library and I bet you want to know how to use all its cool and speedy functions. It is very easy, follow the tips below and you'll learn how to use FastLists in no time.

### Constructors

A new fast structure is about to be born!<br>

You can create a FastList with its constructor

```
    FastList<int> fast_list = FastList<int>();
```

Complexity: O(1)

And you can also specify the initial size of the FastList

```
    FastList<int> fast_list = FastList<int>(64);
```

Complexity: O(1)

You can copy a FastList with the copy constructor. The newly created FastList performes a deep copy of the original FastList. However items stored in the FastList are not deep copied, so if you use the FastList to store pointers you will get a new FastList with pointers to **the same memory adresses**.

```
    FastList<int> fast_list_copy = FastList<int>(fast_list);
```

Complexity: O(len(fast_list))

If you want to move your FastList into a different variable you can with move semantics and the move constructor. Be aware that with this constructor, the fast_list variable below will be unasable after it is moved.

```
    FastList<int> fast_list_copy = FastList<int>(move(fast_list));
```

Complexity: O(1)

You can also use the 'new' operator to allocate the FastList in the heap

```
    FastList<int> * fast_list = new FastList<int>();
```

Complexity: O(1)

And if your really have no time, you can even build a FastList form an initializer list

```
    FastList<int> fast_list = {1,2,3,4};
```

Complexity: O(len({...}))<br>
Items are pushed one by one, the cost is amortized since pushes only take O(len(list)) when the list is full to its current size.

### Destructor

Ok, You don't need this istance of a FastList anymore... what happens when it is destroyed?<br>

Every time you destruct a FAstList, all its content will be freed, but eventual data items for which pointers are stored in the FastList will not be destroyed.

```
    delete fast_list;
```
Complexity: O(1)

### Adding items

When data comes in fast, FastList os ready to jump into action.<br>

There are multiple ways to add items to the FastList:

- **enqueue(T item)**: add the item to the end of the FastList, using it as a queue

```
    int x = 10;
    fast_list.enqueue(x);
```
Complexity: O(len(list))<br>
Amortized cost: O(1)

- **enqueue_all(T... items)**: add all the items to the end of the FastList, using it as a queue

```
    fast_list.enqueue(1,2,3,4);
```
Complexity: O(x)<br>
Same as calling enqueue x times, where x is the number of items to enqueue

- **push(T item)**: add the item to the end of the FastList, using it as a stack

```
    int x = 10;
    fast_list.push(x);
```
Complexity: O(len(list))<br>
Amortized cost: O(1)

- **push_all(T... items)**: add the item to the end of the FastList, using it as a stack

```
    fast_list.push_all(1,2,3,4);
```
Complexity: O(x)<br>
Same as calling push x times, where x is the number of items to push

- **add_first(T item)**: add the item to the beginning of the FastList

```
    int x = 10;
    fast_list.add_first(x);
```
Complexity: O(len(fast_list))

- **add(T item,int index)**: add the item to the FastList at the specified index. The rest of the FastList is shifted by one position.

```
    string hello = "hello";
    fast_list.add(hello,3);
```
Complexity: O(len(fast_list))

### Removing Items

As fast as throwing garbage out of the window! Please, don't throw garbage out of your window. Please.<br>

You can remove items from the front of the FastList by calling the **dequeue()** function, which returns the removed item. If the FastList is empty, an **EmptyListRemovalException** is thrown. You can use the structure as a queue in this way.

```
    string removed = fast_list.dequeue();
```
Complexity: O(len(fast_list))

You can remove items from the end of the FastList by calling the **pop()** function, which returns the removed item. If the FastList is empty, an **EmptyListRemovalException** is thrown. You can use the structure as a stack in this way.

```
    string removed = fast_list.pop();
```
Complexity: O(len(fast_list))<br>
Amortized cost: O(1)

If you want to remove an item at a specific index you can call **remove_at(int index)**. This function will remove the removed item. If the index is out of bounds an **IndexOutOfBoundsException** is thrown.

```
    string removed = fast_list.remove_at(3);
```
Complexity: O(len(fast_list))

If you want to remove a specific item from the FastList you can call **remove(T item)**. This function will remove the first instance that matches the parameter item from the FastList. If the FastList does not contain any instances of the item, an **ItemNotFoundException** is thrown.

```
    string removed = fast_list.remove("hello");
```
Complexity: O(len(fast_list))

If you want to delete all at once multiple items from the FastList you can call the **remove_all(T... items)** function. This will return a FastList containing the removed items. If the FastList does not contain any instances of an item, an **ItemNotFoundException** is thrown.

```
    FastList<string> removed = fast_list.remove("hello","how","are","you");
```
Complexity: O(len(fast_list)^2)<br>

To delete all instances of a specific item at once, call **remove_every(T item)**. This function will return a single istance of the removed item. If the FastList does not contain any instances of the item, an **ItemNotFoundException** is thrown.

```
    string removed = fast_list.remove_every("hello");
```
Complexity: O(len(fast_list)^2)

To delete all instances of a multiple items, call **remove_all_every(T... items)**. The function will return a FastList with an single istance of the removed items (you will not get copies). If the FastList does not contain any instances of some item that you want to remove, an **ItemNotFoundException** is thrown.

```
    FastList<string> removed = fast_list.remove_all_every("hello","how","are","you");
```
Complexity: O(len(fast_list)^2)<br>
Note: this is not O(len(fast_list)^3) because, in the worst case, this function calls **remove()** once on each item of the list.

### Getting items

Seamless access in the fastest way possible, that's how it's done!<br>

To get a copy of the item contained at a specific index, call **get(int index)**. If the index is out of bounds an **IndexOutOfBoundsException** is thrown. This is a copy of the item, editing this copy will have no effect on the item in the FastList.

```
    double number = fast_list.get(5);

    // this will not change the double contained in the FastList
    number++;
```
Complexity: O(1)

To get a reference to the item contained at a specific index, call **getReference(int index)**. If the index is out of bounds an **IndexOutOfBoundsException** is thrown. This is a reference to the item, changes to this variable will reflect inside the FastList.

```
    double& number = fast_list.getReference(5);

    // this will change the value of the double in the FastList
    number++;
```
Complexity: O(1)

You can also get a reference to an item in the FastList through the **[]** operator.

```
   double& number = fast_list[5];

    // this will change the value of the double in the FastList
    number++; 
```
Complexity: O(1)

You can also access the front and the back of the FastList with **front()**, **back()**, **frontReference()** and **backReference()** which will return a copy or a reference to the first or last element of the FastList. All of these functions have O(1) complexity.<br>

If you cannot decide which item to hastily retrieve from your FastList you can retrieve a random one by sampling. To sample from the FastList use the **sample()** function (with optionally a set seed), or with the **sampleReference()** function if you want to edit the value in the list.

```
    // rnd is a random integere from the FastList
    int rnd = fast_list.sample();

    // A random elementel will be incremented inside the FastList
    fast_list.sampleReference() += 1;
```

### Some other useful utility

Let's hastily go over some fundamental functions<br>

You can get the current length of the FastList with the **getLength()** function

```
    int list_length = fast_list.getLength();
```
Complexity: O(1)

You can check if the FastList is empty with the **isEmpty()** function

```
    if ( fast_list.isEmpty() ){
        std::cout << "The fast list is empty";
    }
```
Complexity: O(1)

You can check if the FastList contains a given item with the **contains(T item)** function

```
    if ( fast_list.contains(100) ){
        std::cout << "The fast list contains at least one instance of 100";
    }
```
Complexity: O(len(fast_list))

You can get the first index where a given item appears in the FastList with the **index_of(T item)** function

```
    // fast_list = [1,2,3,100,5,6,7,100]
    
    int index_of_100 = fast_list.index_of(100);
    
    // index_of_100 is 3
```
Complexity: O(len(fast_list))

You can count how many instances of a given item there are in the FastList with the **count_instances_of(T item)** function

```
    int instances_of_100 = fast_list.count_instances_of(100);
    std::cout << "100 appears " << instances_of_100 << " times in the fast list";
```
Complexity: O(len(fast_list))

### Ordering of the items

Your FastList rearranged, in the blink of an eye!<br>

The FastList can be sorted, reversed and shuffled. <br>
There are 2 ways to sort the FastList: you can either sort the original FastList with **sort()** or get another FastList which has the same items as the original but sorted without modifying the original using **getSorted()**

```
    // fast_list will be chaned
    fast_list.sort();

    // old_list will not change
    FastList<char> new_list = old_list.getSorted();
```
Complexity: O(len(fast_list))

There are 2 ways to reverse the FastList: you can either reverse the original FastList with **reverse()** or get a new FastList which has the same items as the original but reversed in order without changing the original using **getReversed()**

```
    // fast_list will be chaned
    fast_list.reverse();

    // old_list will not change
    FastList<char> new_list = old_list.getReversed();
```
Complexity: O(len(fast_list))

There are 4 ways to shuffle the FastList: you can either shuffle the original FastList with **shuffle()** and **shuffle(unsigned int seed)** or get another FastList which has the same items as the original but shuffled without modifying the original using **getShuffled()** or **getShuffled(unsigned int seed)**

```
    // both fast_list and fast_list_with_seed will change
    fast_list.shuffle();
    fast_list_with_seed.shuffle(1000);

    // neither old_list nor old_list_with_seed will not change
    FastList<char> new_list = old_list.getShuffled();
    FastList<char> new_list_with_seed = old_list_with_seed.getShuffled(1234);
```
Complexity: O(len(fast_list))

### Merging FastLists

Seamlessly concatenate all your lists.<br>

You can concatenate FastLists with the **concat(FastList<T>&const list)** and **merge(FastList<T>&const list)** functions. This functions will produce a concatenated FastList by modifying the FastList on which they are called. The parameter of the function is not modified. The FastList are concatenated always at the end

```
    // second_list will not change
    first_list.concat(second_list);

    // third_list will not change
    first_list.merge(third_list);
```
Complexity: O(len(first_list) + len(second_list))

You can also concatenate FastLists by summing them

```
    // second_list and third_list will not change
    first_list = second_list + third_list;

    // fourth_list will not change, first_list will change
    first_list += fourth_list;
```
Complexity: O(len(first_list) + len(second_list))

### Splitting

Cut your FastList as if it was bread!<br>

You can split your FastList into 2 using the **split_at_index(int index)** function. This will return a FastList of FastLists of your chosen type. The first element of this FastList will be a copy of the shortened verion of the original FastList, while the second element will be a newly genertated FastList which contains the tail of the original FastList. The original FastList will be shortened. If the index parameter is out of bounds, an **IndexOutOfBoundsException** will be thrown.

```
    // fast_list = [1,2,3,4,5,6]
    FastList<FastList<T>> list_of_lists = fast_list.split_at_index(3);
    
    // fast_list = [1,2,3]
    // list_of_lists = [[1,2,3],[4,5,6]]
```
Complexity: O(len(fast_list))

If you don't want to split the original FastList you can use the **split_at_index_const(int index)** function 

```
    // fast_list = [1,2,3,4,5,6]
    FastList<FastList<T>> list_of_lists = fast_list.split_at_index_const(3);
    
    // fast_list = [1,2,3,4,5,6]
    // list_of_lists = [[1,2,3],[4,5,6]]
```
Complexity: O(len(fast_list))

Another way to split FastLists is by specifying an item that is used as separator with the **split(T item)** function. This function splits the original FastList into multiple FastLists wherever it finds the separator item, removing it from the generated FastLists. The original FastList will be shrunk to the size of the first element of the returned FastList of FastLists

```
    // fast_list = ['a','b','a','a','c','d']
    FastList<FastList<T>> list_of_lists = fast_list.split('a');
    
    // fast_list = []
    // list_of_lists = [[],['b'],[],['c','d']]
```
Complexity: O(len(fast_list))

Also in this case, you can avoid modifying the original FastList with the **split_const(T item)** function

```
    // fast_list = ['a','b','a','a','c','d']
    FastList<FastList<T>> list_of_lists = fast_list.split_const('a');
    
    // fast_list = ['a','b','a','a','c','d']
    // list_of_lists = [[],['b'],[],['c','d']]
```
Complexity: O(len(fast_list))

### Iterators

The only thing better of working on your FastList quickly, is to do it both quickly and easily!<br>

There are currently 4 types of Iterators for FastLists:

- **FastIter** allows you to iterate over the FastList in order and to edit the items inside the FastList

```
    FastIter fast_iter = fast_list.begin();
    for( ; fast_list.end() != fast_iter ; fast_iter++){
        // This will increment each item of the FastList
        (*fast_iter)++;
    }
```

- **FastIterConst** allows you to iterate over the FastList in order in read only mode

```
    FastIterConst fast_iter = fast_list.cbegin();
    for( ; fast_list.cend() != fast_iter ; fast_iter++){
        // This will not change the values in the FastList
        int x = (*fast_iter) + 1;
    }
```

- **FastIterReverse** allows you to iterate over the FastList in reverse order and to edit values into the FastList

```
    FastIterReverse fast_iter = fast_list.rbegin();
    for( ; fast_list.rend() != fast_iter ; fast_iter++){
        // This will increment each item of the FastList starting from the last position
        (*fast_iter)++;
    }
```

- **FastIterConstReverse** allows you to iterate over the FastList in reverse order in read only mode

```
    FastIterConstReverse fast_iter = fast_list.crbegin();
    for( ; fast_list.crend() != fast_iter ; fast_iter++){
        // This will not change the values in the FastList
        int x = (*fast_iter) + 1;
    }
```

The complexity of all operations with these iterators, including dereferencing, is O(1)

## Applying Functions to the FastList

Quickly and easily transform, filter and accumulate the items in your FastList!<br>

You can transform uniformly all the items in your FastList by applying a function to all its items. To perform this transformation you can call the **apply()** function on a FastList and pass as the parameter of this function the function that you want to apply, as in the example below

```
    int sum_five(int i){
        return i + 5;
    }

    int main(){
        [...]
        // fast_list is [1,2,3]

        fast_list.apply(sum_five);

        // now fast_list is [6,7,8]
    }
```
Complexity: O(len(fast_list) * FC) where FC is the complexity of the argument

If you want to apply the function on your FastList without changing it, you can instead use the **map()** function, which will return a newly generated FastList which is the result of applying the function to the original FastList

```
    int sum_five(int i){
        return i + 5;
    }

    int main(){
        [...]
        // fast_list is [1,2,3]

        FastList<int> mapped = fast_list.map(sum_five);

        // now fast_list is [1,2,3]
        // and mapped is [6,7,8]
    }
```
Complexity: O(len(fast_list) * FC) where FC is the complexity of the argument

Another useful function that you can apply to a FastList is a filter. A filter is a boolean function that decides which items can stay in the FastList and which items have to go. All the items for which the applied function will evaliuate to **true** will be removed from the FastList. You can filter your FastList with the **filter()** function, as in the example

```
    bool is_odd(int i){
        return (i % 2 == 1);
    }

    int main(){
        [...]
        // fast_list is [1,2,3,4,5]

        fast_list.filter(is_odd);

        // now fast_list is [2,4]
    }
```
Complexity: O(len(fast_list) * FC) where FC is the complexity of the argument

One more thing that you can do with FastLists is to accumulate their content. You can use **accumulate(func, T initial_value)** to apply a function that puts together all items in the FastList to produce a single result. The original fastList will not be modified. The example below will help you understand better

```
    int multiply(int i, const int j){
        return i * j;
    }

    int main(){
        [...]
        // fast_list is [1,2,3,4,5]

        int factorial = fast_list.accumulate(multiply,1);

        // factorial is 1 * [ 1 * 2 * 3 * 4 * 5 ]
    }
```
Complexity: O(len(fast_list) * FC) where FC is the complexity of the argument

### Tinkering with RNG

RNG makes things faster!<br>

The FastLists uses the cpp std::mt19937 implementation to generate random numbers. The seed is decided when the structure is constructed, but can be reset or set to an unsigned integer with the **reset_generator()** and **set_genertator(unsigned int seed)** functions

```
    // resets the generator for fast_list
    fast_list.reset_generator();

    // sets the generator with seed 100 for other_fast_list
    other_fast_list.set_generator(100);
```
Complexity: O(1)

### Matching FastLists

Someone once said: "Some FastLists are more equal than others". I am not sure about that, but we can check it!<br>

You can check equality between two FastLists with the **==** (or **!=**) operator(s)

```
    if ( first_list == second_list ){
        std::cout << "The two FastLists are equal"; 
    }
```
Complexity: O(len(first_list))<br>
Note: When the two fastLists have different length, this check is done in O(1)

Two FastLists are consider equal qhen they have the same length and all the items at the same index match between the fastLists.

### Printing FastLists

Output, ladies and gentlemen, output.<br>

You can print FastLists on any output stream by using the **<<** operator. The generated stream will start and end in square brackets, and items will be separated by commas

```
    std::cout << "My fast list: " << std::endl << my_fast_list;
```

## Conclusion

This is the second structure of a series of structures that I will do until I get bored or I don't have time to work on them anymore. Please also check out the CoolList, a different implementation of a list that will definitely make you cooler, which is implemented as a concatenated list of nodes. From me this is all, remember to star the repo and follow my profile for more awesome structures in the future! 