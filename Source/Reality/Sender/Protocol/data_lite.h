#ifndef DATA_LITE_H
#define DATA_LITE_H

#include "half.h"

struct Vector2f_V2
{
    union  FLOAT_32 x;
    union  FLOAT_32 y;
};

struct Vector3f
{
    union  FLOAT_32 x;
    union  FLOAT_32 y;
    union  FLOAT_32 z;
};

struct Vector4f
{
    union  FLOAT_32 x;
    union  FLOAT_32 y;
    union  FLOAT_32 z;
    union  FLOAT_32 w;
};

struct bits8_t
{
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
};

struct bits16_t
{
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
    uint8_t bit8 : 1;
    uint8_t bit9 : 1;
    uint8_t bit10 : 1;
    uint8_t bit11 : 1;
    uint8_t bit12 : 1;
    uint8_t bit13 : 1;
    uint8_t bit14 : 1;
    uint8_t bit15 : 1;
};

struct bits32_t
{
    uint8_t bit0  : 1;
    uint8_t bit1  : 1;
    uint8_t bit2  : 1;
    uint8_t bit3  : 1;
    uint8_t bit4  : 1;
    uint8_t bit5  : 1;
    uint8_t bit6  : 1;
    uint8_t bit7  : 1;
    uint8_t bit8  : 1;
    uint8_t bit9  : 1;
    uint8_t bit10 : 1;
    uint8_t bit11 : 1;
    uint8_t bit12 : 1;
    uint8_t bit13 : 1;
    uint8_t bit14 : 1;
    uint8_t bit15 : 1;
    uint8_t bit16 : 1;
    uint8_t bit17 : 1;
    uint8_t bit18 : 1;
    uint8_t bit19 : 1;
    uint8_t bit20 : 1;
    uint8_t bit21 : 1;
    uint8_t bit22 : 1;
    uint8_t bit23 : 1;
    uint8_t bit24 : 1;
    uint8_t bit25 : 1;
    uint8_t bit26 : 1;
    uint8_t bit27 : 1;
    uint8_t bit28 : 1;
    uint8_t bit29 : 1;
    uint8_t bit30 : 1;
    uint8_t bit31 : 1;
};

enum TeamColor
{
    TEAM_COLOR_BLUE = 0,
    TEAM_COLOR_YELLOW = 1
};

enum FeedbackRequestType
{
    FEEDBACK_TYPE_DEBUG = 0,
    FEEDBACK_TYPE_INFO = 1,
    FEEDBACK_TYPE_FATAL = 2,
    FEEDBACK_TYPE_CUSTOM = 3
};

enum ShootType
{
    SHOOT_TYPE_DIRECT = 0,
    SHOOT_TYPE_CHIP = 1
};

struct RobotCommand
{
    // commands
    struct  Vector2f_V2   velocity;
    union  FLOAT_32    omega;
    union  FLOAT_32    target_orientation;

    union  FLOAT_32    orientation;

    union  FLOAT_32    shoot_power;
    union  FLOAT_32    dribbler;
    union  FLOAT_32    servo;

    // debug
    uint8_t      beep;

    enum ShootType shoot_type;
    enum FeedbackRequestType feedback;
    uint8_t            halt : 1;
    uint8_t            has_orientation : 1;
};

struct RobotCommand_V2
{
    // commands
    struct  Vector2f_V2   velocity;
    union  FLOAT_32    omega;
    union  FLOAT_32    target_orientation;

    union  FLOAT_32    orientation;

    uint8_t    direct_power;
    uint8_t    chip_power;
    union  FLOAT_32    dribbler;

    enum FeedbackRequestType feedback_request;
    uint8_t            halt : 1;
    uint8_t            has_orientation : 1;
};

struct RobotConfig
{
    union FLOAT_32      kp;
    union FLOAT_32      ki;
    union FLOAT_32      kd;
    union FLOAT_32      i_limit;

    union FLOAT_32      head_offset;

    struct Vector3f   direct_coeffs;
    struct Vector3f   chip_coeffs;
};

struct RobotMatrix
{
    struct Vector3f matrix[4];
};

struct RobotFeedback
{
    union FLOAT_32     battery_voltage;
    union FLOAT_32     capacitor_voltage;

    union FLOAT_32     omega;
    union FLOAT_32     orientation;
    
    struct Vector4f  motor_velocity;
    struct Vector4f  motor_target;

    struct bits8_t  motor_fault;
    struct bits8_t button_status;

    uint8_t      fault : 1;
    uint8_t      ball_detected : 1;
    uint8_t      booster_enabled : 1;
    uint8_t      dribbler_connected : 1;
};

struct RobotFeedbackCustom
{
    uint32_t    length;
    void*     debug_dump;
};

#endif
