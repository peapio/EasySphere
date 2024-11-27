#include "config.h"
#include "const.h"



ConfigReader& ConfigReader::getInstance()
{
    static ConfigReader instance;
    return instance;
}

int ConfigReader::loadConf(const std::string& confFile) {
    std::ifstream file(confFile);
    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << confFile << std::endl;
        return 1;
    }

    std::string line, currentSection;
    ConfigData configData;
    configData.clear();

    while (std::getline(file, line)) {
        trim(line);
        if (line.empty() || line[0] == ';') continue;

        if (line[0] == '[' && line.back() == ']') {
            if (!configData.empty())
            {
                m_sections.push_back(SectionData{currentSection, configData});
                configData.clear();
            }
            currentSection = line.substr(1, line.size() - 2);
        } else if (!currentSection.empty()) {
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = line.substr(0, equalPos);
                std::string value = line.substr(equalPos + 1);
                trim(key);
                trim(value);
                configData[key] = value;
            }
        }
    }

    file.close();
    return 0;
}

std::string ConfigReader::conf(const std::string& key, const std::string& sectionName)
{
    for (auto& section : m_sections) {
        if (section.sectionName == sectionName) {
            std::string value = getValue(section.configData, key);
            if (!value.empty()) {
                return value;
            }
        }
    }
    return "";
}

std::string ConfigReader::getValue(ConfigData &configData, const std::string& key)
{
    if (configData.find(key) != configData.end())
    {
        return configData[key];
    }
    return ""; // Return empty string if not found
}


void ConfigReader::trim(std::string& str) {
    str.erase(0, str.find_first_not_of(" \t")); // Trim left
    str.erase(str.find_last_not_of(" \t") + 1); // Trim right
}
