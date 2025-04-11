#pragma once

#include <iosfwd>
#include <string>

namespace stm {

enum class log_level {
    debug = 0,
    info = 1,
    warn = 2,
    error = 3,
    silent = 4,
};

class logger {
public:
    explicit logger(std::ostream& output, log_level threshold = log_level::info);

    void set_level(log_level threshold);
    log_level level() const;

    void debug(const std::string& message) const;
    void info(const std::string& message) const;
    void warn(const std::string& message) const;
    void error(const std::string& message) const;
    void log(log_level level, const std::string& message) const;

private:
    std::ostream* output_;
    log_level threshold_;
};

}  // namespace stm
