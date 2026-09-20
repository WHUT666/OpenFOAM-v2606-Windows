/*
 * POSIX drand48/srand48 for MSVC (UCRT does not provide them).
 *
 * Same linear congruential generator as POSIX:
 *   X(n+1) = (a*X(n) + c) mod 2^48, a = 0x5DEECE66D, c = 0xB
 * drand48 returns X(n+1)/2^48 in [0,1).
 * srand48 seeds with (seed << 16) | 0x330E like the POSIX default.
 */

static unsigned long long _drand48_state =
    (0x1234ABCDULL << 16) | 0x330EULL;  /* default seed 0x1234ABCD */

void srand48(long seed)
{
    _drand48_state = (((unsigned long long)(unsigned long)seed) << 16)
                     | 0x330EULL;
}

double drand48(void)
{
    _drand48_state = (0x5DEECE66DULL * _drand48_state + 0xBULL)
                     & 0xFFFFFFFFFFFFULL;
    return (double)_drand48_state / 281474976710656.0; /* 2^48 */
}
