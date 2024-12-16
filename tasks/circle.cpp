#include <Types.h>
#include <IO.h>
#include <CGAL/draw_surface_mesh.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <Primitives.h>

bool inCircle(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3, CGL::Point3 p4)
{
    return ((p1.x() * p2.y() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))) + (p4.x() * p1.y() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) + (p3.x() * p4.y() * (std::pow(p1.x(), 2) + std::pow(p1.y(), 2))) + (p2.x() * p3.y() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))) - (p4.x() * p3.y() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) - (p3.x() * p2.y() * (std::pow(p1.x(), 2) + std::pow(p1.y(), 2))) - (p2.x() * p1.y() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))) - (p1.x() * p4.y() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2)))) > 0;
}

int main(int argc, char *argv[])
{
    /* Check input */
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " num_vertices" << std::endl;
        return EXIT_FAILURE;
    }

    CGL::Point3 p1, p2, p3, p4;
    CGL::Mesh::Halfedge_index d1, d2, h;

    /* read mesh. */
    CGL::Mesh polygon = CGL::read_mesh(argv[1]);

    CGAL::draw(polygon);

    for (CGL::Mesh::Face_index f : polygon.faces())
    {
        d1 = polygon.halfedge(f);
        d2 = polygon.next(polygon.next(d1));

        p1 = polygon.point(polygon.target(d1));
        p2 = polygon.point(polygon.source(d2));
        p3 = polygon.point(polygon.target(d2));

        while (!CGL::Left(p1, p2, p3))
        {
            d1 = polygon.next(d1);
            d2 = polygon.next(d2);

            p1 = polygon.point(polygon.target(d1));
            p2 = polygon.point(polygon.source(d2));
            p3 = polygon.point(polygon.target(d2));
        }

        h = d1;

        p1 = polygon.point(polygon.source(h));
        p2 = polygon.point(polygon.target(h));
        h = polygon.next(h);
        p4 = polygon.point(polygon.target(h));
        h = polygon.next(h);
        p3 = polygon.point(polygon.target(h));

        CGAL::Euler::split_face(d1, d2, polygon);
        break;
    }

    CGAL::draw(polygon);

    bool incircle = inCircle(p1, p2, p3, p4);

    if (incircle == true)
    {
        std::cout << "Dentro do círculo" << std::endl;
    }
    else
    {
        std::cout << "Fora do círculo" << std::endl;
    }

    return EXIT_SUCCESS;
}
