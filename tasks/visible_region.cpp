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
#include <cmath>

std::vector<float> calculaEquacaoReta(CGL::Point3 point1, CGL::Point3 point2)
{
    float a, b, c;
    std::vector<float> abc;
    a = point1.y() - point2.y();
    b = point2.x() - point1.x();
    c = (point1.x() * point2.y()) - (point2.x() * point1.y());
    abc.push_back(a);
    abc.push_back(b);
    abc.push_back(c * -1);

    return abc;
}

bool calculaInterseccao(std::vector<float> abc1, std::vector<float> abc2, CGL::Point3 &point)
{
    float deter, x, y;

    deter = (abc1[0] * abc2[1]) - (abc2[0] * abc1[1]);

    if (deter == 0)
    {
        return false;
    }

    x = (1 / deter) * ((abc2[1] * abc1[2]) + ((abc1[1] * -1) * abc2[2]));
    y = (1 / deter) * (((abc2[0] * -1) * abc1[2]) + (abc1[0] * abc2[2]));

    point = CGL::Point3(x, y, 0);

    return true;
}

bool naoIntersecta(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Point3 point)
{
    // Candidate segment u and v endpoints.
    CGL::Point3 u1 = polygon.point(polygon.target(h1));
    // CGL::Point3 u2 = polygon.point(polygon.target(h2));

    // Check all non-adjacent edges to uv.
    CGL::Mesh::Halfedge_index h_init = h1;
    CGL::Mesh::Halfedge_index h = h_init;
    do
    {
        // An edge not adjacent to u = target(h1) or v = target(h2).
        if ((h != h1) && (h != polygon.next(h1)))
        {
            CGL::Point3 v1 = polygon.point(polygon.source(h));
            CGL::Point3 v2 = polygon.point(polygon.target(h));

            if (CGL::intersect(u1, point, v1, v2))
                return false;
        }

        h = polygon.next(h);
    } while (h_init != h);

    return true;
}

CGL::Mesh calculaRegiaoVisivel(CGL::Mesh &polygon, CGL::Point3 point)
{
    CGL::Mesh newPolygon;
    CGL::Point3 newPoint, prev, next;
    CGL::Point3 newPoint2;
    CGL::Mesh::Halfedge_index h_init, h, h2;
    std::vector<float> reta1, reta2;
    bool concavo, anterior, achou, interseccao;
    anterior = false;
    concavo = false;
    achou = false;
    interseccao = false;

    std::vector<CGL::Mesh::Vertex_index> vertices;

    int i = 0;

    for (CGL::Mesh::Face_index f : polygon.faces())
    {
        h_init = polygon.halfedge(f);
        h = h_init;

        do
        {
            /*newPoint = polygon.point(polygon.target(h));
            vertices.push_back(newPolygon.add_vertex(newPoint));
            i++;*/
            newPoint = polygon.point(polygon.target(h));
            prev = polygon.point(polygon.source(h));
            next = polygon.point(polygon.target(polygon.next(h)));
            if (!CGL::Left(prev, newPoint, next))
            {
                concavo = true;
            }
            else
            {
                concavo = false;
            }

            if (naoIntersecta(polygon, h, point))
            {
                // newPoint = polygon.point(polygon.target(h));
                vertices.push_back(newPolygon.add_vertex(newPoint));
                anterior = true;
            }
            else if (concavo == true && anterior == true)
            {
                h2 = polygon.next(h);

                reta1 = calculaEquacaoReta(point, prev);

                achou = false;

                do
                {
                    prev = polygon.point(polygon.source(h2));
                    next = polygon.point(polygon.target(h2));

                    reta2 = calculaEquacaoReta(prev, next);

                    interseccao = calculaInterseccao(reta1, reta2, newPoint2);

                    if (!interseccao)
                    {
                        continue;
                    }

                    if ((newPoint2.x() < prev.x() && newPoint.x() > next.x()) || (newPoint2.x() > prev.x() && newPoint2.x() < next.x()))
                    {
                        if ((newPoint2.y() < prev.y() && newPoint2.y() > next.y()) || (newPoint2.y() > prev.y() && newPoint2.y() < next.y()))
                        {
                            vertices.push_back(newPolygon.add_vertex(newPoint2));
                            achou = true;
                        }
                    }

                    h2 = polygon.next(h2);
                } while (h2 != h && achou == false);
            }
            else
            {
                anterior = false;
            }

            h = polygon.next(h);
        } while (h_init != h);
    }

    newPolygon.add_face(vertices);

    CGAL::draw(newPolygon);

    return newPolygon;
}

int main(int argc, char *argv[])
{
    /* Check input. */
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " input.obj" << std::endl;
        return EXIT_FAILURE;
    }

    int x, y;
    CGL::Mesh visibleRegion;

    /* Read polygon as a mesh. */
    CGL::Mesh polygon = CGL::read_mesh(argv[1]);

    CGAL::draw(polygon);

    std::cout << "Digite a coordenada x do ponto: ";
    std::cin >> x;
    std::cout << "Digite a coordenada x do ponto: ";
    std::cin >> y;

    CGL::Point3 point = CGL::Point3(x, y, 0);

    calculaRegiaoVisivel(polygon, point);

    return EXIT_SUCCESS;
}
