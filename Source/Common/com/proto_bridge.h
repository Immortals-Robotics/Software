#pragma once

#include "../protos/messages_immortals_robot_wrapper.pb.h"
#include "data_lite.h"

size_t proto_msg_frame_to_byte_array(const Immortals::Data::RobotMessageFrame &data, uint8_t *const buffer);
bool feedback_bytes_to_proto_feedback(const uint8_t *const buffer, const size_t size, Immortals::Data::RobotMessage *const proto_msg);
