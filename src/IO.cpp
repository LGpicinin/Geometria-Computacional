/**
 * @file IO.cpp  
 * @brief IO functions.
 *
 * Implements input and output functions.
 *
 * \author Ricardo Dutra da Silva
 */


#include <IO.h>


CGL::Mesh CGL::read_mesh(std::string fname)
{
    CGL::Mesh mesh;

  	if(!CGAL::Polygon_mesh_processing::IO::read_polygon_mesh(fname, mesh))
	{
    	std::cerr << "Invalid input file." << std::endl;
		exit(EXIT_FAILURE);
	}

	return mesh;
}


void CGL::print_mesh(CGL::Mesh &mesh)
{
    /* Print vertices. */
    print_mesh_vertices(mesh);
    
    /* Print edges. */
    print_mesh_edges(mesh);
    
    /* Print halfedges. */
    print_mesh_halfedges(mesh);

    /* Print faces. */
    print_mesh_faces(mesh);
}


void CGL::print_mesh_vertices(CGL::Mesh &mesh)
{
    int table_width = 70;

    /* Print header. */
    std::cout << std::endl << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    std::cout << "Vertex Table" << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    printf("Vertex");
    printf("%*s", 10, "");
    printf("X");
    printf("%*s", 11, "");
    printf("Y");
    printf("%*s", 11, "");
    printf("Z");
    printf("%*s", 5, "");
    printf("Halfedge\n");
    std::cout << std::string(table_width, '-') << std::endl;

    /* Print lines. */
    for (Mesh::Vertex_index v : mesh.vertices())
    {
        Point3 pt = mesh.point(v);
        Mesh::Halfedge_index h = mesh.halfedge(v);
        printf("%5d", v.idx());
        printf("%*s", 2, "");
        printf("%+10.2f", pt.x());
        printf("%*s", 2, "");
        printf("%+10.2f", pt.y());
        printf("%*s", 2, "");
        printf("%+10.2f", pt.z());
        printf("%*s", 8, "");
        printf("%5d", h.idx());
        std::cout << std::endl;
    }
    std::cout << std::string(table_width, '=') << std::endl;
}


void CGL::print_mesh_edges(CGL::Mesh &mesh)
{
    int table_width = 70;

    /* Print header. */
    std::cout << std::endl << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    std::cout << "Edge Table" << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    printf("Edge");
    printf("%*s", 4, "");
    printf("Halfedge 1");
    printf("%*s", 3, "");
    printf("Halfedge 2");
    printf("%*s", 5, "");
    printf("Vertex 1");
    printf("%*s", 5, "");
    printf("Vertex 2\n");
    std::cout << std::string(table_width, '-') << std::endl;

    /* Print lines. */
    for (CGL::Mesh::Edge_index e : mesh.edges())
    {
        // First halfedge
        CGL::Mesh::Halfedge_index h1 = mesh.halfedge(e);
        CGL::Mesh::Vertex_index   v1 = mesh.source(h1);
        // Second halfedge
        CGL::Mesh::Halfedge_index h2 = mesh.opposite(h1);
        CGL::Mesh::Vertex_index   v2 = mesh.source(h2);

        printf("%5d", e.idx());
        printf("%*s", 8, "");
        printf("%5d", h1.idx());
        printf("%*s", 8, "");
        printf("%5d", h2.idx());
        printf("%*s", 8, "");
        printf("%5d", v1.idx());
        printf("%*s", 8, "");
        printf("%5d", v2.idx());
        std::cout << std::endl;
    }
    std::cout << std::string(table_width, '=') << std::endl;
}


void CGL::print_mesh_halfedges(CGL::Mesh &mesh)
{
    int table_width = 70;

    /* Print header. */
    std::cout << std::endl << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    std::cout << "Halfedge Table" << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    printf("Halfedge");
    printf("%*s", 6, "");
    printf("Next");
    printf("%*s", 9, "");
    printf("Prev");
    printf("%*s", 5, "");
    printf("Opposite");
    printf("%*s", 7, "");
    printf("Source\n");
    std::cout << std::string(table_width, '-') << std::endl;

    /* Print lines. */
    for (CGL::Mesh::Halfedge_index h : mesh.halfedges())
    {
        printf("%5d", h.idx());
        printf("%*s", 8, "");
        printf("%5d", mesh.next(h).idx());
        printf("%*s", 8, "");
        printf("%5d", mesh.prev(h).idx());
        printf("%*s", 8, "");
        printf("%5d", mesh.opposite(h).idx());
        printf("%*s", 8, "");
        printf("%5d", mesh.source(h).idx());
        std::cout << std::endl;
    }
    std::cout << std::string(table_width, '=') << std::endl;
}


void CGL::print_mesh_faces(CGL::Mesh &mesh)
{
    int table_width = 70;

    /* Print header. */
    std::cout << std::endl << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    std::cout << "Face Table" << std::endl;
    std::cout << std::string(table_width, '=') << std::endl;
    printf("Face");
    printf("%*s", 6, "");
    printf("Halfedge\n");
    std::cout << std::string(table_width, '-') << std::endl;

    /* Print lines. */
    for (CGL::Mesh::Face_index f : mesh.faces())
    {
        printf("%5d", f.idx());
        printf("%*s", 8, "");
        printf("%5d", mesh.halfedge(f).idx());
        std::cout << std::endl;
    }
    std::cout << std::string(table_width, '=') << std::endl;
}
