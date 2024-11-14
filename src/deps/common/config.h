#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>



typedef std::unordered_map<std::string, std::string> ConfigData;

typedef struct _section_data {
    std::string sectionName;
    ConfigData configData;
} SectionData;

class ConfigReader {

public:
    ConfigReader() = default;
    static ConfigReader& getInstance();
    int loadConf(const std::string &confFile);
    std::string conf(const std::string& section="default", const std::string& key);
    
private:
    std::string m_confPath;
    std::vector<SectionData> m_sections;

    void trim(std::string& str);
    std::string getValue(ConfigData &configData, const std::string &key);
};

#define gConf ConfigReader::getInstance()