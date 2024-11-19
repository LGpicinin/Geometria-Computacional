/**
 * @file draw_partition_2.cpp
 * @ingroup cglexamples
 * @brief Example of drawing a 2D partition.
 *
 * This example draws a 2D partition from an OBJ file. The mesh is represented as a surface mesh.
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <IO.h>
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

    /* Read mesh. */
    CGL::Mesh mesh = CGL::read_mesh(argv[1]);

    /* Draw mesh. */
    CGAL::draw(mesh);

    return EXIT_SUCCESS;
}
