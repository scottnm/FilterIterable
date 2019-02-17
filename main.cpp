#include <cstdio>
#include <vector>
#include <iterator>
#include <type_traits>
#include <list>
#include <functional>

template<typename T>
struct IteratorType
{
    typedef typename T::iterator  type;
};

template<class T>
struct IteratorType<T*>
{
    typedef T*  type;
};

template <typename T, size_t N>
struct IteratorType<T[N]>
{
    typedef T*  type;
};

template <typename TIterable, typename TPredicate>
class FilteredIterable
{
public:
    typename typedef IteratorType<TIterable>::type TIterator;
    typename typedef std::iterator_traits<TIterator>::value_type TValue;

    FilteredIterable(
        TIterable& iterable,
        TPredicate predicate
        ) :
        iterable(iterable),
        predicate(predicate)
    {
#ifdef PRINT_ITER_TYPES
        printf("iterable: %s\niterator: %s\n", typeid(TIterable).name(), typeid(TIterator).name());
#endif // PRINT_ITER_TYPES
    }

    class FilterIterator
    {
    public:
        FilterIterator(
            const TIterator& current,
            const TIterator& end,
            TPredicate predicate
            ) :
            current(current),
            end(end),
            predicate(predicate)
        {
            advanceUntilValid();
        }

        FilterIterator&
        operator++()
        {
            ++current;
            advanceUntilValid();
            return *this;
        }

        bool operator==(const FilterIterator& other) { return current == other.current; }
        bool operator!=(const FilterIterator& other) { return !(*this == other); }
        TValue* operator->() { return current; }
        TValue& operator *() { return *current; }
        const TValue* operator->() const { return current; }
        const TValue& operator *() const { return *current; }

    private:
        void
        advanceUntilValid()
        {
            while (current != end && predicate(*current))
            {
                ++current;
            }
        }

        TIterator current;
        const TIterator end;
        TPredicate predicate;
    };

    FilterIterator
    begin()
    {
        return FilterIterator(std::begin(iterable), std::end(iterable), predicate);
    }

    FilterIterator
    end()
    {
        return FilterIterator(std::end(iterable), std::end(iterable), predicate);
    }

private:
    TIterable& iterable;
    TPredicate predicate;
};

template <typename TIterable, typename TPredicate>
FilteredIterable<TIterable, TPredicate>
Filter(
    TIterable& iterable,
    TPredicate predicate
    )
{
    return FilteredIterable<TIterable, TPredicate>{iterable, predicate};
}

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
    T(&arr)[N]
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
        TestIterableConstruction,
        TestFilterStackArrays,
        TestFilterContainerTypes,
        TestFilterNonRandomAccessIterable,
        TestFilterByObjectProperty,
        TestFilterByUniqueObject,
        TestFilterWithHigherOrderFunctions
    };

    for (size_t i = 0; i < CountOf(tests); ++i)
    {
        printf("%llu) ", i);
        tests[i]();
        printf("\n\n");
    }
}
