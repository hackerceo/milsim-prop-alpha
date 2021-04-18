module.exports = function(grunt) {

    // Project configuration.
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        copy: {
            temp: {
                files: []
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
        const DATADIR = './deploy/Arduino/data/web';
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
        const fs = require('fs');

        let buildOptions = jsonfile.readFileSync(configFile);
        let files = grunt.config('copy.temp.files');
        let mp3Dirs = [];
        let mp3FolderIdx = 0;

        // make sure that the default language is on top of the languages array
        let defaultLang = buildOptions.languages.splice(buildOptions.languages.find((lang) => { return lang.default; }),1)[0];
        buildOptions.languages.unshift(defaultLang);
        // build the C defines for language constants
        let code_LANGUAGES_DEFINES = "// ==== languages ==== /\n";
        buildOptions.languages.forEach((lang, lang_id)=> {
            code_LANGUAGES_DEFINES = code_LANGUAGES_DEFINES + "#define LANGUAGE_" + lang.code + "\t" + lang_id + ";\n";
        });
        // build the C defines for game mode constants
        let code_GAME_MODES = "// ==== game modes ==== /\n";



        console.dir(buildOptions);

        // for this part handle the core as a game
        buildOptions.games.unshift('core');

        // handle each game
        buildOptions.games.forEach((game, game_id) => {
            game = game.toLowerCase();

            // build the C defines for game mode constants
            code_GAME_MODES = code_GAME_MODES + "#define GAME_MODE_" + game + "\t" + game_id + ";\n";

            // copy MP3 files
            mp3Dirs[game_id] = [];
            let defaultLangIdx = 0;
            grunt.log.writeln('[' + game + '] MP3 files...');
            buildOptions.languages.forEach((lang, lang_id) => {
                if (grunt.file.isDir("src/" + game + "/mp3/" + lang.code + "/")) {
                    mp3FolderIdx++;
                    // directory exists, add it
                    let destDir = "deploy/SDcard/" + String(mp3FolderIdx).padStart(2, '0') + "/";
                    files.push({
                        cwd: "src/" + game + "/mp3/" + lang.code + "/",
                        src: ["*"],
                        dest: destDir,
                        expand: true
                    });
                    // create a labeling file
                    grunt.file.write(destDir + "desc.txt", "[" + game + "] -> " + lang.name);
                    if (defaultLangIdx == 0) {
                        defaultLangIdx = mp3FolderIdx;
                    }
                    // save folder id mapping
                    mp3Dirs[game_id][lang_id] = mp3FolderIdx;
                } else {
                    // missing translation, use default language
                    mp3Dirs[game_id][lang_id] = defaultLangIdx;
                }
            });

            // copy the MCU code for the selected games
            grunt.log.writeln('[' + game + '] MCU files...');
            files.push({
                src: ["src/" + game + "/mcu/**/*"],
                dest: "deploy/Arduino/",
                expand: true,
                flatten: true
            });

            // copy the data files for the selected games
            grunt.log.writeln('[' + game + '] DATA files...');
            files.push({
                cwd: "src/" + game + "/data/",
                src: ["**/*"],
                dest: "deploy/Arduino/data/data/" + game + "/",
                expand: true
            });

            // copy the web code for the selected games
            grunt.log.writeln('[' + game + '] WEB files...');
            files.push({
                cwd: "src/" + game + "/web/",
                src: ["**/*"],
                dest: "deploy/Arduino/data/web/" + game + "/",
                expand: true
            });
        });

        grunt.config.set('copy.temp.files', files);
        grunt.task.run('copy:temp');


        // build the C lookup matrix
        let code_LANGUAGES_MP3 = "int global_MP3_FOLDER[" + mp3Dirs.length + "][" + mp3Dirs[0].length + "] = " +
            String(JSON.stringify(mp3Dirs, null, 4)).replace(/\[/g, "{").replace(/\]/g, "}") + ";";

        // display generated code
        console.log(code_LANGUAGES_DEFINES);
        console.log(code_LANGUAGES_MP3);
        console.log(code_GAME_MODES);
    });


    // ===============================================================================================================//
    grunt.registerTask('web-deps', 'Adds web dependencies into the deploy directory', function() {
        let funcRename = function(dest, src) {
            return dest + src.replace(".min.", ".");
        };
        let newFiles = {
            files: [{
                cwd: 'node_modules/jquery/dist/',
                src: ['jquery.min.js'],
                dest: 'deploy/Arduino/data/web/js/',
                expand: true,
                rename: funcRename
            }, {
                cwd: 'node_modules/jquery-i18next/',
                src: ['jquery-i18next.min.js'],
                dest: 'deploy/Arduino/data/web/js/',
                expand: true,
                rename: funcRename
            }, {
                cwd: 'node_modules/jquery-mobile/dist/',
                src: ['jquery.mobile.min.js'],
                dest: 'deploy/Arduino/data/web/js/',
                expand: true,
                rename: funcRename
            }, {
                cwd: 'node_modules/jquery-mobile/dist/',
                src: [
                    'jquery.mobile.min.css',
                    'jquery.mobile.icons.min.css',
                    'jquery.mobile.structure.min.css',
                    'jquery.mobile.theme.min.css'
                ],
                dest: 'deploy/Arduino/data/web/css/',
                expand: true,
                rename: funcRename
            }, {
                cwd: 'node_modules/handlebars/dist/',
                src: [
                    'handlebars.amd.min.js',
                    'handlebars.min.js',
                    'handlebars.runtime.amd.min.js',
                    'handlebars.runtime.min.js'
                ],
                dest: 'deploy/Arduino/data/web/js/',
                expand: true,
                rename: funcRename
            }]
        };

        grunt.config.set('copy.webdeps', newFiles);
        grunt.task.run('copy:webdeps');
    });


    // Load the plugins
    // ===============================================================================================================//
    grunt.loadNpmTasks('grunt-contrib-copy');


};