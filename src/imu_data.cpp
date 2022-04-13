// Step 1:  Include Library Headers:
//发布imu的数据到IMU_data

#include <iostream>
#include <fstream>
#include <string>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
using namespace std;
int main(int argc, char **argv)
{

    //read txt.
    double time[122220][1];
    int time_sec[122220][1];
    double time_nsec[122220][1];
    double acc[122220][3];  // x y z
    double gyro[122220][3]; // x y z
    ifstream timefile("/home/tiev_slammer/A_Members/zzy/imu_pub/src/imu_publish/txt/time_lx.txt");
    ifstream accfile("/home/tiev_slammer/A_Members/zzy/imu_pub/src/imu_publish/txt/acc_lx.txt");
    ifstream gyrofile("/home/tiev_slammer/A_Members/zzy/imu_pub/src/imu_publish/txt/gyro_lx.txt");

    if (!timefile.is_open())
    {
        cout << "can not open timefile" << endl;
        return 0;
    }
    if (!gyrofile.is_open())
    {
        cout << "can not open gyrofile" << endl;
        return 0;
    }
    if (!accfile.is_open())
    {
        cout << "can not open accfile" << endl;
        return 0;
    }

    for (int i = 0; i < 122220; i++)
    {
        timefile >> time[i][0];
        // time_sec[i][0]=floor(time[i][0]/1000000000);//19位的时间
        // time_nsec[i][0]=(time[i][0]-time_sec[i][0]*1000000000);
        time_sec[i][0] = floor(time[i][0] / 1000); //13位的时间
        time_nsec[i][0] = (time[i][0] - time_sec[i][0] * 1000) * 1000000;
    }

    for (int i = 0; i < 122220; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            accfile >> acc[i][j];
        }
    }
    for (int i = 0; i < 122220; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gyrofile >> gyro[i][j];
        }
    }
    int flag = 0;
    // Step 2: Initialization:
    ros::init(argc, argv, "imu_gps");
    ros::NodeHandle n;

    ros::Publisher IMU_pub = n.advertise<sensor_msgs::Imu>("/imu", 200);
    ros::Rate loop_rate(10);
    while (ros::ok())
    {

        sensor_msgs::Imu imu_data;

        imu_data.header.stamp.sec = time_sec[flag][0];
        imu_data.header.stamp.nsec = time_nsec[flag][0];

        //  imu_data.header.stamp = time[flag][0];
        imu_data.header.frame_id = "/imu";
        //四元数位姿,所有数据设为固定值，可以自己写代码获取ＩＭＵ的数据，，然后进行传递
        imu_data.orientation.x = 0;
        imu_data.orientation.y = 0;
        imu_data.orientation.z = 0;
        imu_data.orientation.w = 1;
        //线加速度

        imu_data.linear_acceleration.x = acc[flag][0];
        imu_data.linear_acceleration.y = acc[flag][1];
        imu_data.linear_acceleration.z = acc[flag][2];
        //角速度
        imu_data.angular_velocity.x = gyro[flag][0];
        imu_data.angular_velocity.y = gyro[flag][1];
        imu_data.angular_velocity.z = gyro[flag][2];

        IMU_pub.publish(imu_data);

        flag = flag + 1;
        std::cout << flag << std::endl;
        if (flag == 122220)
        {
            cout << "*****to the end*****" << endl;
            break;
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
