/**
 * @file draw_points_2.cpp
 * @ingroup cglexamples
 * @brief Example of drawing 2D points.
 *
 * This example creates and draws a set of random 2D points.
 *
 * @author Ricardo Dutra da Silva
 */

#include <Types.h>
#include <DrawPoints2.h>
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

    /* Vector to store points. */
    std::vector<CGL::Point2> pts;

    /* Create pointss with random endpoints. */
    int ns = 50;              /* Number of points. */
    CGL::Random rng(-10, 10); /* "bounding box" for points. "*/
    for (int i = 1; i <= ns; i++)
    {
        /* Point. */
        CGL::Point2 p(rng.get_int(), rng.get_int());
        std::cout << "Point " << i << " (" << p << ")" << std::endl;

        /* Insert to list. */
        pts.push_back(p);
    }

    /* Draw points. */
    CGL::draw(pts);

    return EXIT_SUCCESS;
}
