#include <sys/time.h>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include "rocket/common/log.h"
#include "rocket/common/util.h"
#include "rocket/common/config.h"



namespace rocket{

static Logger* g_logger = NULL;

Logger* Logger::GetGlobaLogger(){
    return g_logger;
}


Logger* Logger::InitGlobaLogger(){
    std::string st = Config::getGlobalConfig()->m_log_level;
    LogLevel global_log_level = StringToLogLevel(st);
    printf("Init log level [%s]\n",LogLevelToString(global_log_level).c_str());
    g_logger = new Logger(global_log_level);
    return g_logger;

}



std::string LogLevelToString(LogLevel level){
    switch (level)
    {
    case Debug:
        return "DEBUG";
    case Info:
        return "INFO";
    case Error:
        return "ERROR";   
    
    default:
        return "UNKNOW";
    }

}

LogLevel StringToLogLevel(const std::string log_level){
    if(log_level == "DEBUD"){
        return Debug;
    } else if(log_level == "INFO"){
        return Info;
    }else if(log_level == "ERROR"){
        return Error;
    }else{
        return Unknow;
    }

}

std::string LogEvent::toString(){
    struct timeval now_time;
    gettimeofday(&now_time,nullptr);//获取当前时间
    struct tm now_time_t;
    localtime_r(&(now_time.tv_sec),&now_time_t);//将当前时间传入到一个结构体中
    char buf[128];
    strftime(&buf[0],128,"%y-%m-%d %H:%M:%S",&now_time_t);//格式化带有当前时间的结构体（时间只能用结构体格式化）
    std::string time_str(buf);
    int ms = now_time.tv_usec /1000;
    time_str += std::to_string(ms);

    m_pid = getpid();
    m_thread_id = getThreadId();

    std::stringstream ss;

    ss << "[" << LogLevelToString(m_level) << "]\t"
        << "[" << time_str << "]\t"
        << "[" << m_pid << ";" << m_thread_id << "]\t";
    return ss.str();
    
}
void Logger::pushLog(const std::string& msg){
    ScopeMutext<Mutex> lock(mutex);
    m_buffer.push(msg);
    lock.unlock();
}


void Logger::log(){
    ScopeMutext<Mutex> lock(mutex);
    std::queue<std::string> tmp = m_buffer;
    m_buffer.swap(tmp);
    lock.unlock();

    while (!tmp.empty())
    {
        std::string msg = tmp.front();
        tmp.pop();
        printf("%s",msg.c_str());
    }
    
}

}