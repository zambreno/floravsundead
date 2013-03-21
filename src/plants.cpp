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
int16_t plantHealths[NUM_PLANT_TYPE] = {12, 12, 12, 12, 12, 72};
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

        float x = demo_x;
        float y = demo_y;
        float z;

        // Put objects on a different depth level if they're on the bottom
        if (team % 2 == 1) {
            z = index*PLANT_DEPTH_RANGE+OBJECT_BOTTOM_DEPTH;
        }
        else {
            z = index*PLANT_DEPTH_RANGE+OBJECT_TOP_DEPTH;
        }

        if (status > PLANT_STATUS_DEMO) {
            x = game_x;
            y = game_y;
        }
        else if (status == PLANT_STATUS_SKIP) {
            return;
        }

        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(dir, 1.0, 1.0);
        myObject->draw();
        glPopMatrix();
    }


    /*****************************************************************************
    * Function: Plant::Plant
    * Description: Class constructor. Uses an enum type to set plant-specific
    * parameters
    *****************************************************************************/
    Plant::Plant(uint8_t mytype, uint16_t myid) {

        uint32_t anim_count = rand();

        status = PLANT_STATUS_DEFAULT;
        type = mytype;
        id = myid;
        row = 0;col = 0;

        /* Initialize plant information here so we can leave the rest of this function as object assembly */
        health = plantHealths[type];
        speed = plantSpeeds[type];
        for (uint8_t i = 0; i < NUM_PLANT_TRANSITIONS; i++) {
            if (plantTransitions[type][i] == 0)
                break;
            transitions.push_back(plantTransitions[type][i]);
        }


        Object *local_object;
        animation_struct demo_anim, game_anim, zero_anim;

        /* The zero_anim is useful if we don't care about initial positioning of animation */
        zero_anim.start_angle = 0.0;zero_anim.delta_angle = 0.0;zero_anim.end_angle = 0.0;
        zero_anim.start_x     = 0.0;zero_anim.delta_x     = 0.0;zero_anim.end_x     = 0.0;
        zero_anim.start_y     = 0.0;zero_anim.delta_y     = 0.0;zero_anim.end_y     = 0.0;

        switch(type) {
            default:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                myObject = new Object(zero_anim, zero_anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the bottom part of the stalk
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 0.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 0.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_STALK_BOTTOM, STEM_DEPTH, 3, myObject);

                // children[0]->children[0] is the top part of the stalk
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 1.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 17.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_STALK_TOP, STEM_DEPTH, 1, myObject->children[0]);


                // children[0]->children[1] is the backleaf
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -18.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -1.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0]->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF, BACKLEAF_DEPTH, 2, myObject->children[0]);

                // children[0]->children[1]->children[0] is the left tip of the backleaf
                local_object = myObject->children[0]->children[1];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -5.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 9.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF_LEFTTIP, BACKLEAF_DEPTH, 0, local_object);

                // children[0]->children[1]->children[1] is the right tip of the backleaf
                local_object = myObject->children[0]->children[1];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 37.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 13.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF_RIGHTTIP, BACKLEAF_DEPTH, 0, local_object);

                // children[0]->children[2] is the frontleaf
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -25.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -21.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0]->children[2] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF, FRONTLEAF_DEPTH, 2, myObject->children[0]);

                // children[0]->children[2]->children[0] is the left tip of the frontleaf
                local_object = myObject->children[0]->children[2];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -6.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -2.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF_LEFTTIP, FRONTLEAF_DEPTH, 0, local_object);

                // children[0]->children[2]->children[1] is the right tip of the frontleaf
                local_object = myObject->children[0]->children[2];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 61.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 7.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF_RIGHTTIP, FRONTLEAF_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0] is the head
                local_object = myObject->children[0]->children[0];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -28.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 9.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_HEAD, PLANTHEAD_DEPTH, 2, local_object);

                // children[0]->children[0]->children[0]->children[0] is the lips
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 61.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 12.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_LIPS, PLANTHEAD_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[0] is the lips
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -12.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 27.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_PLANTS, PEASHOOTER_SPROUT, FRONTLEAF_DEPTH, 0, local_object);



                break;
        }

    }

    /*****************************************************************************
    * Function: Plant::place
    * Description: Places the plant (both for demo mode and regular game mode)
    * based on the row and column values
    *****************************************************************************/
    void Plant::place(uint8_t myteam, uint16_t myrow, uint16_t mycol) {

        /* Teams 0 and 1 are on the left, 2 and 3 are on the right.
         * Teams 0 and 2 are on the top, 1 and 3 on the bottom.
         * Placement may need to be sprite-specific. */
        status = PLANT_STATUS_PLACED;
        team = myteam;
        row = myrow-1;
        col = mycol-1;

        switch (team) {
            case 0:
            default:
                game_x = left_gridWidths[col];
                game_y = top_gridHeights[row];
                demo_x = game_x;
                demo_y = game_y;
                dir = -1.0;
                break;
            case 1:
                game_x = left_gridWidths[col];
                game_y = bottom_gridHeights[row];
                demo_x = game_x;
                demo_y = game_y;
                dir = -1.0;
                break;
            case 2:
                game_x = right_gridWidths[col];
                game_y = top_gridHeights[row];
                demo_x = game_x;
                demo_y = game_y;
                dir = 1.0;
                break;
            case 3:
                game_x = right_gridWidths[col];
                game_y = bottom_gridHeights[row];
                demo_x = game_x;
                demo_y = game_y;
                dir = 1.0;
                break;
        }

        return;
    }


    /*****************************************************************************
    * Function: Plant::endDemo
    * Description: Places the plant in game mode
    *****************************************************************************/
    void Plant::endDemo() {

        /* Reset all animation counts and angles */
        myObject->endDemo();

        /* Point x,y to game_x and game_y */
        status = PLANT_STATUS_GAME;
    }




    /*****************************************************************************
    * Function: Plant::updateDemo
    * Description: Updates each plant in demo mode
    *****************************************************************************/
    void Plant::updateDemo() {

        if (status == PLANT_STATUS_PLACED) {
            status = PLANT_STATUS_DEMO;
        }

        myObject->update();
    }


    /*****************************************************************************
    * Function: Plant::update
    * Description: Updates each plant
    *****************************************************************************/
    void Plant::update() {


        myObject->update();
    }



    /*****************************************************************************
    * Function: Plant::move
    * Description: Moves the plant around (for testing purposes)
    *****************************************************************************/
    void Plant::move(float delta_x, float delta_y) {

        game_x += delta_x;
        game_y += delta_y;

    }

    /*****************************************************************************
    * Function: Plant::operator <
    * Description: Overloads the < operator for comparison / sort purposes
    *****************************************************************************/
    bool Plant::operator< (const Plant &rhs) const {

        /* We want pseudo-3D, so lower = closer. Otherwise, closer to the sides
         * should be closer to the screen. */
        if (status == PLANT_STATUS_DEMO) {
            if (demo_y == rhs.demo_y) {
                return (fabsf(demo_x) > fabsf(rhs.demo_x));
            }
            else return (demo_y < rhs.demo_y);
        }
        else {
            if (game_y == rhs.game_y) {
                return (fabsf(game_x) > fabsf(rhs.game_x));
            }
            else return (game_y < rhs.game_y);
        }
    }




} // namespace fvu
