#ifndef WRITER_H
#define WRITER_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stddef.h>

#include "data_lite.h"

void write_bytes(uint8_t* const buffer, size_t* const pos, const void* const data, const size_t count);

void write_bits8(uint8_t* const buffer, size_t* const pos, const struct bits8_t* const data);

void write_bits16(uint8_t* const buffer, size_t* const pos, const struct bits16_t* const data);

void write_bits32(uint8_t* const buffer, size_t* const pos, const struct bits32_t* const data);

void write_uint8(uint8_t* const buffer, size_t* const pos, const uint8_t data);

void write_uint16(uint8_t* const buffer, size_t* const pos, const uint16_t data);

void write_uint32(uint8_t* const buffer, size_t* const pos, const uint32_t data);

void write_float_h(uint8_t* const buffer, size_t* const pos, const union FLOAT_32 data);

void write_float(uint8_t* const buffer, size_t* const pos, const union FLOAT_32 data);

void write_v2f_h(uint8_t* const buffer, size_t* const pos, const struct Vector2f_V2* const data);

void write_v2f(uint8_t* const buffer, size_t* const pos, const struct Vector2f_V2* const data);

void write_v3f_h(uint8_t* const buffer, size_t* const pos, const struct Vector3f* const data);

void write_v3f(uint8_t* const buffer, size_t* const pos, const struct Vector3f* const data);

void write_v4f_h(uint8_t* const buffer, size_t* const pos, const struct Vector4f* const data);

void write_v4f(uint8_t* const buffer, size_t* const pos, const struct Vector4f* const data);

size_t write_robot_command_fixed(uint8_t* const buffer, const struct RobotCommand* const data);

size_t write_robot_command_fixed_V2(uint8_t* const buffer, const struct RobotCommand_V2* const data);

size_t write_robot_config_fixed(uint8_t* const buffer, const struct RobotConfig* const data);

size_t write_robot_matrix_fixed(uint8_t* const buffer, const struct RobotMatrix* const data);

size_t write_robot_feedback_fixed(uint8_t* const buffer, const struct RobotFeedback* const data);

size_t write_robot_feedback_custom_fixed(uint8_t* const buffer, const struct RobotFeedbackCustom* const data);

void write_uint16_in_buff(uint8_t* const buffer, const uint16_t data);

#ifdef __cplusplus
}
#endif

#endif
