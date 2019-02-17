#include <cstdio>
#include <vector>

template <typename TIterable, typename TIterator, typename TDerefed, typename TPredicate>
class filter_iter
{
public:
    filter_iter(const TIterable& iter, const TPredicate& pred) :
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

              TDerefed* operator->()       { return i; }
        const TDerefed* operator->() const { return i; }
              TDerefed& operator *()       { return *i; }
        const TDerefed& operator *() const { return *i; }

    private:
        TIterator i;
        const TPredicate pred;
    };

    filter_iter_pos
    begin()
    {
        return filter_iter_pos(std::begin(iter), pred);
    }

    filter_iter_pos
    end()
    {
        return filter_iter_pos(std::end(iter), pred);
    }

private:
    const TIterable& iter;
    const TPredicate pred;
};

/*
template <typename TIterable, typename TIterator, typename TDerefed, typename TPredicate>
filter_iter<TIterable, TIterator, TDerefed, TPredicate>
f_filter_iter(const TIterable& iter, TPredicate& pred)
{
    return filter_iter<TIterable, TIterator, TDerefed, TPredicate>{iter, pos};
}
*/

bool IsEven(int v) { return (v & 1) == 0; }

void test1()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto iter = filter_iter<int(&)[11], int*, int, bool(*)(int)>(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
    printf("\nDone\n");
}

/*
void test2()
{
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto iter = f_filter_iter(values, IsEven);
    for (const int& i : iter)
    {
        printf("%i ", i);
    }
    printf("\nDone\n");
}
*/

int main()
{
    test1();
}
