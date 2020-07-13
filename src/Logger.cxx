/*!
 * @file Logger.cxx
 * @brief 程序日志输出
 */

#include "../include/Logger.hxx"

LogLevel Logger::localDisplayLevel = Info;

/*!
 * 计时
 * @return 经过的时间
 */
std::string elapsedTime () {
    static auto startTime = std::chrono::system_clock::now();
    static auto elapsedTime = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - startTime;
    return fmt::sprintf("[%10.3f]", duration.count() * 1000);
}
