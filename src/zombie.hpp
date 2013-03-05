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

/* Main enum for the zombie types. We specify NUM_SPELLINGS to some reasonable amount to minimize
 * the number of compile errors */
#define NUM_ZOMBIE_SPELLINGS 2
typedef enum {REGULAR_ZOMBIE=0, FLAG_ZOMBIE, CONE_ZOMBIE, POLE_ZOMBIE, BUCKET_ZOMBIE, NEWS_ZOMBIE, SCREEN_ZOMBIE, FOOTBALL_ZOMBIE, DANCING_ZOMBIE, YETI_ZOMBIE, NUM_ZOMBIE_TYPE} ZOMBIE_TYPE;
extern std::string zombieNames[NUM_ZOMBIE_TYPE][NUM_ZOMBIE_SPELLINGS];


namespace fvu {

    /* Object class. We create more links than are needed, as long as the data can be chained it
     * shouldn't be a big problem to have multiple unused links */
     class Object {
        public:
            float x, y, angle;
            ZOMBIE_SPRITE_ENUM sprite;
            DEPTH_ENUM depth;
            Object *parent;
            Object **children;
            uint8_t num_children;
            Object(float x, float y, float angle, ZOMBIE_SPRITE_ENUM sprite, DEPTH_ENUM depth, uint8_t num_children, Object *parent);
            void draw();
     };


    /* Main zombie class */
    class Zombie {
        public:
            Zombie(ZOMBIE_TYPE type);
            void update();
            void place(int16_t location, int16_t delay, uint8_t team);
            void draw();
            void getStatus();

        private:
            ZOMBIE_TYPE type;
            uint16_t health;
            float speed;
            std::vector<uint16_t> transitions;
            float game_x, game_y, demo_x, demo_y;
            uint16_t delay;
            Object *myObject;

    };

} // namespace fvu

