#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/AccelStamped.h"
#include "geometry_msgs/Vector3Stamped.h"

class Accel
{
  private:
    ros::NodeHandle nh;
    ros::Publisher accels, orientation;
    ros::Subscriber js;
    geometry_msgs::AccelStamped accel;
    geometry_msgs::Vector3Stamped orient;

  public:
    Accel();
    void joy_callback(const sensor_msgs::Joy::ConstPtr& joy);
    void loop();
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_accel_set");
  Accel accel;
  accel.loop();
}

Accel::Accel()
{
  js = nh.subscribe<sensor_msgs::Joy>("joy", 1, &Accel::joy_callback, this);
  accels = nh.advertise<geometry_msgs::AccelStamped>("accel_set_pt", 1);
  orientation = nh.advertise<geometry_msgs::Vector3Stamped>("orientation_set_pt",1);
}

void Accel::joy_callback(const sensor_msgs::Joy::ConstPtr& joy)
{
  ros::Time time = ros::Time::now();
  accel.header.stamp = time;
  accel.accel.linear.x = 5 * joy->axes[1];
  accel.accel.linear.y = 0;
  accel.accel.linear.z = 5 * (joy->axes[14] - joy->axes[15]);

  ros::Time timestamp = ros::Time::now();
  orient.header.stamp = timestamp;

  orient.vector.x = 5 * 2 * 3.14159 * joy->axes[2] * -1;
  orient.vector.y = 5 * 2 * 3.14159 * joy->axes[3];
  orient.vector.z = 5 * 2 * 3.14159 * (joy->axes[13] - joy->axes[12]);

  accels.publish(accel);
  orientation.publish(orient);
}

void Accel::loop()
{
  ros::Rate rate(30);
  while(ros::ok())
  {
    ros::spinOnce();
    rate.sleep();
  }
}
