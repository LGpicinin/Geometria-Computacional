/**
 * @file Types.h
 * @brief Kernel and types.
 *
 * Defines the kernel used by CGAL and common types used by the library.
 *
 * @author Ricardo Dutra da Silva
 */


#ifndef TYPES_H
#define TYPES_H


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Surface_mesh.h>
 

/**
 * @brief The library namespace
 *
 * The namespace for CGL.
 */
namespace CGL
{

/** 
 * @ingroup cgltypes
 * CGAL kernel used in all defined types. 
 */
typedef CGAL::Simple_cartesian<double> K;

/**
 * @ingroup cgltypes
 * 2D point. 
 * */
typedef K::Point_2 Point2;

/**
 * @ingroup cgltypes
 * 3D point. 
 * */
typedef K::Point_3 Point3;

/**
 * @ingroup cgltypes
 * 2D vector. 
 * */
typedef K::Vector_2 Vector2;

/**
 * @ingroup cgltypes
 * 3D vector. 
 * */
typedef K::Vector_3 Vector3;

/**
 * @ingroup cgltypes
 * 2D segment. 
 * */
typedef K::Segment_2 Segment2;

/**
 * @ingroup cgltypes
 * 2D segment. 
 * */
typedef K::Segment_3 Segment3;


/**
 * @ingroup cgltypes
 * 2D Polygon. 
 * */
typedef CGAL::Polygon_2<K> Polygon2;

/**
 * @ingroup cgltypes
 * Polygonal mesh. 
 * */
typedef CGAL::Surface_mesh<Point3> Mesh;

}


#endif /* TYPES_H */
