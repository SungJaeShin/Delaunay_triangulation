#include "include.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <limits>
#include <type_traits>

bool almost_equal(cv::Point2f x, cv::Point2f y, int ulp=2)
{
    if(fabsf(x.x-y.x) <= std::numeric_limits<float>::epsilon() * fabsf(x.x+y.x) * static_cast<float>(ulp))
        return true;
    if(fabsf(x.x-y.x) < std::numeric_limits<float>::min())
        return true;
    if(fabsf(x.y-y.y) <= std::numeric_limits<float>::epsilon() * fabsf(x.y+y.y) * static_cast<float>(ulp))
        return true;
    if(fabsf(x.y-y.y) < std::numeric_limits<float>::min())
        return true;       

    return false;
}

struct Triangle
{
    const cv::Point2f *a;
    const cv::Point2f *b;
    const cv::Point2f *c;
	bool isBad = false;

    // Constructor
    Triangle() = default;
    // Constructor overloading 
    Triangle(const cv::Point2f &v1, const cv::Point2f &v2, const cv::Point2f &v3);

    // Check containing circumcircle 
    bool circumcircleContains(const cv::Point2f &v);

    // Check containing isBad false flag
    bool vertexContains(const cv::Point2f &v);
};

Triangle::Triangle(const cv::Point2f &v1, const cv::Point2f &v2, const cv::Point2f &v3)
    : a(&v1), b(&v2), c(&v3), isBad(false) {}

bool Triangle::circumcircleContains(const cv::Point2f &v)
{
    float ax = a -> x;
    float ay = a -> y;
    float bx = b -> x;
    float by = b -> y;
    float cx = c -> x;
    float cy = c -> y;
    
    float l2_a = (ax * ax) + (ay * ay);
    float l2_b = (bx * bx) + (by * by);
    float l2_c = (cx * cx) + (cy * cy);

    // Ref find circum point !!
    // https://hiddenbeginner.github.io/python/mathematicalprogramming/2019/08/29/%ED%8C%8C%EC%9D%B4%EC%8D%AC%EC%9C%BC%EB%A1%9C_%EC%A2%8C%ED%91%9C%EA%B3%B5%EA%B0%84%EC%97%90_%EC%A3%BC%EC%96%B4%EC%A7%84_%EC%82%BC%EA%B0%81%ED%98%95%EC%97%90_%EC%99%B8%EC%A0%91%ED%95%98%EB%8A%94_%EC%9B%90%EC%9D%98_%EC%A4%91%EC%8B%AC_%EA%B5%AC%ED%95%98%EA%B8%B0.html
    const float circum_x = (l2_a * (cy - by) + l2_b * (ay - cy) + l2_c * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
    const float circum_y = (l2_a * (cx - bx) + l2_b * (ax - cx) + l2_c * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

    cv::Point2f circum(circum_x / 2, circum_y / 2);
    const float circum_radius = ((ax - circum.x) * (ax - circum.x)) + ((ay - circum.y) * (ay - circum.y));
    const float distance = ((v.x - circum.x) * (v.x - circum.x)) + ((v.y - circum.y) * (v.y - circum.y));
	return distance <= circum_radius;
}

bool Triangle::vertexContains(const cv::Point2f &v)
{
	// return p1 == v || p2 == v || p3 == v;
	return almost_equal(*a, v) || almost_equal(*b, v) || almost_equal(*c, v);
}

// Edge Information
struct Edge
{
    const cv::Point2f *a;
    const cv::Point2f *b;
    bool isBad = false;

    // Constructor
    Edge() = default;
    // Constructor overloading 
    Edge(const cv::Point2f &v1, const cv::Point2f &v2);
};

Edge::Edge(const cv::Point2f &v1, const cv::Point2f &v2)
    : a(&v1), b(&v2), isBad(false) {}

bool almost_equal(const Edge &e1, const Edge &e2)
{
	return	(almost_equal(*e1.a, *e2.a) && almost_equal(*e1.b, *e2.b)) ||
			(almost_equal(*e1.a, *e2.b) && almost_equal(*e1.b, *e2.a));
}