module.exports = function(grunt) {

    // Project configuration.
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        copy: {
            temp: {
                files: [{
                    src: ['src/core/mcu/**/*'],
                    dest: 'deploy/',
                    expand: true,
                    flatten: true
                }, {
                    cwd: 'src/core/web',
                    src: ['**/*'],
                    dest: 'deploy/data/web/',
                    expand: true
                }, {
                    cwd: 'src/core/data/',
                    src: ['**/*'],
                    dest: 'deploy/data/data/core/',
                    expand: true
                }]
            }
        },
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


    // ===============================================================================================================//
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


    // ===============================================================================================================//
    grunt.registerTask('set-games', 'Configures what game modes will be compiled into the distribution', function() {
        const configFile = 'build-config.json';
        const jsonfile = require('jsonfile');

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
                if (grunt.file.isDir('src/'+game)) {
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



    // ===============================================================================================================//
    grunt.registerTask('clean', "Cleans out the distribution's deploy directory", function() {
        grunt.file.delete("deploy");
        grunt.file.mkdir('deploy');
    });


    // ===============================================================================================================//
    grunt.registerTask('gzip-dist', 'Gzips .js, .css, .html files in the deploy/data/web directory.', function() {
        const DATADIR = './deploy/data/web';
        const TARGET_EXT = [".js",".css", ".html"];


        var taskDone = this.async();
        var fs = require('fs');
        var path = require('path');
        var zlib = require('zlib');

        var walk = function(dir, done) {
            var results = [];
            fs.readdir(dir, function(err, list) {
                if (err) return done(err);
                var pending = list.length;
                if (!pending) return done(null, results);
                list.forEach(function(file) {
                    file = path.resolve(dir, file);
                    fs.stat(file, function(err, stat) {
                        if (stat && stat.isDirectory()) {
                            walk(file, function(err, res) {
                                results = results.concat(res);
                                if (!--pending) done(null, results);
                            });
                        } else {
                            if (TARGET_EXT.includes(path.extname(file))) {
                                results.push(file);
                            }
                            if (!--pending) done(null, results);
                        }
                    });
                });
            });
        };


        walk(DATADIR, function(err, results) {
            grunt.log.writeln("Found " + results.length + " files.");
            if (err) throw err;
            results.forEach((filename) => {
                fs.readFile(filename, 'utf8', (err, contents) => {
                    zlib.gzip(contents, (_, zContents) => {
                        fs.writeFile(filename + '.gz', zContents, (error) => {
                            console.log(filename);
                            fs.unlinkSync(filename);
                        });
                    });
                });
            });
            // wait an extra 5 seconds and then end the task
            setTimeout(()=>{
                taskDone(true);
            }, 5000);

        });
    });


    // ===============================================================================================================//
    grunt.registerTask('build', 'Builds the distribution into the deploy directory', function() {
        const configFile = 'build-config.json';
        const jsonfile = require('jsonfile');
        const path = require('path');

        let buildOptions = jsonfile.readFileSync(configFile);
        let gamesList = buildOptions.games;
        let files = grunt.config('copy.temp.files');


        gamesList.forEach((game) => {
            game = game.toLowerCase();

            // copy the MCU code for the selected games
            grunt.log.writeln('[' + game + '] MCU files...');
            files.push({
                src: ["src/" + game + "/mcu/**/*"],
                dest: "deploy/",
                expand: true,
                flatten: true
            });

            // copy the data files for the selected games
            grunt.log.writeln('[' + game + '] DATA files...');
            files.push({
                cwd: "src/" + game + "/data/",
                src: ["**/*"],
                dest: "deploy/data/data/" + game + "/",
                expand: true
            });

            // copy the web code for the selected games
            grunt.log.writeln('[' + game + '] WEB files...');
            files.push({
                cwd: "src/" + game + "/web/",
                src: ["**/*"],
                dest: "deploy/data/web/" + game + "/",
                expand: true
            });
        });

        grunt.config.set('copy.temp.files', files);
        grunt.task.run('copy:temp');

    });


    // Load the plugin that provides the "uglify" task.
    // ===============================================================================================================//
    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-contrib-copy');


};