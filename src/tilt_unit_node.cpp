#include <stdlib.h>
#include <iostream>

#include <ros/ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/String.h>
#include <tf/transform_broadcaster.h>

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

int main(int argc, char* argv[])
{
    ros::init(argc,argv,"tilt_controller");
    ros::NodeHandle nh;

    ros::Publisher angle_pub = nh.advertise<std_msgs::UInt16>("/servo",1000,true);
    ros::Publisher sweep_pub = nh.advertise<std_msgs::String>("/sweep",1000);
    static tf::TransformBroadcaster br;

    ros::Rate loop_rate(30);

    int angle = 11;
    int increment = 1;

    std_msgs::String string_msg;
    string_msg.data = "sweep";

    ROS_INFO("%s","Starting tilt_controller node...");

    while(ros::ok())
    {
        std_msgs::UInt16 angle_msg;
        angle_msg.data = angle;

        tf::Transform transform;
        transform.setRotation(tf::Quaternion(tf::Vector3(-1,0,0),tfScalar(degreesToRadians(angle))));

        angle_pub.publish(angle_msg);
        br.sendTransform(tf::StampedTransform(transform,ros::Time::now(),"base_link","laser"));

        ros::spinOnce();

        if(angle == 170)
        {
            increment = -1;
            sweep_pub.publish(string_msg);
        }

        if(angle == 10)
        {
            increment = 1;
            sweep_pub.publish(string_msg);
        }

        angle += increment;

        loop_rate.sleep();
    }

    return 0;
}
