/**
 * @file DrawVectors3.h  
 * @brief Vectors drawing.
 *
 * Defines a class and function to draw an array of 3D vectors using CGAL basic viewer.
 *
 * \author Ricardo Dutra da Silva
 */


#ifndef DRAW_VECTORS_3_H
#define DRAW_VECTORS_3_H


#include<Types.h>
#include<CGAL/Qt/Basic_viewer_qt.h>
#include<CGAL/Qt/init_ogl_context.h>


namespace CGL
{

/**
 * @brief Vectors viewer.
 *
 * Extends CGAL basic viewer to draw an array of vectors. 
 */ 
class SimpleVectors3ViewerQt : public CGAL::Basic_viewer_qt
{
    typedef Basic_viewer_qt Base; /**< CGAL basic viewer. */

    protected:
    std::vector<CGL::Vector3> s3; /**< Vectors array. */

    public:
    /** 
     * The constructor. 
     *
     * @param parent Qt Widget.
     * @param seg3 Array of Vector3.
     * @param title Window title.
     */
    SimpleVectors3ViewerQt(QWidget* parent, std::vector<CGL::Vector3> seg3,  const char* title);

    protected:
    /** 
     * Add edges to the viewer. 
     * @param seg Vector2.
     */
    void compute_edge(CGL::Vector3& seg);
    
    /** 
     * Add vertices to the viewer. 
     * @param seg Vector3.
     */
    void compute_vertex(CGL::Vector3& seg);
    
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
 * @brief Draw vectors.
 *
 * Function to draw a list of 3D vectors.
 *
 * @param s3 a list of vectors given as an array vector<Vector3>.
 */
void draw(std::vector<CGL::Vector3> &s3);

}

#endif /* DRAW_VECTORS_3_H */
