/**
 * @file DrawPoints2.h  
 * @brief Points drawing.
 *
 * Defines a class and function to draw an array of 2D points using CGAL basic viewer.
 *
 * @author Ricardo Dutra da Silva
 */


#ifndef DRAW_POINTS_2_H
#define DRAW_POINTS_2_H


#include<Types.h>
#include<CGAL/Qt/Basic_viewer_qt.h>
#include<CGAL/Qt/init_ogl_context.h>


namespace CGL
{

/**
 * @brief Points viewer.
 *
 * Extends CGAL basic viewer to draw an array of points. 
 */ 
class SimplePoints2ViewerQt : public CGAL::Basic_viewer_qt
{
    typedef Basic_viewer_qt Base; /**< CGAL basic viewer. */

    protected:
    std::vector<CGL::Point2> p2; /**< Points array. */

    public:
    /** 
     * The constructor. 
     *
     * @param parent Qt Widget.
     * @param pt2 Vector of Point2.
     * @param title Window title.
     */
    SimplePoints2ViewerQt(QWidget* parent, std::vector<CGL::Point2> pt2,  const char* title);

    protected:
    /** 
     * Add vertices to the viewer. 
     * @param pt Point2.
     */
    void compute_vertex(CGL::Point2& pt);
    
    /** 
     * Add all elements. 
     */
    void compute_elements();

    /** 
     * Delegate events to base class. 
     *
     * @param e Event.
     * @return None.
     */
    virtual void keyPressEvent(QKeyEvent* e);
};


/**
 * @ingroup cgldraw
 * @brief Draw points.
 *
 * Function to draw a list of 2D points.
 *
 * @param s3 a lists of points given as an array vector<Point2>.
 */
void draw(std::vector<CGL::Point2> &s3);

}

#endif /* DRAW_POINTS_2_H */
