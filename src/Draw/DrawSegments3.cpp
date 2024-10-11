/**
 * @file DrawSegments3.cpp  
 * @brief Segments drawing.
 *
 * Implements a class and function to draw a vector of 3D segments using CGAL basic viewer.
 *
 * @author Ricardo Dutra da Silva
 */


#include <DrawSegments3.h>


CGL::SimpleSegments3ViewerQt::SimpleSegments3ViewerQt(QWidget* parent, std::vector<CGL::Segment3> seg3, const char* title = "Basic Segs3 Viewer") :
    //First draw: vertices, edges, faces, multi-color, no inverse normal
    Base(parent, title, true, true, false, false, true),
    s3(seg3)
{
    compute_elements();
}


void CGL::SimpleSegments3ViewerQt::compute_edge(CGL::Segment3& seg)
{
    add_segment(seg.source(), seg.target(), CGAL::IO::blue());
}


void CGL::SimpleSegments3ViewerQt::compute_vertex(CGL::Segment3& seg)
{
    add_point(seg.source(), CGAL::IO::red());
    add_point(seg.target(),CGAL::IO::red());
}


void CGL::SimpleSegments3ViewerQt::compute_elements()
{
    clear();
    for (auto itor = s3.begin(); itor != s3.end(); ++itor)
    {
        compute_vertex(*itor);
        compute_edge(*itor);
    }
}
    

void CGL::SimpleSegments3ViewerQt::keyPressEvent(QKeyEvent* e)
{
    Base::keyPressEvent(e);
}


void CGL::draw(std::vector<CGL::Segment3> &s3)
{
    CGAL::Qt::init_ogl_context(3, 3);
    int argc = 1;
    const char* argv[2] = { "segs3_viewer","\0" };
    QApplication app(argc, const_cast<char**>(argv));
    
    SimpleSegments3ViewerQt mainwindow(app.activeWindow(), s3, "Viewer");
    mainwindow.show();
    app.exec();
}
