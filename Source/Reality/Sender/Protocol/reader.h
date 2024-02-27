#ifndef READER_H
#define READER_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stddef.h>

#include "data_lite.h"

void read_bytes(const uint8_t* const buffer, size_t* const pos, void* const data, const size_t count);

void read_bits8(const uint8_t* const buffer, size_t* const pos, struct bits8_t* const data);

void read_bits16(const uint8_t* const buffer, size_t* const pos, struct bits16_t* const data);

void read_bits32(const uint8_t* const buffer, size_t* const pos, struct bits32_t* const data);

void read_uint8(const uint8_t* const buffer, size_t* const pos, uint8_t* const data);

void read_uint16(const uint8_t* const buffer, size_t* const pos, uint16_t* const data);

void read_uint32(const uint8_t* const buffer, size_t* const pos, uint32_t* const data);

void read_float_h(const uint8_t* const buffer, size_t* const pos, union FLOAT_32* const data);

void read_float(const uint8_t* const buffer, size_t* const pos, union FLOAT_32* const data);

void read_v2f(const uint8_t* const buffer, size_t* const pos, struct Vector2f_V2* const data);

void read_v2f_h(const uint8_t* const buffer, size_t* const pos, struct Vector2f_V2* const data);

void read_v3f(const uint8_t* const buffer, size_t* const pos, struct Vector3f* const data);

void read_v3f_h(const uint8_t* const buffer, size_t* const pos, struct Vector3f* const data);

void read_v4f(const uint8_t* const buffer, size_t* const pos, struct Vector4f* const data);

void read_v4f_h(const uint8_t* const buffer, size_t* const pos, struct Vector4f* const data);

uint8_t read_robot_command_fixed(const uint8_t* const buffer, const size_t size, struct RobotCommand* const data);

uint8_t read_robot_command_fixed_V2(const uint8_t* const buffer, const size_t size, struct RobotCommand_V2* const data);

uint8_t read_robot_config_fixed(const uint8_t* const buffer, const size_t size, struct RobotConfig* const data);

uint8_t read_robot_matrix_fixed(const uint8_t* const buffer, const size_t size, struct RobotMatrix* const data);

uint8_t read_robot_feedback_fixed(const uint8_t* const buffer, const size_t size, struct RobotFeedback* const data);

uint8_t read_robot_feedback_custom_fixed(const uint8_t* const buffer, const size_t size, struct RobotFeedbackCustom* const data);

void read_float_h_from_2x_buf(const uint8_t* const buffer, float* const float_out);

#ifdef __cplusplus
}
#endif

#endif
