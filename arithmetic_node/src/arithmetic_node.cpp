#include <ros/ros.h>
#include <string>
#include <arithmetic_node/arithmetic_reply.h>
#include <message_ui/sent_msg.h>

using namespace std;

ros::Publisher pub;

void msgCallback(const message_ui::sent_msg::ConstPtr& msg) {
  ROS_INFO("Sent: %s", msg->message.c_str());

  arithmetic_node::arithmetic_reply reply;
  reply.time_received = msg->header.stamp.toSec();
  reply.header.stamp = ros::Time::now();

  std::string input = msg->message;
  std::size_t add = input.find('+');
  std::size_t sub = input.find('-');
  std::size_t mul = input.find('*');
  std::size_t div = input.find('/');

  if(add != std::string::npos) {
    reply.oper_type = "Add";
    std::string num1 = input.substr(0,add);
    std::string num2 = input.substr(add+1,std::string::npos);

    reply.answer = float(std::stoi(num1)) + float(std::stoi(num2));
  } 
  else if(sub != std::string::npos) {
    reply.oper_type = "Subtract";
    std::string num1 = input.substr(0,sub);
    std::string num2 = input.substr(sub+1,std::string::npos);

    reply.answer = float(std::stoi(num1)) - float(std::stoi(num2));
  } 
  else if(mul != std::string::npos) {
    reply.oper_type = "Multiply";
    std::string num1 = input.substr(0,mul);
    std::string num2 = input.substr(mul+1,std::string::npos);

    reply.answer = float(std::stoi(num1)) * float(std::stoi(num2));
  }
  else if(div != std::string::npos)
  {
    reply.oper_type = "Divide";
    std::string num1 = input.substr(0,div);
    std::string num2 = input.substr(div+1,std::string::npos);

    reply.answer = float(std::stoi(num1))/float(std::stoi(num2));
  }

  reply.time_answered = ros::Time::now().toSec();
  ros::Duration dt = ros::Time::now() - msg->header.stamp;
  reply.process_time = dt.toSec();
  pub.publish(reply);

}



int main(int argc, char **argv) {

  ros::init(argc, argv, "arithmetic_node");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("sent_msg", 1000, msgCallback);
  pub = n.advertise<arithmetic_node::arithmetic_reply>("arithmetic_reply", 1000);

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}