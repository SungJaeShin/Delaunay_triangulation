#include "plot.h"

// std::vector<cv::Vec3i> triangles_;
// std::vector<cv::Vec3i> neighbors_;
// std::vector<cv::Vec2i> edges_;

void gftt(cv::Mat image, std::vector<cv::KeyPoint> &img_kpt, std::vector<cv::Point2f> &img_p2f)
{
    /* cv::goodFeaturesToTrack(InputArray image, OutputArray corners, int maxCorners, double qualityLevel, double minDistance, InputArray mask=noArray(), int blockSize=3, bool useHarrisDetector=false, double k=0.04)
       [-] Reference Site: https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=pckbj123&logNo=100203116086 
       1) image: input image 
       2) corners: output corner point2f 
       3) maxcorners: return maximum corner points
       4) qualitylevel: minimum value for determination the corner (product maximum value of eigenvalue)
       5) mindistance: euclidean distance between two corners   
       6) mask: interest ROI to find corners 
       7) blocksize: mean size of block 
       8) useHarrisDetector: the flag to use harris corner detector 
       9) k: parameter of harris detector */
    cv::goodFeaturesToTrack(image, img_p2f, 300, 0.01, 30);

    // Convert cv::Point2f to cv::KeyPoint
    // [-] Reference Site: https://docs.opencv.org/4.x/d2/d29/classcv_1_1KeyPoint.html#:~:text=%E2%97%86-,convert(),-%5B2/2%5D
    cv::KeyPoint::convert(img_p2f, img_kpt);
}

void fast(cv::Mat image, std::vector<cv::KeyPoint> &img_kpt)
{
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create(500, 1.2, 8, 31, 0, 2, cv::ORB::FAST_SCORE, 31, 20);
    orb -> detect(image, img_kpt);
}

void delaunary(double time, int index, cv::Mat img, cv::Mat img2 = cv::Mat())
{
    /* ---------------------------------------------------------------------------------------- */ 
    // Initialization for 2D Keypoints and Descriptors 
    std::vector<cv::Point2f> left_img_2d_point, right_img_2d_point;
    std::vector<cv::KeyPoint> left_img_kpt, right_img_kpt;

    /* ---------------------------------------------------------------------------------------- */ 
    // Extract Features
    gftt(img, left_img_kpt, left_img_2d_point);
    if(!img2.empty())
        gftt(img2, right_img_kpt, right_img_2d_point);

    // // Visualization Keypoints
    // cv::Mat vizKeyPoints;
    // cv::drawKeypoints(img, left_img_kpt, vizKeyPoints);
    // cv::imshow( "Visualization KeyPoints", vizKeyPoints);
    // cv::waitKey(10);

    // /* ---------------------------------------------------------------------------------------- */
    // std::cout << "size of img 2d point: " << left_img_2d_point.size() << std::endl;
    std::vector<Triangle> triangle_ = triangulate(left_img_2d_point);
    // std::cout << "size of triangle: " << triangle_.size() << std::endl;

    // /* ---------------------------------------------------------------------------------------- */
    // // Initialization for 2D Delaunary Triangulation 
    // struct triangulateio input_dt, output_dt;
    // int tmp;

    // /* ---------------------------------------------------------------------------------------- */
    // // Setting input DT and output DT
    // // inputs
    // input_dt.numberofpoints = left_img_kpt.size();
    // input_dt.pointlist = (float*)malloc(input_dt.numberofpoints*2*sizeof(float)); // NOLINT
    // tmp = 0;
    // for (int32_t i = 0; i < left_img_kpt.size(); i++) {
    //     input_dt.pointlist[tmp++] = left_img_kpt[i].pt.x;
    //     input_dt.pointlist[tmp++] = left_img_kpt[i].pt.y;
    // }
    // input_dt.numberofpointattributes = 0;
    // input_dt.pointattributelist      = NULL;
    // input_dt.pointmarkerlist         = NULL;
    // input_dt.numberofsegments        = 0;
    // input_dt.numberofholes           = 0;
    // input_dt.numberofregions         = 0;
    // input_dt.regionlist              = NULL;

    // // outputs
    // output_dt.pointlist              = NULL;
    // output_dt.pointattributelist     = NULL;
    // output_dt.pointmarkerlist        = NULL;
    // output_dt.trianglelist           = NULL;
    // output_dt.triangleattributelist  = NULL;
    // output_dt.neighborlist           = NULL;
    // output_dt.segmentlist            = NULL;
    // output_dt.segmentmarkerlist      = NULL;
    // output_dt.edgelist               = NULL;
    // output_dt.edgemarkerlist         = NULL;

    // /* ---------------------------------------------------------------------------------------- */
    // // Delaunary Triangulation
    // // First triangulation 
    // triangulate("zneQB", &input_dt, &output_dt, NULL);
    // // free memory used for triangulation
    // free(input_dt.pointlist);

    // // put resulting triangles into vector tri
    // triangles_.resize(output_dt.numberoftriangles);
    // int p = 0;
    // for (int i = 0; i < output_dt.numberoftriangles; i++) {
    //     triangles_[i] = cv::Vec3i(output_dt.trianglelist[p],
    //                              output_dt.trianglelist[p+1],
    //                              output_dt.trianglelist[p+2]);
    //     p+=3;
    // }  

    // // put neighboring triangles into vector tri
    // neighbors_.resize(output_dt.numberoftriangles);
    // int q = 0;
    // for (int i = 0; i < output_dt.numberoftriangles; i++) {
    //     neighbors_[i] = cv::Vec3i(output_dt.neighborlist[q],
    //                               output_dt.neighborlist[q+1],
    //                               output_dt.neighborlist[q+2]);
    //     q+=3;
    // }

    // // put resulting edges into vector
    // edges_.resize(output_dt.numberofedges);
    // int r = 0;
    // for (int i = 0; i < output_dt.numberofedges; i++) {
    //     edges_[i] = cv::Vec2i(output_dt.edgelist[r], output_dt.edgelist[r+1]);
    //     r+=2;
    // }

    // ----- Output -----
    // Size of triangle & neighbor & edge -> same size triangle and neighbor / size of edge is bigger than others
    // ----- ------ -----

    // Visualization Delaunay Triangulation 
    // drawDelaunaryTriangle(img, triangles_, neighbors_, edges_, output_dt);
    drawDelaunaryTriangle(img, triangle_);

    // // free memory used for triangulation
    // free(output_dt.pointlist);
    // free(output_dt.trianglelist);
    // free(output_dt.edgelist);
    // free(output_dt.neighborlist);

    // output_dt.pointlist = NULL;
    // output_dt.trianglelist = NULL;
    // output_dt.edgelist = NULL;
    // output_dt.neighborlist = NULL;
}