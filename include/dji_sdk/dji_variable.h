#ifndef __DJI_SDK_LOCALS_H__
#define __DJI_SDK_LOCALS_H__

#include <dji_sdk/global_position.h>
#include <dji_sdk/local_position.h>
#include <dji_sdk/attitude_quad.h>
#include <dji_sdk/global_position.h>
#include <dji_sdk/local_position.h>
#include <dji_sdk/velocity.h>
#include <dji_sdk/acc.h>
#include <dji_sdk/gimbal.h>
#include <dji_sdk/rc_channels.h>
#include <nav_msgs/Odometry.h>
#include <dji_sdk/dji_waypoints.h>
namespace dji_variable
{   
    extern bool flag_visit;
    extern bool flag_success;
    extern bool flag_open_or_close;
    extern dji_sdk::local_position local_position_ref;
    extern dji_sdk::global_position global_position_ref;
    extern bool localposbase_use_height;
    extern dji_sdk::attitude_quad attitude_quad;
    extern dji_sdk::velocity velocity;
    extern dji_sdk::acc acc;
    extern dji_sdk::rc_channels rc_channels;
    extern dji_sdk::global_position global_position;
    extern dji_sdk::global_position global_position_degree;
    extern dji_sdk::local_position local_position;
    extern float battery;
    extern uint8_t  flight_status;
    extern uint8_t ctrl_device;
    extern nav_msgs::Odometry odem;
    extern bool opened ;
    extern bool activated;
    void gps_convert_ned(float &ned_x, float &ned_y,
                     double gps_t_lon,
                     double gps_t_lat,
                     double gps_r_lon,
                     double gps_r_lat
    );
    dji_sdk::local_position gps_convert_ned(dji_sdk::global_position loc);
    extern dji_waypoints wp_m;
};

#endif
