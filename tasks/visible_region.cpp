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

struct calculatedPoint
{
    CGL::Point3 point;
    bool changed;
};

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

struct calculatedPoint calculaInterseccao(std::vector<float> abc1, std::vector<float> abc2)
{
    float deter, x, y;
    CGL::Point3 point;
    struct calculatedPoint calcPoint = {point, false};

    deter = (abc1[0] * abc2[1]) - (abc2[0] * abc1[1]);

    if (deter == 0)
    {
        return calcPoint;
    }

    x = (1 / deter) * ((abc2[1] * abc1[2]) + ((abc1[1] * -1) * abc2[2]));
    y = (1 / deter) * (((abc2[0] * -1) * abc1[2]) + (abc1[0] * abc2[2]));

    point = CGL::Point3(x, y, 0);

    calcPoint.point = point;
    calcPoint.changed = true;

    return calcPoint;
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
    CGL::Point3 newPoint2, newPoint3;
    CGL::Mesh::Halfedge_index h_init, h, h2, last, h3, lastConcav;
    std::vector<float> reta1, reta2, reta3;
    struct calculatedPoint interseccao;
    bool concavoAtual, concavoAnterior, anteriorVisivel, atualVisivel, achou, primeiraVez, primeiraAresta;
    bool prolonga = false;
    bool yPositivo = false;
    bool xPositivo = false;
    bool yPositivo2 = true;
    bool xPositivo2 = true;
    primeiraAresta = false;
    anteriorVisivel = false;
    concavoAtual = false;
    concavoAnterior = false;
    achou = false;
    primeiraVez = true;

    std::vector<CGL::Mesh::Vertex_index> vertices;

    for (CGL::Mesh::Face_index f : polygon.faces())
    {
        h_init = polygon.halfedge(f);
        h = h_init;

        newPoint = polygon.point(polygon.target(h));
        prev = polygon.point(polygon.source(h));
        next = polygon.point(polygon.target(polygon.next(h)));

        while (!CGL::Left(prev, newPoint, next) || !naoIntersecta(polygon, h, point))
        {
            h = polygon.next(h);
            newPoint = polygon.point(polygon.target(h));
            prev = polygon.point(polygon.source(h));
            next = polygon.point(polygon.target(polygon.next(h)));
        }

        h_init = h;
        last = h;

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
                concavoAtual = true;
            }
            else
            {
                concavoAtual = false;
            }

            // se existe uma aresta entre o ponto e ponto da aresta atual, insere ponto da aresta na regiao visivel
            std::cout << newPoint.x() << ", " << newPoint.y() << std::endl;
            if (naoIntersecta(polygon, h, point))
            {
                std::cout << "ehhhhh" << std::endl;

                if (concavoAtual == true && anteriorVisivel == false) // se o ponto da aresta anterior não for visivel
                {
                    lastConcav = h;
                    prolonga = false;
                    h2 = polygon.prev(h);

                    if (h == last)
                    {
                        vertices.push_back(newPolygon.add_vertex(newPoint));
                        concavoAnterior = concavoAtual;
                        primeiraVez = false;
                        h = polygon.next(h);
                        anteriorVisivel = true;
                        prolonga = false;
                        continue;
                    }

                    if (primeiraVez == true) // se esta for a primeira interacao
                    {

                        if (naoIntersecta(polygon, h2, point)) // se existe uma aresta entre o ponto e o o ponto da aresta anterior, ignora
                        {
                            vertices.push_back(newPolygon.add_vertex(newPoint));
                            concavoAnterior = concavoAtual;
                            primeiraVez = false;
                            last = h;
                            h = polygon.next(h);
                            anteriorVisivel = true;
                            prolonga = false;
                            continue;
                        }
                        primeiraVez = false;
                    }

                    reta1 = calculaEquacaoReta(point, newPoint);

                    achou = false;

                    do // encontra o ponto atingido pelo prolongamento da aresta formada pelo ponto e pelo ponto da aresta atual
                    {
                        prev = polygon.point(polygon.source(h2));
                        next = polygon.point(polygon.target(h2));

                        reta2 = calculaEquacaoReta(prev, next);

                        interseccao = calculaInterseccao(reta1, reta2);

                        if (interseccao.changed == false)
                        {
                            h2 = polygon.prev(h2);
                            continue;
                        }

                        newPoint2 = interseccao.point;

                        if ((newPoint2.x() < prev.x() && newPoint2.x() > next.x()) || (newPoint2.x() > prev.x() && newPoint2.x() < next.x()))
                        {
                            if ((newPoint2.y() < prev.y() && newPoint2.y() > next.y()) || (newPoint2.y() > prev.y() && newPoint2.y() < next.y()))
                            {
                                std::cout << "Novo Ponto: " << newPoint2.x() << ", " << newPoint2.y() << std::endl;

                                h3 = h2;
                                if ((newPoint2.x() - newPoint.x()) > 0)
                                {
                                    xPositivo = true;
                                }
                                else
                                {
                                    xPositivo = false;
                                }
                                if ((newPoint2.y() - newPoint.y()) > 0)
                                {
                                    yPositivo = true;
                                }
                                else
                                {
                                    yPositivo = false;
                                }

                                // if ((prev.x() - newPoint2.x()) > 0)
                                // {
                                //     xPositivo2 = true;
                                // }
                                // else
                                // {
                                //     xPositivo2 = false;
                                // }
                                // if ((prev.y() - newPoint2.y()) > 0)
                                // {
                                //     yPositivo2 = true;
                                // }
                                // else
                                // {
                                //     yPositivo2 = false;
                                // }

                                // if ((xPositivo != xPositivo2 || yPositivo != yPositivo2))
                                // {
                                //     h2 = polygon.prev(h2);
                                //     continue;
                                // }
                                do
                                {
                                    prev = polygon.point(polygon.source(h3));
                                    next = polygon.point(polygon.target(h3));

                                    reta3 = calculaEquacaoReta(prev, next);

                                    interseccao = calculaInterseccao(reta1, reta3);

                                    if (interseccao.changed == false)
                                    {
                                        h3 = polygon.next(h3);
                                        continue;
                                    }

                                    newPoint3 = interseccao.point;

                                    if ((newPoint3.x() < prev.x() && newPoint3.x() > next.x()) || (newPoint3.x() > prev.x() && newPoint3.x() < next.x()))
                                    {
                                        if ((newPoint3.y() < prev.y() && newPoint3.y() > next.y()) || (newPoint3.y() > prev.y() && newPoint3.y() < next.y()))
                                        {
                                            if ((newPoint3.x() - newPoint.x()) > 0)
                                            {
                                                xPositivo2 = true;
                                            }
                                            else
                                            {
                                                xPositivo2 = false;
                                            }
                                            if ((newPoint3.y() - newPoint.y()) > 0)
                                            {
                                                yPositivo2 = true;
                                            }
                                            else
                                            {
                                                yPositivo2 = false;
                                            }

                                            /*if ((xPositivo != xPositivo2 || yPositivo != yPositivo2))
                                            {
                                                h3 = polygon.next(h3);
                                                continue;
                                            }*/

                                            if ((xPositivo == xPositivo2 && yPositivo == yPositivo2))
                                            {
                                                if ((abs((newPoint3.x() - newPoint.x())) < abs(newPoint2.x() - newPoint.x())) && (abs((newPoint3.y() - newPoint.y())) < abs(newPoint2.y() - newPoint.y())))
                                                {
                                                    break;
                                                }
                                                /*else
                                                {
                                                    vertices.push_back(newPolygon.add_vertex(newPoint2));
                                                    achou = true;
                                                }*/
                                            }
                                        }
                                    }
                                    h3 = polygon.next(h3);
                                } while (h3 != h2);
                                if (h3 == h2)
                                {
                                    vertices.push_back(newPolygon.add_vertex(newPoint2));
                                    achou = true;
                                }
                            }
                        }

                        h2 = polygon.prev(h2);
                    } while (h2 != h && achou == false);
                }
                last = h;
                vertices.push_back(newPolygon.add_vertex(newPoint));
                primeiraVez = false;
                anteriorVisivel = true;
            }

            // se a aresta anterior ter um ponto concavo e visivel OU for a primeira iteração
            else if ((concavoAnterior == true && anteriorVisivel == true) || (primeiraVez == true))
            {

                std::cout << "ohhhhh" << std::endl;
                if (primeiraVez == true) // se for a primeira iteração
                {
                    h2 = polygon.prev(h);
                    newPoint2 = polygon.point(polygon.target(h2));
                    prev = polygon.point(polygon.source(h2));
                    next = polygon.point(polygon.target(polygon.next(h2)));

                    // se a aresta anterior não tem um ponto concavo ou visivel
                    if (CGL::Left(prev, newPoint2, next) || !naoIntersecta(polygon, h2, point))
                    {
                        primeiraVez = false;
                        anteriorVisivel = false;
                        h = polygon.next(h);
                        continue;
                    }
                }
                primeiraVez = false;
                h2 = polygon.next(h);

                reta1 = calculaEquacaoReta(point, prev);

                achou = false;

                do // encontra o ponto atingido pelo prolongamento da aresta formada pelo ponto e pelo ponto da aresta anterior
                {
                    prev = polygon.point(polygon.source(h2));
                    next = polygon.point(polygon.target(h2));

                    reta2 = calculaEquacaoReta(prev, next);

                    interseccao = calculaInterseccao(reta1, reta2);

                    if (interseccao.changed == false)
                    {
                        h2 = polygon.next(h2);
                        continue;
                    }

                    newPoint2 = interseccao.point;

                    if ((newPoint2.x() < prev.x() && newPoint2.x() > next.x()) || (newPoint2.x() > prev.x() && newPoint2.x() < next.x()))
                    {
                        if ((newPoint2.y() < prev.y() && newPoint2.y() > next.y()) || (newPoint2.y() > prev.y() && newPoint2.y() < next.y()))
                        {
                            std::cout << "Novo Ponto: " << newPoint2.x() << ", " << newPoint2.y() << std::endl;

                            h3 = h2;
                            if ((newPoint2.x() - newPoint.x()) > 0)
                            {
                                xPositivo = true;
                            }
                            else
                            {
                                xPositivo = false;
                            }
                            if ((newPoint2.y() - newPoint.y()) > 0)
                            {
                                yPositivo = true;
                            }
                            else
                            {
                                yPositivo = false;
                            }
                            do
                            {
                                prev = polygon.point(polygon.source(h3));
                                next = polygon.point(polygon.target(h3));

                                reta3 = calculaEquacaoReta(prev, next);

                                interseccao = calculaInterseccao(reta1, reta3);

                                if (interseccao.changed == false)
                                {
                                    h3 = polygon.next(h3);
                                    continue;
                                }

                                newPoint3 = interseccao.point;

                                if ((newPoint3.x() < prev.x() && newPoint3.x() > next.x()) || (newPoint3.x() > prev.x() && newPoint3.x() < next.x()))
                                {
                                    if ((newPoint3.y() < prev.y() && newPoint3.y() > next.y()) || (newPoint3.y() > prev.y() && newPoint3.y() < next.y()))
                                    {
                                        if ((newPoint3.x() - newPoint.x()) > 0)
                                        {
                                            xPositivo2 = true;
                                        }
                                        else
                                        {
                                            xPositivo2 = false;
                                        }
                                        if ((newPoint3.y() - newPoint.y()) > 0)
                                        {
                                            yPositivo2 = true;
                                        }
                                        else
                                        {
                                            yPositivo2 = false;
                                        }

                                        if ((xPositivo == xPositivo2 && yPositivo == yPositivo2))
                                        {
                                            if ((abs((newPoint3.x() - newPoint.x())) < abs(newPoint2.x() - newPoint.x())) && (abs((newPoint3.y() - newPoint.y())) < abs(newPoint2.y() - newPoint.y())))
                                            {
                                                break;
                                            }
                                        }
                                    }
                                }
                                h3 = polygon.next(h3);
                            } while (h3 != h2);
                            if (h3 == h2)
                            {
                                vertices.push_back(newPolygon.add_vertex(newPoint2));
                                last = h2;
                                achou = true;
                            }
                        }
                    }

                    h2 = polygon.next(h2);
                } while (h2 != h && achou == false);
                anteriorVisivel = false;
            }
            else
            {
                std::cout << "ihhhhh" << std::endl;
                primeiraVez = false;
                anteriorVisivel = false;
            }
            concavoAnterior = concavoAtual;
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
    std::cout << "Digite a coordenada y do ponto: ";
    std::cin >> y;

    CGL::Point3 point = CGL::Point3(x, y, 0);

    calculaRegiaoVisivel(polygon, point);

    return EXIT_SUCCESS;
}
