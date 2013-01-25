
#include <vector>
#include <map>
#include <random> //for Mersenne Twister RNG
#include <cmath> //for calculating distance between two Points
#include <math.h> //for some reason cmath's abs function rounds floats, idk why
#include <algorithm> //for iteration

#include "Triangle.struct"

const float RANDOMNESS_SCALE = 0.1f;
const float RESOLUTION = 5; //8 is a good max

const float DIMENSIONS = 2;

std::mt19937 mersenneTwister; //Mersenne Twister PRNG. WAY better randomness!
std::uniform_real_distribution<float> randomFloat(0, 1);


/* Produces a vector with the coordinates randomized and in the range of (0, 1) */
Point randVector()
{
	return { //return random coordinates, each in the range of (0, 1)
		randomFloat(mersenneTwister),
		randomFloat(mersenneTwister)
	};
}



/* Returns the distance between the two given Points */
float length(const Point& a, const Point& b)
{
	return sqrt(pow(fabs(a.x - b.x), 2) + pow(fabs(a.y - b.y), 2)); //sqrt of a^2 + b^2
}



/*	Returns the midpoint between the two points with a randomized offset.
	Uses memoizing to avoid tearing across repeated lookups. */
Point getMidpoint(const Point& a, const Point& b)
{
	static std::map<std::pair<Point, Point>, Point> memo;

	auto foundResult = memo.find(std::make_pair(a, b));
	if (foundResult != memo.end())
		return foundResult->second; //if found in cache, return pre-computed midpoint
	
	auto result = (a + b) / 2; //create midpoint
	result += randVector() * length(a, b) * RANDOMNESS_SCALE; //add random offset

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
	if (depth >= maxDepth) //base case
	{
		gasketTriangles.push_back(baseTriangle);
		return;
	}
		
	//subdivide and recurse
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
		Triangle baseTriangle = {{0.1, 0.9}, {-0.9, -0.9}, {0.9, -0.9}};
		createGasket(gasketTriangles, baseTriangle, 0, maxDepth);
		return gasketTriangles;
	}
	else
		return gasketTriangles;
}



/* Adds the line formed between the two Points to the vector of vertices */
void appendLine(std::vector<GLfloat>& vertices, const Point& a, const Point& b)
{
	vertices.push_back(a.x);
	vertices.push_back(a.y);
	//vertices.push_back(a.z);

	vertices.push_back(b.x);
	vertices.push_back(b.y);
	//vertices.push_back(a.z);
}



/* Returns the vertices that describe the on-screen shapes */
std::pair<int, std::vector<GLfloat>> getVertices()
{
	auto gasket = getGasket(RESOLUTION);
	std::cout << "Triangle count: " << gasket.size() << std::endl;

	std::vector<GLfloat> vertices;
	for_each (gasket.begin(), gasket.end(), //iterate through all the gasket's triangles
		[&](const Triangle& triangle)
		{
			appendLine(vertices, triangle.A, triangle.B); //express each triangle as three lines
			appendLine(vertices, triangle.B, triangle.C);
			appendLine(vertices, triangle.C, triangle.A);

			//std::cout << "A: " << gasketTriangle.A.x << ", " << gasketTriangle.A.y << std::endl;
			//std::cout << "B: " <<gasketTriangle.B.x << ", " << gasketTriangle.B.y << std::endl;
			//std::cout << "C: " <<gasketTriangle.C.x << ", " << gasketTriangle.C.y << std::endl << std::endl;
		});
	
	std::cout << "Coord count: " << vertices.size() << std::endl;

	return std::make_pair(DIMENSIONS, vertices);
}
