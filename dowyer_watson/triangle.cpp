#include "triangle.h"

std::vector<Triangle> triangulate(std::vector<cv::Point2f> vertices)
{
	std::vector<cv::Point2f> _vertices;
	std::vector<Triangle> _triangles;
	std::vector<Edge> _edges;

    // Store the vertices locally
	_vertices = vertices;

	// Determinate the super triangle
	float minX = vertices[0].x;
	float minY = vertices[0].y;
	float maxX = minX;
	float maxY = minY;

	for(std::size_t i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i].x < minX) minX = vertices[i].x;
		if (vertices[i].y < minY) minY = vertices[i].y;
		if (vertices[i].x > maxX) maxX = vertices[i].x;
		if (vertices[i].y > maxY) maxY = vertices[i].y;
	}

	const float dx = maxX - minX;
	const float dy = maxY - minY;
	const float deltaMax = std::max(dx, dy);
	const float midx = (minX + maxX) / 2;
	const float midy = (minY + maxY) / 2;

	const cv::Point2i p1(midx - 20 * deltaMax, midy - deltaMax);
	const cv::Point2i p2(midx, midy + 20 * deltaMax);
	const cv::Point2i p3(midx + 20 * deltaMax, midy - deltaMax);

	std::cout << "2d point size: " << vertices.size() << std::endl;
	std::cout << "maxX: " << maxX << ", minX: " << minX << std::endl;
	std::cout << "maxY: " << maxY << ", minY: " << minY << std::endl;
	std::cout << "p1 x: " << p1.x << ", y: " << p1.y << std::endl;
	std::cout << "p2 x: " << p2.x << ", y: " << p2.y << std::endl;
	std::cout << "p3 x: " << p3.x << ", y: " << p3.y << std::endl;

	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back(Triangle(p1, p2, p3));

	std::cout << "triangle size 1: " << _triangles.size() << std::endl;


	for(auto p = begin(vertices); p != end(vertices); p++)
	{
		std::vector<Edge> polygon;

		for(auto & t : _triangles)
		{
			if(t.circumcircleContains(*p))
			{
				t.isBad = true;
				polygon.push_back(Edge(*t.a, *t.b));
				polygon.push_back(Edge(*t.b, *t.c));
				polygon.push_back(Edge(*t.c, *t.a));
			}
		}

		_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [](Triangle &t){
			return t.isBad;
		}), end(_triangles));

		std::cout << "triangle size 2: " << _triangles.size() << std::endl;
		std::cout << "edge size 1: " << polygon.size() << std::endl;


		for(auto e1 = begin(polygon); e1 != end(polygon); ++e1)
		{
			for(auto e2 = e1 + 1; e2 != end(polygon); ++e2)
			{
				if(almost_equal(*e1, *e2))
				{
					e1->isBad = true;
					e2->isBad = true;
				}
			}
		}

		polygon.erase(std::remove_if(begin(polygon), end(polygon), [](Edge &e){
			return e.isBad;
		}), end(polygon));

		for(const auto e : polygon)
			_triangles.push_back(Triangle(*e.a, *e.b, *p));

		std::cout << "triangle size 3: " << _triangles.size() << std::endl;
		std::cout << "edge size 2: " << polygon.size() << std::endl;

	}

	_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [p1, p2, p3](Triangle &t){
		return t.vertexContains(p1) || t.vertexContains(p2) || t.vertexContains(p3);
	}), end(_triangles));

	for(const auto t : _triangles)
	{
		_edges.push_back(Edge(*t.a, *t.b));
		_edges.push_back(Edge(*t.b, *t.c));
		_edges.push_back(Edge(*t.c, *t.a));
	}

	std::cout << "triangle size 4: " << _triangles.size() << std::endl;

	return _triangles;

}