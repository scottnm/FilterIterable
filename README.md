# FilterIterable

A tiny, header-only c++ helper to enable iterating over only a filtered subset of a source iterable.

```cpp
std::string someString = "Catsup OOgLer";
auto isUpperCase = [](char c) { return c >= 'A' && c <= 'Z'; };
for (char c : Filter(someString, isUpperCase))
{
    printf("%c", c); // prints "COOL"
}
```

Check out the `Samples.cpp` for more examples of other ways this can be used.

## Features

* supports filtering by function pointers and stateless lambdas as well as stateful closures 
* Only iterates. No extra allocations or temporary copies.
