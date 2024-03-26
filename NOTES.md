# Notes

## Key value pairing

To support the developmnet of this project I have to learn to use `std::unordered_map` which is a hash map provided by STL (c++ standard library).

## Multiple types as values

The key value pairing is pretty straight forward for keys since they are always text (`std::string`) representations but for values, there's the need to use `std::variant` to support multiple types as values. JSON values can be classified roughly in 5 types.

* __text__: represented with `std::string`
* __digits__: represented either with `int` or `double`
* __boolean__: represented as __true__ or __false__ according to it's c/c++ representation that is 1 or 0.
* __json object__: a JSON object can be a value in a nested JSON message.
* __json arrays__: a JSON array that can be represented as the value of specific key.

```c++
std::variant<int, double, std::string, json_object, json_array>
```

## Performance

At the begining of writing this project I felt that it was well structured. However with time and re reading my code I did some research on how to improve it's performance.

1. I was making __unnecesary copies__: In my `json_object` and `json_array` classes, when passing elements to the `put` or `add` functions, I was making __unnecessary copies__ of the objects.
cpp

```c++
void json_object::put(const std::string& key, const std::string& value) {
    key_value[key] = value;  // Instead of copying, move it
}

void json_array::add(const std::string &element) {
    elements.push_back(element);  // Instead of copying, move it
}
```

Instead, you can use move semantics to avoid these copies. For example:

```c++
void json_object::put(const std::string& key, const std::string& value) {
    key_value[key] = std::move(value);
}

void json_array::add(const std::string &element) {
    elements.push_back(std::move(element));
}
```

## Memory management

Since the `json_object` and `json_array` classes don't directly manage any dynamic resources like pointers, there's no need to explicitly deallocate any resources in their destructors. The default destructors provided by the compiler will handle the cleanup of member variables automatically when an object of these classes goes out of scope. So there's no need to have default destructor on each class. However, I ended up debating myself if the use of pointers as containers for the JSON object/array (`std::variant<int, bool, double, std::string, json_object, json_array>>* key_value;` `std::vector<std::variant<int, bool, double, std::string, json_object, json_array>> * elements;`) will it be beneficial? What I found is that:

> If I declare the private members `key_value` and `elements` as `pointers`, then I'll need to manage the memory allocation and deallocation manually. This means the implementation of custom constructors, destructors, copy constructors, and copy assignment operators to ensure proper memory management.

>> * *__Constructor__*: Allocate memory for the pointers and initialize them properly.
>> * *__Destructor__*: Free the allocated memory in the destructor to prevent memory leaks.
>> * *__Copy Constructor__*: If I plan to create copies of objects, I'll need to implement a copy constructor that performs a deep copy of the dynamically allocated memory.
>> * *__Copy Assignment Operator__*: Similarly, if I plan to assign one object to another, I'll need to implement a copy assignment operator that performs a deep copy.

Therefore I ended up debating myself.
> What's the benefit doing that?
>> These are the scenarios I found could be benficial:
>>
>> 1. *__Dynamic Memory Management__*: Pointers allow me to manage memory dynamically, which can be useful when the size of the data structures is not known at compile time or when you need to allocate memory on the heap.
>>
>> 2. *__Reduced Memory Footprint__*: In some cases, using pointers can reduce the memory footprint of objects. Instead of storing large data structures directly within the object, you store a pointer to the data structure, which can be more memory-efficient, especially if the data structures are large or if they're not always needed.
>>
>> 3. *__Deferred Initialization__*: Pointers allow me to defer the initialization of objects until they're actually needed. This can improve performance by avoiding unnecessary initialization of objects that might not be used.
>>
>> 4. *__Custom Memory Management__*: Using pointers gives you more control over memory management. You can choose when to allocate and deallocate memory, which can be beneficial in resource-constrained environments or when you need to implement custom memory management strategies.
>>
>> However, using pointers also comes with some potential drawbacks:
>>
>>
>> * *__Manual Memory Management__*: With great power comes great responsibility. I need to manually manage memory allocation and deallocation, which can be error-prone and lead to memory leaks or segmentation faults if not done correctly.
>>
>> * *__Complexity__*: Using pointers can increase the complexity of my code, especially when dealing with ownership semantics, copying, and assignment. I need to ensure proper handling of ownership and avoid issues like dangling pointers or double deletion.
>>
>> * *__Potential Performance Overhead__*: Dynamic memory allocation and deallocation can incur a performance overhead compared to stack-based allocation. Additionally, accessing data through pointers might introduce additional indirection and cache misses, impacting performance.

### TLDR

Using pointers for class members can be beneficial in certain scenarios where dynamic memory management or deferred initialization is required. However, it also introduces complexity and requires careful management to avoid memory-related issues.

#### Leason LEARNED

Always consider the specific requirements and trade-offs of your application before deciding whether to use pointers.

## Is simple always best?

In the context of my project, using references for class members (`key_value`, `elements`) can be a bit different.

### For json_object Class

* *__key_value__*: This member represents the key-value pairs in my JSON object. Since the size of the map may vary, and dynamic memory management might not be necessary, using a reference (`std::unordered_map<std::string, std::variant<int, bool, double, std::string, json_object, json_array>>&`) would be a suitable choice. References provide simplicity and safety and avoid unnecessary overhead associated with pointers.

### For json_array Class:

* *__elements__: This member stores the elements of your JSON array. Similar to key_value, using a reference (`std::vector<std::variant<int, bool, double, std::string, json_object, json_array>>&`) would be appropriate if the size of the array is known at construction and dynamic memory management isn't needed.

> Overall, in my JSON classes, using references for class members would likely be beneficial due to their simplicity, safety, and avoidance of unnecessary dynamic memory overhead.
> However, if my design requires dynamic memory management or deferred initialization of members, you may consider using pointers (possibly smart pointers like std::unique_ptr or std::shared_ptr) instead.

## Smart pointers

This project (somehow) highlight is the utilization of smart pointers. Or better say... Me learning about smart pointers.
Since I am new to the topic of smart pointers I did some research and I hope have summarized things right.
> Smart pointers are a feature introduced in C++11 to manage dynamic memory more safely and conveniently compared to raw pointers. They are part of the C++ Standard Library and provide automatic memory management, meaning they handle memory deallocation when the object they point to is no longer needed. There are three main types of smart pointers:
>
> 1. *__`std::unique_ptr<T>`:
>    * Represents exclusive ownership of a dynamically allocated object.
>    * Only one unique_ptr can own a resource at a time.
>    * When a unique_ptr is destroyed, it automatically deallocates the memory it owns.
>    * Transfer of ownership can be done using move semantics, making it useful for resource management in C++.
>
> Example:
>
> ```c++
> std::unique_ptr<int> ptr(new int(5));
> ```
>
> 2. *__`std::shared_ptr<T>`__*:
>    * Represents shared ownership of a dynamically allocated object.
>    * Multiple shared_ptr instances can point to the same resource.
>    * Uses reference counting to track the number of shared_ptr instances pointing to the resource.
>    * The resource is deallocated only when the last shared_ptr pointing to it is destroyed.
>    * Provides a form of garbage collection for shared resources.
>
> Example:
>
> ```c++
> std::shared_ptr<int> ptr1 = std::make_shared<int>(5);
> std::shared_ptr<int> ptr2 = ptr1; // Shared ownership
> ```
>
> 3. *__`std::weak_ptr<T>`__*:
>    * Represents non-owning "weak" references to an object managed by std::shared_ptr.
>    * Does not contribute to the reference count of the shared resource.
>    * Used to break circular references between shared_ptr instances to avoid memory leaks.
>    * It can be converted to a shared_ptr to access the resource but without affecting its reference count.
>
> Example:
> ```c++
> std::shared_ptr<int> sharedPtr = std::make_shared<int>(10);
> std::weak_ptr<int> weakPtr = sharedPtr; // Weak reference
> ```
>
> Smart pointers provide several benefits over raw pointers, including automatic memory management, prevention of memory leaks, and improved code clarity and safety. They are essential tools for modern C++ memory management.

## Time to choose my poison

Based on my code and considering factors such as ease of use, performance, and memory management, here are some choices I have made:

1. *__For JSON Object and JSON Array Containers__*:
   * It's beneficial to use smart pointers (`std::shared_ptr`) for managing objects like `json_object` and `json_array` within the container types (`std::unordered_map` and `std::vector`).
   * Smart pointers offer automatic memory management with shared ownership semantics, ensuring that objects are properly deallocated when they are no longer needed.
   * This approach simplifies memory management for users and reduces the risk of memory leaks or premature deallocation.

2. *__For Container References__*:
   * Using references (`&`) for containers like `std::unordered_map` and `std::vector` can provide efficient access to container elements without introducing the complexity of manual memory management.
   * References allow users to work directly with the containers without worrying about memory allocation or deallocation.

3. *__For Individual Elements__*:
   * For individual elements stored within the containers (e.g., values in `std::variant` or elements in `std::vector`), using values (not pointers or references) is generally the simplest and most efficient approach.
   * Values provide straightforward access to data without the overhead of indirection.

### Here's a summary of my choicess

* Use smart pointers (`std::shared_ptr`) for managing `json_object` and `json_array` instances within container types (`std::unordered_map` and `std::vector`).
* Use references (`&`) for accessing container objects (`std::unordered_map` and `std::vector`) to avoid unnecessary copying and ensure efficient access.
* Use values for individual elements stored within the containers for simplicity and efficiency.

By following these choices, I believe I can create a JSON library that strikes a balance between ease of use, performance, and memory management, providing users with a convenient and efficient way to work with JSON data.
