#include <cstdio>
#include <windows.h>
#include <vector>
#include <iterator>
#include <type_traits>
#include <iostream>
#include <list>
#include <functional>

template<typename T>
struct iterator_for
{
    typedef typename T::iterator  type;
};

template<class T>
struct iterator_for<T*>
{
    typedef T*  type;
};

template <typename T, size_t N>
struct iterator_for<T[N]>
{
    typedef T*  type;
};

template <typename TIterable, typename TPredicate>
class filter_iter
{
public:
    typename typedef iterator_for<TIterable>::type TIterator;
    typename typedef std::iterator_traits<TIterator>::value_type value_type;

    filter_iter(TIterable& iter, const TPredicate& pred) :
        iter(iter),
        pred(pred)
    {
        std::cout << typeid(TIterable).name() << std::endl;
        std::cout << typeid(TIterator).name() << std::endl;
    }

    class filter_iter_pos
    {
    public:
        filter_iter_pos(const TIterator& c, const TIterator& e, const TPredicate& pred) : c(c), e(e), pred(pred)
        {
            advanceUntilValid();
        }

        bool operator ==(const filter_iter_pos& pos) {
            return pos.c == c;
        }
        bool operator !=(const filter_iter_pos& pos) {
            return !(*this == pos);
        }

        filter_iter_pos&
        operator ++()
        {
            ++c;
            advanceUntilValid();
            return *this;
        }

              value_type* operator->()       { return c; }
        const value_type* operator->() const { return c; }
              value_type& operator *()       { return *c; }
        const value_type& operator *() const { return *c; }

    private:
        void
        advanceUntilValid()
        {
            while (c != e && pred(*c))
            {
                ++c;
            }
        }

        TIterator c;
        TIterator e;
        const TPredicate pred;
    };

    filter_iter_pos
    begin()
    {
        return filter_iter_pos(std::begin(iter), std::end(iter), pred);
    }

    filter_iter_pos
    end()
    {
        return filter_iter_pos(std::end(iter), std::end(iter), pred);
    }

    typedef filter_iter_pos iterator;

private:
    TIterable& iter;
    const TPredicate pred;
};

template <typename TIterable, typename TPredicate>
filter_iter<TIterable, TPredicate>
f_filter_iter(TIterable& iter, TPredicate pred)
{
    return filter_iter<TIterable, TPredicate>{iter, pred};
}

bool IsEven(int v) { return (v & 1) == 0; }

void test1()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto iter = filter_iter<int[11], bool(*)(int)>(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
}

void test2()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const int& i : f_filter_iter(values, IsEven))
    {
        printf("%i ", i);
    }
}

void test3()
{
    std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (const int& i : f_filter_iter(values, IsEven))
    {
        printf("%i ", i);
    }
}

void test4()
{
    std::list<int> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.emplace_back(i);
    }
    auto iter = f_filter_iter(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
}

struct SomeObject
{
    uint32_t i;
};

void test5()
{
    std::list<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.push_back({i});
    }
    auto iter = f_filter_iter(values, [](const SomeObject& so) { return so.i & 1 == 1; });
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


void test6()
{
    std::vector<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.push_back({i});
    }
    const SomeObject& so3 = values[3];
    auto iter = f_filter_iter(values, [&so3](const SomeObject& so) { return &so == &so3; });
    for (const SomeObject& so : iter)
    {
        printf("so(%i) ", so.i);
    }
}

void test7()
{
    std::vector<SomeObject> values;
    for (uint32_t i = 0; i <= 10; ++i)
    {
        values.push_back({i});
    }
    const SomeObject& so3 = values[3];
    for (const SomeObject& so : f_filter_iter(values, Excluder(so3)))
    {
        printf("so(%i) ", so.i);
    }
}

int main()
{
    typedef void(*testfunc)(void);

    testfunc tests[] = {test1, test2, test3, test4, test5, test6, test7};
    int i = 1;
    for (auto test : tests)
    {
        test();
        printf("\nDone: %i\n\n", i++);
    }
}
