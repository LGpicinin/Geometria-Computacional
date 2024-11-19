/**
 * @file IO.h
 * @brief IO functions.
 *
 * Defines input and output functions.
 *
 * @author Ricardo Dutra da Silva
 */


#ifndef IO_H
#define IO_H


#include <Types.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
 

/**
 * @brief The library namespace
 *
 * The namespace for CGL.
 */
namespace CGL
{

/**
 * @ingroup cglio
 * @brief Mesh read.
 *
 * Reads a mesh given as an OBJ file.
 *
 * @param fname OBJ file path.
 * @return A Mesh data structure.
 */
Mesh read_mesh(std::string fname);    

/**
 * @ingroup cglio
 * @brief Print mesh.
 *
 * Prints a set of tables for vertices, edges, halfedges and faces with geometric and topological information.
 *
 * @param mesh The mesh.
 */
void print_mesh(CGL::Mesh &mesh);

/**
 * @ingroup cglio
 * @brief Print mesh vertices.
 *
 * Prints a table of vertices with columns corresponding to vertex ID (VID), coordinates X, Y and Z, halfedge ID (HID) of an edge leaving the vertex.
 *
 * @param mesh The mesh.
 */
void print_mesh_vertices(CGL::Mesh &mesh);
 
/**
 * @ingroup cglio
 * @brief Print mesh edges (comprised by two halfedges).
 *
 * Prints a table of edges with columns corresponding to edge ID (VID), first hafedge ID, second halfedge ID, first vertex ID, and second vertex ID.
 *
 * @param mesh The mesh.
 */
void print_mesh_edges(CGL::Mesh &mesh);
   
 
/**
 * @ingroup cglio
 * @brief Print mesh halfedges.
 *
 * Prints a table of halfedges with columns corresponding to halfedge ID (VID), next hafedge ID, previous halfedge ID, opposite halfedge ID, and source vertex ID.
 *
 * @param mesh The mesh.
 */
void print_mesh_halfedges(CGL::Mesh &mesh);
 
/**
 * @ingroup cglio
 * @brief Print mesh faces.
 *
 * Prints a table of faces with columns corresponding to face ID and hafedge ID.
 *
 * @param mesh The mesh.
 */
void print_mesh_faces(CGL::Mesh &mesh);  
}


#endif /* IO_H */
