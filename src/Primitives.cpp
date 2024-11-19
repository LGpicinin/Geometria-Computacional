/**
 * @file Primitives.cpp  
 * @brief Geometric primitives.
 *
 * Implements a set of basic geometric operations.
 *
 * \author Ricardo Dutra da Silva
 */


#include <Primitives.h>


float CGL::TriangleArea2(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3)
{
    CGL::Vector3 u = p2-p1;
    CGL::Vector3 v = p3-p1;

    return ((p2.x()-p1.x())*(p3.y()-p1.y())-(p2.y()-p1.y())*(p3.x()-p1.x()));
}


bool CGL::Left(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3)
{
    return (CGL::TriangleArea2(p1, p2, p3) > 0.0);
}


bool CGL::LeftOn(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3)
{
    return (TriangleArea2(p1, p2, p3) >= 0.0);
}


bool CGL::Collinear(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3)
{
    return (CGL::TriangleArea2(p1, p2, p3) == 0.0);
}


bool CGL::in_segment(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v)
{
    if (!CGL::Collinear(u1,u2,v))
        return false;

    if (u1.x() != u2.x())
        return (((u1.x() <= v.x()) && (v.x() <= u2.x())) || ((u2.x() <= v.x()) && (v.x() <= u1.x())));
    else
        return (((u1.y() <= v.y()) && (v.y() <= u2.y())) || ((u2.y() <= v.y()) && (v.y() <= u1.y())));
}


bool CGL::intersect(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v1, CGL::Point3 v2)
{
    if (CGL::properly_intersect(u1, u2, v1, v2))
        return true;

    if (CGL::improperly_intersect(u1, u2, v1, v2))
        return true;

    return false;
}


bool CGL::improperly_intersect(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v1, CGL::Point3 v2)
{
    if (CGL::in_segment(u1, u2, v1) ||
        CGL::in_segment(u1, u2, v2) ||
        CGL::in_segment(v1, v2, u1) ||
        CGL::in_segment(v1, v2, u2))
        return true;

    return false;        
}


bool CGL::properly_intersect(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v1, CGL::Point3 v2)
{
    if (Collinear(u1,u2,v1) || Collinear(u1,u2,v2) || Collinear(v1,v2,u1) || Collinear(v1,v2,u2))
        return false;

    if ((Left(u1,u2,v1) ^ Left(u1,u2,v2)) && (Left(v1,v2,u1) ^ Left(v1,v2,u2)))
        return true;

    return false;
}
