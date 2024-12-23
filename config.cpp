#include "config.h"

void Config::readConfig(string filePath) {
    std::string homeDir = getenv("HOME");
    // std::string configPath = homeDir + "/.config/LinuxLiveSplitHotkeys.yml";
    std::string configPath = "./exampleConfig.yml";

    try {
        YAML::Node configFile = YAML::LoadFile(configPath);

        for (auto node : configFile["hotkeys"]) {
            string hotkey = node.first.as<string>();
            uint16_t key = node.second.as<uint16_t>();

            hotkeys.emplace_back(hotkey, key);
            kh->registerKey(key);
        }

    } catch (YAML::BadFile) {
        std::cout << "Could not load config file\n";
    }
}