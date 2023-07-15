#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <eigen3/Eigen/Dense>
#include "sensor_msgs/Image.h"
#include "sensor_msgs/CompressedImage.h"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using Vertex = cv::Point2f;
using Triangle = cv::Vec3i;
using Edge = cv::Vec2i;