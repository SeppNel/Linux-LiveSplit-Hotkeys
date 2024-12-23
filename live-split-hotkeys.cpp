#include "argparse/argparse.hpp"
#include "config.h"
#include "keyboardHook.h"
#include "network.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

using std::cout;
using std::string;

struct Arguments {
    string configPath = "./configFile.yml";
    string serverIp = "127.0.0.1";
    uint serverPort = 16834;
};

Arguments handleArguments(int argc, char *argv[]) {
    Arguments output;

    argparse::ArgumentParser program("Linux-LiveSplit-Hotkeys", "1.0.0");
    program.add_argument("-c")
        .help("Config file path")
        .default_value(string{"./configFile.yml"});
    program.add_argument("-a")
        .help("Server Ip Address")
        .default_value(string{"127.0.0.1"});
    program.add_argument("-p")
        .help("Server port")
        .default_value(uint{16834})
        .scan<'u', uint>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    output.configPath = program.get("-c");
    output.serverIp = program.get("-a");
    output.serverPort = program.get<uint>("-p");
    return output;
}

int main(int argc, char *argv[]) {
    std::unordered_map<string, string> commandsMap = {{"Split", "startorsplit"},
                                                      {"Reset", "reset"},
                                                      {"Undo", "unsplit"},
                                                      {"Skip", "skipsplit"},
                                                      {"Pause", "pause"}};

    Arguments args = handleArguments(argc, argv);
    Config config(args.configPath);
    KeyboardHook *keyboardHook = KeyboardHook::get(config.kb_num);

    cout << "Config file readed\n";

    int socketFd = initSocket(args.serverIp, args.serverPort);

    bool paused = false;
    while (true) {
        keyboardHook->run();

        for (const Hotkey &hk : config.hotkeys) {
            if (keyboardHook->isActive(hk.key)) {
                string command = commandsMap[hk.name];

                if (hk.name == "Pause") {
                    if (paused) {
                        command = "resume";
                    }
                    paused = !paused;
                }

                SendPacket(socketFd, command + "\r\n");
            }
        }
    }

    return 0;
}