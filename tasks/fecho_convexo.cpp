#include <Types.h>
#include <DrawPoints2.h>
#include <CGAL/draw_polygon_2.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <time.h>

std::vector<CGL::Point2> readfile(char *str)
{
    char point[10], *c;
    int i = 0;
    int coords[2] = {0, 0};
    FILE *arq;
    std::vector<CGL::Point2> pts;

    arq = fopen(str, "r");
    if (arq == NULL)
        printf("Erro");
    else
    {
        while (fgets(point, 10, arq))
        {
            c = strtok(point, " ");
            i = 0;
            while (c)
            {
                std::cout << c << " ";
                coords[i] = atoi(c);
                c = strtok(NULL, " ");
                i++;
            }
            std::cout << std::endl;
            CGL::Point2 p(coords[0], coords[1]);
            pts.push_back(p);
        }
    }
    fclose(arq);
    return pts;
}

float calculaReta(CGL::Point2 p1, CGL::Point2 p2)
{
    return std::sqrt((std::pow(p1.x() - p2.x(), 2)) + (std::pow(p1.y() - p2.y(), 2)));
}

float calculaArea(CGL::Point2 p1, CGL::Point2 p2, CGL::Point2 p3)
{
    return ((p2.x() - p1.x()) * (p3.y() - p1.y()) - (p2.y() - p1.y()) * (p3.x() - p1.x()));
}

float calculaAltura(CGL::Point2 p1, CGL::Point2 p2, CGL::Point2 p3)
{
    float area = abs(calculaArea(p1, p2, p3) / 2);
    float base = calculaReta(p1, p2);
    float altura = (2 * area) / base;

    return altura;
}

bool Left(CGL::Point2 p1, CGL::Point2 p2, CGL::Point2 p3)
{
    float area = calculaArea(p1, p2, p3);

    if (area < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void achaYextremos(std::vector<CGL::Point2> &pts, int *maior, int *menor)
{
    int size = pts.size();
    int i;

    *maior = pts[0].y();
    *menor = pts[0].y();

    for (i = 1; i < size; i++)
    {
        if (pts[i].y() < *menor)
        {
            *menor = pts[i].y();
            // menorY = menor->y();
        }

        if (pts[i].y() > *maior)
        {
            *maior = pts[i].y();
            // maiorY = maior->y();
        }
    }
}

float calculaXmedio(std::vector<CGL::Point2> &pts)
{
    int size = pts.size();
    int sum, i, j;
    float media;

    sum = 0;

    srand(time(NULL));

    i = 0;
    while (i != 4)
    {
        j = rand() % size;
        sum = sum + pts[j].x();

        i++;
    }

    media = sum / 4;

    return media;
}

/*void consertaOrdem(std::vector<CGL::Point2> &polygon, bool sentido)
{
    int size = polygon.size();
    int i;
    bool s;
    CGL::Point2 primeiro, segundo, terceiro, aux;

    primeiro = polygon[0];
    segundo = polygon[1];
    terceiro = polygon[2];

    for (i = 3; i < size; i++)
    {

        s = Left(primeiro, segundo, terceiro);

        if (s != sentido)
        {
            polygon.push_back(terceiro);
            polygon.push_back(segundo);
        }

        primeiro = segundo;
        segundo = terceiro;
        terceiro = polygon[i];
    }
}*/

void quickStep(std::vector<CGL::Point2> &pts, CGL::Point2 p1, CGL::Point2 p2, std::vector<CGL::Point2> &polygon)
{

    int size = pts.size();

    if (size == 0)
    {
        return;
    }

    int i, iRemove;
    float maiorAltura, altura;
    CGL::Point2 maior, aux;
    std::vector<CGL::Point2> pts1, pts2;
    bool sentido;

    maiorAltura = calculaAltura(p1, p2, pts[0]);
    maior = pts[0];

    for (i = 1; i < size; i++)
    {
        altura = calculaAltura(p1, p2, pts[i]);

        if (altura > maiorAltura)
        {
            maiorAltura = altura;
            maior = pts[i];
            iRemove = i;
        }
    }

    // aux = pts[size - 1];
    // pts[size - 1] = pts[iRemove];
    // pts[iRemove] = aux;
    // pts.pop_back();

    sentido = Left(p1, p2, maior);

    size = pts.size();

    for (i = 0; i < size; i++)
    {
        if (i == iRemove)
        {
            continue;
        }
        if (Left(p2, maior, pts[i]) != sentido && calculaArea(p2, maior, pts[i]) != 0)
        {
            pts1.push_back(pts[i]);
        }
        else if (Left(maior, p1, pts[i]) != sentido && calculaArea(maior, p1, pts[i]) != 0)
        {
            pts2.push_back(pts[i]);
        }
    }

    quickStep(pts2, maior, p1, polygon);
    polygon.push_back(maior);
    quickStep(pts1, p2, maior, polygon);
}

CGL::Polygon2 quickHull(std::vector<CGL::Point2> &pts)
{
    int maior, menor;
    std::vector<CGL::Point2> ptsMenor, ptsMaior, polygon1, polygon2, polygon3;
    CGL::Point2 aux(0, 0);
    CGL::Point2 primeiro, segundo;
    CGL::Polygon2 polygon;
    bool sentido = false;
    float xMedio, sum;
    int size = pts.size();
    int i;

    sum = 0;

    xMedio = calculaXmedio(pts);
    achaYextremos(pts, &maior, &menor);

    CGL::Point2 p1(xMedio, maior - 1);
    CGL::Point2 p2(xMedio, menor + 1);

    for (i = 0; i < size; i++)
    {
        if (pts[i].x() < xMedio)
        {
            ptsMenor.push_back(pts[i]);
        }
        else
        {
            ptsMaior.push_back(pts[i]);
        }
    }

    quickStep(ptsMaior, p1, p2, polygon1);
    quickStep(ptsMenor, p2, p1, polygon2);

    size = polygon2.size();

    for (i = 0; i < size; i++)
    {
        polygon1.push_back(polygon2[i]);
    }

    CGL::draw(polygon1);

    size = polygon1.size();

    for (i = 0; i < size; i++)
    {
        polygon.push_back(polygon1[i]);
    }

    return polygon;
}

int main(int argc, char *argv[])
{
    /* Check input. */
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " input.txt" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<CGL::Point2> pts;
    CGL::Polygon2 polygon;

    pts = readfile(argv[1]);

    CGL::draw(pts);

    polygon = quickHull(pts);

    // CGAL::draw(polygon);
}