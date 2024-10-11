/**
 * @file Random.h  
 * @brief Random numbers.
 *
 * Defines random number generator.
 *
 * \author Ricardo Dutra da Silva
 */


#ifndef CGL_RANDOM_H
#define CGL_RANDOM_H


#include <random>


namespace CGL /* START NAMESPACE */
{

/**
 * @brief Random.
 *
 * Random number generator. 
 */ 
class Random
{
    private:
    std::random_device dev;
    typedef std::mt19937 Rng;
    Rng rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;

    public:
    /** 
     * The constructor. 
     * @param lower Lowe limit. 
     * @param higher Higher limit.
     */
    Random(int lower, int higher);

    /** 
     * Return a random int number. 
     * @return Integer random number.
     */
    int get_int();
};

} /* END NAMESPACE. */

#endif /* CGL_RANDOM_H */
