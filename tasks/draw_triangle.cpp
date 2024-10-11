#include <Types.h>
#include <CGAL/draw_polygon_2.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

/**
 * Code example.
 * @return Success.
 */
int main()
{

    CGL::Polygon2 P;

    /* Create vertices. */
    int ns = 3; /* Number of vertices. */
    float x, y, i, sum1, sum2;

    float area;
    bool orient = true;

    std::vector<int> xList;
    std::vector<int> yList;
    // vector<int> x, y;

    for (i = 1; i <= 3; i++)
    {
        std::cout << "Digite o x do ponto " << i << ":" << std::endl;
        std::cin >> x;

        std::cout << "Digite o y do ponto " << i << ":" << std::endl;
        std::cin >> y;

        P.push_back(CGL::Point2(x, y));
        xList.push_back(x);
        yList.push_back(y);
    }

    sum1 = xList[0] * yList[1] + xList[1] * yList[2] + xList[2] * yList[0];
    sum2 = xList[1] * yList[0] + xList[2] * yList[1] + xList[0] * yList[2];

    area = (sum1 - sum2) / 2;

    if (area < 0)
    {
        area = area * -1;
        orient = false;
    }

    std::cout << "Area: " << area << std::endl;

    if (orient == true)
    {
        std::cout << "Antihorário" << std::endl;
    }

    else
    {
        std::cout << "Horário" << std::endl;
    }

    /* Draw points. */
    CGAL::draw(P);

    return EXIT_SUCCESS;
}
