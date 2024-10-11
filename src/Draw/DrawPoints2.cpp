/**
 * @file DrawPoints2.cpp  
 * @brief Points drawing.
 *
 * Implements a class and function to draw 2D points using CGAL basic viewer.
 *
 * @author Ricardo Dutra da Silva
 */


#include <DrawPoints2.h>


CGL::SimplePoints2ViewerQt::SimplePoints2ViewerQt(QWidget* parent, std::vector<CGL::Point2> pt2, const char* title = "Basic Viewer") :
    //First draw: vertices, edges, faces, multi-color, no inverse normal
    Base(parent, title, true, true, false, false, true),
    p2(pt2)
{
    compute_elements();
}


void CGL::SimplePoints2ViewerQt::compute_vertex(CGL::Point2& pt)
{
    add_point(pt, CGAL::IO::red());
}


void CGL::SimplePoints2ViewerQt::compute_elements()
{
    clear();
    for (auto itor = p2.begin(); itor != p2.end(); ++itor)
    {
        compute_vertex(*itor);
    }
}
    

void CGL::SimplePoints2ViewerQt::keyPressEvent(QKeyEvent* e)
{
    Base::keyPressEvent(e);
}


void CGL::draw(std::vector<CGL::Point2> &p2)
{
    CGAL::Qt::init_ogl_context(3, 3);
    int argc = 1;
    const char* argv[2] = { "segs3_viewer","\0" };
    QApplication app(argc, const_cast<char**>(argv));
    
    SimplePoints2ViewerQt mainwindow(app.activeWindow(), p2, "Viewer");
    mainwindow.show();
    app.exec();
}
