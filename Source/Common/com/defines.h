#ifndef DEFINES_H
#define DEFINES_H

#define MAX_PAYLOAD_SIZE 32
#define MAX_ON_BOARD_SIZE 64

#define PROTO_VERSION_FIXED    0x1
#define PROTO_VERSION_VAR      0x2

#define TYPE_COMMAND           0x1
#define TYPE_CONFIG_CONTROL    0x2
#define TYPE_CONFIG_SHOOT      0x3
#define TYPE_CONFIG_ON_BOARD   0x4
#define TYPE_MATRIX            0x5
#define TYPE_FEEDBACK_BASE     0x6
#define TYPE_FEEDBACK_MAX      0x9

#define MESSAGE_HEADER(v,t) (((v) << 4) | (t))
#define MESSAGE_VERSION(a) ((a) >> 4)
#define MESSAGE_TYPE(a) ((a) & 0x0F)

#define PARSE_RESULT_SUCCESS 0
#define PARSE_RESULT_HEADER_CORRUPTED 1
#define PARSE_RESULT_SIZE_MISMATCH 2

#endif
