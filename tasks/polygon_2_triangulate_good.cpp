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

float calculaReta(CGL::Point3 p1, CGL::Point3 p2)
{
    return std::sqrt((std::pow(p1.x() - p2.x(), 2)) + (std::pow(p1.y() - p2.y(), 2)));
}

float qualidade(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3)
{
    float x = calculaReta(p1, p2);
    float y = calculaReta(p1, p3);
    float z = calculaReta(p3, p2);
    float media = (x + y + z) / 3;

    float maior = 0;
    float menores[2] = {0, 0};
    float sum = 0;

    if (x > z && x > y)
    {
        maior = x;
        menores[0] = z;
        menores[1] = y;
    }
    else if (y > x && y > z)
    {
        maior = y;
        menores[0] = x;
        menores[1] = z;
    }
    else
    {
        maior = z;
        menores[0] = x;
        menores[1] = y;
    }

    if (menores[0] <= media)
    {
        sum = sum + (1 - (menores[0] / maior));
    }
    if (menores[1] <= media)
    {
        sum = sum + (1 - (menores[1] / maior));
    }

    return sum;
}

void flipEdge(CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index d1, CGL::Mesh::Halfedge_index d2, CGL::Mesh &polygon)
{
    CGAL::Euler::join_face(h1, polygon);
    CGAL::Euler::split_face(d1, d2, polygon);
}

void improvePolygon(CGL::Mesh &polygon)
{
    CGL::Point3 p1, p2, p3;
    CGL::Mesh::Halfedge_index d1;
    CGL::Mesh::Halfedge_index d2;
    CGL::Mesh::Halfedge_index h1_init;
    CGL::Mesh::Halfedge_index h1;
    CGL::Mesh::Halfedge_index h2;
    CGL::Mesh::Halfedge_index h2_init;
    float sumPontuacao1, sumPontuacao2;
    int count = 0;
    int limit = 0;
    bool improve = true;
    bool diagonal = false;
    int i = 0;

    while (improve == true)
    {
        improve = false;

        for (CGL::Mesh::Face_index f : polygon.faces())
        {
            h1_init = polygon.halfedge(f);
            h1 = h1_init;
            h1 = polygon.next(h1);

            // percorre face f até que todas as suas arestas tenham sido visitadas OU até que haja um flip-edge
            while (!improve && h1 != h1_init)
            {

                h2_init = polygon.opposite(h1);
                h2 = h2_init;
                h2 = polygon.next(h2);
                count = 0;

                // verifica de o oposto de h1 (h2) faz parte de um triângulo
                while (count < 2 && h2 != h2_init)
                {
                    h2 = polygon.next(h2);
                    count++;
                }

                // se h2 faz parte de um triângulo
                if (count == 2 && h2 == h2_init)
                {
                    sumPontuacao2 = 0;
                    sumPontuacao1 = 0;
                    d1 = polygon.next(h1);
                    d2 = polygon.next(h2);
                    p2 = polygon.point(polygon.source(h1));
                    p1 = polygon.point(polygon.target(d1));
                    p3 = polygon.point(polygon.target(d2));

                    // verifica se h1 corta um quadrilatero convexo
                    if (!CGL::Left(p1, p2, p3))
                    {
                        h1 = polygon.next(h1);
                        continue;
                    }
                    // soma pontuação do triangulo composto pela diagonal presente entre d1 e d2
                    sumPontuacao2 = sumPontuacao2 + qualidade(p1, p2, p3);

                    p2 = p3;
                    p3 = polygon.point(polygon.target(h1));

                    // verifica se h1 corta um quadrilatero convexo
                    if (!CGL::Left(p1, p2, p3))
                    {
                        h1 = polygon.next(h1);
                        continue;
                    }
                    // soma pontuação do triangulo composto pela diagonal presente entre d1 e d2
                    sumPontuacao2 = sumPontuacao2 + qualidade(p1, p2, p3);

                    // soma pontuação dos triangulos compostos pela diagonal h1
                    p1 = polygon.point(polygon.target(h1));
                    p2 = polygon.point(polygon.source(h1));
                    p3 = polygon.point(polygon.target(polygon.next(h1)));
                    sumPontuacao1 = sumPontuacao1 + qualidade(p1, p2, p3);
                    p1 = polygon.point(polygon.target(h2));
                    p2 = polygon.point(polygon.source(h2));
                    p3 = polygon.point(polygon.target(polygon.next(h2)));
                    sumPontuacao1 = sumPontuacao1 + qualidade(p1, p2, p3);

                    std::cout << "Sum1: " << sumPontuacao1 << "\nSum2: " << sumPontuacao2 << std::endl;

                    // se a nova diagonal formar triângulos melhores que o anterior, cria ela; se não, pula pra próxima aresta
                    if (sumPontuacao2 < sumPontuacao1)
                    {
                        improve = true;
                        flipEdge(h1, d1, d2, polygon);
                    }
                    else
                    {
                        h1 = polygon.next(h1);
                    }
                }
                else
                {
                    h1 = polygon.next(h1);
                }
            }
        }
        i++;
    }
}
int main(int argc, char *argv[])
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
    CGL::polygon_2_triangulate_naive(polygon);
    CGAL::draw(polygon);

    improvePolygon(polygon);
    CGAL::draw(polygon);

    return EXIT_SUCCESS;
}
