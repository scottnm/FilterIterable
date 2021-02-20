#ifndef __FILTER_ITERABLE_H__
#define __FILTER_ITERABLE_H__

#include <iterator>
#include <type_traits>

//
// An iterable over all elements of the source iterable which match the predicate
//
template <typename TIterable, typename TPredicate>
class FilteredIterable
{
public:
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

    typedef typename IteratorType<TIterable>::type TIterator;
    typedef typename std::iterator_traits<TIterator>::value_type TItem;

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
            AdvanceUntilValid();
        }

        FilterIterator&
        operator++()
        {
            ++current;
            AdvanceUntilValid();
            return *this;
        }

        bool operator==(const FilterIterator& other) { return current == other.current; }
        bool operator!=(const FilterIterator& other) { return !(*this == other); }
        TItem* operator->() { return current; }
        TItem& operator *() { return *current; }
        const TItem* operator->() const { return current; }
        const TItem& operator *() const { return *current; }

    private:
        void
        AdvanceUntilValid()
        {
            while (current != end && !predicate(*current))
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

//
// Template parameter inference apparently only works for functions and not classes.
// Define this filter function to construct the FilteredIterable for us with inferred template args.
//
template <typename TIterable, typename TPredicate>
FilteredIterable<TIterable, TPredicate>
Filter(
    TIterable& iterable,
    TPredicate predicate
    )
{
    return FilteredIterable<TIterable, TPredicate>{iterable, predicate};
}

#endif // __FILTER_ITERABLE_H__
