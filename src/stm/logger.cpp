#include "logger.hpp"

#include <ostream>

namespace stm {
namespace {

const char* level_label(log_level level) {
    switch (level) {
        case log_level::debug:
            return "DEBUG";
        case log_level::info:
            return "INFO";
        case log_level::warn:
            return "WARN";
        case log_level::error:
            return "ERROR";
        case log_level::silent:
            return "SILENT";
    }

    return "INFO";
}

}  // namespace

logger::logger(std::ostream& output, log_level threshold)
    : output_(&output), threshold_(threshold) {
}

void logger::set_level(log_level threshold) {
    threshold_ = threshold;
}

log_level logger::level() const {
    return threshold_;
}

void logger::debug(const std::string& message) const {
    log(log_level::debug, message);
}

void logger::info(const std::string& message) const {
    log(log_level::info, message);
}

void logger::warn(const std::string& message) const {
    log(log_level::warn, message);
}

void logger::error(const std::string& message) const {
    log(log_level::error, message);
}

void logger::log(log_level level, const std::string& message) const {
    if (!output_ || level < threshold_ || threshold_ == log_level::silent) {
        return;
    }

    (*output_) << "[" << level_label(level) << "] " << message << '\n';
}

}  // namespace stm
