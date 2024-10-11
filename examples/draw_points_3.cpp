#include <Types.h>
#include <DrawPoints2.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

int main()
{
    /* Vector to store points. */
    std::vector<CGL::Point2> pts;

    /* Create points with random endpoints. */
    int ns = 3;               /* Number of points. */
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