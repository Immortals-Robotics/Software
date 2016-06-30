#ifndef READER_H
#define READER_H

#include <stdint.h>
#include <stddef.h>

#include "data_lite.h"

#ifdef __cplusplus
extern "C"
{
#endif

void read_bytes(const uint8_t* const buffer, size_t* const pos, void* const data, const size_t count);

void read_bits8(const uint8_t* const buffer, size_t* const pos, struct bits8_t* const data);

void read_bits16(const uint8_t* const buffer, size_t* const pos, struct bits16_t* const data);

void read_bits32(const uint8_t* const buffer, size_t* const pos, struct bits32_t* const data);

void read_uint8(const uint8_t* const buffer, size_t* const pos, uint8_t* const data);

void read_uint16(const uint8_t* const buffer, size_t* const pos, uint16_t* const data);

void read_uint32(const uint8_t* const buffer, size_t* const pos, uint32_t* const data);

void read_float_h(const uint8_t* const buffer, size_t* const pos, union float_32_u_t* const data);

void read_float(const uint8_t* const buffer, size_t* const pos, union float_32_u_t* const data);

void read_v2f(const uint8_t* const buffer, size_t* const pos, struct vector2f_t* const data);

void read_v2f_h(const uint8_t* const buffer, size_t* const pos, struct vector2f_t* const data);

void read_v3f(const uint8_t* const buffer, size_t* const pos, struct vector3f_t* const data);

void read_v3f_h(const uint8_t* const buffer, size_t* const pos, struct vector3f_t* const data);

void read_v4f(const uint8_t* const buffer, size_t* const pos, struct vector4f_t* const data);

void read_v4f_h(const uint8_t* const buffer, size_t* const pos, struct vector4f_t* const data);

uint8_t read_robot_command_fixed(const uint8_t* const buffer, const size_t size, struct robot_command_msg_t* const data);

uint8_t read_robot_control_config_fixed(const uint8_t* const buffer, const size_t size, struct robot_control_config_msg_t* const data);

uint8_t read_robot_shoot_config_fixed(const uint8_t* const buffer, const size_t size, struct robot_shoot_config_msg_t* const data);

uint8_t read_robot_on_board_config_fixed(const uint8_t* const buffer, const size_t size, struct robot_on_board_config_t* const data);

uint8_t read_robot_matrix_fixed(const uint8_t* const buffer, const size_t size, struct robot_matrix_msg_t* const data);

uint8_t read_robot_feedback_fixed(const uint8_t* const buffer, const size_t size, struct robot_feedback_msg_t* const data);

uint8_t read_robot_feedback_custom_fixed(const uint8_t* const buffer, const size_t size, struct robot_feedback_custom_t* const data);

uint8_t read_robot_wrapper_fixed(const uint8_t* const buffer, const size_t size, struct robot_wrapper_msg_t* const data);

#ifdef __cplusplus
}
#endif

#endif
