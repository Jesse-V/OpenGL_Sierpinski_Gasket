#include <vector>
#include <map>
#include <random> //for Mersenne Twister RNG
#include <cmath> //for sqrt, pow, and abs
#include <algorithm> //for transform

#include "Triangle.struct"

const int SCALE = 0.1;


/* Produces a vector with the coordinates randomized and in the range of (0, 1) */
Point randVector()
{
	std::mt19937 generator; //Mersenne Twister PRNG. WAY better randomness!
	std::uniform_real_distribution<float> distribution(0, 1);

	Point pt;
	pt.x = distribution(generator);
	pt.y = distribution(generator);
	return pt;
}



/* Returns the distance between the two given Points */
float length(const Point& a, const Point& b)
{
	return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2)); //sqrt of a^2 + b^2
}



/* Returns the midpoint between the two points. Uses memoizing.. */
Point getMidpoint(const Point& a, const Point& b)
{
	static std::map<std::pair<Point, Point>, Point> memo;

	auto foundResult = memo.find(std::make_pair(a, b));
	if (foundResult != memo.end())
		return foundResult->second; //if found in cache, return pre-computed midpoint
	
	auto result = (a + b) / 2; //create midpoint
	//result += randVector() * (length(a, b) * SCALE); //create random offset

	memo.insert(std::make_pair(std::make_pair(a, b), result)); //memoize
	memo.insert(std::make_pair(std::make_pair(b, a), result));

	return result;
}



/* Subdivides the triangle into four separate equilateral triangles and returns the result */
std::vector<Triangle> subdivideTriangle(const Triangle& triangle)
{
	/*	
				  A
				  /\
				 /  \
			  c'/____\b'
			   /\    /\
			  /  \  /  \
			 /____\/____\
			B     a'     C
	*/

	auto aPrime = getMidpoint(triangle.B, triangle.C);
	auto bPrime = getMidpoint(triangle.C, triangle.A);
	auto cPrime = getMidpoint(triangle.A, triangle.B);

	return {
		{ triangle.A, cPrime, bPrime }, //top triangle
		{ cPrime, triangle.B, aPrime }, //bottom left triangle
		{ bPrime, aPrime, triangle.C }, //bottom right triangle
		{ cPrime, aPrime, bPrime }      //center triangle
	};
}



/* Creates the gasket */
void createGasket(std::vector<Triangle>& gasketTriangles, const Triangle& baseTriangle, int depth, const int& maxDepth)
{
	if (depth >= maxDepth)
	{
		gasketTriangles.push_back(baseTriangle);
		return;
	}
		

	auto subTriangles = subdivideTriangle(baseTriangle);
	for_each (subTriangles.begin(), subTriangles.end(), 
		[&](const Triangle& subTri)
		{
			createGasket(gasketTriangles, subTri, depth + 1, maxDepth);
		});
}



/* Creates the Sierpinksi Gasket. Uses memoizing to create the gasket only once. */
std::vector<Triangle> getGasket(const int& maxDepth)
{
	static std::vector<Triangle> gasketTriangles;

	if (gasketTriangles.empty())
	{
		Triangle baseTriangle = {{0, 1}, {-1, -1}, {1, -1}};
		createGasket(gasketTriangles, baseTriangle, 0, maxDepth);
		return gasketTriangles;
	}
	else
		return gasketTriangles;
}



/* Returns the vertices that describe the on-screen shapes */
std::pair<int, std::vector<GLfloat>> getVertices()
{
	int coordinateSize = 2;

	auto gasketTriangles = getGasket(3);
	std::cout << "Triangle count: " << gasketTriangles.size() << std::endl;

	std::vector<GLfloat> vertices;
	for_each (gasketTriangles.begin(), gasketTriangles.end(), 
		[&](const Triangle& gasketTriangle)
		{
			vertices.push_back(gasketTriangle.A.x);
			vertices.push_back(gasketTriangle.A.y);
			vertices.push_back(gasketTriangle.B.x);
			vertices.push_back(gasketTriangle.B.y);
			vertices.push_back(gasketTriangle.C.x);
			vertices.push_back(gasketTriangle.C.y);

			//std::cout << "A: " << gasketTriangle.A.x << ", " << gasketTriangle.A.y << std::endl;
			//std::cout << "B: " <<gasketTriangle.B.x << ", " << gasketTriangle.B.y << std::endl;
			//std::cout << "C: " <<gasketTriangle.C.x << ", " << gasketTriangle.C.y << std::endl << std::endl;
		});
	
	std::cout << "Coord count: " << vertices.size() << std::endl;

	return std::make_pair(coordinateSize, vertices);
}
