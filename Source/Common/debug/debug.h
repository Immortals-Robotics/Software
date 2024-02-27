#pragma once
#include "../network/udp_server.h"

#include "../Vector.h"

struct Color
{
    Color(const int t_r, const int t_g, const int t_b)
        : r(t_r), g(t_g), b(t_b)
    {}

    int r, g, b;
};

class Debug
{
private:
    Debug(NetworkAddress t_address, bool t_enabled);
    ~Debug() = default;

    friend struct Services;

public:
    Debug(const Debug &)            = delete;
    Debug &operator=(const Debug &) = delete;

    void broadcast();

    // Plotting
    void plotPoint(std::string_view t_func_name, double t_x, double t_y, std::string_view t_point_name = "") const;

    void drawPoint(TVec2 t_pos, std::string_view t_layer = "point_0", Color t_color = Color(255, 0, 0)) const;
    void drawLineSegment(TVec2 t_pos1, TVec2 t_pos2, std::string_view t_layer = "line_0",
                  Color t_color = Color(0, 255, 0)) const;
    void drawRect(const TVec2& p, float w, float h, std::string_view t_layer = "rect_0", Color t_color = Color(0, 0, 255)) const;
    void drawCircle(const TVec2 &center, float r, std::string_view t_layer = "cir_0",
                    Color t_color = Color(255, 255, 0)) const;
    void drawRobot(TVec2 t_pos, float t_angle, int t_id, std::string_view t_layer = "bot_0",
                   Color t_color = Color(255, 255, 0)) const;

    // Text Logging
    void logMessage(std::string_view t_title, std::string_view t_text) const;
    void logMessage(std::string_view t_title, double t_num) const;

protected:
    bool m_enabled;

private:
    static inline Debug* m_instance;

    // UDP_connection
    NetworkAddress             m_address;
    std::unique_ptr<UdpServer> m_udp;

    std::unique_ptr<Imm_DBG_wrapper> m_wrapper;

    uint32_t m_last_sent_frame_id = 0;
};
