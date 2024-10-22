/**
 * @file draw_polygon_2.cpp
 * @ingroup cglexamples
 * @brief Example of drawing a 2D polygon.
 *
 * This example creates and draws a 2D polygon.
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <CGAL/draw_polygon_2.h>
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

    /* Simple polygon with random vertices. */
    CGL::Polygon2 P;

    /* Create vertices. */
    int ns = 10; /* Number of vertices. */
    CGL::Random rng(-10,10); /* "bounding box" for points. "*/
    do 
    {
        P.clear();
        for (int i = 1; i <= ns; i++)
            P.push_back(CGL::Point2(rng.get_int(),rng.get_int()));
    }
    while (!P.is_simple() || !P.is_counterclockwise_oriented());

    /* Draw points. */
    CGAL::draw(P);

    return EXIT_SUCCESS;
}
