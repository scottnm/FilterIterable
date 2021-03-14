#include <cstdio>
#include <functional>
#include <list>
#include <string>
#include <vector>

#include "FilterIterable.h"

// N.B. sprintf_s doesn't exist outside of windows
#if !defined(_WIN32) && !defined(WIN32)
#define sprintf_s sprintf
#endif

// Define a helper macro to make appending formatted text to a std::string easy.
// I just don't want to bother with iostream shenanigans. Hurts my eyes.
#define APPEND_STR(acc, format, ...)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        char appendStrTmpBuffer[1024];                                                                                 \
        (void)sprintf_s(appendStrTmpBuffer, format, __VA_ARGS__);                                                      \
        acc += appendStrTmpBuffer;                                                                                     \
    } while (0)

bool
IsOdd(int v)
{
    return (v & 1) == 1;
}

std::string
TestIterableConstruction()
{
    std::string result;

    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto iter = FilteredIterable<int[11], bool (*)(int)>(values, IsOdd);
    for (const int& i : iter)
    {
        APPEND_STR(result, "%i ", i);
    }

    result.pop_back();
    return result;
}

std::string
TestFilterStackArrays()
{
    std::string result;

    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const int& i : Filter(values, IsOdd))
    {
        APPEND_STR(result, "%i ", i);
    }

    result.pop_back();
    return result;
}

std::string
TestFilterContainerTypes()
{
    std::string result;

    std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const int& i : Filter(values, IsOdd))
    {
        APPEND_STR(result, "%i ", i);
    }

    result.pop_back();
    return result;
}

std::string
TestFilterNonRandomAccessIterable()
{
    std::string result;

    std::list<int> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.emplace_back(i);
    }

    auto iter = Filter(values, IsOdd);
    for (const int& i : iter)
    {
        APPEND_STR(result, "%i ", i);
    }

    result.pop_back();
    return result;
}

struct SomeObject
{
    uint32_t i;

    // Delete copy construction, copy assignment, and move assignment to verify
    // that no part of the filter performs copies. Leave the move constructor
    // alone since it's used by the array initializers
    SomeObject(int i) : i(i) {}
    SomeObject(SomeObject&&) = default;
    SomeObject(const SomeObject&) = delete;
    SomeObject&
    operator=(const SomeObject&) = delete;
    SomeObject&
    operator=(SomeObject&&) = delete;
};

std::string
TestFilterByObjectProperty()
{
    std::string result;

    std::list<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.emplace_back(i);
    }

    auto iter = Filter(values, [](const SomeObject& so) { return !IsOdd(so.i); });
    for (const SomeObject& so : iter)
    {
        APPEND_STR(result, "so(%i) ", so.i);
    }

    result.pop_back();
    return result;
}

std::string
TestFilterByUniqueObject()
{
    std::string result;

    SomeObject values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const SomeObject& so3 = values[3];
    auto iter = Filter(values, [&so3](const SomeObject& so) { return &so != &so3; });
    for (const SomeObject& so : iter)
    {
        APPEND_STR(result, "so(%i) ", so.i);
    }

    result.pop_back();
    return result;
}

template <typename T>
std::function<bool(const T&)>
Excluder(const T& elementToExclude)
{
    return [&elementToExclude](const T& t) { return &t != &elementToExclude; };
}

std::string
TestFilterWithHigherOrderFunctions()
{
    std::string result;

    SomeObject values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const SomeObject& so : Filter(values, Excluder(values[4])))
    {
        APPEND_STR(result, "so(%i) ", so.i);
    }

    result.pop_back();
    return result;
}

int
main()
{
    typedef std::string (*testfunc)(void);

    struct Test
    {
        const char* name;
        testfunc func;
        std::string expectedResult;
    };

    Test tests[] = {{"IterableConstruction", TestIterableConstruction, "1 3 5 7 9"},
                    {"FilterStackArrays", TestFilterStackArrays, "1 3 5 7 9"},
                    {"FilterContainerTypes", TestFilterContainerTypes, "1 3 5 7 9"},
                    {"FilterNonRandomAccessIterable", TestFilterNonRandomAccessIterable, "1 3 5 7 9"},
                    {"FilterByObjectProperty", TestFilterByObjectProperty, "so(0) so(2) so(4) so(6) so(8) so(10)"},
                    {"FilterByUniqueObject",
                     TestFilterByUniqueObject,
                     "so(0) so(1) so(2) so(4) so(5) so(6) so(7) so(8) so(9) so(10)"},
                    {"FilterWithHigherOrderFunctions",
                     TestFilterWithHigherOrderFunctions,
                     "so(0) so(1) so(2) so(3) so(5) so(6) so(7) so(8) so(9) so(10)"}};

#define ANSI_RED "\033[31m"
#define ANSI_RED_BOLD "\033[1;31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"

    bool passed = true;
    for (const auto& test : tests)
    {
        printf("Testing %s... ", test.name);
        std::string result = test.func();
        if (result == test.expectedResult)
        {
            printf(ANSI_GREEN "PASSED!\n" ANSI_RESET);
        }
        else
        {
            printf(ANSI_RED_BOLD "FAILED!\n" ANSI_RESET);
            printf(
                ANSI_RED "    expected \"%s\", found \"%s\"\n" ANSI_RESET, test.expectedResult.c_str(), result.c_str());
            passed = false;
        }
    }

    return passed ? 0 : 1;
}
