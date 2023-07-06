#pragma once
#include "setting.h"
#include "logging/logging.h"
#include "debug/debug.h"

struct Services
{
    static void initialize()
    {
        const ConfigReader config("config.toml");
        s_setting = new Setting();
        s_setting->load(config.getRoot());

        s_logger = new Logger();

        s_debug = new Debug(s_setting->debug_address, s_setting->enable_debug);
    }

    static void shutdown()
    {
        delete s_debug;
        delete s_setting;
        delete s_logger;
    }

    static const Setting &setting()
    {
        return *s_setting;
    }

    static Debug& debug()
    {
        return *s_debug;
    }

private:
    static inline Setting* s_setting;
    static inline Debug* s_debug;
    static inline Logger*  s_logger;
};

static const Setting &setting()
{
    return Services::setting();
}

static Debug& debug()
{
    return Services::debug();
}
