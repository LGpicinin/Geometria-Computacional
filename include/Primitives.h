/**
 * @file Primitives.h
 * @brief Basic geometric functions.
 *
 * Defines some basic handful geometric functions.
 *
 * @author Ricardo Dutra da Silva
 */


#ifndef PRIMITIVES_H
#define PRIMITIVES_H


#include <Types.h>
#include <CGAL/boost/graph/Euler_operations.h>
 

/**
 * @brief The library namespace
 *
 * The namespace for CGL.
 */
namespace CGL
{

/**
 * @ingroup cglprimitives
 * @brief Left predicate.
 *
 * Computes whether vertex w is to the left of the oriented segment uv.
 *
 * @param u First vertex.
 * @param v Second vertex.
 * @param w Third vertex.
 * @return The double area.
 */
bool Left(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3);


/**
 * @ingroup cglprimitives
 * @brief Left/On predicate.
 *
 * Computes whether vertex w is to the left or on the line define by the oriented segment uv.
 *
 * @param u First vertex.
 * @param v Second vertex.
 * @param w Third vertex.
 * @return The double area.
 */
bool LeftOn(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3);


/**
 * @ingroup cglprimitives
 * @brief In segment.
 *
 * Computes whether vertex w is collinear to u and v and lies on the segment uv.
 *
 * @param u First vertex.
 * @param v Second vertex.
 * @param w Third vertex.
 * @return The double area.
 */
bool in_segment(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3);

/**
 * @ingroup cglprimitives
 * @brief Collinear predicate.
 *
 * Computes whether vertices u, v and w are collinear.
 *
 * @param u First vertex.
 * @param v Second vertex.
 * @param w Third vertex.
 * @return The double area.
 */
bool Collinear(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3);


/**
 * @ingroup cglprimitives
 * @brief Double triangle area.
 *
 * Computes the signed double area of a triangle given by three points.
 *
 * @param p1 First vertex of the triangle.
 * @param p2 Second vertex of the triangle.
 * @param p3 Third vertex of the triangle.
 * @return The double area.
 */
float TriangleArea2(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3);


 /**
 * @ingroup cglprimitives
 * @brief Check intersection.
 *
 * Given two segments defined by vertices (u1,u2) and (v1,v2)), check whether the segments intersect.
 *
 * @param u1 A vertex of the first segment.
 * @param u1 Another vertex of the first segment.
 * @param v1 A vertex of the second segment.
 * @param v1 Another vertex of the second segment.
 * @return true or false.
 */
bool intersect(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v1, CGL::Point3 v2);


/**
 * @ingroup cglprimitives
 * @brief Check improper intersection.
 *
 * Given two segments defined by vertices (u1,u2) and (v1,v2)), check whether the segments intersect improperly. An improper intesection point P must  coincide with at least one of u1, u2, v1 or v2. 
 *
 * @param u1 A vertex of the first segment.
 * @param u1 Another vertex of the first segment.
 * @param v1 A vertex of the second segment.
 * @param v1 Another vertex of the second segment.
 * @return true or false.
 */
bool improperly_intersect(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v1, CGL::Point3 v2);


/**
 * @ingroup cglprimitives
 * @brief Check proper intersection.
 *
 * Given two segments defined by vertices (u1,u2) and (v1,v2)), check whether the segments intersect properly. A proper intesection point P must lie in the open segments, i.e., P should not include u1, u2, v1 aor v2. 
 *
 * @param u1 A vertex of the first segment.
 * @param u1 Another vertex of the first segment.
 * @param v1 A vertex of the second segment.
 * @param v1 Another vertex of the second segment.
 * @return true or false.
 */
bool properly_intersect(CGL::Point3 u1, CGL::Point3 u2, CGL::Point3 v1, CGL::Point3 v2);



}


#endif /* PRIMITIVES_H */
