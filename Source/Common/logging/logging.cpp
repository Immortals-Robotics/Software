#include "logging.h"

#include <iomanip>
#include <iostream>
#include <regex>

#include <quill/Quill.h>

namespace Immortals
{
// Filter class for our file handler
class LogFilter final : public quill::FilterBase
{
private:
    quill::LogLevel m_log_level;

public:
    explicit LogFilter(const quill::LogLevel t_log_level) : FilterBase("LogFilter"), m_log_level(t_log_level){};

    QUILL_NODISCARD bool filter(char const *t_thread_id, std::chrono::nanoseconds t_log_message_timestamp,
                                quill::MacroMetadata const &t_metadata,
                                quill::fmt_buffer_t const  &t_formatted_record) noexcept override
    {
        return t_metadata.level() >= m_log_level;
    }
};

Logger::Logger()
{
    // set configuration
    // Config using the custom ts class and the stdout handler
    quill::Config cfg;

    // stdout handler
    std::shared_ptr<quill::Handler> stdout_handler = quill::stdout_handler(); // for stdout
    stdout_handler->set_pattern("%(ascii_time) [%(thread)] %(fileline:<28) %(level_name) %(message)",
                                "%Y-%m-%d %H:%M:%S.%Qms");
    // Enable console colors
    dynamic_cast<quill::ConsoleHandler *>(stdout_handler.get())->enable_console_colours();
    stdout_handler->add_filter(std::make_unique<LogFilter>(quill::LogLevel::Debug));
    cfg.default_handlers.emplace_back(stdout_handler);

    // Find the target path for the log file
    const std::filesystem::path log_file_path = getNewLogFilePath();
    // file handler
    std::shared_ptr<quill::Handler> file_handler = quill::file_handler(log_file_path, "w"); // for writing to file
    file_handler->set_pattern("%(ascii_time) [%(thread)] %(fileline:<28) %(level_name) %(message)",
                              "%Y-%m-%d %H:%M:%S.%Qms");
    file_handler->add_filter(std::make_unique<LogFilter>(quill::LogLevel::TraceL3));
    cfg.default_handlers.emplace_back(file_handler);

    configure(cfg);

    // Change the LogLevel to print everything
    // No need to worry, The debugger
    quill::Logger *logger = quill::get_logger();
    logger->set_log_level(quill::LogLevel::TraceL3);

    // Start the logging backend thread
    quill::start();

    LOG_INFO("Initialized the logger");
}

std::filesystem::path Logger::getNewLogFilePath() const
{
    const std::filesystem::path log_dir(LOG_DIR);
    if (!std::filesystem::is_directory(log_dir))
    {
        std::cout << "Log directory doesn't exist. Going to create it under: " << LOG_DIR << std::endl;
        std::filesystem::create_directory(log_dir);
    }
    unsigned log_file_idx = 0;
    for (const auto &log_entry : std::filesystem::directory_iterator(LOG_DIR))
    {
//        std::cout << log_entry.path() << std::endl;

        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << log_file_idx;

        std::regex log_file_name("/ai_log_" + ss.str(), std::regex_constants::ECMAScript | std::regex_constants::icase);
        if (std::regex_search(log_entry.path().string(), log_file_name))
        {
            //            std::cout << "Path shows a log file: " <<  log_entry.path() << std::endl;
            log_file_idx++;
        }
    }
        std::cout << "Found " << log_file_idx << " log files." << std::endl;

    std::time_t t  = std::time(nullptr);
    std::tm     tm = *std::localtime(&t);

    std::stringstream ss;
    ss << LOG_DIR << "/ai_log_" << std::setw(4) << std::setfill('0') << log_file_idx
        << std::put_time(&tm, "_%Y_%m_%d__%H_%M_%S") << ".imm_ai_log";

    std::string new_log_file_path = ss.str();
    std::cout << "Creating new log file:  " << new_log_file_path << std::endl;

    std::filesystem::path log_file(new_log_file_path);
    return log_file;
}
} // namespace Immortals
