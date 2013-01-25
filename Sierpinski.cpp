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
		return foundResult->second; //if found in cache, return pre-computed randomized midpoint

	auto result = (a + b) / 2; //create midpoint
	result += randVector() * (length(a, b) * SCALE); //create random offset

	memo.insert(std::make_pair(std::make_pair(a, b), result)); //memoize
	memo.insert(std::make_pair(std::make_pair(b, a), result));

	return result;
}



/* Subdivides the triangle into four separate equilateral triangles and returns the result */
std::vector<Triangle> subdivideTriangle(const Triangle& triangle)
{
	/*		That's some very nice ASCII art you have there...
	
				  A
				  /\
				 /  \
			  c'/____\b'
			   /\    /\
			  /  \  /  \
			 /____\/____\
			B     a'     C
	*/

	auto cPrime = getMidpoint(triangle.A, triangle.B);
	auto aPrime = getMidpoint(triangle.B, triangle.C);
	auto bPrime = getMidpoint(triangle.C, triangle.A);

	return {
		{ triangle.A, cPrime, bPrime }, //top triangle
		{ cPrime, triangle.B, aPrime }, //bottom left triangle
		{ bPrime, aPrime, triangle.C }, //bottom right triangle
		{ cPrime, aPrime, bPrime }      //center triangle
	};
}



/* Creates the gasket */
std::vector<Triangle> createGasket(const Triangle& baseTriangle, int depth, const int& maxDepth)
{
	/*
	static std::vector<Triangle> triangles;

	if (depth >= maxDepth)
		return triangles;

	auto subTriangles = subdivideTriangle(baseTriangle);
	std::transform(std::begin(subTriangles), std::end(subTriangles), std::back_inserter(triangles),
		[&](const Triangle& t)
		{
			return createGasket(t, depth + 1, maxDepth);
		});
	*/
}



/* Returns the vertices that describe the on-screen shapes */
std::pair<int, std::vector<GLfloat>> getVertices()
{
	//subdivideTriangle({{1,0}, {0,1}, {0,0}});

	int coordinateSize = 2;

	std::vector<GLfloat> vertices = {
		 0.0,  0.8,
		-0.8, -0.8,
		 0.8, -0.8,
	};

	return std::make_pair(coordinateSize, vertices);
}