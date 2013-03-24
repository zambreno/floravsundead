/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * plants.hpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main plant class.
 *
 *
 * NOTES:
 * 2/28/13 by JAZ::Design created.
 *****************************************************************************/


#pragma once

#include "floravsundead.hpp"
#include "object.hpp"

/* Main enum for the plant types. We specify NUM_SPELLINGS to some reasonable amount to minimize
 * the number of compile errors */
#define NUM_PLANT_SPELLINGS 2
typedef enum {PEASHOOTER_PLANT=0, REPEATER_PLANT, GATLING_PLANT, SNOW_PLANT, CHOMPER_PLANT, WALLNUT_PLANT, NUM_PLANT_TYPE} PLANT_TYPE;
extern std::string plantNames[NUM_PLANT_TYPE][NUM_PLANT_SPELLINGS];

/* Data arrays for the main plant types */
extern uint16_t plantCosts[NUM_PLANT_TYPE];
extern int16_t plantHealths[NUM_PLANT_TYPE];
#define NUM_PLANT_TRANSITIONS 3
extern uint16_t plantTransitions[NUM_PLANT_TYPE][NUM_PLANT_TRANSITIONS];
extern float plantSpeeds[NUM_PLANT_TYPE];

/* Plant depth enum, front to back */
typedef enum {PLANTHEAD_DEPTH=0, FRONTLEAF_DEPTH, STEM_DEPTH, BACKLEAF_DEPTH, PLANT_OUTERARM_LOWER_DEPTH, PLANT_SHADOW_DEPTH, PLANT_DEPTH_RANGE} PLANT_DEPTH_ENUM;

/* Plant status enum */
typedef enum {PLANT_STATUS_DEFAULT=0, PLANT_STATUS_PLACED, PLANT_STATUS_SKIP, PLANT_STATUS_DEMO, PLANT_STATUS_GAME, PLANT_STATUS_INACTIVE} PLANT_STATUS_ENUM;

namespace fvu {

    /* Main plant class */
    class Plant {
        public:
            Plant(uint8_t type, uint16_t myid);
            void update();
            void place(uint8_t team, uint16_t row, uint16_t col);
            void endDemo();
            void updateDemo();
            void updateTransition(uint16_t val);
            void draw(uint16_t index);
            uint8_t getStatus() {return status;}
            uint16_t getID() {return id;}
            uint8_t getType() {return type;}
            uint16_t getRow() {return row;}
            uint16_t getCol() {return col;}
            void bite() {health--;}
            bool operator< (const Plant &rhs) const;

        private:
            uint8_t type;
            uint8_t status;
            int16_t health;
            uint16_t id;
            uint16_t row, col;
            float speed;
            std::vector<uint16_t> transitions;
            float game_x, game_y, demo_x, demo_y;
            float dir;
            uint8_t team;
            uint16_t delay;
            Object *myObject;
    };

} // namespace fvu

