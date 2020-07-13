/*!
 * @file Logger.hxx
 *
 * @brief 程序日志输出
 *
 * 使用fmtlib提供的高效的格式化输出方法，增加了日志级别、运行时间显示，可以指定日志显示级别来抑制低级别日志显示。
 *
 * 目前可以使用的级别有（从小到大）：verbose，debug，detail，trace，info，warning，error，运行时间的单位是秒，保留三位小数
 */

#ifndef UTILS_LOGGER_HXX
#define UTILS_LOGGER_HXX

#include <chrono>
#include <iostream>
#include "../../3rd-party/include/fmt/format.h"
#include "../../3rd-party/include/fmt/printf.h"

std::string elapsedTime ();

enum LogLevel : uint8_t {
    Verbose = 0,
    Debug,
    Detail,
    Trace,
    Info,
    Warning,
    Error,
};

/*!
 * covert level enum to string
 */
auto LogLevel2String = [] (LogLevel level) -> std::string {
    switch (level) {
        case Verbose:
            return "[Verbose]";
        case Debug:
            return "[Debug]";
        case Detail:
            return "[Detail]";
        case Trace:
            return "[Trace]";
        case Info:
            return "[Info]";
        case Warning:
            return "[Warning]";
        case Error:
            return "[Error]";
        default:
            throw std::invalid_argument(fmt::format("Unsupported log level: {}.\n", (int)level));
    }
};

class Logger {
private:
    template<typename... Args> static std::string format(const std::string &level, Args&&... args);

public:
    static LogLevel localDisplayLevel;

    template<typename... Args> static void print(LogLevel level, Args&&... args);
    template<typename... Args> static void verbose_print(Args&&... args);
    template<typename... Args> static void debug_print(Args&&... args);
    template<typename... Args> static void detail_print(Args&&... args);
    template<typename... Args> static void trace_print(Args&&... args);
    template<typename... Args> static void info_print(Args&&... args);
    template<typename... Args> static void warning_print(Args&&... args);
    template<typename... Args> static void error_print(Args&&... args);
};

/*!
 * call for fmtlib and generate string with elapsed time
 * @tparam Args
 * @param level log level
 * @param args log strings
 * @return
 */
template<typename... Args> inline
std::string Logger::format(const std::string &level, Args&&... args) {
    return fmt::format("{}{} {}", elapsedTime(), level, fmt::format(std::forward<Args>(args)...));
}

/*!
 * generate string to print
 * @tparam Args
 * @param level log level
 * @param args log strings
 */
template<typename... Args> inline
void Logger::print(LogLevel level, Args&&... args) {
    if (localDisplayLevel > level) return;

    auto string = format(LogLevel2String(level), std::forward<Args>(args)...);

    if (localDisplayLevel <= level) {
        std::cout << string << std::endl;
    }
}

template<typename... Args> inline
void Logger::verbose_print(Args&&... args) {
    print(Verbose, std::forward<Args>(args)...);
}

template<typename... Args> inline
void Logger::debug_print(Args&&... args) {
    print(Debug, std::forward<Args>(args)...);
}

template<typename... Args> inline
void Logger::detail_print(Args&&... args) {
    print(Detail, std::forward<Args>(args)...);
}

template<typename... Args> inline
void Logger::trace_print(Args&&... args) {
    print(Trace, std::forward<Args>(args)...);
}

template<typename... Args> inline
void Logger::info_print(Args&&... args) {
    print(Info, std::forward<Args>(args)...);
}

template<typename... Args> inline
void Logger::warning_print(Args&&... args) {
    print(Warning, std::forward<Args>(args)...);
}

template<typename... Args> inline
void Logger::error_print(Args&&... args) {
    print(Error, std::forward<Args>(args)...);
}

#endif //UTILS_LOGGER_HXX
