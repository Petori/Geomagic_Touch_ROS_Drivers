#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <std_msgs/Int8.h>
#include <unistd.h>   // for function usleep(microseconds)
#include <cstdlib>

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Wrench.h>
#include <geometry_msgs/WrenchStamped.h>
#include <sensor_msgs/JointState.h>

#include "omni_msgs/OmniFeedback.h"
#include "omni_msgs/OmniState.h"

// Global Variables
std::ofstream fout1("data/Touch/phantomJointStates[dR].txt");
std::ofstream fout2("data/Touch/phantomPose[dR].txt");
std::ofstream fout3("data/Touch/phantomState[dR].txt");
//std::ofstream fout4("data/phantomForceFeedback[dR].txt");

// Function definition
void recordJointState(sensor_msgs::JointState curJointState);
void recordPose(geometry_msgs::PoseStamped curPose);
void recordState(omni_msgs::OmniState curJointState);
//void recordForceFeedback(omni_msgs::OmniFeedback curForce);

using namespace std;
// Main
int main(int argc, char **argv)
{
      ros::init(argc, argv, "dataRecorder");
      ros::NodeHandle n;
      ros::AsyncSpinner spinner(1);
      spinner.start();

      ros::Subscriber recorder1 = n.subscribe<sensor_msgs::JointState>("/phantom/joint_states", 1, recordJointState);// Subscribing the joint_states and record them.
      ros::Subscriber recorder2 = n.subscribe<geometry_msgs::PoseStamped>("/phantom/pose", 1, recordPose);
      ros::Subscriber recorder3 = n.subscribe<omni_msgs::OmniState>("/phantom/state", 1, recordState);
      //ros::Subscriber recorder4 = n.subscribe<omni_msgs::OmniFeedback>("/phantom/force_feedback", 1, recordForceFeedback);
      
      usleep(1000000);//Leave 0.5s for building the subscribers and publishers

      while(ros::ok()){};
      fout1.close();
      fout2.close();
      fout3.close();
      //fout4.close();

      return 0;
}

void recordJointState(sensor_msgs::JointState curJointState)
{
      std::vector<std::string> curName;
      std::vector<double> curPos;
      std::vector<double> curVel;
      std::vector<double> curEff;

      curName = curJointState.name;
      curPos = curJointState.position;
      curVel = curJointState.velocity;
      curEff = curJointState.effort;

      // I dont record these data because I dont use them.
      fout1<<"header:"<<std::endl;
      fout1<<"  seq: "<<"000000"<<std::endl;
      fout1<<"  stamp:"<<std::endl;
      //fout1<<"    secs: "<<ros::Time::now()<<std::endl;
      fout1<<"    secs: "<<curJointState.header.stamp<<std::endl;
      fout1<<"    nsecs: "<<"000000"<<std::endl;
      fout1<<"  frame_id: \'\'"<<std::endl;

      // write the name;
      fout1 <<"name: [";
      for(int i=0; i<(curName.size()-1); ++i)
      {
        fout1 <<'\'';
        fout1 << curName[i] << "\', ";
      }
      fout1<<'\''<<curName[curName.size()-1]<<'\''<<']'<<std::endl;

      // write the position;
      fout1 <<"position: [";
      for(int i=0; i<(curPos.size()-1); ++i)
      {
        fout1 << curPos[i] << ", ";
      }
      fout1<<curPos[curPos.size()-1]<<']'<<std::endl;

      // nodata from velocity and effort
      // write the velocity
      // fout1 <<"velocity: [";
      // for(int i=0; i<(curVel.size()-1); ++i)
      // {
      //   cout<<"10"<<endl;
      //   fout1 << curVel[i] <<", ";
      // }
      // fout1<<curVel[curVel.size()-1]<<']'<<std::endl;
      //
      // // write the effort
      // fout1 <<"effort: [";
      // for(int i=0; i<(curEff.size()-1); ++i)
      // {
      //   fout1 << curEff[i] << ", ";
      // }
      // fout1<<curEff[curEff.size()-1]<<']'<<std::endl;
      fout1<<"---"<<std::endl;
}

void recordPose(geometry_msgs::PoseStamped curPose)
{
      geometry_msgs::PoseStamped ppose;
      geometry_msgs::Point temp1;
      geometry_msgs::Quaternion temp2;

      ppose = curPose;
      temp1 = ppose.pose.position;
      temp2 = ppose.pose.orientation;

      fout2<<"[";
      fout2<<ros::Time::now()<<",";
      fout2<<temp1.x<<", ";
      fout2<<temp1.y<<", ";
      fout2<<temp1.z<<", ";
      fout2<<temp2.x<<", ";
      fout2<<temp2.y<<", ";
      fout2<<temp2.z<<", ";
      fout2<<temp2.w<<"]"<<std::endl;
}

void recordState(omni_msgs::OmniState curState)
{
  geometry_msgs::Point temp1;
  geometry_msgs::Quaternion temp2;
  geometry_msgs::Vector3 ccurrent;
  geometry_msgs::Vector3 vvelocity;

  temp1 = curState.pose.position;
  temp2 = curState.pose.orientation;
  ccurrent = curState.current;
  vvelocity = curState.velocity;

  fout3<<"[";
  fout3<<ros::Time::now()<<",";
  fout3<<temp1.x<<", ";
  fout3<<temp1.y<<", ";
  fout3<<temp1.z<<", ";
  fout3<<temp2.x<<", ";
  fout3<<temp2.y<<", ";
  fout3<<temp2.z<<", ";
  fout3<<temp2.w<<",";
  fout3<<ccurrent.x<<",";
  fout3<<ccurrent.y<<",";
  fout3<<ccurrent.z<<",";
  fout3<<vvelocity.x<<",";
  fout3<<vvelocity.y<<",";
  fout3<<vvelocity.z<<"]"<<std::endl;
}

// void recordForceFeedback(omni_msgs::OmniFeedback curForce)
// {
//   geometry_msgs::Vector3 fforce;
//   geometry_msgs::Vector3 pposition;
//
//   fforce = curForce.force;
//   pposition = curForce.position;
//
//   fout4<<"[";
//   fout4<<ros::Time::now()<<",";
//   fout4<<fforce.x<<", ";
//   fout4<<fforce.y<<", ";
//   fout4<<fforce.z<<", ";
//   fout4<<pposition.x<<", ";
//   fout4<<pposition.y<<", ";
//   fout4<<pposition.z<<"]"<<std::endl;
// }
