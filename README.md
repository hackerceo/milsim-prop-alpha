# Milsim-Prop-Alpha1
![License](https://img.shields.io/github/license/hackerceo/milsim-prop-alpha) 
![Maintenance](https://img.shields.io/maintenance/yes/2023) 
![GitHub last commit](https://img.shields.io/github/last-commit/hackerceo/milsim-prop-alpha) 
![Lines of code](https://img.shields.io/tokei/lines/github/hackerceo/milsim-prop-alpha) 
![GitHub contributors](https://img.shields.io/github/contributors-anon/hackerceo/milsim-prop-alpha) 

This project is an open-source game prop for military simulation games such as airsoft, paintball and others.  It is designed to run on the ESP-family of IoT devices created by Espressif Systems. It specifically targets the [ESP32](https://www.wikipedia.org/wiki/ESP32) and ESP32S modules which can be bought from a wide range of sources.  

These controllers have build in WiFi which enables the prop to be configured and controlled from a smart phone using a web browser.

# Patreon
If you want to support me in developing this project please join my Patreon at https://www.patreon.com/nbenik. Every little bit helps with the parts, board development, shipping and energy drinks required to keep pounding through this big project.

# Table of Contents
* [Licenses](#licenses)
  + [Artwork](#artwork)
  + [Audio Files](#audio-files)
* [Setup](docs/BUILD.md)
* Hardware
  + [Schematic Diagrams](docs/hardware/Schematic.svg)
  + Components
    - [LCD (4 lines x 20 char)](https://www.aliexpress.com/item/1704800307.html)
    - [ADXL345 Accelerometer](https://www.aliexpress.com/item/32843886686.html)
    - [RC522 RFID Card Reader](https://www.aliexpress.us/item/3256802845719808.html)
    - [Halo Buttons](https://www.aliexpress.com/item/4001362443186.html)
    - [Electric Key Switches](https://www.aliexpress.com/item/4000851248444.html)
    - [DFPlayer audio component](https://www.dfrobot.com/product-1121.html)
    - [ESP32 Module](https://www.aliexpress.com/item/32959541446.html) (uses-30pin model)
    - [Buck SMPS](https://www.aliexpress.com/item/1005001922186305.html)
* Firmware
  + [Architecture](docs/core/ARCHITECTURE.md)
  + [Core Engine](docs/core/ENGINE.md)
  + [Game Engines](docs/core/GAMES.md)
* Game Modes
  + [Domination](docs/games/domination/README.md)
  + [Sabotage](docs/games/sabotage/README.md)
  + [Heist](docs/games/heist/README.md)
  + [Contagion](docs/games/contagion/README.md)
  + [Broken Arrow](docs/games/brokenarrow/README.md)
  + [Broken Quiver](docs/games/brokenquiver/README.md)
  + [BioWeapon](docs/games/bioweapon/README.md)


# Licenses

Designed by Nick Benik for STEALTH_CORP. STEALTH_CORP owns all protected IP. Everything that is not otherwise stated as being protected IP is released under permissive copyleft licenses.

| Sector        | License      | Verison |
| ------------- | ------------ | -------:|
| Hardware      | [CERN-OHL-S](docs/licenses/CERN-OHL-S.txt) |     2.0 |
| Firmware      | [GNU GPL](docs/licenses/GNU-GPL.txt)    |     3.0 |
| Documentation | [CC BY-SA](docs/licenses/CC-BY-SA.txt)   |     4.0 |
| Game Stories  | [CC BY-SA](docs/licenses/CC-BY-SA.txt)   |     4.0 |
| Audio Content | See LICENSE files in MP3 directories | |

Files of all licenses are required with the distribution of files. All files are available in easy-to-modify formats for remixing. 

### Artwork
The design and layout, the main artwork on the board and the PCB layers and traces, are *NOT* protected Intellectual Property ("IP"), see LICENSE files for appropriate details. Any additional writing or images such as the Brooklyn Tactical name, logo or copyright and designer notices, and certifications *ARE* considered protected IP and notice is given that they are protected by all applicable IP laws. 

### Audio Files
All associated audio files are licensed for use in the building and operation of the Milsim-Prop-Alpha1 product only!  Additional reuse of the audio clips in other products or the redistribution of audio clips is prohibited and in violation of our licensing and sub-licensing agreements. Please behave and don't get us sued. 
If you contribute your own audio files for this project you may agree to have your audio files used under a more permissive license if you document your file(s) and chosen license in the associated LICENSE.md file in the audio file's directory.

*TLDR; Audio files are not Open Source compatable.*


# Task List

- [x] Hardware design
- [x] PCB prototype
- [x] Commercial Product: 3D prints design
- [x] Commercial Product: prototype
- [ ] Commercial Product: field testing
- [x] Compile/Build system
- [x] Internationalization design/build
- [x] Firmware: main loop
- [ ] Firmware: wifi configurator
- [ ] Firmware: game mode selector
- [ ] Firmware: web server - game template
- [x] Firmware: Buttons Service
- [x] Firmware: LCD Service
- [x] Firmware: MP3 Service
- [ ] Design networking protocols
- [ ] Firmware: Networking Service
- [x] Firmware: RFID Service
- [x] Firmware: Accelerometer Service

- [ ] Game Mode: Team Picker & Traitor Picker

- [x] Game Mode: Domination - Story
- [x] Game Mode: Domination - Design
- [x] Game Mode: Domination - Coding

- [x] Game Mode: Sabotage - Story
- [x] Game Mode: Sabotage - Design
- [ ] Game Mode: Sabotage - Coding

- [x] Game Mode: Heist - Story
- [ ] Game Mode: Heist - Design
- [ ] Game Mode: Heist - Coding

- [x] Game Mode: Contagion - Story
- [ ] Game Mode: Contagion - Design
- [ ] Game Mode: Contagion - Coding

- [x] Game Mode: Broken Arrow - Story
- [ ] Game Mode: Broken Arrow - Design
- [ ] Game Mode: Broken Arrow - Coding

- [x] Game Mode: Broken Quiver - Story
- [ ] Game Mode: Broken Quiver - Design
- [ ] Game Mode: Broken Quiver - Coding

- [x] Game Mode: Bio Weapon - Story
- [ ] Game Mode: Bio Weapon - Design
- [ ] Game Mode: Bio Weapon - Coding
- [ ] Add-on prop: Bio Weapon - Bio Container
- [ ] Add-on prop: Bio Weapon - Bio Freezer

- [ ] Add-on module: Keypad
- [ ] Add-on module: Cut wire module
- [ ] Add-on module: Prop wired communicator
- [ ] Add-on module: GPS module
