# Linux LiveSplit Hotkeys
Linux app to restore global hotkeys to LiveSplit when run under wine/proton.

# Usage
Enable the TCP server in LiveSplit: Right Click -> Control -> Enable TCP server.

Next you need to know wich id does your keyboard have, for that run either: `ls -la /dev/input/by-id/` or `sudo evtest` and look for wich event number is assigned to your keyboard.
Once you know wich one is it, write it in the config file (instructions below)

Then open a terminal and run `sudo ./liveSplit-hotkeys`

## Configuration
You can configure wich keyboard to listen for and the hotkeys by editing the config file in the program folder.

To set wich keyboard to listen to, open `configFile.yml` and set the value of `keyboard`

In that same file, you can set the hotkeys by modifying the default values. Here is a list of wich values correspond to wich keys:

| Key | Id |
| :---: | :---: |
| ESC | 1 |
| 1 | 2 |
| 2 | 3 |
| 3 | 4 |
| 4 | 5 |
| 5 | 6 |
| 6 | 7 |
| 7 | 8 |
| 8 | 9 |
| 9 | 10 |
| 0 | 11 |
| MINUS | 12 |
| EQUAL | 13 |
| BACKSPACE | 14 |
| TAB | 15 |
| Q | 16 |
| W | 17 |
| E | 18 |
| R | 19 |
| T | 20 |
| Y | 21 |
| U | 22 |
| I | 23 |
| O | 24 |
| P | 25 |
| LEFTBRACE | 26 |
| RIGHTBRACE | 27 |
| ENTER | 28 |
| LEFTCTRL | 29 |
| A | 30 |
| S | 31 |
| D | 32 |
| F | 33 |
| G | 34 |
| H | 35 |
| J | 36 |
| K | 37 |
| L | 38 |
| SEMICOLON | 39 |
| APOSTROPHE | 40 |
| LEFTSHIFT | 42 |
| BACKSLASH | 43 |
| Z | 44 |
| X | 45 |
| C | 46 |
| V | 47 |
| B | 48 |
| N | 49 |
| M | 50 |
| COMMA | 51 |
| DOT | 52 |
| SLASH | 53 |
| RIGHTSHIFT | 54 |
| LEFTALT | 56 |
| SPACE | 57 |
| CAPSLOCK | 58 |
| F1 | 59 |
| F2 | 60 |
| F3 | 61 |
| F4 | 62 |
| F5 | 63 |
| F6 | 64 |
| F7 | 65 |
| F8 | 66 |
| F9 | 67 |
| F10 | 68
| NUMLOCK | 69 |
| SCROLLLOCK | 70 |
| KP7 | 71 |
| KP8 | 72 |
| KP9 | 73 |
| KPMINUS | 74 |
| KP4 | 75 |
| KP5 | 76 |
| KP6 | 77 |
| KPPLUS | 78 |
| KP1 | 79 |
| KP2 | 80 |
| KP3 | 81 |
| KP0 | 82 |
| KPDOT | 83 |
| F11 | 87 |
| F12 | 88 |
| RIGHTCTRL | 97 |
| RIGHTALT | 100 |
| HOME | 102 |
| UP | 103 |
| PAGEUP | 104 |
| LEFT | 105 |
| RIGHT | 106 |
| END | 107 |
| DOWN | 108 |
| PAGEDOWN | 109 |
| INSERT | 110 |
| DELETE | 111 |
| PAUSE | 119 |

### Example config file
```
---
keyboard: 22
hotkeys:
    Split: 57 #Space
    Reset: 19 #r
    Undo: 14 #Backspace
    Skip: 28 #Enter
    Pause: 119 #Pause
```
# Building
Install dependencies and just need to run `make`  

## Dependencies
For ubuntu:  
`sudo apt-get install libyaml-cpp-dev`

For Fedora:  
`sudo dnf install yaml-cpp-devel`  
Static:  
`sudo dnf install yaml-cpp-static libstdc++-static glibc-static`  

# Acknowledgements
[LiveSplit](https://livesplit.org/) for the amazing program.  
[descawed](https://github.com/descawed) for his [live-split-hotkeys](https://github.com/descawed/live-split-hotkeys) wich is basically the same but in rust and served as an inspiration.