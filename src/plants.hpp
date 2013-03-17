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


/* Plant depth enum, front to back */
typedef enum {BACKLEAF_DEPTH, PLANT_OUTERARM_LOWER_DEPTH = 0, PLANT_DEPTH_RANGE} PLANT_DEPTH_ENUM;


/* Plant status enum */
typedef enum {PLANT_STATUS_DEFAULT=0, PLANT_STATUS_PLACED, PLANT_STATUS_SKIP, PLANT_STATUS_DEMO, PLANT_STATUS_INACTIVE} PLANT_STATUS_ENUM;

namespace fvu {

    /* Main plant class */
    class Plant {
        public:
            Plant(uint8_t type, uint16_t myid);
            void update();
            bool place(uint8_t team, uint16_t row, uint16_t col);
            void move(float delta_x, float delta_y);
            void endDemo();
            void updateDemo();
            void draw(uint16_t index);
            uint8_t getStatus() {return status;}
            uint16_t getID() {return id;}
            uint8_t getType() {return type;}
            uint16_t getRow() {return row;}
            uint16_t getCol() {return col;}
            bool operator< (const Plant &rhs) const;

        private:
            uint8_t type;
            uint8_t status;
            uint16_t health;
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
