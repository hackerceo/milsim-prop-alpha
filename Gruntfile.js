module.exports = function(grunt) {

    // Project configuration.
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        uglify: {
            options: {
                banner: '/*! <%= pkg.name %> <%= grunt.template.today("yyyy-mm-dd") %> */\n'
            },
            build: {
                src: 'src/<%= pkg.name %>.js',
                dest: 'build/<%= pkg.name %>.min.js'
            }
        }
    });



    grunt.registerTask('set-gzip', 'Enables/disables gzip compression of hosted files', function() {
        const configFile = 'build-config.json';
        const jsonfile = require('jsonfile');
        const optEnable = grunt.option('enable');
        const optDisable = grunt.option('disable');
        let buildOptions = jsonfile.readFileSync(configFile);
        if (!optEnable && !optDisable) {
            if (buildOptions.options.gzipWebFiles) {
                grunt.log.writeln("Web deployment files ARE compressed.");
            } else {
                grunt.log.writeln("Web deployment files ARE NOT compressed.");
            }
            return;
        }
        if (optEnable && !optDisable) {
            grunt.log.writeln("SAVED: Web deployment files WILL be compressed.");
            buildOptions.options.gzipWebFiles = true;
        }
        if (!optEnable && optDisable) {
            grunt.log.writeln("SAVED: Web deployment files WILL NOT be compressed.");
            buildOptions.options.gzipWebFiles = false;
        }
        jsonfile.writeFileSync(configFile, buildOptions, { spaces: 2 });
    });
    grunt.registerTask('set-games', 'Configures what game modes will be compiled into the distribution', function() {
        const configFile = 'build-config.json';
        const jsonfile = require('jsonfile');
        const fs = require("fs");

        let buildOptions = jsonfile.readFileSync(configFile);
        let gamesList = buildOptions.games;

        // remove games from the list
        let delGames = grunt.option('del');
        if (delGames) {
            delGames = delGames.split(',');
            if (delGames.length > 0) {
                gamesList = gamesList.filter((game) => {
                    return !delGames.includes(game);
                });
            }
        }

        // adds games to the list
        let addGames = grunt.option('add');
        if (addGames) {
            addGames = addGames.split(',');
            addGames.forEach((game) => {
                game = game.toLowerCase();
                if (game == 'core') return;
                // make sure the game exists within the src directory
                if (fs.existsSync('src/'+game)) {
                    grunt.log.writeln('ADDED: ' + game);
                    gamesList.push(game);
                } else {
                    grunt.log.writeln('ERROR: "' + game + '" does not have a source code directory!');
                }
            });
        }

        // remove duplicates
        gamesList = gamesList.filter((value, idx, self) => {
            return self.indexOf(value) === idx;
        });

        if (delGames || addGames) {
            buildOptions.games = gamesList;
            jsonfile.writeFileSync(configFile, buildOptions, { spaces: 2 });
        }
        // print current list of games
        console.dir(gamesList);
    });


    // Load the plugin that provides the "uglify" task.
    grunt.loadNpmTasks('grunt-contrib-uglify');

    // Default task(s).
    grunt.registerTask('default', ['run-web']);

};