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
Mat image_gray, image, triangulated;
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

static void detect_edges(int, void *)
{
    blur(image_gray, detected_edges, Size(3, 3));
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
    // namedWindow("Display Image", WINDOW_AUTOSIZE);
    // imshow("Display Image", detected_edges);
    dst = Scalar::all(0);
}

void fillLine()
{
}

void scanLine(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1)
{
    CGL::Point3 p1, p2, p3;
    std::vector<CGL::Point3> pts;
    int yMin, yMax, x1, x2;
    float dx1, dx2;
    p1 = polygon.point(polygon.target(h1));
    p2 = polygon.point(polygon.target(polygon.next(h1)));
    p3 = polygon.point(polygon.source(h1));
    pts.push_back(p1);
    pts.push_back(p2);
    pts.push_back(p3);

    std::sort(pts.begin(), pts.end(), order_y());

    yMin = pts[0].y();
    yMax = pts[1].y();

    dx1 = (pts[1].x() - pts[0].x()) / (pts[1].y() - pts[0].y());
    dx2 = (pts[2].x() - pts[0].x()) / (pts[2].y() - pts[0].y());

    x1 = pts[0].x();
    x2 = pts[0].x();

    for (int i = yMin; i < yMax; i++)
    {
        fillLine();
        x1 = std::round(x1 + dx1);
        x2 = std::round(x2 + dx2);
    }

    yMin = pts[1].y();
    yMax = pts[2].y();

    dx1 = (pts[2].x() - pts[1].x()) / (pts[2].y() - pts[1].y());

    x1 = pts[1].x();

    for (int i = yMin; i < yMax; i++)
    {
        fillLine();
        x1 = std::round(x1 + dx1);
        x2 = std::round(x2 + dx2);
    }
}

void triangulariza(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Vertex_index v4)
{
    CGL::Mesh::Vertex_index v1, v2, v3;
    // CGL::Mesh::Edge_index e14, e43, e31, e23, e12, e24;
    CGL::Mesh::Halfedge_index h14, h43, h31, h23, h12, h24;
    CGL::Mesh::Face_index f;

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

    // f = polygon.add_face(v1, v2, newPoint);
    // if (f == polygon.null_face())
    // {
    //     f = polygon.add_face(newPoint, v2, v1);
    // }
    // f = polygon.add_face(v2, v3, newPoint);
    // if (f == polygon.null_face())
    // {
    //     f = polygon.add_face(newPoint, v3, v2);
    // }
    // f = polygon.add_face(v1, v3, newPoint);
    // if (f == polygon.null_face())
    // {
    //     f = polygon.add_face(newPoint, v3, v1);
    // }
}

GoodEdge achaPonto(CGL::Mesh &polygon, int i, int j)
{
    GoodEdge ge;
    CGL::Mesh::Halfedge_index h1;
    CGL::Mesh::Vertex_index newPoint;
    CGL::Point3 p1, p2;
    CGL::Point3 p3 = CGL::Point3(i, j, 0);
    int count = 0;
    bool ehTriangulo = false;
    for (CGL::Mesh::Halfedge_index h : polygon.halfedges())
    {
        h1 = h;
        if (polygon.is_border(h1))
        {
            h1 = polygon.opposite(h1);
        }
        // verifica se h1 faz parte de um triângulo
        while (ehTriangulo == false)
        {
            while (count != 3)
            {
                h1 = polygon.next(h1);
                count++;
            }
            if (h1 != h)
            {
                h1 = polygon.opposite(h1);
                h1 = polygon.next(h1);
            }
            else
            {
                ehTriangulo = true;
            }
        }
        count = 0;
        h = h1;
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
        if (h1 != h)
        {
            std::cout << "ERRO - H1 E H NÃO IGUAIS";
            exit(1);
        }
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
    std::cout << image_gray.rows << ", " << image_gray.cols << " and " << image_gray.type() << std::endl;
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

    waitKey(0);

    // criando poligono
    CGL::Mesh polygon;

    CGL::Mesh::Vertex_index p1 = polygon.add_vertex(CGL::Point3(0, 0, 0));
    CGL::Mesh::Vertex_index p2 = polygon.add_vertex(CGL::Point3(image_gray.rows, 0, 0));
    CGL::Mesh::Vertex_index p3 = polygon.add_vertex(CGL::Point3(image_gray.rows, image_gray.cols, 0));
    CGL::Mesh::Vertex_index p4 = polygon.add_vertex(CGL::Point3(0, image_gray.cols, 0));

    polygon.add_face(p1, p2, p3, p4);

    CGL::Mesh::Halfedge_index d1, d2;

    for (CGL::Mesh::Face_index f : polygon.faces())
    {
        d1 = polygon.halfedge(f);
        d2 = polygon.next(polygon.next(d1));
        CGAL::Euler::split_face(d1, d2, polygon);

        break;
    }
    CGL::Point3 po1, po2, po3;
    // for (CGL::Mesh::Face_index f : polygon.faces())
    // {
    //     d1 = polygon.halfedge(f);
    //     po1 = polygon.point(polygon.source(d1));
    //     po2 = polygon.point(polygon.target(d1));
    //     po3 = polygon.point(polygon.target(polygon.next(d1)));
    //     p1 = polygon.add_vertex(CGL::Point3((po1.x() + po2.x() + po3.x()) / 3, (po1.y() + po2.y() + po3.y()) / 3, 0));
    //     triangulariza(polygon, d1, p1);

    //     break;
    // }

    // CGAL::draw(polygon);

    triangulatePolygon(polygon, np, rowsEdges, colsEdges);

    CGAL::draw(polygon);

    return 0;
}