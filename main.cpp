#include <cstdio>
#include <windows.h>
#include <vector>
#include <iterator>
#include <type_traits>
#include <iostream>

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
    typename typedef std::iterator_traits<TIterator>::reference reference;
    typename typedef std::iterator_traits<TIterator>::pointer pointer;
    typename typedef std::iterator_traits<TIterator>::difference_type difference_type;
    typename typedef std::iterator_traits<TIterator>::iterator_category iterator_category;

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
        filter_iter_pos(const TIterator& c, const TIterator& e, const TPredicate& pred) : c(c), e(e), pred(pred) {}

        bool operator ==(const filter_iter_pos& pos) {
            return pos.c == c;
        }
        bool operator !=(const filter_iter_pos& pos) {
            return !(*this == pos);
        }

        filter_iter_pos&
        operator ++()
        {
            do
            {
                ++c;
            } while (c != e && !pred(*c));
            return *this;
        }

              value_type* operator->()       { return c; }
        const value_type* operator->() const { return c; }
              value_type& operator *()       { return *c; }
        const value_type& operator *() const { return *c; }

    private:
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
    printf("\nDone\n");
}

void test2()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto iter = f_filter_iter<int[11], bool(*)(int)>(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
    printf("\nDone\n");
}

int main()
{
    test1();
    test2();
}
