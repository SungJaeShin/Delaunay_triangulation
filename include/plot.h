#include "include.h"

// Using External Code about Delaunary Triangulation
#include "../src/dowyer_watson/triangle.cpp"


// void drawDelaunaryTriangle(cv::Mat image, std::vector<cv::Vec3i> triangles, std::vector<cv::Vec3i> neighbors, std::vector<cv::Vec2i> edges, triangulateio info)
void drawDelaunaryTriangle(cv::Mat image, std::vector<Triangle> triangles)
{   
    // Check the triangle results
    if(triangles.size() == 0)
        return;

    cv::Mat drawDTimg = image.clone();
    std::vector<int> counts(triangles.size(), 0);

    /* ---------------------------------------------------------------------------------------- */
    // Draw All Triangle
    for(int i = 0; i < triangles.size(); i++)
    {        
        // // Get Triangle Indices
        // int point1_idx = triangles[i][0];
        // int point2_idx = triangles[i][1];
        // int point3_idx = triangles[i][2];

        // cv::Point2f point1;
        // point1.x = info.pointlist[point1_idx];
        // point1.y = info.pointlist[point1_idx + 1];
    
        // // Get Three Image Points      
        // cv::Point2f point2;
        // point2.x = info.pointlist[point2_idx];
        // point2.y = info.pointlist[point2_idx + 1];

        // cv::Point2f point3;
        // point3.x = info.pointlist[point3_idx];
        // point3.y = info.pointlist[point3_idx + 1];

        cv::Point2f point1 = *triangles[i].a;
        cv::Point2f point2 = *triangles[i].b;
        cv::Point2f point3 = *triangles[i].c;

        std::cout << "point 1: " << point1 << std::endl;
        std::cout << "point 2: " << point2 << std::endl;
        std::cout << "point 3: " << point3 << std::endl;

        cv::line(drawDTimg, point1, point2, cv::Scalar(255, 255, 0), 2);
        cv::line(drawDTimg, point2, point3, cv::Scalar(255, 255, 0), 2);
        cv::line(drawDTimg, point3, point1, cv::Scalar(255, 255, 0), 2);
    }

    cv::imshow("Delaunay Triangulation", drawDTimg);
    cv::waitKey(10);        
}