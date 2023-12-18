#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H

#include <string>
#include<queue>
#include <memory>
#include "rocket/common/mutex.h"
namespace rocket{

template<typename... Args>//可变参数模板实现消息格式化打印
std::string formatString(const char* str, Args&&... args){//传入字符串和可变参数，（就是说后面的参数是可变的，数量和类型）
    int size = snprintf(nullptr,0,str,args...);//nullptr表示不需要结果，目的是获取整个格式化后的一个size
    std::string result;
    if(size>0){
        result.resize(size);
        snprintf(&result[0],size+1,str,args...);
    }
    return result;
}

//定义打印宏，用于在test_log.cpp中打印日志消息
#define DEBUGLOG(str,...)\
    if(rocket::Logger::GetGlobaLogger()->getLogLevel()<=rocket::Debug)\
    {\
        rocket::Logger::GetGlobaLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Debug))->toString()\
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str,##__VA_ARGS__)+"\n");  \
        rocket::Logger::GetGlobaLogger()->log();  \
    }\

#define INFOLOG(str,...)\
    if(rocket::Logger::GetGlobaLogger()->getLogLevel()<=rocket::Info)\
    {\
        rocket::Logger::GetGlobaLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Info))->toString()\
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str,##__VA_ARGS__)+"\n");  \
        rocket::Logger::GetGlobaLogger()->log();  \
    }\

#define ERRORLOG(str,...)\
    if(rocket::Logger::GetGlobaLogger()->getLogLevel()<=rocket::Error)\
    {\
        rocket::Logger::GetGlobaLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Error))->toString()\
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str,##__VA_ARGS__)+"\n");  \
        rocket::Logger::GetGlobaLogger()->log();  \
    }\



enum LogLevel{
    Unknow = 0,
    Debug = 1,
    Info = 2,
    Error = 3
};


class Logger{
public:
    typedef std::shared_ptr<Logger> s_ptr;

    Logger(LogLevel level):m_set_level(level){};//??????????
    void pushLog(const std::string &msg);
    void log();
    LogLevel getLogLevel()const{
        return m_set_level;
    }

public:
    static Logger* GetGlobaLogger();
    static Logger* InitGlobaLogger();

private:
    LogLevel m_set_level;
    std::queue<std::string> m_buffer;
    Mutex mutex;
};

std::string LogLevelToString(LogLevel level);
LogLevel StringToLogLevel(std::string log_level);


class LogEvent{
public:
    LogEvent(LogLevel level):m_level(level){};//有参构造，为啥要继承m_level??????????

    std::string getFileName() const{
        return m_file_name;
    }

    LogLevel getLogLevel() const {
        return m_level;
    }

    std::string toString();

private:
    std::string m_file_name;
    std::string m_file_line;
    int32_t m_pid;
    int32_t m_thread_id;
    //std::string m_time;
    LogLevel m_level;

    //Logger::s_ptr m_logger;

};



}




#endif // ROCKET_COMMON_LOG_H