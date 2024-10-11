/**
 * @file draw_segments_3.cpp
 * @ingroup cglexamples
 * @brief Example of drawing 3D segments.
 *
 * This example creates and draws a set of random 3D segments.
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <DrawSegments3.h>
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

    /* Vector to store segments. */
    std::vector<CGL::Segment3> segs;

    /* Create segments with random endpoints. */
    int ns = 20; /* Number of segments. */
    CGL::Random rng(-10,10); /* "bounding box" for segments. "*/
    for (int i = 1; i <= ns; i++)
    {
        /* Endpoints. */
        CGL::Point3 p1(rng.get_int(), rng.get_int(), rng.get_int());
        CGL::Point3 p2(rng.get_int(), rng.get_int(), rng.get_int());
        std::cout << "Segment " << i << " (" << p1 << ") -- (" << p2 << ")" << std::endl;

        /* Segment. */
        CGL::Segment3 s(p1, p2);

        /* Insert to list. */
        segs.push_back(s);
    }

    /* Draw segments. */
    CGL::draw(segs);

    return EXIT_SUCCESS;
}
