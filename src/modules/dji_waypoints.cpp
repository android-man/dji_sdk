//
// Created by Hao Xu on 15/6/8.
//

#include "dji_sdk/dji_waypoints.h"
#include "dji_sdk/dji_variable.h"
#include "dji_sdk/motion_controls.h"
#include <fstream>

using namespace std;


dji_waypoints::dji_waypoints(std::string path)
{
    this->load(path);
}

dji_waypoints::dji_waypoints()
{
    mission mission1;
    this->missions.push_back(mission1);
}
int dji_waypoints::load(std::string path)
{
    std::ifstream wp_file(path);
    mission mission1;
    std::cout << "loading.....wp at " << path<<"\n";
    wp_file >> mission1;

    if (this->missions.size() == 1 && this->missions[0].size() == 0)
    {
        this->missions[0] = mission1;
    }
    else {
        this->missions.push_back(mission1);
    }
    std::cout << mission1;
    return mission1.size();
}

int dji_waypoints::add_waypoints(mission mission1)
{
    missions.push_back(mission1);
    return (int)(missions.size()-1);

    init_state_machine();
}

int dji_waypoints::begin_fly_waypoints(int id)
{
    int status = switch_status(status,ACTION_START_MISSION);
    if (status == 0 || id >= missions.size() || id < 0)
    {
        return 0;
    }

    mission_id = id;
    waypoint_ptr = 0;
    this->status = status;
    return 1;
}


int dji_waypoints::pause_flying()
{
    int status = switch_status(status,ACTION_PAUSE_MISSION);
    if (status == 0)
    {
        return 0;
    }
    this -> status = status;
    return 1;
}

int dji_waypoints::cont_flying()
{

    int status = switch_status(status,ACTION_CONT_MISSION);
    if (status == 0)
    {
        return 0;
    }
    this -> status = status;
    return 1;
}

int dji_waypoints::loop()
{
    switch (status)
    {
        case STATUS_STANDBY:
        case STATUS_PAUSE :
            break;
        case STATUS_FLYWAYPOINT:
            continue_mission();
            break;
        default:
            break;
    }
    return 1;
}

int dji_waypoints::continue_mission()
{
    auto wp = missions[mission_id][waypoint_ptr];
    if (approach(wp , dji_variable::global_position ,wp.uncertain  ))
    {
        waypoint_ptr ++;
        if (waypoint_ptr >= missions[mission_id].size())
        {
            status = switch_status(status,ACTION_STOP_MISSION);
            return 1;
        }
    }
    else
    {
        motion_controls::fly_to_globalpos(wp,false);
    }

    return 0;
}

bool dji_waypoints::approach(dji_sdk::global_position wp1, dji_sdk::global_position wp2,float uncertain)
{
    dji_sdk::local_position local_position1,local_position2;
    dji_variable::gps_convert_ned(local_position1,wp1);
    dji_variable::gps_convert_ned(local_position2,wp2);
    local_position1.height = wp1.alti;
    local_position2.height = wp2.alti;
    float raduis =
            (local_position1.x - local_position2.x) *  (local_position1.x - local_position2.x) +
            (local_position1.y - local_position2.y) *  (local_position1.y - local_position2.y) +
            (local_position1.height - local_position2.height) *  (local_position1.height - local_position2.height);

    if ( raduis > uncertain * uncertain )
    {
        return false;
    }
    return true;
}

int dji_waypoints::switch_status(int status, int action)
{
    return state_machine[status][action];
}

void dji_waypoints::init_state_machine()
{
    state_machine[STATUS_STANDBY][ACTION_START_MISSION] = STATUS_FLYWAYPOINT;
    state_machine[STATUS_STANDBY][ACTION_STOP_MISSION] = STATUS_STANDBY;
    state_machine[STATUS_STANDBY][ACTION_PAUSE_MISSION] = STATUS_STANDBY;

    state_machine[STATUS_PAUSE][ACTION_START_MISSION] = STATUS_FLYWAYPOINT;
    state_machine[STATUS_PAUSE][ACTION_CONT_MISSION] = STATUS_FLYWAYPOINT;
    state_machine[STATUS_PAUSE][ACTION_STOP_MISSION] = STATUS_STANDBY;
    state_machine[STATUS_PAUSE][ACTION_PAUSE_MISSION] = STATUS_PAUSE;

    state_machine[STATUS_FLYWAYPOINT][ACTION_START_MISSION] = STATUS_FLYWAYPOINT;
    state_machine[STATUS_FLYWAYPOINT][ACTION_PAUSE_MISSION] = STATUS_PAUSE;
    state_machine[STATUS_FLYWAYPOINT][ACTION_CONT_MISSION] = STATUS_FLYWAYPOINT;
    state_machine[STATUS_FLYWAYPOINT][ACTION_STOP_MISSION] = STATUS_STANDBY;
}