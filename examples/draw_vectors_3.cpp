/**
 * @file draw_vectors_3.cpp
 * @ingroup cglexamples
 * @brief Example of drawing 3D vectors.
 *
 * This example creates and draws a set of random 3D vectors.
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <DrawVectors3.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>


/**
 * Code example.
 * @return Success.
 */
int main()
{

    /* Vector to store vectors. */
    std::vector<CGL::Vector3> segs;

    /* Create vectors with random endpoints (origin is (0,0,0)). */
    int ns = 20; /* Number of vectors. */
    CGL::Random rng(-10,10); /* "bounding box" for vectors. "*/
    for (int i = 1; i <= ns; i++)
    {
        /* Vector. */
        CGL::Vector3 v(rng.get_int(), rng.get_int(), rng.get_int());
        std::cout << "Vector " << i << " (" << v << ")" << std::endl;

        /* Insert to list. */
        segs.push_back(v);
    }

    /* Draw vectors. */
    CGL::draw(segs);

    return EXIT_SUCCESS;
}
