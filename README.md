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
and [ESP8266](https://www.wikipedia.org/wiki/ESP8266) modules which can 
be bought from a wide range of sources.  These controllers have build in 
WiFi which enables the prop to be configured and controlled from a smart
phone using a web browser.


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