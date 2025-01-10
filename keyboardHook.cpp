#include "keyboardHook.h"
#include <string>

// Initialize the static instance pointer to nullptr
KeyboardHook *KeyboardHook::instance = nullptr;

KeyboardHook *KeyboardHook::get() {
    if (nullptr == instance) {
        instance = new KeyboardHook();
    }
    return instance;
}

void KeyboardHook::hook(int kb_num) {
    std::string dev = "/dev/input/event" + std::to_string(kb_num);
    fd = open(dev.c_str(), O_RDONLY);
}

void KeyboardHook::hook(std::string kb_path) {
    fd = open(kb_path.c_str(), O_RDONLY);
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