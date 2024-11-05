#include <Types.h>
#include <CGAL/draw_polygon_2.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

typedef CGL::Polygon2::Vertex_iterator VertexIterator;

typedef struct
{
    int x;
    int y;
    bool convex;
    std::string classification;

} PointAngle;

float calculaAreaTriangulo(int *xList, int *yList)
{
    float sum1, sum2;
    sum1 = xList[0] * yList[1] + xList[1] * yList[2] + xList[2] * yList[0];
    sum2 = xList[1] * yList[0] + xList[2] * yList[1] + xList[0] * yList[2];

    return ((sum1 - sum2) / 2);
}

int left(float area)
{
    if (area > 0)
    {
        return 1;
    }
    else if (area < 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

bool colinear(float area, int *x, int *y)
{
    if (area == 0)
    {
        if ((x[2] >= x[0] && x[2] <= x[1]) || (x[2] >= x[1] && x[2] <= x[0]))
        {
            if ((y[2] >= y[0] && y[2] <= y[1]) || (y[2] >= y[1] && y[2] <= y[0]))
            {
                return true;
            }
        }
    }
    return false;
}

bool antiHorario(CGL::Polygon2 P)
{
    int x[3] = {0, 0, 0};
    int y[3] = {0, 0, 0};

    int xPos = 1;
    int yPos = 1;

    int xValue = 0;
    int yValue = 0;

    int count = 0;
    int i = 0;
    float soma = 0;

    for (VertexIterator it = P.vertices_begin(); it != P.vertices_end(); ++it)
    {

        x[xPos] = it[0][0];
        y[yPos] = it[0][1];

        count++;
        xPos++;
        yPos++;

        if (count == 2)
        {
            soma = soma + calculaAreaTriangulo(x, y);
            count = 1;
            xPos = 2;
            yPos = 2;
            x[1] = x[2];
            y[1] = y[2];
        }
    }

    if (soma > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<PointAngle> classificaVertice(CGL::Polygon2 P)
{

    std::vector<PointAngle> points;

    int x[3] = {0, 0, 0};
    int y[3] = {0, 0, 0};

    int xPos = 0;
    int yPos = 0;

    int count = 0;
    int testCount = 0;
    int i = 0;
    float aux = 1;
    float area = 0;

    bool sentido = antiHorario(P);

    if (sentido == false)
    {
        aux = -1;
    }

    for (VertexIterator it = P.vertices_begin(); it != P.vertices_end(); ++it)
    {
        x[xPos] = it[0][0];
        y[yPos] = it[0][1];

        count++;
        xPos++;
        yPos++;

        if (count == 3)
        {
            area = calculaAreaTriangulo(x, y);

            if (area * aux < 0)
            {
                points.push_back({x[1], y[1], false, ""});
            }
            else
            {
                points.push_back({x[1], y[1], true, ""});
            }

            for (i = 0; i < 2; i++)
            {
                x[i] = x[i + 1];
                y[i] = y[i + 1];
            }

            count = 2;
            xPos = 2;
            yPos = 2;
        }
    }

    count = 0;
    xPos = 2;
    yPos = 2;

    for (VertexIterator it = P.vertices_begin(); count != 2; ++it)
    {
        x[xPos] = it[0][0];
        y[yPos] = it[0][1];

        area = calculaAreaTriangulo(x, y);

        if (area * aux < 0)
        {
            points.push_back({x[1], y[1], false, ""});
        }
        else
        {
            points.push_back({x[1], y[1], true, ""});
        }

        for (i = 0; i < 2; i++)
        {
            x[i] = x[i + 1];
            y[i] = y[i + 1];
        }

        count++;
    }
    return points;
}

bool verificaMenor(PointAngle u, PointAngle v)
{ // verifica se v menor que u
    if (u.y > v.y)
    {
        return true;
    }
    else if (u.y == v.y)
    {
        if (u.x < v.x)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

std::vector<PointAngle> achaStartEndSplitMerge(CGL::Polygon2 P)
{
    std::vector<PointAngle> points;
    int length, i, iMenos, iMais, j, iMenosEhMenor, iMaisEhMenor;

    // classifica vértices em convexo e concâvo
    points = classificaVertice(P);
    length = points.size();

    iMenos = length - 1;
    iMais = 1;

    for (i = 0; i < length; i++)
    {
        if (points[i].y == points[iMenos].y && points[i].y == points[iMais % length].y)
        {
            points[i].classification = "regular";
            iMenos = i;
            iMais = i + 2;
            continue;
        }
        iMenosEhMenor = verificaMenor(points[i], points[iMenos]);
        iMaisEhMenor = verificaMenor(points[i], points[iMais % length]);
        if (points[i].convex == true)
        {

            if (iMenosEhMenor == true && iMaisEhMenor == true)
            {
                points[i].classification = "start";
            }
            else if (iMenosEhMenor == false && iMaisEhMenor == false)
            {
                points[i].classification = "end";
            }
            else
            {
                points[i].classification = "regular";
            }
        }
        else
        {
            if (iMenosEhMenor == true && iMaisEhMenor == true)
            {
                points[i].classification = "split";
            }
            else if (iMenosEhMenor == false && iMaisEhMenor == false)
            {
                points[i].classification = "merge";
            }
            else
            {
                points[i].classification = "regular";
            }
        }
        iMenos = i;
        iMais = i + 2;
    }
    return points;
}

int main()
{
    CGL::Polygon2 P;
    std::vector<PointAngle> points;
    bool antih = false;
    int i = 0;
    int option = 0;

    int ns = 10;
    CGL::Random rng(-10, 10);
    do
    {
        P.clear();
        for (int i = 1; i <= ns; i++)
            P.push_back(CGL::Point2(rng.get_int(), rng.get_int()));
    } while (!P.is_simple());

    points = achaStartEndSplitMerge(P);

    if (points.size() != ns)
    {
        std::cout << "ERRO" << std::endl;
        return 0;
    }
    for (i = 0; i < ns; i++)
    {
        std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") é " << points[i].classification << std::endl;
    }

    CGAL::draw(P);

    return EXIT_SUCCESS;
}
