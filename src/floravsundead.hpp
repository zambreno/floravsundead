/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * floravsundead.cpp - FloraVsUndead application. Takes as input move files
 * and an optional zombie file and runs a Plants vs Zombies-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/


#pragma once



#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <iostream>
#include "resources.hpp"

#define EXEC_NAME "floravsundead"
#define WINDOW_TITLE "FloraVsUndead, v-1.0a"

#define ERR_USAGE 1
#define ERR_SFML 2
#define ERR_NOFILE1 3
#define ERR_NOFILE2 4
#define ERR_NOFILE3 5
#define ERR_BADFILE1 6
#define ERR_BADFILE2 7
#define ERR_BADFILE3 8
#define ERR_BADFILE4 9
#define ERR_AUDIO 10
#define ERR_OVERBUDGET 20
#define ERR_NOMEM 50
#define ERR_UNDEFINED 100

#define FRAME_RATE 30
#define SCREEN_WIDTH_DEFAULT 1920
#define SCREEN_HEIGHT_DEFAULT 1080
#define SCREEN_DEPTH_DEFAULT 32
#define DEBUG_DEFAULT 0
#define ZOM_FNAME_DEFAULT "default.zom"
#define TEAM_FNAME_DEFAULT "default.fpl"

/* Globals for resource data (resources.cpp) */
extern std::string sfxFiles[NUM_SFX];
extern std::string musicFiles[NUM_MUSIC];
extern std::string texFiles[NUM_TEXTURES];

/* Function prototypes (utils.cpp) */
void strlower(char *in);

namespace fvu {

    /* Game configuration information */
    class Config {
        public:
            int32_t debug_level;
            uint16_t screen_width;
            uint16_t screen_height;
            uint8_t screen_depth;
            char *team_fname[4];
            char *zom_fname;
    };

    /* Texture structure, so that we can more easily swap textures out */
    class Texture {
        public:
            uint32_t width;
            uint32_t height;
            float scale;
    };

    /* Main Game class */
    class Game {
        public:
            /* Main functions (floravsundead.cpp) */
            Game(int argc, char **argv);
            ~Game();
            void init();
            void compileZombies();
            void compileTeams();
            void mainLoop();

            /* SFML functions (sfml_utils.cpp) */
            void initSFML();
            void loadResources();

            /* Utility functions (utils.cpp) */
            void print_help();
            void raise_error(uint32_t, const char *msg);
            void printConfig();
            void printStatus();

        private:
            fvu::Config myConfig;
            sf::ContextSettings mySettings;
            sf::RenderWindow myWindow;
            Texture myTextures[NUM_TEXTURES];
            GLuint myTextureHandles[NUM_TEXTURES];
            sf::Music myMusic[NUM_MUSIC];
            sf::SoundBuffer mySoundBuffers[NUM_SFX];
            sf::Sound mySound;
            sf::Clock myClock;

    };

} // namespace fvu

