/**
 * @file polygon_2_triangulate.cpp
 * @ingroup cglexamples
 * @brief Example of a triangulatiion of a polygon.
 *
 * Triangulates a polygon using a naive algorithm (O(n^4)).
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <IO.h>
#include <Partition.h>
#include <CGAL/draw_surface_mesh.h>
#include <iostream>
#include <stdlib.h>
#include <vector>


/**
 * Code example.
 * @return Success.
 */
int main(int argc, char* argv[])
{
    /* Check input. */
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " input.obj" << std::endl;
        return EXIT_FAILURE;
    }

    /* Read polygon as a mesh. */
    CGL::Mesh polygon = CGL::read_mesh(argv[1]);

    /* Triangulate polygon. */
    CGAL::draw(polygon);
    CGL::polygon_2_triangulate_naive(polygon);    
    CGAL::draw(polygon);

    return EXIT_SUCCESS;
}
