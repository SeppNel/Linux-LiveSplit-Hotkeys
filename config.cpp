#include "config.h"
#include <cstdlib>

void Config::readConfig(const string &configPath) {
    try {
        YAML::Node configFile = YAML::LoadFile(configPath);

        kb_num = configFile["keyboard"].as<int>();
        kh = KeyboardHook::get(kb_num);

        for (auto node : configFile["hotkeys"]) {
            string hotkey = node.first.as<string>();
            uint16_t key = node.second.as<uint16_t>();

            hotkeys.emplace_back(hotkey, key);
            kh->registerKey(key);
        }

    } catch (YAML::BadFile) {
        std::cout << "Could not load config file\n";
        exit(EXIT_FAILURE);
    }
}