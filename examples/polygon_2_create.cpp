/**
 * @file polygon_2_create.cpp
 * @ingroup cglexamples
 * @brief Example of create a random polygon and writing an OBJ file.
 *
 * Create an OBJ file with a random polygon.
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <CGAL/draw_polygon_2.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Random.h>
#include <CGAL/algorithm.h>
#include <fstream>
#include <list>

typedef std::list<CGL::Point2>                           Container;
typedef CGAL::Polygon_2<CGL::K, Container>               Polygon_2;
typedef CGAL::Creator_uniform_2<int, CGL::Point2>        Creator;
typedef CGAL::Random_points_in_square_2<CGL::Point2, Creator> Point_generator;


/**
 * Code example.
 * @return Success.
 */
int main(int argc, char* argv[])
{
    /* Check input */
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " num_vertices" << std::endl;
        return EXIT_FAILURE;

    }
    
    int nv = atoi(argv[1]); /* Number of vertices. */

    Polygon_2            P;
    std::list<CGL::Point2>   point_set;
    CGAL::Random         rand;

    rand.get_seed();

    CGAL::copy_n_unique(Point_generator(100), nv, std::back_inserter(point_set));
    CGAL::random_polygon_2(point_set.size(), std::back_inserter(P), point_set.begin());

    /* Print OBJ file. */
    for (Polygon_2::Vertex_iterator vi = P.vertices_begin(); vi != P.vertices_end(); ++vi)
        std::cout << "v " << *vi << " 0.0" << std::endl;
    std::cout << std::endl;
    std::cout << "f ";
    for (int i = 1; i <= nv; i++)
        std::cout << " " << i;
    std::cout << std::endl;

    ///* Draw polygon. */
    //CGAL::draw(P);

    return EXIT_SUCCESS;
}
