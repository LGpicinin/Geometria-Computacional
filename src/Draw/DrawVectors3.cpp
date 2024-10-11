/**
 * @file DrawVectors3.cpp  
 * @brief Vectors drawing.
 *
 * Implements a class and function to draw an array of 3D vectors using CGAL basic viewer.
 *
 * @author Ricardo Dutra da Silva
 */


#include <DrawVectors3.h>


CGL::SimpleVectors3ViewerQt::SimpleVectors3ViewerQt(QWidget* parent, std::vector<CGL::Vector3> seg3, const char* title = "Basic Segs3 Viewer") :
    //First draw: vertices, edges, faces, multi-color, no inverse normal
    Base(parent, title, true, true, false, false, true),
    s3(seg3)
{
    compute_elements();
}


void CGL::SimpleVectors3ViewerQt::compute_edge(CGL::Vector3& seg)
{
    /* Define vector origin and origin+Coordinates. */
    CGL::Point3 o(0.0, 0.0, 0.0);
    CGL::Point3 e(seg.x(), seg.y(), seg.z());
    add_segment(o, e, CGAL::IO::blue());
}


void CGL::SimpleVectors3ViewerQt::compute_vertex(CGL::Vector3& seg)
{
    CGL::Point3 e(seg.x(), seg.y(), seg.z());
    add_point(e, CGAL::IO::red());
}


void CGL::SimpleVectors3ViewerQt::compute_elements()
{
    clear();
    for (auto itor = s3.begin(); itor != s3.end(); ++itor)
    {
        compute_vertex(*itor);
        compute_edge(*itor);
    }
}
    

void CGL::SimpleVectors3ViewerQt::keyPressEvent(QKeyEvent* e)
{
    Base::keyPressEvent(e);
}


void CGL::draw(std::vector<CGL::Vector3> &s3)
{
    CGAL::Qt::init_ogl_context(3, 3);
    int argc = 1;
    const char* argv[2] = { "segs3_viewer","\0" };
    QApplication app(argc, const_cast<char**>(argv));
    
    SimpleVectors3ViewerQt mainwindow(app.activeWindow(), s3, "Viewer");
    mainwindow.show();
    app.exec();
}
