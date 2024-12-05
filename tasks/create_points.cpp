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
int main(int argc, char *argv[])
{
    /* Check input */
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " num_vertices" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<CGL::Point2> pts;

    /* Create points with random endpoints. */
    int ns = atoi(argv[1]);   /* Number of vertices. */
    CGL::Random rng(-10, 10); /* "bounding box" for points. "*/
    for (int i = 1; i <= ns; i++)
    {
        /* Point. */
        CGL::Point2 p(rng.get_int(), rng.get_int());
        std::cout << p << std::endl;

        /* Insert to list. */
        pts.push_back(p);
    }

    /* Draw points. */
    CGL::draw(pts);

    return EXIT_SUCCESS;
}
