#include <Types.h>
#include <CGAL/draw_polygon_2.h>
#include <Random.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

typedef CGL::Polygon2::Vertex_iterator VertexIterator;

typedef struct
{
	int x;
	int y;
	bool convex;
	bool ear;

} PointAngle;

float calculaAreaTriangulo(int *xList, int *yList)
{
	float sum1, sum2;
	sum1 = xList[0] * yList[1] + xList[1] * yList[2] + xList[2] * yList[0];
	sum2 = xList[1] * yList[0] + xList[2] * yList[1] + xList[0] * yList[2];

	return ((sum1 - sum2) / 2);
}

int left(float area)
{
	if (area > 0)
	{
		return 1;
	}
	else if (area < 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

bool colinear(float area, int *x, int *y)
{
	if (area == 0)
	{
		if ((x[2] >= x[0] && x[2] <= x[1]) || (x[2] >= x[1] && x[2] <= x[0]))
		{
			if ((y[2] >= y[0] && y[2] <= y[1]) || (y[2] >= y[1] && y[2] <= y[0]))
			{
				return true;
			}
		}
	}
	return false;
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
	float soma = 0;

	for (VertexIterator it = P.vertices_begin(); it != P.vertices_end(); ++it)
	{

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

std::vector<PointAngle> classificaVertices(CGL::Polygon2 P)
{

	std::vector<PointAngle> points;

	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};

	int xPos = 0;
	int yPos = 0;

	int count = 0;
	int testCount = 0;
	int i = 0;
	float aux = 1;
	float area = 0;

	bool sentido = antiHorario(P);

	if (sentido == false)
	{
		aux = -1;
	}

	for (VertexIterator it = P.vertices_begin(); it != P.vertices_end(); ++it)
	{
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
				points.push_back({x[1], y[1], false, false});
			}
			else
			{
				points.push_back({x[1], y[1], true, false});
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
		x[xPos] = it[0][0];
		y[yPos] = it[0][1];

		area = calculaAreaTriangulo(x, y);

		if (area * aux < 0)
		{
			points.push_back({x[1], y[1], false, false});
		}
		else
		{
			points.push_back({x[1], y[1], true, false});
		}

		for (i = 0; i < 2; i++)
		{
			x[i] = x[i + 1];
			y[i] = y[i + 1];
		}

		count++;
	}
	return points;
}

std::vector<PointAngle> achaOrelha(CGL::Polygon2 P, bool antih)
{
	std::vector<PointAngle> points;

	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};
	int i, j, jMenos, jMais, length, a, b, count;
	float area1, area2, area3, area4;
	int left1, left2, left3, left4, aux;
	bool colinea;

	// verifica orientação polígono
	aux = 1;
	if (antih == false)
	{
		aux = -1;
	}

	// classifica vértices em convexos e concâvos
	points = classificaVertices(P);
	length = points.size();

	for (i = 0; i < length; i++)
	{
		if (points[i].convex == false) // se ponto for concâvo, não é orelha
		{
			std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por ser côncavo" << std::endl;
			points[i].ear = false;
			continue;
		}
		else
		{
			jMais = i + 1;
			jMenos = i - 1;

			if (jMenos < 0)
			{
				jMenos = length - 1;
			}

			x[0] = points[jMenos].x;
			x[1] = points[jMais % length].x;
			x[2] = points[i].x;
			y[0] = points[jMenos].y;
			y[1] = points[jMais % length].y;
			y[2] = points[i].y;

			area1 = calculaAreaTriangulo(x, y);
			if (area1 == 0) // se ponto for colinear a j- e j+, não é orelha
			{
				std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por ser colinear a j- e j+" << std::endl;
				points[i].ear = false;
				continue;
			}

			if (points[jMais % length].convex == true) // analisa aresta composta por j+ e 1 + j+
			{
				a = (jMais % length) + 1;
				x[2] = points[a].x;
				y[2] = points[a].y;
				area1 = calculaAreaTriangulo(x, y);
				colinea = colinear(area1, x, y);
				if (colinea == true || (area1 * aux) < 0)
				{
					std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: ";
					if (colinea)
					{
						std::cout << "colinear nos pontos " << x << ", " << y << std::endl;
					}
					else
					{
						std::cout << "left1" << std::endl;
					}
					points[i].ear = false;
					continue;
				}
			}

			if (points[jMenos].convex == true) // analisa aresta composta por j- e j- - 1
			{
				a = jMenos - 1;
				x[2] = points[a].x;
				y[2] = points[a].y;
				area1 = calculaAreaTriangulo(x, y);
				colinea = colinear(area1, x, y);
				if (colinea == true || (area1 * aux) < 0)
				{
					std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: ";
					if (colinea)
					{
						std::cout << "colinear nos pontos " << x << ", " << y << std::endl;
					}
					else
					{
						std::cout << "left2" << std::endl;
					}
					points[i].ear = false;
					continue;
				}
			}

			a = jMais + 1;
			b = a + 1;

			for (count = 0; count < length - 4; count++) // analisa todas as outras arestas
			{
				x[0] = points[jMenos].x;
				x[1] = points[jMais % length].x;
				x[2] = points[a % length].x;
				y[0] = points[jMenos].y;
				y[1] = points[jMais % length].y;
				y[2] = points[a % length].y;
				area1 = calculaAreaTriangulo(x, y);
				if (colinear(area1, x, y) == true)
				{
					std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: " << "colinear nos pontos " << x << ", " << y << std::endl;
					points[i].ear = false;
					break;
				}
				x[2] = points[b % length].x;
				y[2] = points[b % length].y;
				area2 = calculaAreaTriangulo(x, y);
				if (colinear(area2, x, y) == true)
				{
					std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: " << "colinear nos pontos " << x << ", " << y << std::endl;
					points[i].ear = false;
					break;
				}
				x[0] = points[a % length].x;
				x[1] = points[b % length].x;
				x[2] = points[jMenos].x;
				y[0] = points[a % length].y;
				y[1] = points[b % length].y;
				y[2] = points[jMenos].y;
				area3 = calculaAreaTriangulo(x, y);
				if (colinear(area3, x, y) == true)
				{
					std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: " << "colinear nos pontos " << x << ", " << y << std::endl;
					points[i].ear = false;
					break;
				}
				x[2] = points[jMais % length].x;
				y[2] = points[jMais % length].y;
				area4 = calculaAreaTriangulo(x, y);
				if (colinear(area4, x, y) == true)
				{
					std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: " << "colinear nos pontos " << x << ", " << y << std::endl;
					points[i].ear = false;
					break;
				}

				left1 = left(area1);
				left2 = left(area2);
				left3 = left(area3);
				left4 = left(area4);

				if ((left1 != left2) && (left3 != left4))
				{
					if ((left1 != -1 && left2 != -1) || (left3 != -1 && left4 != -1))
					{
						std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eliminado por: " << "left" << std::endl;
						points[i].ear = false;
						break;
					}
				}

				a++;
				b++;
			}

			if (count == length - 4) // se todas as arestas foram testadas com sucesso, ponto é orelha
			{
				points[i].ear = true;
			}
		}
	}

	return points;
}

int main()
{
	CGL::Polygon2 P;
	std::vector<PointAngle> points;
	bool antih = false;
	int i = 0;
	int option = 0;

	int ns = 10;
	CGL::Random rng(-10, 10);
	do
	{
		P.clear();
		for (int i = 1; i <= ns; i++)
			P.push_back(CGL::Point2(rng.get_int(), rng.get_int()));
	} while (!P.is_simple());

	std::cout << "Escolha uma opção:\n1 - Verificar orientação\n2 - Classificar vértices em convexo e côncavo\n3 - Classificar vértices em orelha e não orelha\n";
	std::cin >> option;

	switch (option)
	{
	case 1:
		antih = antiHorario(P);
		if (antih)
		{
			std::cout << "Antihorário" << std::endl;
		}
		else
		{
			std::cout << "Horário" << std::endl;
		}
		break;

	case 2:

		points = classificaVertices(P);

		if (points.size() != ns)
		{
			std::cout << "ERRO" << std::endl;
			return 0;
		}

		for (i = 0; i < ns; i++)
		{
			std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") eh ";

			if (points[i].convex == true)
			{
				std::cout << "convexo" << std::endl;
			}
			else
			{
				std::cout << "côncavo" << std::endl;
			}
		}

		break;

	case 3:
		antih = antiHorario(P);
		points = achaOrelha(P, antih);

		if (points.size() != ns)
		{
			std::cout << "ERRO" << std::endl;
			return 0;
		}

		for (i = 0; i < ns; i++)
		{
			std::cout << "Ponto (" << points[i].x << ", " << points[i].y << ") ";

			if (points[i].ear == 1)
			{
				std::cout << "é orelha" << std::endl;
			}
			else
			{
				std::cout << "não é orelha" << std::endl;
			}
		}
		break;

	default:
		break;
	}

	CGAL::draw(P);

	return EXIT_SUCCESS;
}
