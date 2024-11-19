/**
 * @file halfedge_info.cpp
 * @ingroup cglexamples
 * @brief Print halfedge.
 *
 * Shows halfedge tables with connectivity information representing a mesh.
 *
 * @author Ricardo Dutra da Silva
 */


#include <Types.h>
#include <IO.h>
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

    /* Print mesh. */
    CGL::print_mesh(mesh);

    return EXIT_SUCCESS;
}
