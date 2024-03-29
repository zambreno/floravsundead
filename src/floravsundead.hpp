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
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include "resources.hpp"
#include "zombie.hpp"
#include "plants.hpp"
#include "particles.hpp"
#include "object.hpp"

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

#define FRAME_RATE 60
#define SORT_DEMO_FRAMES 600
#define SORT_GAME_FRAMES 60
#define NUM_SOUNDS 64
#define SCREEN_WIDTH_DEFAULT 1920
#define SCREEN_HEIGHT_DEFAULT 1080
#define SCREEN_DEPTH_DEFAULT 32
#define DEBUG_DEFAULT 0
#define ZOM_FNAME_DEFAULT "default.zom"
#define TEAM_FNAME_DEFAULT "default.fpl"
#define TIME_MS_DEFAULT 300000
#define DEMO_ZOMBIE_COUNT 40
#define BITE_FRAMES 45
#define FREEZE_LENGTH 150
#define CHOMPER_EAT_COUNT 2000
#define PLANT_MOVE_INACTIVE CHOMPER_EAT_COUNT/2

#define KILL_SCORE 5
#define ZOMBIE_SCORE -10

typedef enum {DEMO_START=0, DEMO_MID, DEMO_END, GAME_START, GAME_MID, GAME_END} MODE_TYPE;


/* Function prototypes (utils.cpp) */
void strlower(char *in);

namespace fvu {

    /* Player command type */
    class cmd_type {
        public:
            char label_str[16];
            char target_str[16];
            bool has_label;
            bool inv_pred;
            bool has_pred;
            bool has_plant_pred;
            uint8_t pred;
            uint8_t cmd;
            uint16_t plant;
            uint16_t plant_pred;
            uint32_t line;
            uint16_t opt[2];
    };

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


    /* Per-team state information */
    class Team {
        public:
            uint32_t status;
            char *name;
            uint16_t budget;
            uint16_t zombie_index;
            int32_t timer_ms;
            uint16_t cur_cmd;
            bool zombies_done;
            bool cmds_done;
            uint8_t groan_counter;
            std::vector<fvu::cmd_type> cmds;
    };


    /* Texture structure, so that we can more easily swap textures out */
    class Texture {
        public:
            uint32_t width;
            uint32_t height;
            float scale;
            GLuint texHandle;
            uint16_t (*spriteMap)[4];
    };

    /* Status structure that contains score and other details */
    class Status {
        public:
            float pan, pan_prev;
            MODE_TYPE mode;
            int16_t scores[4];
            float time_ms;
            uint16_t budget;
            uint8_t main_song;
            bool day;
            bool firstZombie;
            bool end_music;
            uint8_t vol_counter;
            uint16_t music_buffer;
    };


    /* Main Game class */
    class Game {
        public:
            /* Main functions (floravsundead.cpp) */
            Game(int argc, char **argv);
            ~Game();
            void init();
            void reset();
            void compileZombies();
            void compileTeams();
            void mainLoop();
            void updateGame();
            void updateDemo();
            void restartGame();
            void demoMode();
            void endGame();

            /* SFML functions (sfml_utils.cpp) */
            void initSFML();
            void loadResources();
            void drawWorld();
            void drawMap();
            void drawScoreboard();
            void processEvents();

            /* Utility functions (utils.cpp) */
            void print_help();
            void raise_error(uint32_t, const char *msg);
            float *getTexCoords(uint8_t texID, uint16_t spriteID, float *texCoords);
            void playSound(uint16_t sfxID, uint8_t vol, bool force);
            void printConfig();
            void printStatus();

            /* Some apsects of the game state are needed globally */
            fvu::Texture myTextures[NUM_TEXTURES];
            bool plantGrid[4][NUM_ROWS][NUM_COLS];
            fvu::Status myStatus;
            std::vector<fvu::Zombie> myZombies[4];
            std::vector<fvu::Plant> myPlants[4];
            std::vector<fvu::Particle> myParticles[4];
            fvu::Team myTeams[4];


        private:
            fvu::Config myConfig;
            sf::Time myTime;
            uint32_t framecount;
            sf::ContextSettings mySettings;
            sf::RenderWindow myWindow;
            sf::Music myMusic[2];
            sf::SoundBuffer mySoundBuffers[NUM_SFX];
            sf::Sound mySounds[NUM_SOUNDS];
            sf::Clock myClock;

    };

} // namespace fvu


extern fvu::Game *myGame;

