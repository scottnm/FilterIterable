#include <cstdio>
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
    }

    class filter_iter_pos
    {
    public:
        filter_iter_pos(const TIterator& i, const TPredicate& pred) : i(i), pred(pred) {}

        bool operator ==(const filter_iter_pos& pos) { return pos.i == i; }
        bool operator !=(const filter_iter_pos& pos) { return !(*this == pos); }

        filter_iter_pos&
        operator ++()
        {
            do
            {
                ++i;
            } while (!pred(*i));
            return *this;
        }

              value_type* operator->()       { return i; }
        const value_type* operator->() const { return i; }
              value_type& operator *()       { return *i; }
        const value_type& operator *() const { return *i; }

    private:
        TIterator i;
        const TPredicate pred;
    };

    filter_iter_pos
    begin()
    {
        filter_iter_pos x(std::begin(iter), pred);
        return x;
    }

    filter_iter_pos
    end()
    {
        filter_iter_pos y(std::end(iter), pred);
        return y;
    }

private:
    TIterable& iter;
    const TPredicate pred;
};

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

int main()
{
    test1();
}

/*
template<typename T>
void
PrintTypes(const T& v)
{
    typename typedef iterator_for<T>::type iterator_type;
    std::cout <<  "TYPES" << std::endl;
    (void)v;
    std::cout << typeid(iterator_type).name() << std::endl;
    std::cout << std::endl;
}

int main()
{
    int a[5];
    PrintTypes(a);

    typedef iterator_for<int[5]>::type test_type;
    test_type i = nullptr;

    std::vector<int> b;
    PrintTypes(b);
}
*/
