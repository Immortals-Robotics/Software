#ifndef DATA_LITE_H
#define DATA_LITE_H

#include <stdint.h>
#include <stdbool.h>

#include "half.h"
#include "defines.h"

struct vector2f_t
{
	union  float_32_u_t x;
	union  float_32_u_t y;
};

struct vector3f_t
{
	union  float_32_u_t x;
	union  float_32_u_t y;
	union  float_32_u_t z;
};

struct vector4f_t
{
	union  float_32_u_t x;
	union  float_32_u_t y;
	union  float_32_u_t z;
	union  float_32_u_t w;
};

struct bits8_t
{
	bool bit0 : 1;
	bool bit1 : 1;
	bool bit2 : 1;
	bool bit3 : 1;
	bool bit4 : 1;
	bool bit5 : 1;
	bool bit6 : 1;
	bool bit7 : 1;
};

struct bits16_t
{
	bool bit0 : 1;
	bool bit1 : 1;
	bool bit2 : 1;
	bool bit3 : 1;
	bool bit4 : 1;
	bool bit5 : 1;
	bool bit6 : 1;
	bool bit7 : 1;
	bool bit8 : 1;
	bool bit9 : 1;
	bool bit10 : 1;
	bool bit11 : 1;
	bool bit12 : 1;
	bool bit13 : 1;
	bool bit14 : 1;
	bool bit15 : 1;
};

struct bits32_t
{
	bool bit0  : 1;
	bool bit1  : 1;
	bool bit2  : 1;
	bool bit3  : 1;
	bool bit4  : 1;
	bool bit5  : 1;
	bool bit6  : 1;
	bool bit7  : 1;
	bool bit8  : 1;
	bool bit9  : 1;
	bool bit10 : 1;
	bool bit11 : 1;
	bool bit12 : 1;
	bool bit13 : 1;
	bool bit14 : 1;
	bool bit15 : 1;
	bool bit16 : 1;
	bool bit17 : 1;
	bool bit18 : 1;
	bool bit19 : 1;
	bool bit20 : 1;
	bool bit21 : 1;
	bool bit22 : 1;
	bool bit23 : 1;
	bool bit24 : 1;
	bool bit25 : 1;
	bool bit26 : 1;
	bool bit27 : 1;
	bool bit28 : 1;
	bool bit29 : 1;
	bool bit30 : 1;
	bool bit31 : 1;
};

enum team_color_e
{
	TEAM_COLOR_BLUE = 0,
	TEAM_COLOR_YELLOW = 1
};

enum feedback_type_e
{
	FEEDBACK_TYPE_DEBUG = 0,
	FEEDBACK_TYPE_INFO = 1,
	FEEDBACK_TYPE_FATAL = 2,
	FEEDBACK_TYPE_CUSTOM = 3
};

enum shoot_type_e
{
	SHOOT_TYPE_DIRECT = 0,
	SHOOT_TYPE_CHIP = 1
};

struct robot_command_msg_t
{
	// commands
	struct  vector2f_t   velocity;
	union  float_32_u_t    omega;
	union  float_32_u_t    target_orientation;

	union  float_32_u_t    orientation;

	union  float_32_u_t    shoot_power;
	union  float_32_u_t    dribbler;
	union  float_32_u_t    servo;

	// debug
	uint8_t      beep;

	enum shoot_type_e shoot_type;
	enum feedback_type_e feedback_request;
	bool            halt : 1;
	bool            has_orientation : 1;
};

struct robot_control_config_msg_t
{
	union float_32_u_t      motor_kp;
	union float_32_u_t      motor_ki;
	union float_32_u_t      motor_kd;
	union float_32_u_t      motor_i_limit;

	union float_32_u_t      gyro_kp;
	union float_32_u_t      gyro_ki;
	union float_32_u_t      gyro_kd;
	union float_32_u_t      gyro_i_limit;

	union float_32_u_t      max_w_acc;
	union float_32_u_t      max_w_dec;
};

struct robot_shoot_config_msg_t
{
	struct vector3f_t   direct_coeffs;
	struct vector3f_t   chip_coeffs;
};

struct robot_on_board_config_t
{
	struct robot_control_config_msg_t   control_config;
	struct robot_shoot_config_msg_t   shoot_config;
	union float_32_u_t gyro_offset;
	uint8_t nrf_channel_rx;
	uint8_t nrf_channel_tx;
	uint8_t use_encoders;
};

struct robot_matrix_msg_t
{
	struct vector3f_t matrix[4];
};

struct robot_feedback_msg_t
{
	union float_32_u_t     battery_voltage;
	union float_32_u_t     capacitor_voltage;

	union float_32_u_t     omega;
	union float_32_u_t     orientation;
	
	struct vector4f_t  motor_velocity;
	struct vector4f_t  motor_target;

	struct bits8_t  motor_fault;
	struct bits8_t button_status;

	unsigned robot_id : 4;

	bool      fault : 1;
	bool      ball_detected : 1;
	bool      booster_enabled : 1;
	bool      dribbler_connected : 1;
};

struct robot_feedback_custom_t
{
	uint8_t    length;
	void*     debug_dump;
};

struct robot_wrapper_msg_t
{
	uint8_t  length;
	uint8_t  data[MAX_PAYLOAD_SIZE - 1];
};

#endif