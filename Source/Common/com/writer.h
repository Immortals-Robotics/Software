#ifndef WRITER_H
#define WRITER_H

#include <stdint.h>
#include <stddef.h>

#include "data_lite.h"

#ifdef __cplusplus
extern "C"
{
#endif

void write_bytes(uint8_t* const buffer, size_t* const pos, const void* const data, const size_t count);

void write_bits8(uint8_t* const buffer, size_t* const pos, const struct bits8_t* const data);

void write_bits16(uint8_t* const buffer, size_t* const pos, const struct bits16_t* const data);

void write_bits32(uint8_t* const buffer, size_t* const pos, const struct bits32_t* const data);

void write_uint8(uint8_t* const buffer, size_t* const pos, const uint8_t data);

void write_uint16(uint8_t* const buffer, size_t* const pos, const uint16_t data);

void write_uint32(uint8_t* const buffer, size_t* const pos, const uint32_t data);

void write_float_h(uint8_t* const buffer, size_t* const pos, const union float_32_u_t data);

void write_float(uint8_t* const buffer, size_t* const pos, const union float_32_u_t data);

void write_v2f_h(uint8_t* const buffer, size_t* const pos, const struct vector2f_t* const data);

void write_v2f(uint8_t* const buffer, size_t* const pos, const struct vector2f_t* const data);

void write_v3f_h(uint8_t* const buffer, size_t* const pos, const struct vector3f_t* const data);

void write_v3f(uint8_t* const buffer, size_t* const pos, const struct vector3f_t* const data);

void write_v4f_h(uint8_t* const buffer, size_t* const pos, const struct vector4f_t* const data);

void write_v4f(uint8_t* const buffer, size_t* const pos, const struct vector4f_t* const data);

size_t write_robot_command_fixed(uint8_t* const buffer, const struct robot_command_msg_t* const data);

size_t write_robot_control_config_fixed(uint8_t* const buffer, const struct robot_control_config_msg_t* const data);

size_t write_robot_shoot_config_fixed(uint8_t* const buffer, const struct robot_shoot_config_msg_t* const data);

size_t write_robot_on_board_config_fixed(uint8_t* const buffer, const struct robot_on_board_config_t* const data);

size_t write_robot_matrix_fixed(uint8_t* const buffer, const struct robot_matrix_msg_t* const data);

size_t write_robot_feedback_fixed(uint8_t* const buffer, const struct robot_feedback_msg_t* const data, enum feedback_type_e type);

size_t write_robot_feedback_custom_fixed(uint8_t* const buffer, const struct robot_feedback_custom_t* const data);

size_t write_robot_wrapper_fixed(uint8_t* const buffer, const struct robot_wrapper_msg_t* const data);

#ifdef __cplusplus
}
#endif

#endif
