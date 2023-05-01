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
npm run proxy api [your-device-ip]
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
