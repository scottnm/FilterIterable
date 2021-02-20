#include <windows.h>
#include <stdint.h>
#include "FilterIterable.h"

constexpr
uint64_t
ms_to_ns(
    uint64_t ms
    )
{
    return ms * 1'000'000;
}

void
profile(
    uint64_t(*f)(),
    const char* name
    )
{
    uint64_t start = GetTickCount64();
    uint64_t trial_count = f();
    uint64_t end = GetTickCount64();

    uint64_t duration_ms = end - start;
    uint64_t avg_duration_ns = ms_to_ns(duration_ms) / trial_count;

    printf("%s: %llu trials in %llu ms. Avg = %llu ns.\n", name, trial_count, duration_ms, avg_duration_ns);
}

bool is_even(int i)
{
    return (i & 1) == 0;
}

constexpr uint64_t TRIALS = 10'000'000;
uint64_t using_filter()
{
    uint64_t sum = 0;
    for (uint64_t trial = 0; trial < TRIALS; ++trial)
    {
        int ints[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        for (const int& i : Filter(ints, is_even))
        {
            sum += i;
        }
    }
    printf("    sum %llu\n", sum);
    return TRIALS;
}

uint64_t not_using_filter()
{
    uint64_t sum = 0;
    for (uint64_t trial = 0; trial < TRIALS; ++trial)
    {
        int ints[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        for (const int& i : ints)
        {
            if (is_even(i))
            {
                sum += i;
            }
        }
    }

    printf("    sum %llu\n", sum);
    return TRIALS;
}

int main()
{
    profile(&using_filter, "using_filter");
    profile(&not_using_filter, "not_using_filter");
}
