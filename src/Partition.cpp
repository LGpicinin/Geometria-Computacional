/**
 * @file Partiton.cpp  
 * @brief Planar maps partition functions.
 *
 * Implements functions to partition a planar map.
 *
 * \author Ricardo Dutra da Silva
 */


#include <Partition.h>
#include <DrawSegments3.h>
#include <CGAL/draw_surface_mesh.h>


bool CGL::non_intersecting(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2)
{
    // Candidate segment u and v endpoints.
    CGL::Point3 u1 = polygon.point(polygon.target(h1));
    CGL::Point3 u2 = polygon.point(polygon.target(h2));

    // Check all non-adjacent edges to uv.
    CGL::Mesh::Halfedge_index h_init = h1;
    CGL::Mesh::Halfedge_index h = h_init;
    do 
    {
        // An edge not adjacent to u = target(h1) or v = target(h2).
        if ((h != h1) && (h != polygon.next(h1)) && (h != h2) && (h != polygon.next(h2)))
        {
            CGL::Point3 v1 = polygon.point(polygon.source(h));
            CGL::Point3 v2 = polygon.point(polygon.target(h));

            if (CGL::intersect(u1, u2, v1, v2))             
                return false;
        }
        
        h = polygon.next(h);
    }
    while (h_init != h);

    return true;
}


bool CGL::in_cone(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2)
{
    // Vertex u = target(h1)
    CGL::Point3 u     = polygon.point(polygon.target(h1)); 
    CGL::Point3 uprev = polygon.point(polygon.source(h1)); 
    CGL::Point3 unext = polygon.point(polygon.target(polygon.next(h1))); 

    // Vertex v = target(h2)
    CGL::Point3 v     = polygon.point(polygon.target(h2)); 
    CGL::Point3 vprev = polygon.point(polygon.source(h2)); 
    CGL::Point3 vnext = polygon.point(polygon.target(polygon.next(h2))); 
    
    // Check whether uv is locally interior in uprev, u, unext neighborhood.
    // u is convext
    if (LeftOn(uprev, u, unext))
    {
        if (!Left(u, v, uprev) || !Left(v, u, unext))
            return false;
    }
    else // u is reflex
    {
        if (LeftOn(u, v, unext) && LeftOn(v, u, uprev))
            return false;
    }

    
    // Check whether vu is locally interior in vprev, v, vnext neighborhood.
    // v is convext
    if (LeftOn(vprev, v, vnext))
    {
        if (!LeftOn(v, u, vprev) || !LeftOn(u, v, vnext))
            return false;
    }
    else // v is reflex
    {
        if (LeftOn(v, u, vnext) && LeftOn(u, v, vprev))
            return false;
    }

    return true;
         
}


bool CGL::is_diagonal(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2)
{
    if (!CGL::in_cone(polygon, h1, h2))
        return false;

    if (!CGL::non_intersecting(polygon, h1, h2))
        return false;

    return true;
}


void draw_candidate(CGL::Mesh &polygon, CGL::Mesh::Halfedge_index h1, CGL::Mesh::Halfedge_index h2)
{
    std::vector<CGL::Segment3> segs;

    CGL::Mesh::Halfedge_index h_init = h1;
    CGL::Mesh::Halfedge_index h = h1;

    do
    {
        CGL::Segment3 s(polygon.point(polygon.source(h)), polygon.point(polygon.target(h)));
        segs.push_back(s);

        h = polygon.next(h);
    }
    while (h != h_init);
        
    CGL::Segment3 s(polygon.point(polygon.target(h1)), polygon.point(polygon.target(h2)));
    segs.push_back(s);

    CGL::draw(segs);

}


void CGL::polygon_2_triangulate_naive(CGL::Mesh &polygon)    
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        /* Iterate over faces in partition. */     
        for (CGL::Mesh::Face_index f : polygon.faces())
        {
            /* Check pairs of halfedges/vertices for a diagonal. */
            bool found_diagonal = false;
            CGL::Mesh::Halfedge_index d1;
            CGL::Mesh::Halfedge_index d2;

            CGL::Mesh::Halfedge_index h1_init = polygon.halfedge(f);
            CGL::Mesh::Halfedge_index h1 = h1_init;
            do
            {
                // Avoid adjacent halfedges/vertices.
                CGL::Mesh::Halfedge_index h2_end = polygon.prev(h1);
                CGL::Mesh::Halfedge_index h2 = polygon.next(polygon.next(h1));

                do
                {
                    if (CGL::is_diagonal(polygon, h1, h2))
                    {
                        found_diagonal = true;
                        d1 = h1;
                        d2 = h2;
                    }
                    else
                    {
                        h2 = polygon.next(h2);
                    }
                }
                while ((h2 != h2_end) && (!found_diagonal));

                if (!found_diagonal)
                    h1 = polygon.next(h1);
            }
            while ((h1 != h1_init) && (!found_diagonal));

            /* Insert diagonal.  */
            if (found_diagonal)
            {
                CGAL::Euler::split_face(d1, d2, polygon);
                changed = true;
                //CGAL::draw(polygon);
            }
        }
    }

}


bool CGL::mesh_is_adjacent(CGL::Mesh &mesh, CGL::Mesh::Vertex_index u, CGL::Mesh::Vertex_index v)
{
    CGL::Mesh::Halfedge_index h_init = mesh.opposite(mesh.halfedge(u));
    CGL::Mesh::Halfedge_index h = h_init;

    do 
    {
        if (mesh.target(h) == v)
            return true;
        h = mesh.opposite(mesh.prev(h));
    }
    while (h_init != h);

    return false;
}

bool CGL::mesh_in_cone(CGL::Mesh &mesh, CGL::Mesh::Halfedge_index u, CGL::Mesh::Halfedge_index v)
{
    /* Check u */    

    return true;

}
