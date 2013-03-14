/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * zombies.hpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main zombie class.
 *
 *
 * NOTES:
 * 2/28/13 by JAZ::Design created.
 *****************************************************************************/


#pragma once

#include "floravsundead.hpp"
#include "object.hpp"

/* Main enum for the zombie types. We specify NUM_SPELLINGS to some reasonable amount to minimize
 * the number of compile errors */
#define NUM_ZOMBIE_SPELLINGS 2
typedef enum {REGULAR_ZOMBIE=0, FLAG_ZOMBIE, CONE_ZOMBIE, POLE_ZOMBIE, BUCKET_ZOMBIE, NEWS_ZOMBIE, SCREEN_ZOMBIE, FOOTBALL_ZOMBIE, DANCING_ZOMBIE, YETI_ZOMBIE, NUM_ZOMBIE_TYPE} ZOMBIE_TYPE;
extern std::string zombieNames[NUM_ZOMBIE_TYPE][NUM_ZOMBIE_SPELLINGS];


/* Zombie depth enum, front to back */
typedef enum {ZOMBIE_OUTERARM_LOWER_DEPTH = 0, ZOMBIE_OUTERARM_UPPER_DEPTH,ZOMBIE_OUTERARM_HAND_DEPTH, ZOMBIE_ACCESSORY_DEPTH, ZOMBIE_HEAD_ACCESSORY_DEPTH, ZOMBIE_HEAD_DEPTH, ZOMBIEBODY_DEPTH, ZOMBIE_OUTERLEG_UPPER_DEPTH, ZOMBIE_OUTERLEG_LOWER_DEPTH, ZOMBIE_OUTERLEG_FOOT_DEPTH, ZOMBIE_INNERLEG_FOOT_DEPTH, ZOMBIE_INNERLEG_LOWER_DEPTH,
ZOMBIE_INNERLEG_UPPER_DEPTH,  ZOMBIE_INNERARM_LOWER_DEPTH, ZOMBIE_INNERARM_HAND_DEPTH, ZOMBIE_INNERARM_UPPER_DEPTH, ZOMBIE_SHADOW_DEPTH, ZOMBIE_DEPTH_RANGE} ZOMBIE_DEPTH_ENUM;


/* Zombie status enum */
typedef enum {ZOMBIE_STATUS_DEFAULT=0, ZOMBIE_STATUS_PLACED, ZOMBIE_STATUS_SKIP, ZOMBIE_STATUS_DEMO, ZOMBIE_STATUS_GAME, ZOMBIE_STATUS_INACTIVE} ZOMBIE_STATUS_ENUM;

namespace fvu {

    /* Main zombie class */
    class Zombie {
        public:
            Zombie(uint8_t type);
            void update();
            void place(int16_t location, int16_t delay, uint8_t team);
            void move(float delta_x, float delta_y);
            void endDemo();
            void updateDemo();
            void draw(uint16_t index);
            uint8_t getStatus() {return status;}
            uint8_t getType() {return type;}
            bool operator< (const Zombie &rhs) const;

        private:
            uint8_t type;
            uint8_t status;
            uint16_t health;
            float speed;
            std::vector<uint16_t> transitions;
            float game_x, game_y, demo_x, demo_y;
            float dir;
            uint8_t team;
            uint16_t delay;
            Object *myObject;
    };

} // namespace fvu

