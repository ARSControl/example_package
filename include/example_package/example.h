#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <signal.h>
#include <fstream>

// Include ROS Libraries
#include <rclcpp/rclcpp.hpp>
// #include <ros/package.h>

// Include ROS Messages and Services
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <std_srvs/srv/set_bool.hpp>

// Include ROS Action Servers and Clients
#include <rclcpp_action/create_client.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>

// Include Custom Messages, Services and Actions
#include <example_package/msg/example_msg.hpp>
#include <example_package/srv/example_srv.hpp>
#include "example_package/action/example_action.hpp"

// Include MoveIt! Libraries
// #include <moveit/robot_model_loader/robot_model_loader.h>
// #include <moveit/robot_model/robot_model.h>
// #include <moveit/robot_state/robot_state.h>

// Include Eigen Libraries
#include <Eigen/Eigen>

using namespace Eigen;

typedef Matrix<double, 7, 1> Vector7d;
typedef Matrix<double, 6, 1> Vector6d;
typedef Matrix<double, 6, 6> Matrix6d;
typedef Array<double, 6, 1> Array6d;

struct example_struct {
    double a;
    std::string b;
    std::vector<int> c;
};

// Chrono Namespace for Time Functions
using namespace std::chrono_literals;

#define GET_VARIABLE_NAME(Variable) (#Variable)

#define BOOL_DEFINITION true
#define INT_DEFINITION 10
#define STRING_DEFINITION "String Definition"

class ExamplePackage : public rclcpp::Node {

    public:
    
      ExamplePackage(std::string example_data, std::vector<double> example_vector);

      ~ExamplePackage();

      void spinner(void);

    private:

        // ---- GLOBAL VARIABLES ---- //
        bool example_bool_ = false;
        int example_int_ = 100;
        std::string example_string_ = "Default";
        std::vector<double> example_vector_ = {1, 1.1, 0.0};
        Vector6d example_eigen_vector_;
        example_struct example_struct_;

        // ---- ROS - PARAMETERS ---- //
        std::string example_string_param_;
        double example_double_param_;
        bool example_bool_param_;
        std::vector<double> example_vector_param_;

        // ---- ROS - PUBLISHERS ---- //
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr string_publisher_;
        rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr example_publisher_;
        rclcpp::Publisher<example_package::msg::ExampleMsg>::SharedPtr example_custom_publisher_;

        // ---- ROS - SUBSCRIBERS & CALLBACKS ---- //
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr string_subscriber_;
        rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr example_subscriber_;
        rclcpp::Subscription<example_package::msg::ExampleMsg>::SharedPtr example_custom_subscriber_;
        void stringSubscriberCallback(const std_msgs::msg::String::SharedPtr msg);
        void exampleSubscriberCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
        void exampleCustomSubscriberCallback(const example_package::msg::ExampleMsg::SharedPtr msg);

        // ---- ROS - SERVICE CLIENTS ---- //
        rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr example_client_;
        rclcpp::Client<example_package::srv::ExampleSrv>::SharedPtr example_custom_client_;

        // ---- ROS - SERVICE SERVERS & CALLBACKS ---- //
        rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr example_server_;
        rclcpp::Service<example_package::srv::ExampleSrv>::SharedPtr example_custom_server_;
        bool exampleServerCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request, std::shared_ptr<std_srvs::srv::SetBool::Response> response);
        bool exampleCustomServerCallback(const std::shared_ptr<example_package::srv::ExampleSrv::Request> request, std::shared_ptr<example_package::srv::ExampleSrv::Response> response);

        // ---- ROS - ACTION CLIENT ---- //
        rclcpp_action::Client<control_msgs::action::FollowJointTrajectory>::SharedPtr trajectory_action_client_;
        void goalResponseCallback(std::shared_future<rclcpp_action::ClientGoalHandle<control_msgs::action::FollowJointTrajectory>::SharedPtr> future);
        void feedbackCallback(rclcpp_action::ClientGoalHandle<control_msgs::action::FollowJointTrajectory>::SharedPtr, const std::shared_ptr<const control_msgs::action::FollowJointTrajectory::Feedback> feedback);
        void resultCallback(const rclcpp_action::ClientGoalHandle<control_msgs::action::FollowJointTrajectory>::WrappedResult & result);

        // ---- MoveIt! ROBOT MODEL ---- //
        // robot_model_loader::RobotModelLoader robot_model_loader_;
        // robot_model::RobotModelPtr kinematic_model_;
        // robot_state::RobotStatePtr kinematic_state_;
        // const robot_state::JointModelGroup *joint_model_group_;
        std::vector<std::string> joint_names_;
        Eigen::MatrixXd J_;

        // ---- KINEMATIC FUNCTIONS ---- //
        Eigen::Matrix4d computeFK (std::vector<double> joint_position, std::vector<double> joint_velocity);
        Eigen::MatrixXd computeArmJacobian (std::vector<double> joint_position, std::vector<double> joint_velocity);
        Matrix6d getEE_RotationMatrix (std::vector<double> joint_position, std::vector<double> joint_velocity);

        // ---- PUBLISH - CALL FUNCTIONS ---- //
        void PublishMessage (void);
        void CallService (void);
        void CallAction (void);

        // ---- USEFUL FUNCTIONS ---- //
        int getSign(double data);

};

#endif /* EXAMPLE_H */