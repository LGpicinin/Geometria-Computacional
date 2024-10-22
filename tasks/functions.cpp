#include <Types.h>
#include <CGAL/draw_polygon_2.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

typedef CGL::Polygon2::Vertex_iterator VertexIterator;

int calculaAreaTriangulo(int *xList, int *yList)
{
	int sum1, sum2;
	sum1 = xList[0] * yList[1] + xList[1] * yList[2] + xList[2] * yList[0];
	sum2 = xList[1] * yList[0] + xList[2] * yList[1] + xList[0] * yList[2];

	return ((sum1 - sum2) / 2);
}

bool antiHorario(CGL::Polygon2 P)
{
	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};

	int xPos = 1;
	int yPos = 1;

	int xValue = 0;
	int yValue = 0;

	int count = 0;
	int i = 0;
	int soma = 0;

	for (VertexIterator it = P.vertices_begin(); it != P.vertices_end(); ++it)
	{
		std::cout << it[0][0] << ", " << it[0][1] << std::endl;

		x[xPos] = it[0][0];
		y[yPos] = it[0][1];

		count++;
		xPos++;
		yPos++;

		if (count == 2)
		{
			soma = soma + calculaAreaTriangulo(x, y);
			count = 1;
			xPos = 2;
			yPos = 2;
			x[1] = x[2];
			y[1] = y[2];
		}
	}

	if (soma > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void classificaVertices(CGL::Polygon2 P)
{

	std::vector<int> convexConcav;

	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};

	int xPos = 0;
	int yPos = 0;

	int count = 0;
	int testCount = 0;
	int i = 0;
	int aux = 1;
	int area = 0;

	bool sentido = antiHorario(P);

	if (sentido == false)
	{
		aux = -1;
	}

	for (VertexIterator it = P.vertices_begin(); it != P.vertices_end(); ++it)
	{

		std::cout << it[0][0] << ", " << it[0][1] << std::endl;

		x[xPos] = it[0][0];
		y[yPos] = it[0][1];

		count++;
		xPos++;
		yPos++;

		if (count == 3)
		{
			area = calculaAreaTriangulo(x, y);

			if (area * aux < 0)
			{
				// convexConcav.push_back(0);
				std::cout << x[1] << ", " << y[1] << " eh concavo " << std::endl;
			}
			else
			{
				// convexConcav.push_back(1);
				std::cout << x[1] << ", " << y[1] << " eh convexo " << std::endl;
			}

			for (i = 0; i < 2; i++)
			{
				x[i] = x[i + 1];
				y[i] = y[i + 1];
			}

			count = 2;
			xPos = 2;
			yPos = 2;
		}
	}

	count = 0;
	xPos = 2;
	yPos = 2;

	for (VertexIterator it = P.vertices_begin(); count != 2; ++it)
	{
		std::cout << it[0][0] << ", " << it[0][1] << std::endl;

		x[xPos] = it[0][0];
		y[yPos] = it[0][1];

		area = calculaAreaTriangulo(x, y);

		if (area * aux < 0)
		{
			// convexConcav.push_back(0);
			std::cout << x[1] << ", " << y[1] << " eh concavo " << std::endl;
		}
		else
		{
			// convexConcav.push_back(1);
			std::cout << x[1] << ", " << y[1] << " eh convexo " << std::endl;
		}

		for (i = 0; i < 2; i++)
		{
			x[i] = x[i + 1];
			y[i] = y[i + 1];
		}

		count++;
	}
}

int main()
{

	/* Simple polygon with random vertices. */
	CGL::Polygon2 P;

	bool antih = false;

	/* Create vertices. */
	int ns = 10;			  /* Number of vertices. */
	CGL::Random rng(-10, 10); /* "bounding box" for points. "*/
	do
	{
		P.clear();
		for (int i = 1; i <= ns; i++)
			P.push_back(CGL::Point2(rng.get_int(), rng.get_int()));
	} while (!P.is_simple());

	antih = antiHorario(P);

	if (antih)
	{
		std::cout << "Antihorário" << std::endl;
	}
	else
	{
		std::cout << "Horário" << std::endl;
	}

	classificaVertices(P);

	/* Draw points. */
	CGAL::draw(P);

	return EXIT_SUCCESS;
}
