#include "rocket/common/config.h"
#include <tinyxml/tinyxml.h>


//创建宏，读取xml文件内容
#define READ_XML_NODE(name,parent)\
TiXmlElement * name##_node = parent->FirstChildElement(#name);\
    if(!name##_node){\
        printf("Start rocket server error,failed to read node [%s]\n",#name);\
        exit(0);\
    }\

#define READ_STR_FROM_XML_NODE(name,parent)\
TiXmlElement * name##_node = parent->FirstChildElement(#name);\
    if(!name##_node||!name##_node->GetText()){\
        printf("Start rocket server error,failed to read node [%s]\n",#name);\
        exit(0);\
    } \
    std::string  name##_str = std::string(name##_node->GetText());\



namespace rocket{


static Config* g_config = NULL;

Config::Config(const char* xmlfile){//Config初始化并读取
    TiXmlDocument* xml_document = new TiXmlDocument();
    bool rt = xml_document->LoadFile(xmlfile);
    if(!rt){
        printf("Start rocket server error,failed to read config file %s\n",xmlfile);
        exit(0);
    }

    // TiXmlElement * root_node = xml_document->FirstChildElement("root");
    // if(!root_node){
    //     printf("Start rocket server error,failed to read node [%s]\n","root");
    //     exit(0);
    // }

    // TiXmlElement * log_node = root_node->FirstChildElement("root");
    // if(!root_node){
    //     printf("Start rocket server error,failed to read node [%s]\n","log");
    //     exit(0);
    // } 
    READ_XML_NODE(root,xml_document);
    READ_XML_NODE(log,root_node);
    READ_STR_FROM_XML_NODE(log_level,log_node);
    // TiXmlElement * log_level_node = log_node->FirstChildElement("log_level");
    // if(!log_level_node||!log_level_node->GetText()){
    //     printf("Start rocket server error,failed to read node [%s]\n","log_level");
    //     exit(0);
    // } 
    // std::string  log_level = std::string(log_level_node->GetText());
    m_log_level = log_level_str;
}

void Config::setGlobalConfig(const char* xmlfile){
    if(g_config == NULL){
        g_config = new Config(xmlfile);
    }
}

Config* Config::getGlobalConfig(){
    return g_config;
}


}