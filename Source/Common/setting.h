#pragma once

#include <array>
#include <iostream>

#include "config/config.h"

enum class TeamColor
{
    Blue   = 0,
    Yellow = 1,
};

inline TeamColor operator!(const TeamColor t_color)
{
    return t_color == TeamColor::Blue ? TeamColor::Yellow : TeamColor::Blue;
}

enum class TeamSide
{
    Left  = 0,
    Right = 1,
};

struct NetworkAddress final : public IConfig
{
    NetworkAddress() = default;

    NetworkAddress(const std::string_view ip, const unsigned short port)
        : ip(ip), port(port)
    {}

    void load(toml::node_view<const toml::node> t_node) override
    {
        ip   = t_node["ip"].value_or(ip);
        port = t_node["port"].value_or(port);
    }

    std::string    ip;
    unsigned short port = 0;
};

template <typename Enum>
void fillEnum(toml::node_view<const toml::node> t_node, Enum &t_enum)
{
    t_enum = static_cast<Enum>(t_node.value_or(static_cast<int>(t_enum)));
}

struct RobotPhysicalStatus
{
    bool has_dribbler = true;
    bool has_direct_kick = true;
    bool has_chip_kick = true;
    bool is_3D_printed = false;
};

struct Setting : IConfig
{
private:
    Setting()  = default;
    ~Setting() = default;

    void load(toml::node_view<const toml::node> t_node) override;

    friend struct Services;

public:
    Setting(const Setting &)            = delete;
    Setting &operator=(const Setting &) = delete;

    static constexpr size_t kMaxUdpPacketSize = 1024 * 16;//TODO what should the size be really?

    // The variety of standard patterns that we can have is 12
    static constexpr unsigned kMaxRobots = 16;

    bool immortals_is_the_best_team = true;

    bool enable_debug = false;

    double vision_frame_rate = 60.0;

    TeamColor our_color;

    bool side_wall = false;

    NetworkAddress tracker_address          = {"224.5.23.2", 10010};
    NetworkAddress debug_address            = {"127.0.0.1", 10066};
    NetworkAddress vision_interface_address = {"127.0.0.1", 10067};

    // vision
    static constexpr unsigned kCamCount = 8;
    static constexpr unsigned kMaxBalls = 10;

    NetworkAddress vision_address = {"224.5.23.2", 10006};

    std::array<bool, kCamCount> use_camera = {};

    int max_ball_hist = 10;

    double merge_distance        = 70.0;
    double ball_merge_distance   = 70.0;
    double max_ball_2_frame_dist = 1000.0;

    int max_robot_frame_not_seen = 120; // 2 seconds
    int max_ball_frame_not_seen  = 120; // 2 seconds

    bool use_kalman_pos = true;
    bool use_kalman_ang = true; // TODO: check if this is in serious need in reality

    // soccer
    static constexpr int kMaxOnFieldTeamRobots = 11;

    NetworkAddress referee_address = {"224.5.23.1", 10003};

    NetworkAddress sender_address     = {"224.5.92.5", 60005};
    NetworkAddress sender_rec_address = {"", 0}; // TODO: unused?

    NetworkAddress commands_address = {"224.5.92.6", 60006};

    NetworkAddress control_simulation_address = {"127.0.0.1", 10300};
    NetworkAddress blue_robot_simulation_address         = {"127.0.0.1", 10301};
    NetworkAddress yellow_robot_simulation_address     = {"127.0.0.1", 10302};

    TeamSide our_side = TeamSide::Left;

    bool command_opp_robots = false;
    bool enable_simulation  = false;

    unsigned char nrf_frq; // TODO: unused

    // Field dimensions since 2023 (Maybe earlier. Check the rules)
    double field_width  = 12000;
    double field_length = 9000;
    double goal_length  = 1800;
    double penalty_area_r = 1800;
    double penalty_area_width = 3600;

    int init_gk_id  = 0;

    RobotPhysicalStatus robot_physical_status[kMaxRobots];


    double gamma_reality        = 19;
    double gamma_simulation     = 900;
    double ang_gamma_reality    = 1;
    double ang_gamma_simulation = 1;

    std::string robot_behavior_tree_config_filename = "bt_default.xml";
};

inline void Setting::load(const toml::node_view<const toml::node> t_node)
{
    const auto common = t_node["common"];

    immortals_is_the_best_team = common["immortals_is_the_best_team"].value_or(immortals_is_the_best_team);

    fillEnum(common["our_color"], our_color);

    side_wall = common["side_wall"].value_or(side_wall);

    enable_debug = common["enable_debug"].value_or(enable_debug);

    const auto network = t_node["network"];

    tracker_address.load(network["tracker"]);
    debug_address.load(network["debug"]);
    vision_interface_address.load(network["vision_interface"]);

    vision_address.load(network["vision"]);

    referee_address.load(network["referee"]);

    sender_address.load(network["sender"]);

    commands_address.load(network["commands"]);
    //sender_rec_address.load(network["referee"]);

    control_simulation_address.load(network["control_simulation"]);
    blue_robot_simulation_address.load(network["blue_robot_simulation"]);
    yellow_robot_simulation_address.load(network["yellow_robot_simulation"]);

    // vision
    const auto vision = t_node["vision"];

    if (auto *use_camera_array = vision["use_camera"].as_array())
    {
        for (unsigned cam_idx = 0; cam_idx < kCamCount; ++cam_idx)
        {
            use_camera[cam_idx] = (*use_camera_array)[cam_idx].value_or(use_camera[cam_idx]);
        }
    }

    vision_frame_rate = vision["vision_frame_rate"].value_or(vision_frame_rate);

    max_ball_hist = vision["max_ball_hist"].value_or(max_ball_hist);

    merge_distance        = vision["merge_distance"].value_or(merge_distance);
    ball_merge_distance   = vision["ball_merge_distance"].value_or(ball_merge_distance);
    max_ball_2_frame_dist = vision["max_ball_2_frame_dist"].value_or(max_ball_2_frame_dist);

    max_robot_frame_not_seen = vision["max_robot_frame_not_seen"].value_or(max_robot_frame_not_seen);
    max_ball_frame_not_seen  = vision["max_ball_frame_not_seen"].value_or(max_ball_frame_not_seen);

    use_kalman_pos = vision["use_kalman_pos"].value_or(use_kalman_pos);
    use_kalman_ang = vision["use_kalman_ang"].value_or(use_kalman_ang);

    // soccer
    const auto soccer = t_node["soccer"];

    fillEnum(soccer["our_side"], our_side);

    command_opp_robots = soccer["command_opp_robots"].value_or(command_opp_robots);
    enable_simulation  = soccer["enable_simulation"].value_or(enable_simulation);

    nrf_frq = soccer["nrf_frq"].value_or(nrf_frq);

    field_width  = soccer["field_width"].value_or(field_width);
    field_length = soccer["field_length"].value_or(field_length);
    goal_length  = soccer["goal_length"].value_or(goal_length);

    // Gamma represents the relation between the robots velocity parameter and the velocity in mm/s
    // Gamma is 48 in the GR Simulation
    // and 19 in reality
    // Velocity Gamma
    gamma_reality        = soccer["gamma_reality"].value_or(gamma_reality);
    gamma_simulation     = soccer["gamma_simulation"].value_or(gamma_simulation);
    // Angular Gamma
    ang_gamma_reality    = soccer["ang_gamma_reality"].value_or(ang_gamma_reality);
    ang_gamma_simulation = soccer["ang_gamma_simulation"].value_or(ang_gamma_simulation);

    robot_behavior_tree_config_filename = soccer["robot_behavior_tree_config_filename"].value_or(robot_behavior_tree_config_filename);


    init_gk_id  = soccer["init_gk_id"].value_or(init_gk_id);

    if (auto *robot_physical_status_array = soccer["robot_physical_status"].as_array())
    {
        std::cout << "Array size of received robot_physical_status: " << robot_physical_status_array->size() << std::endl;
        for (int i=0; i<robot_physical_status_array->size(); i++)
        {
            int id = soccer["robot_physical_status"][i]["id"].value_or(-1);
            if(id == -1){
                std::cerr << "Robot ID for #" << i << " is not define in the .toml config file" << std::endl;
            }
            if(id != i){
                // This check is to make sure the config file is written in order so
                // less mistake will happen during the match setup
                std::cerr << "Robot ID is out of order in iteration #" << i << " the id should is " << id << " but should be " << i << std::endl;
            }
            robot_physical_status[id].has_dribbler = soccer["robot_physical_status"][i]["has_dribbler"].value_or(false);
            robot_physical_status[id].has_direct_kick = soccer["robot_physical_status"][i]["has_direct_kick"].value_or(false);
            robot_physical_status[id].has_chip_kick = soccer["robot_physical_status"][i]["has_chip_kick"].value_or(false);
            robot_physical_status[id].is_3D_printed = soccer["robot_physical_status"][i]["is_3D_printed"].value_or(false);

            std::cout << "Robot #" << id << " status:" << std::endl;
            std::cout << " -has_dribbler: " << robot_physical_status[id].has_dribbler << std::endl;
            std::cout << " -has_direct_kick: " << robot_physical_status[id].has_direct_kick << std::endl;
            std::cout << " -has_chip_kick: " << robot_physical_status[id].has_chip_kick << std::endl;
            std::cout << " -is_3D_printed: " << robot_physical_status[id].is_3D_printed << std::endl;
        }
    }
}
