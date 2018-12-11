#include <cstdio>
#include <vector>
#include <ros/ros.h>
//#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <fstream>

using namespace std;




ros::Publisher tpub_path;
ros::Publisher spub_path;
nav_msgs::Path tpath;
nav_msgs::Path spath;



void msgs_callback(const geometry_msgs::PointStamped::ConstPtr& msg)
{
    //ROS_INFO("odom callback!");
   

    geometry_msgs::PoseStamped point_stamped;
    point_stamped.header.stamp = msg->header.stamp;
    point_stamped.pose.position.x= msg->point.x-4.78229971748;
    point_stamped.pose.position.y= msg->point.y+1.81557362771;
    point_stamped.pose.position.z= msg->point.z-0.844627073703;
    point_stamped.pose.orientation.x= 0;
    point_stamped.pose.orientation.y= 0;
    point_stamped.pose.orientation.z= 0;
    point_stamped.pose.orientation.w= 1;
    tpath.header.frame_id ="map";
    tpath.poses.push_back(point_stamped);
    tpub_path.publish(tpath);
}


void slam_msgs_callback(const geometry_msgs::PointStamped::ConstPtr& msg)
{
    //ROS_INFO("odom callback!");
   

    geometry_msgs::PoseStamped point_stamped;
    point_stamped.header.stamp = msg->header.stamp;
    point_stamped.pose.position.x= msg->point.x;
    point_stamped.pose.position.y= msg->point.y;
    point_stamped.pose.position.z= msg->point.z;
    point_stamped.pose.orientation.x= 0;
    point_stamped.pose.orientation.y= 0;
    point_stamped.pose.orientation.z= 0;
    point_stamped.pose.orientation.w= 1;
    spath.header.frame_id ="map";
    spath.poses.push_back(point_stamped);
    spub_path.publish(spath);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "path_viz");
    ros::NodeHandle n;
    ros::NodeHandle m;
    tpub_path = n.advertise<nav_msgs::Path>("/vio/path_viz", 1000);
    ros::Subscriber sub_odom = n.subscribe("/leica/position", 1000, msgs_callback);
    

    spub_path = m.advertise<nav_msgs::Path>("/vio/slam_viz", 1000);
    ros::Subscriber slam_odom = m.subscribe("/Stereo/position", 1000, slam_msgs_callback);
    
    ros::Rate r(20);
    ros::spin();
}


