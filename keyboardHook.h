#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <linux/input.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#pragma once

#define PRESSED 1
#define RELEASED 0

template <typename T>
bool inVector(const T &needle, const std::vector<T> &haystack) {
    for (const T &e : haystack) {
        if (needle == e) {
            return true;
        }
    }

    return false;
}

class KeyboardHook {
  public:
    static KeyboardHook *get();
    ~KeyboardHook() {
        return;
    }
    KeyboardHook(const KeyboardHook &obj) = delete;

    void registerKey(uint16_t key);
    bool isActive(uint16_t key);
    void run();

  private:
    static KeyboardHook *instance;
    KeyboardHook(){};

    const char *device = "/dev/input/event6"; // Replace with your keyboard device
    struct input_event ev;
    int fd = open(device, O_RDONLY);

    std::vector<uint16_t> registeredKeys;
    std::vector<uint16_t> activeKeys;
};