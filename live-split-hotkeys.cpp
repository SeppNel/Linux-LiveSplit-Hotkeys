#include "argparse/argparse.hpp"
#include "config.h"
#include "keyboardHook.h"
#include "network.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

using std::cout;
using std::string;

#define INPUT_EVENT_PATH "/dev/input/" // standard path

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

// https://github.com/kernc/logkeys
// executes cmd and returns string ouput
std::string execute(const char *cmd) {
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        std::cout << "Pipe error\n";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    pclose(pipe);
    return result;
}

// https://github.com/kernc/logkeys
string determine_input_device() {
    // better be safe than sory: while running other programs, switch user to nobody
    setegid(65534);
    seteuid(65534);

    const char *cmd = "grep -B8 -E 'KEY=.*e$' /proc/bus/input/devices | "
                      "grep -E 'Name|Handlers|KEY' ";
    std::stringstream output(execute(cmd));

    std::vector<std::string> devices;
    std::vector<unsigned short> scores;
    std::string line;

    unsigned short line_type = 0;
    unsigned short score = 0;

    while (std::getline(output, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        // Generate score based on device name
        if (line_type == 0) {
            if (line.find("keyboard") != std::string::npos) {
                score += 100;
            }
        }
        // Add the event handler
        else if (line_type == 1) {
            std::string::size_type i = line.find("event");
            if (i != std::string::npos)
                i += 5; // "event".size() == 5
            if (i < line.size()) {
                int index = atoi(&line.c_str()[i]);

                if (index != -1) {
                    std::stringstream input_dev_path;
                    input_dev_path << INPUT_EVENT_PATH;
                    input_dev_path << "event";
                    input_dev_path << index;

                    devices.push_back(input_dev_path.str());
                }
            }
        }
        // Generate score based on size of key bitmask
        else if (line_type == 2) {
            std::string::size_type i = line.find("=");
            std::string full_key_map = line.substr(i + 1);
            score += full_key_map.length();
            scores.push_back(score);
            score = 0;
        }
        line_type = (line_type + 1) % 3;
    }

    if (devices.size() == 0) {
        std::cout << "Couldn't determine keyboard device. :/";
    }

    // Choose device with the best score
    int max_device = std::max_element(scores.begin(), scores.end()) - scores.begin();

    // now we reclaim those root privileges
    seteuid(0);
    setegid(0);

    return devices[max_device];
}

int main(int argc, char *argv[]) {
    std::unordered_map<string, string> commandsMap = {{"Split", "startorsplit"},
                                                      {"Reset", "reset"},
                                                      {"Undo", "unsplit"},
                                                      {"Skip", "skipsplit"},
                                                      {"Pause", "pause"}};

    Arguments args = handleArguments(argc, argv);
    Config config(args.configPath);
    cout << "Config file readed\n";

    KeyboardHook *keyboardHook = KeyboardHook::get();
    if (config.autoKeyboard) {
        cout << "Determining which keyboard to use\n";
        string kb = determine_input_device();
        keyboardHook->hook(kb);
    } else {
        cout << "Using keyboard specified in config file\n";
        keyboardHook->hook(config.kb_num);
    }

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