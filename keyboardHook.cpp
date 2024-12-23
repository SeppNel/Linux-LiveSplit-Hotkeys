#include "keyboardHook.h"
#include <string>

// Initialize the static instance pointer to nullptr
KeyboardHook *KeyboardHook::instance = nullptr;

KeyboardHook *KeyboardHook::get(int kb_num) {
    if (nullptr == instance) {
        instance = new KeyboardHook(kb_num);
    }
    return instance;
}

KeyboardHook::KeyboardHook(int kb_num) {
    std::string dev = "/dev/input/event" + std::to_string(kb_num);
    fd = open(dev.c_str(), O_RDONLY);
}

void KeyboardHook::registerKey(const uint16_t key) {
    registeredKeys.push_back(key);
}

bool KeyboardHook::isActive(const uint16_t key) {
    return inVector(key, activeKeys);
}

void KeyboardHook::run() {
    activeKeys.clear();
    ssize_t n = read(fd, &ev, sizeof(ev));
    if (n == (ssize_t)sizeof(ev) && ev.type == EV_KEY && ev.value == PRESSED) {
        if (inVector(ev.code, registeredKeys)) {
            activeKeys.push_back(ev.code);
        }
    }
}