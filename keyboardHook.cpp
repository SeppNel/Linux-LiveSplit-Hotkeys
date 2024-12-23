#include "keyboardHook.h"

// Initialize the static instance pointer to nullptr
KeyboardHook *KeyboardHook::instance = nullptr;

KeyboardHook *KeyboardHook::get() {
    if (nullptr == instance)
        instance = new KeyboardHook;
    return instance;
}

void KeyboardHook::registerKey(uint16_t key) {
    registeredKeys.push_back(key);
}

bool KeyboardHook::isActive(uint16_t key) {
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