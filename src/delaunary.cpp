
#include "parameter.h"
#include "utility.h"
#include "delaunary.h"

// Publisher related
int sequence = 0;
ros::Publisher pub_pano_img;

// For node cpp
std::mutex buffer, m_buf;
std::queue<sensor_msgs::ImageConstPtr> img0_buf;
std::queue<sensor_msgs::ImageConstPtr> img1_buf;

void left_callback(const sensor_msgs::ImageConstPtr &image_msg)
{
    buffer.lock();
    img0_buf.push(image_msg);
    buffer.unlock();
}

void right_callback(const sensor_msgs::ImageConstPtr &image_msg)
{
    buffer.lock();
    img1_buf.push(image_msg);
    buffer.unlock();
}

void sync_process()
{
    while(1)
	{
		sequence++;

        if(STEREO)
        {
            cv::Mat image0, image1;
            std_msgs::Header header;
            double time = 0;
            m_buf.lock();
            if (!img0_buf.empty() && !img1_buf.empty())
            {
                double time0 = img0_buf.front()->header.stamp.toSec();
                double time1 = img1_buf.front()->header.stamp.toSec();
                // 0.003s sync tolerance
                if(time0 < time1 - 0.003)
                {
                    img0_buf.pop();
                    printf("throw img0\n");
                }
                else if(time0 > time1 + 0.003)
                {
                    img1_buf.pop();
                    printf("throw img1\n");
                }
                else
                {
                    time = img0_buf.front()->header.stamp.toSec();
                    header = img0_buf.front()->header;
                    image0 = sensorMsg2cvMat(img0_buf.front());
                    img0_buf.pop();
                    image1 = sensorMsg2cvMat(img1_buf.front());
                    img1_buf.pop();
                    //printf("find img0 and img1\n");
                }
            }
            m_buf.unlock();
            if(!image0.empty())
                delaunary(time, sequence, image0, image1);
        }
        else
        {
            cv::Mat image;
            std_msgs::Header header;
            double time = 0;
            m_buf.lock();
            if(!img0_buf.empty())
            {
                time = img0_buf.front()->header.stamp.toSec();
                header = img0_buf.front()->header;
                image = sensorMsg2cvMat(img0_buf.front());
                img0_buf.pop();
            }
            m_buf.unlock();
            if(!image.empty())
                delaunary(time, sequence, image);
        }

        std::chrono::milliseconds dura(2);
        std::this_thread::sleep_for(dura);
    
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "delaunary_triangulation_node");
    ros::NodeHandle nh;
    
    if(argc != 2)
    {
        std::cout << "\033[1;32m please input: rosrun Delaunary_triangulation delaunary_triangulation_node [config file] \033[0m" << std::endl;
        return 0;
    }

    std::string config = argv[1];
    parameterSetting(config);

    ros::Subscriber sub_left_img =  nh.subscribe(LeftImgTopic,  1000, left_callback);
    ros::Subscriber sub_right_img;
    if(STEREO)
        sub_right_img = nh.subscribe(RightImgTopic, 1000, right_callback);
    
    std::thread sync_thread{sync_process};
    ros::spin();

    return 0;
}