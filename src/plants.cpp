/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * plants.cpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main plant class.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"


/* Plant names */
std::string plantNames[NUM_PLANT_TYPE][NUM_PLANT_SPELLINGS] = {
    {"peashooter", "regular"},
    {"repeater", "double"},
    {"gatling", "quadruple"},
    {"snowpea", "snow"},
    {"chomper", "chomp"},
    {"wallnut", "walnut"}
    };


/* Plant costs */
uint16_t plantCosts[NUM_PLANT_TYPE] = {100, 200, 450, 175, 150, 50};
/* Plant healths */
int16_t plantHealths[NUM_PLANT_TYPE] = {90, 90, 90, 90, 90, 720};
/* Plant speeds. This variable is plant-specific */
float plantSpeeds[NUM_PLANT_TYPE] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
/* Plant transitions */
uint16_t plantTransitions[NUM_PLANT_TYPE][NUM_PLANT_TRANSITIONS] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {48, 24, 0}
    };

namespace fvu {

    /*****************************************************************************
    * Function: Plant::draw
    * Description: Plant drawing function. Sets the initial modeling position
    * and draws each Object recursively.
    *****************************************************************************/
    void Plant::draw(uint16_t index) {

        float x, y, z;

        // In DEMO mode, we can do a pure depth sort of all the plants
        if (status == PLANT_STATUS_DEMO) {
            x = demo_x;
            y = demo_y;
            z = index*PLANT_DEPTH_RANGE;

            // Put objects on a different depth level if they're on the bottom
            if (team % 2 == 1) {
                z += OBJECT_BOTTOM_DEPTH;
            }
            else {
                z += OBJECT_TOP_DEPTH;
            }

        }

        // In GAME/ACTIVE mode, we need to be row-aware with our sorting
        else {
            x = game_x;
            y = game_y;
            z = index*PLANT_DEPTH_RANGE;

            // Put objects on a different depth level if they're on the bottom
            if (team % 2 == 1) {
                switch (row) {
                    case 0:
                    default:
                        z += BOTTOMROW_1_DEPTH;
                        break;
                    case 1:
                        z += BOTTOMROW_2_DEPTH;
                        break;
                    case 2:
                        z += BOTTOMROW_3_DEPTH;
                        break;
                    case 3:
                        z += BOTTOMROW_4_DEPTH;
                         break;
                    case 4:
                        z += BOTTOMROW_5_DEPTH;
                        break;
                }
            }
            else {
                switch (row) {
                    case 0:
                    default:
                        z += TOPROW_1_DEPTH;
                        break;
                    case 1:
                        z += TOPROW_2_DEPTH;
                        break;
                    case 2:
                        z += TOPROW_3_DEPTH;
                        break;
                    case 3:
                        z += TOPROW_4_DEPTH;
                         break;
                    case 4:
                        z += TOPROW_5_DEPTH;
                        break;
                }
            }


        }

        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(dir, 1.0, 1.0);
        myObject->draw();
        glPopMatrix();
    }


    /*****************************************************************************
    * Function: Particle::Particle
    * Description: Class constructor. Uses an enum type to set particle-specific
    * parameters
    *****************************************************************************/
    Particle::Particle(uint8_t mytype) {


    }



    /*****************************************************************************
    * Function: Particle::update
    * Description: Updates each particle
    *****************************************************************************/
    void Particle::update() {


        myObject->update();
    }



    /*****************************************************************************
    * Function: Particle::operator <
    * Description: Overloads the < operator for comparison / sort purposes
    *****************************************************************************/
    bool Particle::operator< (const Particle &rhs) const {

        /* We want pseudo-3D, so lower = closer. Otherwise, closer to the sides
         * should be closer to the screen. */
        if (game_y == rhs.game_y) {
            return (fabsf(game_x) > fabsf(rhs.game_x));
        }
        else return (game_y < rhs.game_y);
    }




} // namespace fvu
