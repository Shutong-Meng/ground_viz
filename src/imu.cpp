#include <cstdio>
#include <vector>
#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseStamped.h>
#include <fstream>

using namespace std;

ros::Publisher pub_imu_pose;
nav_msgs::Path path;
float vx,vy,vz=0;
float sx,sy,sz=0;	

void preintegration(const sensor_msgs::Imu::ConstPtr& msg)
{
    //ROS_INFO("odom callback!");
   

    geometry_msgs::PoseStamped point_stamped;
    point_stamped.header.stamp = msg->header.stamp;
    vx=vx+0.005*msg->linear_acceleration.x;
    vy=vy+0.005*msg->linear_acceleration.y;
    vz=vz+0.005*msg->linear_acceleration.z;
    sx=sx+0.005*vx;
    sy=sy+0.005*vy;
    sz=sz+0.005*vz;
    point_stamped.pose.position.x=sx;
    point_stamped.pose.position.y=sy;
    point_stamped.pose.position.z=sz;


    point_stamped.pose.orientation.x= 0;
    point_stamped.pose.orientation.y= 0;
    point_stamped.pose.orientation.z= 0;
    point_stamped.pose.orientation.w= 1;
    path.header.frame_id ="map";
    path.poses.push_back(point_stamped);
    pub_imu_pose.publish(path);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu");
    ros::NodeHandle n;
    pub_imu_pose = n.advertise<nav_msgs::Path>("/imu0/path_viz", 1000);
    //ros::Rate r(10);  //hz
    ros::Subscriber sub_imu = n.subscribe("/imu0", 1000, preintegration);
    ros::spin();
}