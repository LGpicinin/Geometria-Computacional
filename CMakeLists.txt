cmake_minimum_required(VERSION 3.1)
cmake_policy(VERSION 3.1)

# Set the project name and version.
project(CGL VERSION 1.0)

# Find required packages.
find_package(CGAL REQUIRED Qt5)

# Build type for performance.
set(CMAKE_BUILD_TYPE Release)

# Set library directories.
set(CGL_SOURCE ${PROJECT_SOURCE_DIR}/src)
set(CGL_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(CGL_DRAW_SOURCE ${CGL_SOURCE}/Draw)
set(CGL_DRAW_INCLUDE ${CGL_INCLUDE}/Draw)

# Set examples directory.
set(CGL_EXAMPLES ${PROJECT_SOURCE_DIR}/examples)

# Tasks codes
set(CGL_TASKS ${PROJECT_SOURCE_DIR}/tasks)

add_executable(draw_points_2 ${CGL_EXAMPLES}/draw_points_2.cpp ${CGL_DRAW_SOURCE}/DrawPoints2.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(draw_points_2 PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(draw_points_2 PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(draw_vectors_3 ${CGL_EXAMPLES}/draw_vectors_3.cpp ${CGL_DRAW_SOURCE}/DrawVectors3.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(draw_vectors_3 PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(draw_vectors_3 PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(draw_segments_3 ${CGL_EXAMPLES}/draw_segments_3.cpp ${CGL_DRAW_SOURCE}/DrawSegments3.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(draw_segments_3 PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(draw_segments_3 PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(draw_polygon_2 ${CGL_EXAMPLES}/draw_polygon_2.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(draw_polygon_2 PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(draw_polygon_2 PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(draw_partition_2 ${CGL_EXAMPLES}/draw_partition_2.cpp ${CGL_SOURCE}/IO.cpp)
target_include_directories(draw_partition_2 PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(draw_partition_2 PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(halfedge_info ${CGL_EXAMPLES}/halfedge_info ${CGL_SOURCE}/IO.cpp)
target_include_directories(halfedge_info PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(halfedge_info PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(polygon_2_triangulate ${CGL_EXAMPLES}/polygon_2_triangulate.cpp ${CGL_SOURCE}/IO.cpp ${CGL_SOURCE}/Partition.cpp ${CGL_SOURCE}/Primitives.cpp ${CGL_DRAW_SOURCE}/DrawSegments3.cpp)
target_include_directories(polygon_2_triangulate PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(polygon_2_triangulate PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(polygon_2_create ${CGL_EXAMPLES}/polygon_2_create.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(polygon_2_create PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(polygon_2_create PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(draw_triangle ${CGL_TASKS}/draw_triangle.cpp)
target_include_directories(draw_triangle PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(draw_triangle PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(functions ${CGL_TASKS}/functions.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(functions PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(functions PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(startEndSplitMerge ${CGL_TASKS}/startEndSplitMerge.cpp ${CGL_SOURCE}/Random.cpp)
target_include_directories(startEndSplitMerge PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(startEndSplitMerge PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(polygon_2_triangulate_good ${CGL_TASKS}/polygon_2_triangulate_good.cpp ${CGL_SOURCE}/IO.cpp ${CGL_SOURCE}/Partition.cpp ${CGL_SOURCE}/Primitives.cpp ${CGL_DRAW_SOURCE}/DrawSegments3.cpp)
target_include_directories(polygon_2_triangulate_good PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(polygon_2_triangulate_good PUBLIC CGAL::CGAL_Basic_viewer)

add_executable(visible_region ${CGL_TASKS}/visible_region.cpp ${CGL_SOURCE}/IO.cpp ${CGL_SOURCE}/Partition.cpp ${CGL_SOURCE}/Primitives.cpp ${CGL_DRAW_SOURCE}/DrawSegments3.cpp)
target_include_directories(visible_region PRIVATE ${CGL_INCLUDE} ${CGL_DRAW_INCLUDE})
target_link_libraries(visible_region PUBLIC CGAL::CGAL_Basic_viewer)