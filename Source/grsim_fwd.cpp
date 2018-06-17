//
// Created by Ali Salehi on 16.06.18.
//

#include "grsim_fwd.h"

#include "Network/Protobuf/grSim_Packet.pb.h"

GrsimForwarder::GrsimForwarder(const char *const ip, const short port) :
    ip(ip), port(port)
{
    socket = new UDPSocket;
}

void GrsimForwarder::SendData(const Robot *const robots, const int robot_count, bool color)
{
    grSim_Packet packet;
    packet.mutable_commands()->set_isteamyellow(color);
    packet.mutable_commands()->set_timestamp(0.0);

    for (int robot_idx = 0; robot_idx < robot_count; ++robot_idx)
    {
        const Robot *const robot = robots + robot_idx;

        grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();
        command->set_id(robot->vision_id);

        command->set_wheelsspeed(false);
        /*command->set_wheel1(edtV1->text().toDouble());
        command->set_wheel2(edtV2->text().toDouble());
        command->set_wheel3(edtV3->text().toDouble());
        command->set_wheel4(edtV4->text().toDouble());*/

        const int cmd_idx = robots[robot_idx].lastCMDs[10].X;
        const TVec3 motion = robots[robot_idx].lastCMDs[cmd_idx];
        float robot_ang = (90-robot->State.Angle) * 3.1415/180.0;
        float new_VelX = motion.X * cos(robot_ang) - motion.Y * sin(robot_ang);
        float new_VelY = motion.X * sin(robot_ang) + motion.Y * cos(robot_ang);


        command->set_veltangent(new_VelY / 20.0);
        command->set_velnormal(-new_VelX / 20.0);
        command->set_velangular(motion.Z / 50.0);
        //command->set_velangular(0);

        command->set_kickspeedx(robot->shoot);
        command->set_kickspeedz(robot->chip);
        command->set_spinner(robot->dribbler);

        const size_t dgram_len = packet.ByteSize();
        char *const dgram = new char[dgram_len];
        packet.SerializeToArray(dgram, dgram_len);
        socket->sendTo(dgram, dgram_len, ip, port);
        delete[] dgram;
    }
}

GrsimForwarder::~GrsimForwarder()
{
    delete socket;
}
