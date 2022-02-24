#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <arithmetic_node/arithmetic_reply.h>
#include "counter_node/counter.h"

int reply_msg_num = 0;
int sent_msg_num = 0;
ros::Subscriber reply_sub;
ros::Subscriber arithmetic_sub;
ros::Subscriber sent_sub;
ros::Time last_reply_t;
ros::Time last_sent_t;

void sent_msg_callback(const message_ui::sent_msg msg) {
	sent_msg_num++;
	last_sent_t = msg.header.stamp;
}

void reply_msg_callback(const chatbot_node::reply_msg msg) {
	reply_msg_num++;
	last_reply_t = msg.header.stamp;
}

void arithmetic_reply_msg_callback(const arithmetic_node::arithmetic_reply msg) {
 	reply_msg_num++;
 	last_reply_t = msg.header.stamp;
}

bool counterCallback(counter_node::counter::Request& request, counter_node::counter::Response& response)
{
  if(request.req_id == 0) {
    response.reply = sent_msg_num + reply_msg_num;
  } else if(request.req_id == 1) {
    response.reply = reply_msg_num;
  } else if(request.req_id == 2) {
    response.reply = sent_msg_num;
  } else if(request.req_id == 3) {
    ros::Duration t = ros::Time::now() - last_reply_t;
    response.reply = t.toSec(); 
  } else if(request.req_id == 4) {
    ros::Duration t = ros::Time::now() - last_sent_t;
    response.reply = t.toSec(); 
  } else
      return false;
  return true;
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "counter_node");
  ros::NodeHandle n;

  reply_sub = n.subscribe("reply_msg", 1000, reply_msg_callback);
  sent_sub = n.subscribe("sent_msg", 1000, sent_msg_callback);
  arithmetic_sub = n.subscribe("arithmetic_reply", 1000, arithmetic_reply_msg_callback);
  ros::ServiceServer service = n.advertiseService("message_counter", counterCallback);

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}