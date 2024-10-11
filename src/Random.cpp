/**
 * @file Random.h  
 * @brief Random numbers.
 *
 * Implements random number generator.
 *
 * \author Ricardo Dutra da Silva
 */


#include <Random.h>


CGL::Random::Random(int lower, int higher) : rng(dev()), dist(lower,higher)
{
}


int CGL::Random::get_int()
{
    return dist(rng);
}
