#include <cstdio>
#include <vector>
#include <list>
#include <functional>

#include "FilterIterable.h"

bool
IsEven(
    int v
    )
{
    return (v & 1) == 0;
}

void
TestIterableConstruction()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto iter = FilteredIterable<int[11], bool(*)(int)>(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
}

void
TestFilterStackArrays()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const int& i : Filter(values, IsEven))
    {
        printf("%i ", i);
    }
}

void
TestFilterContainerTypes()
{
    std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const int& i : Filter(values, IsEven))
    {
        printf("%i ", i);
    }
}

void
TestFilterNonRandomAccessIterable()
{
    std::list<int> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.emplace_back(i);
    }
    auto iter = Filter(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
}

struct SomeObject
{
    uint32_t i;
};

void
TestFilterByObjectProperty()
{
    std::list<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.push_back({i});
    }
    auto iter = Filter(values, [](const SomeObject& so) { return !IsEven(so.i); });
    for (const SomeObject& so : iter)
    {
        printf("so(%i) ", so.i);
    }
}

void
TestFilterByUniqueObject()
{
    std::vector<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.push_back({i});
    }
    const SomeObject& so3 = values[3];
    auto iter = Filter(values, [&so3](const SomeObject& so) { return &so == &so3; });
    for (const SomeObject& so : iter)
    {
        printf("so(%i) ", so.i);
    }
}

template<typename T>
std::function<bool (const T&)>
Excluder(
    const T& elementToExclude
    )
{
    return [&elementToExclude](const T& t) { return &t == &elementToExclude; };
}

void
TestFilterWithHigherOrderFunctions()
{
    std::vector<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.push_back({i});
    }
    for (const SomeObject& so : Filter(values, Excluder(values[4])))
    {
        printf("so(%i) ", so.i);
    }
}

template<typename T, size_t N>
constexpr
size_t
CountOf(
    T(&)[N]
    )
{
    return N;
}

int
main()
{
    typedef void(*testfunc)(void);
    testfunc tests[] =
    {
        // 0) 1 3 5 7 9
        TestIterableConstruction,

        // 1) 1 3 5 7 9
        TestFilterStackArrays,

        // 2) 1 3 5 7 9
        TestFilterContainerTypes,

        // 3) 1 3 5 7 9
        TestFilterNonRandomAccessIterable,

        // 4) so(0) so(2) so(4) so(6) so(8) so(10)
        TestFilterByObjectProperty,

        // 5) so(0) so(1) so(2) so(4) so(5) so(6) so(7) so(8) so(9) so(10)
        TestFilterByUniqueObject,

        // 6) so(0) so(1) so(2) so(3) so(5) so(6) so(7) so(8) so(9) so(10)
        TestFilterWithHigherOrderFunctions
    };

    for (size_t i = 0; i < CountOf(tests); ++i)
    {
        printf("%llu) ", i);
        tests[i]();
        printf("\n\n");
    }
}
