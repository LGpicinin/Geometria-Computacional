#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <Types.h>
#include <IO.h>
#include <CGAL/draw_surface_mesh.h>
#include <stdlib.h>
#include <cmath>
#include <Primitives.h>
#include <DrawPoints2.h>
#include <vector>
#include <time.h>
#include <algorithm>
using namespace cv;

Mat dst, detected_edges;
Mat image_gray, image;
int lowThreshold = 10;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char *window_name = "Edge Map";

struct GoodEdge
{
    bool bomPonto;
    CGL::Mesh::Halfedge_index h1;
};

struct order_y
{
    inline bool operator()(const CGL::Point3 p1, const CGL::Point3 p2)
    {
        return (p1.y() < p2.y());
    }
};

struct Faced
{
    CGL::Mesh::Halfedge_index half_edge;
};

float inCircle(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3, CGL::Point3 p4)
{
    float ac1 = (p1.x() * (p2.y() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))) + (p4.y() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) + (p3.y() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))) - (p4.y() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))) - (p3.y() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) - (p2.y() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))));
    float ac2 = (p1.y() * (-1) * (p2.x() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))) + (p4.x() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) + (p3.x() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))) - (p4.x() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))) - (p3.x() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) - (p2.x() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))));
    float ac3 = ((std::pow(p1.x(), 2) + std::pow(p1.y(), 2)) * (p2.x() * p3.y()) + (p2.y() * p4.x()) + (p3.x() * p4.y()) - (p4.x() * p3.y()) - (p3.x() * p2.y()) - (p2.x() * p4.y()));
    float ac4 = ((-1) * (p2.x() * p3.y() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))) + (p4.x() * p2.y() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))) + (p3.x() * p4.y() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) - (p4.x() * p3.y() * (std::pow(p2.x(), 2) + std::pow(p2.y(), 2))) - (p3.x() * p2.y() * (std::pow(p4.x(), 2) + std::pow(p4.y(), 2))) - (p2.x() * p4.y() * (std::pow(p3.x(), 2) + std::pow(p3.y(), 2))));
    return (ac1 + ac2 + ac3 + ac4);
}

bool dentroTriangulo(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1)
{
    CGL::Point3 p1, p2, p3;
    int count = 0;

    while (count != 3)
    {
        p1 = polygon.point(polygon.source(h1));
        p2 = polygon.point(polygon.target(h1));

        if (CGL::LeftOn(p1, p2, p3))
        {
            if (CGL::Left(p1, p2, p3))
            {
                count++;
                h1 = polygon.next(h1);
            }
            else
            {
                h1 = polygon.opposite(h1);
            }
        }
        else
        {
            h1 = polygon.opposite(h1);
        }
    }
    return true;
}

static void detect_edges(int, void *)
{
    blur(image_gray, detected_edges, Size(3, 3));
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
    dst = Scalar::all(0);
}

float calculaReta(float x1, float x2, float y1, float y2)
{
    return std::sqrt((std::pow(x1 - x2, 2)) + (std::pow(y1 - y2, 2)));
}

float calculaDif(float distxn, float distxy)
{
    if (distxn / distxy > 1)
    {
        return 0;
    }
    else
    {
        return (distxn / distxy);
    }
}

void fillLine(int x1, int x2, int y, std::vector<CGL::Point3> pts)
{
    float dist1n, dist2n, dist3n, sub1, sub2, sub3;
    int xStart, xEnd, x, v1, v2, v3, value;

    v1 = image_gray.at<uchar>(std::round(pts[0].x()), std::round(pts[0].y()));
    v2 = image_gray.at<uchar>(std::round(pts[1].x()), std::round(pts[1].y()));
    v3 = image_gray.at<uchar>(std::round(pts[2].x()), std::round(pts[2].y()));

    // const float dist12 = calculaReta(pts[1].x(), pts[0].x(), pts[1].y(), pts[0].y());
    // const float dist13 = calculaReta(pts[2].x(), pts[0].x(), pts[2].y(), pts[0].y());
    // const float dist23 = calculaReta(pts[2].x(), pts[1].x(), pts[2].y(), pts[1].y());

    // std::cout << "fill" << std::endl;

    if (x1 < x2)
    {
        xStart = x1;
        xEnd = x2;
    }
    else
    {
        xStart = x2;
        xEnd = x1;
    }

    for (x = xStart; x < xEnd; x++)
    {
        // dist1n = calculaReta(pts[0].x(), float(x), pts[0].y(), float(y));
        // dist2n = calculaReta(pts[1].x(), float(x), pts[1].y(), float(y));
        // dist3n = calculaReta(pts[2].x(), float(x), pts[2].y(), float(y));

        // value = std::round((v1 * (calculaDif(dist1n, dist12) * calculaDif(dist1n, dist13))) + (v2 * (calculaDif(dist2n, dist12) * calculaDif(dist2n, dist23))) + (v3 * (calculaDif(dist3n, dist13) * calculaDif(dist3n, dist23))));
        value = (v1 + v2 + v3) / 3;
        if (dst.rows <= x)
        {
            std::cout << "x errado - " << dst.rows << '-' << x << std::endl;
        }
        if (dst.cols <= y)
        {
            std::cout << "y errado" << dst.cols << '-' << y << std::endl;
        }
        dst.at<uchar>(x, y) = value;
        // std::cout << "fill2" << std::endl;
    }
}

void scanLine(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1)
{
    CGL::Point3 p1, p2, p3;
    std::vector<CGL::Point3> pts;
    int yMin, yMax, x1, x2;
    float dx1, dx2, x1aux, x2aux;
    p1 = polygon.point(polygon.target(h1));
    p2 = polygon.point(polygon.target(polygon.next(h1)));
    p3 = polygon.point(polygon.source(h1));
    pts.push_back(p1);
    pts.push_back(p2);
    pts.push_back(p3);

    std::sort(pts.begin(), pts.end(), order_y());

    yMin = std::round(pts[0].y());
    yMax = std::round(pts[1].y());

    // const float dist12 = calculaReta(pts[1].x(), pts[0].x(), pts[1].y(), pts[0].y());
    // const float dist13 = calculaReta(pts[2].x(), pts[0].x(), pts[2].y(), pts[0].y());
    // const float dist23 = calculaReta(pts[2].x(), pts[1].x(), pts[2].y(), pts[1].y());

    dx1 = (pts[1].x() - pts[0].x()) / (pts[1].y() - pts[0].y());

    dx2 = (pts[2].x() - pts[0].x()) / (pts[2].y() - pts[0].y());

    x1 = pts[0].x();
    x2 = pts[0].x();
    x1aux = x1;
    x2aux = x2;

    for (int i = yMin; i < yMax; i++)
    {
        fillLine(x1, x2, i, pts);
        x1aux = x1aux + dx1;
        x2aux = x2aux + dx2;
        x1 = std::round(x1aux);
        x2 = std::round(x2aux);
    }

    yMin = pts[1].y();
    yMax = pts[2].y();

    dx1 = (pts[2].x() - pts[1].x()) / (pts[2].y() - pts[1].y());

    x1 = pts[1].x();
    x1aux = x1;
    x2aux = x2;

    for (int i = yMin; i < yMax; i++)
    {
        fillLine(x1, x2, i, pts);
        x1aux = x1aux + dx1;
        x2aux = x2aux + dx2;
        x1 = std::round(x1aux);
        x2 = std::round(x2aux);
    }
}

void triangulariza(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Vertex_index v4)
{
    CGL::Mesh::Vertex_index v1, v2, v3;
    // CGL::Mesh::Edge_index e14, e43, e31, e23, e12, e24;
    CGL::Mesh::Halfedge_index h14, h43, h31, h23, h12, h24;
    // Faced f1, f2, f3;

    v1 = polygon.target(h1);
    h31 = h1;

    v2 = polygon.target(polygon.next(h1));
    h12 = polygon.next(h1);

    v3 = polygon.target(polygon.next(polygon.next(h1)));
    h23 = polygon.next(h12);

    h14 = polygon.add_edge(v1, v4);
    h24 = polygon.add_edge(v2, v4);
    h43 = polygon.add_edge(v4, v3);

    polygon.set_next(h31, h14);
    polygon.set_next(h43, h31);
    polygon.set_next(h23, polygon.opposite(h43));
    polygon.set_next(polygon.opposite(h14), h12);
    polygon.set_next(h12, h24);
    polygon.set_next(h24, polygon.opposite(h14));
    polygon.set_next(polygon.opposite(h43), polygon.opposite(h24));
    polygon.set_next(polygon.opposite(h24), h23);
    polygon.set_next(h14, h43);

    scanLine(polygon, h31);
    scanLine(polygon, h12);
    scanLine(polygon, h23);
}

// nao usado
int achaFace(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, std::vector<Faced> faces)
{
    CGL::Mesh::Halfedge_index h2, h3;
    int i = 0;

    h2 = polygon.next(h1);
    h3 = polygon.next(h2);

    for (auto f = faces.begin(); f != faces.end(); ++f)
    {
        if (f->half_edge == h1 || f->half_edge == h2 || f->half_edge == h3)
        {
            return i;
        }
        i++;
    }

    return -1;
}

GoodEdge achaPonto(CGL::Mesh &polygon, int i, int j)
{
    GoodEdge ge;
    CGL::Mesh::Halfedge_index h1;
    CGL::Mesh::Vertex_index newPoint;
    CGL::Point3 p1, p2;
    CGL::Point3 p3 = CGL::Point3(i, j, 0);
    int pos;
    int count = 0;
    bool ehTriangulo = false;
    for (CGL::Mesh::Halfedge_index h : polygon.halfedges())
    {
        // verifica se aresta eh de borda
        h1 = h;
        if (polygon.is_border(h1))
        {
            h1 = polygon.opposite(h1);
        }
        // acha aresta de um triangulo
        // h1 = achaTriangulo(polygon, h1);

        // acha triangulo que contem ponto
        while (count < 3)
        {
            p1 = polygon.point(polygon.source(h1));
            p2 = polygon.point(polygon.target(h1));

            if (CGL::LeftOn(p1, p2, p3))
            {
                if (CGL::Left(p1, p2, p3))
                {
                    count++;
                    h1 = polygon.next(h1);
                }
                else
                {
                    ge.bomPonto = false;
                    return ge;
                }
            }
            else
            {
                count = 0;
                h1 = polygon.opposite(h1);
                h = h1;
            }
        }

        // verifica se realmente eh um triangulo
        if (h1 != h)
        {
            std::cout << "ERRO - H1 E H NÃO IGUAIS";
            exit(1);
        }

        // adiciona ponto e triangulariza
        newPoint = polygon.add_vertex(CGL::Point3(i, j, 0));
        triangulariza(polygon, h1, newPoint);

        ge.bomPonto = true;
        return ge;
    }
    ge.bomPonto = false;
    return ge;
}

void triangulatePolygon(CGL::Mesh &polygon, int np, std::vector<int> rowsEdges, std::vector<int> colsEdges)
{
    GoodEdge ge;
    int i, j, row, col, posRow, posCol;
    CGL::Mesh::Vertex_index newPoint;
    bool bomPonto = false;
    int size = rowsEdges.size();
    std::vector<CGL::Point2> pts;

    srand(time(NULL));

    i = 0;
    while (i < np && size != 0)
    {
        posRow = rand() % size;
        posCol = posRow;

        row = rowsEdges[posRow];
        col = colsEdges[posCol];

        // std::cout << "acha o ponto" << std::endl;
        ge = achaPonto(polygon, row, col);
        if (ge.bomPonto == true)
        {
            // newPoint = polygon.add_vertex(CGL::Point3(row, col, 0));
            // triangulariza(polygon, ge.h1, newPoint);
            i++;
            CGL::Point2 p(row, col);
            pts.push_back(p);
        }

        // std::cout << size << std::endl;

        rowsEdges.erase(rowsEdges.begin() + posRow);
        colsEdges.erase(colsEdges.begin() + posCol);

        size = rowsEdges.size();
    }

    CGL::draw(pts);
}

int main(int argc, char **argv)
{
    std::vector<int> rowsEdges;
    std::vector<int> colsEdges;
    if (argc != 3)
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    // le imagem
    image = imread(argv[1], IMREAD_COLOR);
    int np = atoi(argv[2]);

    // converte imagem para preto e branco
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    // std::cout << image_gray.rows << ", " << image_gray.cols << " and " << image_gray.type() << std::endl;
    dst.create(image_gray.size(), image_gray.type());

    // accept only char type matrices
    CV_Assert(image_gray.depth() == CV_8U);

    // MatIterator_<uchar> it, end;
    // for (it = image_gray.begin<uchar>(); end = image_gray.end<uchar>(); it != end; ++it)
    // {
    //     *it = (*it) / 2;
    // }

    // namedWindow("x2", WINDOW_AUTOSIZE);
    // imshow("x2", image_gray);

    // detecta borda
    detect_edges(0, 0);
    int row, col;
    for (row = 0; row < detected_edges.rows; row++)
    {
        for (col = 0; col < detected_edges.cols; col++)
        {
            if (detected_edges.at<uchar>(row, col) == 255)
            {
                rowsEdges.push_back(row);
                colsEdges.push_back(col);
            }
        }
    }

    // criando poligono
    CGL::Mesh polygon;

    CGL::Mesh::Vertex_index p4 = polygon.add_vertex(CGL::Point3(0, 0, 0));
    CGL::Mesh::Vertex_index p3 = polygon.add_vertex(CGL::Point3(0, image_gray.cols, 0));
    CGL::Mesh::Vertex_index p2 = polygon.add_vertex(CGL::Point3(image_gray.rows, image_gray.cols, 0));
    CGL::Mesh::Vertex_index p1 = polygon.add_vertex(CGL::Point3(image_gray.rows, 0, 0));

    polygon.add_face(p1, p2, p3, p4);

    CGL::Mesh::Halfedge_index d1, d2;

    for (CGL::Mesh::Face_index f : polygon.faces())
    {
        d1 = polygon.halfedge(f);
        d2 = polygon.next(polygon.next(d1));
        CGAL::Euler::split_face(d1, d2, polygon);

        break;
    }
    // CGL::Point3 po1, po2, po3;

    // for (CGL::Mesh::Face_index f : polygon.faces())
    // {
    //     d1 = polygon.halfedge(f);
    //     scanLine(polygon, d1);
    // }

    // scanLine(polygon, d1);
    // scanLine(polygon, d2);

    // Faced faced, faced2;
    // std::vector<Faced> faces;

    triangulatePolygon(polygon, np, rowsEdges, colsEdges);

    CGAL::draw(polygon);

    // for (auto f = faces.begin(); f != faces.end(); ++f)
    // {
    //     std::cout << f->half_edge << " - eh isso?" << std::endl;
    //     scanLine(polygon, f->half_edge);
    //     std::cout << "1" << std::endl;
    // }

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", dst);

    waitKey(0);

    return 0;
}