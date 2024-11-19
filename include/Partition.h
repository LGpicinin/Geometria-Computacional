/**
 * @file Partition.h
 * @brief Planar maps partition functions.
 *
 * Defines functions to partition a planar map.
 *
 * @author Ricardo Dutra da Silva
 */


#ifndef PARTITION_H
#define PARTITION_H


#include <Primitives.h>
 

/**
 * @brief The library namespace
 *
 * The namespace for CGL.
 */
namespace CGL
{

/**
 * @ingroup cglpartition
 * @brief Polygon 2D partition (naive).
 *
 * A naive algorithm to partition a polygon of n vertices. The algorithm includes n-3 diagonals to the polygon by iterating over pairs of vertices.
 * 
 * @param polygon A CGL::Mesh representing the polygon.
 */
void polygon_2_triangulate_naive(Mesh &polygon);    


/**
 * @ingroup cglpartition
 * @brief Check diagonal.
 *
 * Given two halfedges h1 and h2 of a polygonal face, check whether the segment (u,v) = (target(h1),target(h2)) is a diagonal of the polygon: the segment must be internal to the polygon and must not intersect any edge that is not incident in u or v.
 * * 
 * @param polygon A CGL::Mesh representing the polygon.
 * @param h1 A Halfedge of the polygon (must belong to the same face of h2).
 * @param h2 A Halfedge of the polygon (must belong to the same face of h1).
 * @return true or false.
 */
bool is_diagonal(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2);


/**
 * @ingroup cglpartition
 * @brief Check in cone.
 *
 * Given two halfedges h1 and h2 of a polygonal face, check whether the segment (u,v) = (target(h1),target(h2)) is locally inside the polygon: the segment must be internal in the neighborhood of vertices u-, u and u+ as well as in v-, v and v+.
 *  
 * @param polygon A CGL::Mesh representing the polygon.
 * @param h1 A Halfedge of the polygon (must belong to the same face of h2).
 * @param h2 A Halfedge of the polygon (must belong to the same face of h1).
 * @return true or false.
 */
bool in_cone(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2);


/**
 * @ingroup cglpartition
 * @brief Check intersection.
 *
 * Given two halfedges h1 and h2 of a polygonal face, check whether the segment (u,v) = (target(h1),target(h2)) is non-intersecting: the segment must not intersect any edge of the polygon, except for the edges incident on u and v.
 *  
 * @param polygon A CGL::Mesh representing the polygon.
 * @param h1 A Halfedge of the polygon (must belong to the same face of h2).
 * @param h2 A Halfedge of the polygon (must belong to the same face of h1).
 * @return true or false.
 */
bool non_intersecting(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2);


/**
 * @ingroup cglpartition
 * @brief Check adjacency.
 *
 * Given two vertices u and v of a face in a polygonal mesh, check if they are adjacent by comparing if v lies in the orbit of u.
 * 
 * @param mesh A CGL::Mesh representing the polygon.
 * @param u Vertex of the mesh.
 * @param v Vertex of the mesh.
 * @return true or false.
 */
bool mesh_is_adjacent(Mesh &mesh, Mesh::Vertex_index u, Mesh::Vertex_index v);


/**
 * @ingroup cglpartition
 * @brief Check in cone.
 *
 * Given two ertices u and v, representing a segment uv, check if uv is locally inside polygon by the in cone test.
 * 
 * @param mesh A CGL::Mesh representing the polygon.
 * @param u Vertex of the mesh.
 * @param v Vertex of the mesh.
 * @return true or false.
 */
bool mesh_in_cone(CGL::Mesh &mesh, CGL::Mesh::Halfedge_index u, CGL::Mesh::Halfedge_index v);

}


float TriangleArea2(CGL::Point3 p1, CGL::Point3 p2, CGL::Point3 p3);


#endif /* PARTITION_H */
