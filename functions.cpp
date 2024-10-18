#include <Types.h>
#include <CGAL/draw_polygon_2.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

typedef Polygon_2::Vertex_iterator VertexIterator;


int calculaAreaTriangulo(int *xList, int *yList)
{
	int sum1, sum2;
	sum1 = xList[0] * yList[1] + xList[1] * yList[2] + xList[2] * yList[0];
    	sum2 = xList[1] * yList[0] + xList[2] * yList[1] + xList[0] * yList[2];

    	return ((sum1 - sum2) / 2);
}
 
bool horario(CGL::Polygon2 P)
{	
	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};
	
	int xPos = 1;
	int yPos = 1;
	
	int count = 0;
	int i = 0;
	int soma = 0;
	
	for (VertexIterator vi = P.vertices_begin(); vi != P.vertices_end(); ++vi){
		x[xPos] = vi.x();
		y[yPos] = vi.y();
		
		count++;
		xPos++;
		yPos++;
		
		if (count == 2){
			soma = soma + calculaAreaTriangulo(x, y);
			count = 0;
			xPos = 1;
			yPos = 1;
		}
	}
	
	if (soma > 0) {
		return false;
	}
	else {
		return true;
	}
}

int* classificaVertices(CGL::Polygon2 P){

	std::vector<int> convexConcav;
	
	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};
	
	int xPos = 0;
	int yPos = 0;
	
	int count = 0;
	int i = 0;
	int aux = 1;
	int area = 0;
	
	bool sentido = horario(P);
	
	if(sentido == true) {
		aux = -1;
	}
	
	
	for (VertexIterator vi = P.vertices_begin(); vi != P.vertices_end(); ++vi){
		x[xPos] = vi.x();
		y[yPos] = vi.y();
		
		count++;
		xPos++;
		yPos++;
		
		if (count == 3){
			area = calculaAreaTriangulo(x, y);
			
			if (area*aux < 0) {
				convexConcav.push_back(0);
			}
			else {
				convexConcav.push_back(1);
			}
			
			for(i = 0; i<2; i++){
				x[i] = x[i+1];
				y[i] = y[i+1];
			}
			
			count = 2;
			xPos = 2;
			yPos = 2;
		}
	}
	
}

int main()
{

    std::cout << "DEHHEHEHEHEHEHEEH" << std::endl;

    return EXIT_SUCCESS;
}
