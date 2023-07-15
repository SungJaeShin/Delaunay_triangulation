#include "include.h"

// Using External Code about Delaunary Triangulation
#include "../src/external/triangle.h"


void drawDelaunaryTriangle(cv::Mat image, std::vector<cv::Vec3i> triangles, std::vector<cv::Vec2i> edges, triangulateio info)
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
        // Get Triangle Indices
        int point1_idx = triangles[i][0];
        int point2_idx = triangles[i][1];
        int point3_idx = triangles[i][2];

        if(counts[point1_idx] == 0)
        {
            cv::Point2f point1;
            point1.x = info.pointlist[point1_idx];
            point1.y = info.pointlist[point1_idx + 1];
        }


        // Get Three Image Points
        

        cv::Point2f point2;
        point2.x = info.pointlist[point2_idx];
        point2.y = info.pointlist[point2_idx + 1];

        cv::Point2f point3;
        point3.x = info.pointlist[point3_idx];
        point3.y = info.pointlist[point3_idx + 1];

        cv::line(drawDTimg, point1, point2, cv::Scalar(255, 255, 0), 2);
        cv::line(drawDTimg, point2, point3, cv::Scalar(255, 255, 0), 2);
        cv::line(drawDTimg, point3, point1, cv::Scalar(255, 255, 0), 2);
    }

    cv::imshow("Delaunay Triangulation", drawDTimg);
    cv::waitKey(10);        
}