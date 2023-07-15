#include "include.h"

// For yaml file
int STEREO;
int Show_Depth;
int Preprocessing;
cv::Mat LeftCAM, RightCAM;
cv::Mat Left_K, Right_K;
std::string LeftImgTopic, RightImgTopic; 
double left_fx, left_fy, left_cx, left_cy;
double right_fx, right_fy, right_cx, right_cy;

void parameterSetting(std::string config)
{

    cv::FileStorage configSettings(config, cv::FileStorage::READ);
    if(!configSettings.isOpened())
        std::cerr << "ERROR: Wrong path to settings" << std::endl;

    STEREO = configSettings["stereo"];

    std::cout << "here" << std::endl;

    Preprocessing = configSettings["preprocessing"];



    configSettings["left_img_topic"] >> LeftImgTopic;
    configSettings["right_img_topic"] >> RightImgTopic;



    configSettings["imu_T_left"] >>  LeftCAM;
    configSettings["imu_T_right"] >> RightCAM;



    // Visualization
    Show_Depth = configSettings["show_depth_img"];

    left_fx = configSettings["left_fx"];
    left_fy = configSettings["left_fy"];
    left_cx = configSettings["left_cx"];
    left_cy = configSettings["left_cy"];

    right_fx = configSettings["right_fx"];
    right_fy = configSettings["right_fy"];
    right_cx = configSettings["right_cx"];
    right_cy = configSettings["right_cy"];

    // Compute Matrix K
    Left_K = (cv::Mat_<double>(3, 3) << left_fx, 0,       left_cx,
                                        0,       left_fy, left_cy,
                                        0,       0,       1);
    Right_K = (cv::Mat_<double>(3, 3) << right_fx,  0,        right_cx,
                                         0,         right_fy, right_cy,
                                         0,         0,        1);

    std::cout << "-------------------- [Left_K]  --------------------\n" << Left_K << std::endl;
    std::cout << "-------------------- [Right_K] --------------------\n" << Right_K << std::endl;
    std::cout << "---------------------------------------------------\n"; 

}