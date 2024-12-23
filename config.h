#include "keyboardHook.h"
#include <cstdint>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#pragma once

typedef unsigned int uint;

using std::string;

struct Hotkey {
    string name;
    uint16_t key;

    Hotkey(string n, uint16_t k) {
        name = n;
        key = k;
    }
};

class Config {
  public:
    Config(string file) {
        readConfig(file);
    }

    std::vector<Hotkey> hotkeys;

  private:
    KeyboardHook *kh = KeyboardHook::get();
    void readConfig(string filePath);
};