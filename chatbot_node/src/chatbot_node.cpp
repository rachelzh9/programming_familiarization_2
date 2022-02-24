#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <string>

using namespace std;

//Add your code here
ros::Publisher pub;
std::string name = "";

void replyCallback(const message_ui::sent_msg::ConstPtr& msg)
{
  ROS_INFO("Sent: %s", msg->message.c_str());

  std::string s1 ("Hello");
  std::string s2 ("What is your name?");
  std::string s3 ("How are you?");

  chatbot_node::reply_msg reply;
  reply.header.stamp = ros::Time::now();

  if(s1.compare(msg->message)==0) {
    reply.message = "Hello, " + name;
    pub.publish(reply);
  } else if(s2.compare(msg->message)==0) {
    reply.message = "My name is MRSD Siri";
    pub.publish(reply);
  } else if(s3.compare(msg->message)==0) {
    reply.message = "I am fine, thank you";
    pub.publish(reply);
  }

}



int main(int argc, char **argv) {

  ros::init(argc, argv, "chatbot_node");
  ros::NodeHandle n;

  //Add your code here
  ros::Subscriber sub = n.subscribe("sent_msg", 1000, replyCallback);
  pub = n.advertise<chatbot_node::reply_msg>("reply_msg", 1000);

  if(!n.getParam("name", name)) {
    ROS_WARN("Could not get name");
  }

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}