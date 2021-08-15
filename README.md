# Nass Group
A tuple like class with fast compile times
- [Nass Group](#nass-group)
  - [Compile time benchmarks](#compile-time-benchmarks)
    - [Tuple times (GCC)](#tuple-times-gcc)
    - [Tuple times (GCC) with taotuple](#tuple-times-gcc-with-taotuple)
    - [Tuple times (MSVC)](#tuple-times-msvc)
    - [Group times (GCC)](#group-times-gcc)
    - [Group times (MSVC)](#group-times-msvc)
  - [Runtime benchmark](#runtime-benchmark)
    - [GCC](#gcc)
    - [MSVC](#msvc)
    - [MSVC with /DNASSGROUP_MSVC_GET=0](#msvc-with-dnassgroup_msvc_get0)
  - [Usage](#usage)
    - [Construction](#construction)
      - [make_group](#make_group)
      - [make_value_group](#make_value_group)
      - [make_forward_group](#make_forward_group)
      - [make_group_of](#make_group_of)
    - [Creating copies](#creating-copies)
    - [Getter](#getter)
    - [Continuation](#continuation)
  - [How its works](#how-its-works)
    - [A group of references](#a-group-of-references)
    - [Getting the values](#getting-the-values)
      - [Deafult get](#deafult-get)
      - [MSVC get](#msvc-get)
  - [Why should you not is this repo in your code](#why-should-you-not-is-this-repo-in-your-code)
## Compile time benchmarks
See ./compile_benchmarking for source
The following tests are for group/tuple of 440 ints
### Tuple times (GCC)
|Operation|Time|
|---------|----|
|Construction|0m39.497s|
|Construction and get| 1m14.209s|
### Tuple times (GCC) with taotuple
|Operation|Time|
|---------|----|
|Construction|0m1.463s|
|Construction and get| 0m3.868s|
### Tuple times (MSVC)
|Operation|Time|
|---------|----|
|Construction|14.5341529s|
|Construction and get| 16.2622472s|
### Group times (GCC)
|Operation|Time|
|---------|----|
|Construction|0m0.301s|
|Construction and get| 0m18.507s|
### Group times (MSVC)
|Operation|Time|
|---------|----|
|Construction|0.6200778s|
|Construction and get| 22.4537721s|
|Construction and get with /DNASSGROUP_MSVC_GET=0| 7.3228709s|
## Runtime benchmark
See ./benchmarking for source
### GCC
|Benchmark|Time|CPU|Iterations|
|---------|----|---|----------|
BM_make_group|                 35 ns|         35 ns|   20363636|
BM_make_value_group|           34 ns|         35 ns|   20363636|
BM_make_forward_group|        167 ns|        165 ns|    4072727|
BM_make_group_from|            20 ns|         19 ns|   34461538|
BM_make_tuple|                 44 ns|         44 ns|   16000000|
BM_make_value_tuple|           44 ns|         43 ns|   16000000|
BM_make_forward_tuple|        107 ns|        105 ns|    6400000|
MB_group_get|                  71 ns|         70 ns|    8960000|
MB_tuple_get|                  71 ns|         70 ns|    8960000|
### MSVC
|Benchmark|Time|CPU|Iterations|
|---------|----|---|----------|
BM_make_group|                555 ns|          544 ns|      1120000|
BM_make_value_group|          520 ns|          516 ns|      1120000|
BM_make_forward_group|        510 ns|          502 ns|      1120000|
BM_make_group_from|           433 ns|          430 ns|      1600000|
BM_make_tuple|              15995 ns|        16044 ns|        44800|
BM_make_value_tuple|        16350 ns|        16113 ns|        40727|
BM_make_forward_tuple|      16758 ns|        16497 ns|        40727|
MB_group_get|                74.7 ns|         75.0 ns|      8960000|
MB_tuple_get|                74.9 ns|         75.0 ns|      8960000|
### MSVC with /DNASSGROUP_MSVC_GET=0
|Benchmark|Time|CPU|Iterations|
|---------|----|---|----------|
BM_make_group|                562 ns|          558 ns|      1120000|
BM_make_value_group|          572 ns|          578 ns|      1000000|
BM_make_forward_group|        549 ns|          544 ns|      1120000|
BM_make_group_from|           456 ns|          460 ns|      1493333|
BM_make_tuple|              17569 ns|        17648 ns|        40727|
BM_make_value_tuple|        17652 ns|        17578 ns|        37333|
BM_make_forward_tuple|      16455 ns|        16497 ns|        40727|
MB_group_get|               78831 ns|        77424 ns|         7467|
MB_tuple_get|                77.1 ns|         76.7 ns|      8960000|
## Usage
### Construction
Unlike tuple, groups should not be created from a constructor, but rether be created with the folowing helper functions:
#### make_group
Creates a group that stores the values defined by the function's template parameters
```
nass::make_group<int, int&, int&&>(some_int, some_int, std::move(some_int)); // Creates a group of int, int& and int&&

// Can replace
std::tuple<int, int&, int&&>{ some_int, some_int, std::move(some_int) };
```
#### make_value_group
Creates a group that stores lvalues duplicates of the function's paramters
```
struct A { ... } some_object;
nass::make_value_group(5, some_int, std::move(some_object)); // Creates a group of int, int and A where some_object is moved

// Can replace
std::make_tuple(5, some_int, std::move(some_object));
```
#### make_forward_group
Creates a group of references to the function's parameter
```
struct A { ... } some_object;
nass::make_forward_group(some_int, std::move(some_object)); // Creates a group of int& and A&&

// Can replace
std::forward_as_tuple(some_int, std::move(some_object));
```
#### make_group_of
Accepts functors and holds their results in a group (helpfull for types that cant be copied or moved)
```
struct non_movable
{
    non_movable() = default;
    non_movable(const non_movable&) = delete;
    non_movable(non_movable&&) = default;
};
nass::make_group_of([] { return 5; }, [] { return non_movable{}; }); // Creates a group of int and non_movable
```
### Creating copies
To copy a group by passing a const ref of each value in the group to a new group that holds the same exect types you can use the member function `copy`; To do the same thing but with r references you can use `steal`; And finally to pass the values "as is" you can use `duplicate`
### Getter
To get a member of the tuple you can use the `get` method int the following way
```
auto g = nass::make_value_group(int(0), short(1), char(2));
assert(g.get<0> == 0);
static_assert(std::is_same_v<std::remove_cvref_t<decltype(g.get<0>)>, int>);
assert(g.get<1> == 1);
static_assert(std::is_same_v<std::remove_cvref_t<decltype(g.get<1>)>, short>);
assert(g.get<2> == 2);
static_assert(std::is_same_v<std::remove_cvref_t<decltype(g.get<2>)>, char>);

g.get<0> = 5;
assert(g.get<0> == 5);
```

*** NOTICE ***

Unlike tuple, groups getter keep the l-r value of the type, so with
```
auto g = nass:group<A&&>(std::move(some_object));
```
`g.get<0>` will return `A&&` and not `A&`
### Continuation
Just a cool utility to spread the stored values over a functor, for example:
```
void print_coordinates(int x, int y)
{
    std::cout 
        << "Longitude: << x
        << '\n'
        << "Latitude: << y
        << std::endl;
}

int main()
{
    auto g = nass::make_value_group(5, 8);
    g >> print_coordinates; 
    // prints:
    //  Longtitude: 5
    //  Latitude: 8
}
```
## How its works
In perfect world tuple will be implemented kinda like so:
```
template <typename... Ts>
struct tuple
{
    Ts... values;
    ... // methods impl
};
```
but sadly this syntax is not valid, but C++ has a trick up its sleeve:
```
template <typename... Ts>
auto func(Ts... vals)
{
    return [...vals]{};
}
``` 
Does work! So now we just have to figure out what can we do to implement the abilites that we can expect from a tuple
### A group of references
The first problem with we encounter with the lambda syntax is that we can only hold either values or references exclusivly, so to create a group that values and references at the same time we are using a halper type:
```
template <typename T>
struct holder_value
{
    using item_t = T;
    item_t item;
    constexpr auto&& inner_ref()
    {
        if constexpr (std::is_same_v<std::remove_reference_t<item_t>, item_t>)
        {
            return item;
        }
        else
        {
            return std::forward<item_t>(item);
        }
    }
};
```
With this type we can simply store value of `holder_value`, that hold either a value or a reference
### Getting the values
So now that we are holding those values/references, we need to figure out a way to get them, for that we can make the lambda to accept a functor, and from there we have the ability to filter/forward those value
#### Deafult get
The default get logic uses the same method that tuple uses to get the N'th value; by using the types
```
template <typename T, size_t Idx>
struct retriever
{
    T& holder;
    constexpr retriever operator*(std::integral_constant<size_t, Idx>)
    {
        return *this;
    }
};

template <typename... RetrieversTs>
struct retriever_group: RetrieversTs...
{
    using RetrieversTs::operator*...;
};
```
we can store the holders in `retriever`s inside a retriever_group, and let the compiler pick the right overload when using `std::integral_constant<N>`
#### MSVC get
Oh boy thats a nice one, we can define a type 
```
template <typename T, bool SelectedV>
struct item_retriever;

template <typename T>
struct item_retriever<T, false>
{
    T& item;

    constexpr operator bool() const
    {
        return true;
    }

    template <typename ThatT>
    constexpr ThatT operator|| (ThatT that)
    {
        return that;
    }
};

template <typename T>
struct item_retriever<T, true>
{
    T& holder;

    constexpr operator bool() const
    {
        return false;
    }

    template <typename ThatT>
    constexpr item_retriever<T, true> operator|| (ThatT)
    {
        return *this;
    }
};
```
And use this types where T is the holder and Selected is the index of holder - N == 0, that way we have many types that can be 'or'ed with each other and return the correct `item_retriever` so basically we can use fold expressions:
```
(item_retriever<ArgsTs, (IdxsVs - IdxV == 0)>{args} || ...)
```
## Why should you not is this repo in your code
* I'm a random dude on the internet that uploaded some code to github
* I'm not sure that the `const_cast`s that i use are entirely safe 
* Right now MSVC is using a different method for the `get` method because the original generates slow code, and the alternative method has long compile time, There's probably a way to modify the original code to run faster (althogh MSVC's code is pretty cool and it will be a shame to erase it)
* Types that are not copyable and not movable are not working with MSVC because the compiler's copy elision doesn't work with lambdas **Won't Fix**
* MSVC is having a hard time with the constexpr tests, probably because of the same reason of the last point
