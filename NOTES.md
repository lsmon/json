# Notes

## std::string Basics

While working with std::string, I encountered a lot of times that my code was leaking memory (correct term memory leak due to not de-allocating the memory that std::string was creating). Thanks to a youtuber 'theCherno' who dedicates his content to dev and he has a c++ series, I learned a lot about `std::string`, `std::move`, `std::async`, `std::future`, etc.
So I started to apply what I learned about `std::string_view`.

If I switch from using `std::string` to `std::string_view` for checking whether a value represented by a string is an integer, double, or boolean, a few key changes and considerations come into play:

### Efficiency Gains



8 **Performance Improvement:** `std::string_view` does not own the string data it points to; it merely provides a view into the data of some other string, like a `std::string` or a character array. This means that using `std::string_view` can avoid unnecessary copies and allocations, potentially improving performance, especially in scenarios whereEquals you're frequently creating and destroying temporary `std::string` objects for validation purposes.

### Semantic Changes

* **Constness:** Since `std::string_view` is non-owning and intended to offer a view into an existing string, it inherently treats the data it points to as immutable from its perspective. While you can't directly modify the data through the `std::string_view`, the underlying data could still be changed by others who own it, which might lead to dangling views if the original data is destroyed or reallocated.

* **String Lifetime:** I need to ensure that the lifetime of the original string data outlasts the `std::string_view`. If the original data is modified or destroyed while a `string_view` pointing to it is still in use, this could lead to undefined behavior due to accessing invalid memory.

### Modifications to the Validation Functions

* The functions for checking integers, doubles, and booleans would work similarly with `std::string_view`, but I need to construct an `std::istringstream` from the `std::string_view`. This typically requires converting the `std::string_view` to a `std::string` first, as `std::istringstream` constructors do not accept `std::string_view` directly (as of my last update). This conversion step reintroduces a potential performance penalty, which somewhat negates the benefits of using `std::string_view` for these specific operations.

### Adding Boolean Check

To extend the capability to check for boolean values, I can simply compare the `std::string_view` (or `std::string`) directly to known boolean representations, like `"true"` and `"false"`:

```cpp
bool isBoolean(const std::string_view str) {
    return str == "true" || str == "false";
}
```

"__To Remember__", when using `std::string_view` over `std::string`, the main benefit lies in avoiding copies for read-only operations. However, in contexts whereEquals I need to manipulate strings or rely on behaviors specific to `std::string` (such as automatic memory management or compatibility with certain library functions), the advantages might be outweighed by these considerations.

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
std::variant<int, double, std::string, JSONObject, JSONArray>
```

### Custom variant

However since `std::variant` is only available from standard library 17 and above. But most of the default linux versions have c++ 11 as default.
A custom Variant class was needed to be developed.

### Overview

The `Variant` class is a custom implementation that mimics the behavior of `std::variant`. It can store one value from a set of predefined types and provides mechanisms to access and manipulate the stored value safely.

### Components

#### Header File: `Variant.hpp`

The header file defines the `Variant` class template and necessary internal structures. Here's a detailed breakdown:

1. Forward Declarations and Includes:

    * The file includes necessary standard headers like `<iostream>`, `<stdexcept>`, `<utility>`, and `<type_traits>` for various functionalities.

2. Namespace `detail`:

    * Contains helper templates and functions used internally by the `Variant` class.

3. Template Class `IndexOf`:

    * A meta-function to find the index of a type in a list of types.
    * Specializations for recursive type searching and base case when the type is not found.

4. Template Class `TypeAt`:

    * A meta-function to get the type at a specific index in a list of types.
    * Specializations for accessing the type recursively and base case.

5. Function `visit_impl`:

    * Implements the visiting functionality, which calls a visitor on the active type stored in the `Variant`.

6. Class Template Variant:

    * The main `Variant` class template, which can store one value of a specified set of types.
    * Contains member functions and utilities for type-safe storage and retrieval of values.

#### Implementation File: Variant.cpp

This file contains the implementation of the Variant class's member functions. Here's the detailed explanation:

1. Template Functions for Type Construction and Destruction:

    * `destroy<T>()`: Destroys the stored value if it is of type `T`.
    * `construct<T>(const T& value)`: Constructs a value of type `T` in the storage using copy semantics.
    * `construct<T>(T&& value)`: Constructs a value of type `T` in the storage using move semantics.

2. Constructor Template:

    * Initializes the `Variant` with a value of one of the specified types.
    * Uses SFINAE (Substitution Failure Is Not An Error) to ensure that only valid types can be stored.

3. Destructor:

    * Calls the `visit` function to destroy the stored value.

4. Copy and Move Constructors:

    * `Variant(const Variant& other)`: Copy constructor that creates a copy of the stored value.
    * `Variant(Variant&& other) noexcept`: Move constructor that moves the stored value.

5. Assignment Operators:

    * `operator=(const Variant& other)`: Copy assignment operator that assigns the value from another Variant.
    * `operator=(Variant&& other) noexcept`: Move assignment operator that moves the value from another Variant.

6. Index and Type Check Functions:

    * `index() const`: Returns the index of the currently stored type.
    * `holds_alternative<T>() const`: Checks if the `Variant` holds a value of type `T`.

7. Get Functions:

    * `get<T>()`: Returns a reference to the stored value if it is of type `T`, otherwise throws an exception.
    * `get<T>() const`: Const version of `get<T>()`.

8. Visitor Functions:

    * `visit<Visitor>(Visitor&& visitor)`: Applies a visitor to the stored value.
    * `visit<Visitor>(Visitor&& visitor) const`: Const version of `visit<Visitor>(Visitor&& visitor)`.

### Detailed Breakdown of Important Parts

#### IndexOf Meta-function

This meta-function helps to find the index of a type in a variadic template list. For example, in `Variant<int, double, std::string>`, `IndexOf<double, int, double, std::string>::value` will be `1`.

``` c++
template <typename T, typename First, typename... Rest>
struct IndexOf<T, First, Rest...> {
    static constexpr size_t value = std::is_same<T, First>::value ? 0 : 1 + IndexOf<T, Rest...>::value;
};

template <typename T>
struct IndexOf<T> {
    static constexpr size_t value = static_cast<size_t>(-1);
};
```

#### TypeAt Meta-function

This meta-function retrieves the type at a specific index in a variadic template list. For example, `TypeAt<1, int, double, std::string>::type` will be `double`.

``` c++
template <size_t Index, typename First, typename... Rest>
struct TypeAt<Index, First, Rest...> {
    using type = typename TypeAt<Index - 1, Rest...>::type;
};

template <typename First, typename... Rest>
struct TypeAt<0, First, Rest...> {
    using type = First;
};
```

#### Variant Class Template

The `Variant` class uses a union-like storage (`std::aligned_union_t`) to store one of the possible types. It manages the construction, destruction, and visitation of the stored value.

``` c++
template <typename... Types>
class Variant {
    static_assert(sizeof...(Types) > 0, "Variant must have at least one type");

    using Storage = std::aligned_union_t<0, Types...>;
    Storage storage;
    size_t type_index;
    ...
}
```

#### Visiting Functionality

The `visit_impl` function template applies a visitor to the stored value, providing a type-safe way to operate on the value without knowing its type at compile time.

``` c++
template <typename Visitor, typename Variant, typename... Types, size_t... Is>
decltype(auto) visit_impl(Visitor&& visitor, Variant& var, std::index_sequence<Is...>) {
    using ReturnType = decltype(visitor(std::declval<typename TypeAt<0, Types...>::type&>()));
    using Functor = ReturnType(*)(Visitor&&, Variant&);

    Functor funcs[] = {
        [](Visitor&& vis, Variant& v) -> ReturnType {
            return std::forward<Visitor>(vis)(v.template get<typename TypeAt<Is, Types...>::type>());
        }...
    };

    return funcs[var.index()](std::forward<Visitor>(visitor), var);
}
```

#### Variant Member Functions

The member functions handle construction, destruction, and access to the stored value. They ensure that only the correct type operations are performed, and throw exceptions when incorrect types are accessed.

```c++
template <typename... Types>
template <typename T>
void Variant<Types...>::construct(const T& value) {
    new (&storage) T(value);
}

template <typename... Types>
template <typename T>
void Variant<Types...>::construct(T&& value) {
    new (&storage) T(std::move(value));
}

template <typename... Types>
template <typename T>
T& Variant<Types...>::get() {
    if (!holds_alternative<T>()) {
        throw std::bad_variant_access();
    }
    return *reinterpret_cast<T*>(&storage);
}

template <typename... Types>
template <typename Visitor>
decltype(auto) Variant<Types...>::visit(Visitor&& visitor) {
    return detail::visit_impl<Visitor, Variant<Types...>, Types...>(
        std::forward<Visitor>(visitor), *this, std::index_sequence_for<Types...>{});
}
```

### Conclusion

This custom `Variant` class provides a flexible and type-safe way to store and operate on one of several types. It includes type indexing, construction, destruction, and visitation mechanisms. The design ensures type safety, proper resource management, and a clean interface for users. This approach avoids dependencies on third-party libraries like Boost and can be used in environments whereEquals C++11 is the target, and `Variant` is unavailable.


## Performance

At the begining of writing this project I felt that it was well structured. However with time and re reading my code I did some research on how to improve it's performance.

1. I was making __unnecesary copies__: In my `JSONObject` and `JSONArray` classes, when passing elements to the `put` or `add` functions, I was making __unnecessary copies__ of the objects.
cpp

```c++
void JSONObject::put(const std::string& key, const std::string& value) {
    key_value[key] = value;  // Instead of copying, move it
}

void JSONArray::add(const std::string &element) {
    elements.push_back(element);  // Instead of copying, move it
}
```

Instead, you can use move semantics to avoid these copies. For example:

```c++
void JSONObject::put(const std::string& key, const std::string& value) {
    key_value[key] = std::move(value);
}

void JSONArray::add(const std::string &element) {
    elements.push_back(std::move(element));
}
```

## Memory management

Since the `JSONObject` and `JSONArray` classes don't directly manage any dynamic resources like pointers, there's no need to explicitly deallocate any resources in their destructors. The default destructors provided by the compiler will handle the cleanup of member variables automatically when an object of these classes goes out of scope. So there's no need to have default destructor on each class. However, I ended up debating myself if the use of pointers as containers for the JSON object/array (`Variant<int, bool, double, std::string, JSONObject, JSONArray>>* key_value;` `std::vector<Variant<int, bool, double, std::string, JSONObject, JSONArray>> * elements;`) will it be beneficial? What I found is that:

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

Using pointers for class members can be beneficial in certain scenarios whereEquals dynamic memory management or deferred initialization is required. However, it also introduces complexity and requires careful management to avoid memory-related issues.

#### Leason LEARNED

Always consider the specific requirements and trade-offs of your application before deciding whether to use pointers.

## Is simple always best?

In the context of my project, using references for class members (`key_value`, `elements`) can be a bit different.

### For JSONObject Class

* *__key_value__*: This member represents the key-value pairs in my JSON object. Since the size of the map may vary, and dynamic memory management might not be necessary, using a reference (`std::unordered_map<std::string, Variant<int, bool, double, std::string, JSONObject, JSONArray>>&`) would be a suitable choice. References provide simplicity and safety and avoid unnecessary overhead associated with pointers.

### For JSONArray Class:

* *__elements__: This member stores the elements of your JSON array. Similar to key_value, using a reference (`std::vector<Variant<int, bool, double, std::string, JSONObject, JSONArray>>&`) would be appropriate if the size of the array is known at construction and dynamic memory management isn't needed.

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
   * It's beneficial to use smart pointers (`std::shared_ptr`) for managing objects like `JSONObject` and `JSONArray` within the container types (`std::unordered_map` and `std::vector`).
   * Smart pointers offer automatic memory management with shared ownership semantics, ensuring that objects are properly deallocated when they are no longer needed.
   * This approach simplifies memory management for users and reduces the risk of memory leaks or premature deallocation.

2. *__For Container References__*:
   * Using references (`&`) for containers like `std::unordered_map` and `std::vector` can provide efficient access to container elements without introducing the complexity of manual memory management.
   * References allow users to work directly with the containers without worrying about memory allocation or deallocation.

3. *__For Individual Elements__*:
   * For individual elements stored within the containers (e.g., values in `Variant` or elements in `std::vector`), using values (not pointers or references) is generally the simplest and most efficient approach.
   * Values provide straightforward access to data without the overhead of indirection.

### Here's a summary of my choicess

* Use smart pointers (`std::shared_ptr`) for managing `JSONObject` and `JSONArray` instances within container types (`std::unordered_map` and `std::vector`).
* Use references (`&`) for accessing container objects (`std::unordered_map` and `std::vector`) to avoid unnecessary copying and ensure efficient access.
* Use values for individual elements stored within the containers for simplicity and efficiency.

By following these choices, I believe I can create a JSON library that strikes a balance between ease of use, performance, and memory management, providing users with a convenient and efficient way to work with JSON data.

## Learning about multthreading the modern way

Using `std::async` and `std::future` might help expedite parsing time if I am have multiple JSON strings to parse concurrently. However, it's essential to use them judiciously, considering the overhead they introduce and whether the parsing workload can benefit from parallelism.

```c++
// Helper function for asynchronous parsing
template<typename Function, typename... Args>
static auto asyncParse(Function&& f, Args&&... args) {
    return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(args)...);
}
```

The helper function `asyncParse` can be used to complement the asynchronous parsing by utilizing it to simplify the process of creating asynchronous tasks.

* The `asyncParse` helper function access tallable `f` and its arguments `args` and forwards them to `std::async` to create asynchronous tasks.

```c++
// Helper function for asynchronous parsing
template<typename Function, typename... Args>
static auto asyncParse(Function&& f, Args&&... args) {
    return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(args)...);
}
```

* `asyncParseArray` and `asyncParseObject` now utilize `asyncParse` to create asynchronous tasks to parse arrays and objects.

```c++
static std::future<JSONArray> asyncParseArray(std::string_view jsonView) {
    return async_parse(parseArray, jsonView); // Utilize the asyncParse helper function
}

static std::future<JSONObject> asyncParseObject(std::string_view jsonView) {
    return async_parse(parseObject, jsonView); // Utilize the asyncParse helper function
}
```

* By using `asyncParse` the creation of asynchronous parsing functions is simplyfied, making the code look cleaner and readable. (I think that's the case for now)

This integration ensures that the asynchronous parsing functions remain concistent and maintainable while leveraging the helper function for asynchronous task creation.
