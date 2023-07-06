#pragma once

class Logger
{
protected:
    Logger();
    ~Logger() = default;

    friend struct Services;

private:
    std::filesystem::path getNewLogFilePath() const;
};
