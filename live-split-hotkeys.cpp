#include "config.h"
#include "keyboardHook.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

using std::cout;
using std::string;

int main() {

    KeyboardHook *keyboardHook = KeyboardHook::get();
    Config config("./exampleInput.yml");
    cout << "Config file readed\n";

    while (true) {
        keyboardHook->run();

        for (const Hotkey &hk : config.hotkeys) {
            if (keyboardHook->isActive(hk.key)) {
                cout << hk.name << " active\n";
            }
        }
    }

    return 0;
}