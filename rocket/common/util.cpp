#include "rocket/common/util.h"
#include <sys/syscall.h> 

namespace rocket{

static int  g_pid = 0;//进程是公有的，全局变量即可
static thread_local int g_thread_id = 0;//线程是独有的，需要设置线程局部变量

pid_t getPid(){
    if(g_pid != 0){//????????缓存，当已经获取过进程id之后，直接返回进程id
        return g_pid;
    }
    return getpid();//调用getpid库函数，获取进程号
}

pid_t getThreadId(){
    if(g_thread_id != 0){//????????一样的
        return g_thread_id;
    }
    return syscall(SYS_gettid);//调用syscall库函数，获取线程号
}

}