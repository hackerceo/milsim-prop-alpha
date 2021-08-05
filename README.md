# Milsim-Prop-Alpha1
![License](https://img.shields.io/github/license/hackerceo/milsim-prop-alpha) 
![Maintenance](https://img.shields.io/maintenance/yes/2021) 
![GitHub last commit](https://img.shields.io/github/last-commit/hackerceo/milsim-prop-alpha) 
![Lines of code](https://img.shields.io/tokei/lines/github/hackerceo/milsim-prop-alpha) 
![GitHub contributors](https://img.shields.io/github/contributors-anon/hackerceo/milsim-prop-alpha) 

This project is an open-source game prop for military simulation games 
such as airsoft, paintball and others.  It is designed to run on the 
ESP-family of IoT devices created by Espressif Systems.
It specifically targets the [ESP32](https://www.wikipedia.org/wiki/ESP32)
and ESP32S modules which can be bought from a wide range of sources.  
These controllers have build in WiFi which enables the prop to be configured
and controlled from a smart phone using a web browser.


## Artwork
The design and layout, the main artwork on the board and the PCB layers and traces, are *NOT* protected Intellectual Property ("IP"), see LICENSE files for appropriate details. Any additional writing or images such as the STEALTH_CORP name, logo or copyright and designer notices, and certifications *ARE* considered protected IP and notice is given that they are protected by all applicable IP laws. 

## Audio Files
All associated audio files are licensed for use in the building and operation of the Milsim-Prop-Alpha1 product only!  Additional reuse of the audio clips in other products or the redistribution of audio clips is prohibited and in violation of our licensing and sub-licensing agreements. Please behave and don't get us sued. 
If you contribute your own audio files for this project you may agree to have your audio files used under a more permissive license if you document your file(s) and chosen license in the associated LICENSE.md file in the audio file's directory.

*TLDR; Audio files are not Open Source compatable.*


## Licence
Designed by Nick Benik for STEALTH_CORP. STEALTH_CORP owns all protected IP. Everything that is not otherwise stated as being protected IP (e.g. firmware, PCB design, documentation) is released under permissive copyleft licenses.

| Sector        | License      | Verison |
| ------------- | ------------ | -------:|
| Hardware      | [CERN-OHL-S] |     2.0 |
| Firmware      | [GNU GPL]    |     3.0 |
| Documentation | [CC BY-SA]   |     4.0 |
| Game Stories  | [CC BY-SA]   |     4.0 |
| Audio Content | See LICENSE files in MP3 directories | xxx|

Files of all licenses are required with the distribution of files. All files are available in easy-to-modify formats for remixing. 




### Setup
Clone the repo from Github and install the dependencies:
```
git clone http://github.com/hackerceo/Milsim-Prop-Alpha1.git
cd Milsim-Prop-Alpha1
npm install
```


### Running Web Development Proxy Server
This repo contains a specialized proxy server to allow you to dynamically 
develop the web-based interface on your local machine without needing to
build the project and/or upload changed web assets to the ESP 
microcontroller.
To start the controller run:
```
npm run-proxy [your-device-ip]
```


### Build Commands
The following commands can be used to configure and build a custom 
deployment for your prop. 

#### `set-gzip`
This command is used to configure whether the web files hosted on the ESP 
controller are compressed or not:
```
grunt set-gzip
grunt set-gzip --enable
grunt set-gzip --disable
```

#### `set-games`
This command is used to configure which game modes will be built on your
prop:
```
grunt set-games
grunt set-games --add=game1,game2,game3
grunt set-games --del=game1,game2,game3
grunt set-games --add=game1 --del=game2
```

#### `build`
This command is used to build the deploy directory with Arduino-compatable
code files as well as a data directory to be written to the SPIFF filesystem:
```
grunt build
```
NOTE: all files in the deploy directory will be deleted by this command!
