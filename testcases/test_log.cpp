#include "rocket/common/log.h"
#include <pthread.h>
#include "rocket/common/config.h"

void* fun(void*){
    DEBUGLOG("this is thread in %s","fun");
    INFOLOG("this is thread in %s","fun");

    return NULL;
}
int main(){

    rocket::Config::setGlobalConfig("../conf/rocket.xml");
    rocket::Logger::InitGlobaLogger();
    pthread_t thread;
    pthread_create(&thread,NULL,&fun,NULL);
    DEBUGLOG("test Debug log %s","11");
    INFOLOG("test Info log %s","11");
    pthread_join(thread,NULL);
    return 0;
}