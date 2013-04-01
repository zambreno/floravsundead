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
    {"peashooter", "regular", "pea", "peeshooter"},
    {"repeater", "double", "repeater", "double"},
    {"gatling", "quadruple", "machine", "quad"},
    {"snowpea", "snow", "ice", "icepea"},
    {"chomper", "chomp", "venus", "eater"},
    {"wallnut", "walnut", "wall", "nut"},
    {"tallnut", "talnut", "tall", "bignut"},
    {"portal", "h_portal", "v_portal", "d_portal"}
    };


/* Plant costs */
uint16_t plantCosts[NUM_PLANT_TYPE] = {100, 200, 450, 175, 150, 50, 125, 700};
/* Plant healths */
int16_t plantHealths[NUM_PLANT_TYPE] = {9, 9, 9, 9, 9, 72, 144, 1};
/* Plant speeds. This variable is plant-specific */
uint16_t plantSpeeds[NUM_PLANT_TYPE] = {80, 40, 20, 80, 300, 1, 1, 1};
/* Plant transitions */
uint16_t plantTransitions[NUM_PLANT_TYPE][NUM_PLANT_TRANSITIONS] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {48, 24, 0},
    {96, 48, 0},
    {0, 0, 0}
    };

namespace fvu {

    /*****************************************************************************
    * Function: Plant::draw
    * Description: Plant drawing function. Sets the initial modeling position
    * and draws each Object recursively.
    *****************************************************************************/
    void Plant::draw(uint16_t index) {

        float x, y, z;


        // In DEMO mode, we can do a pure depth sort of all the zombies
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
        action_count = 0;
        fire_count = 0;
        has_fired = false;
        has_hit = false;

        /* Initialize plant information here so we can leave the rest of this function as object assembly */
        health = plantHealths[type];
        speed = plantSpeeds[type];
        //transitions = std::vector<uint16_t>();
        for (uint8_t i = 0; i < NUM_PLANT_TRANSITIONS; i++) {
            if (plantTransitions[type][i] == 0)
                break;
            transitions.push_back(plantTransitions[type][i]);
        }


        Object *local_object;
        std::vector<animation_struct> anim;
        animation_struct local_anim;

        switch(type) {

            case PORTAL_PLANT:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 3, NULL);

                // children[0] is the outer glow
                local_anim.set_defaults();
                local_anim.set_xy(-22.0, -47.0);
                local_anim.set_xscale(1.0, 0.005, 1.15);
                local_anim.set_yscale(0.9);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PORTAL_CIRCLE_GLOW, PLANTHEAD_DEPTH, 0, myObject);


                // children[2] is the outer circle
                local_anim.set_defaults();
                local_anim.set_xy(-42.0, -47.0);
                local_anim.set_xscale(1.0, 0.005, 1.15);
                local_anim.set_yscale(0.9);
                anim.clear();anim.push_back(local_anim);
                myObject->children[2] = new Object(anim, anim_count, TEX_PLANTS, PORTAL_CIRCLE_OUTER, PLANTHEAD_DEPTH, 0, myObject);

                // children[1] is the inner circle
                local_anim.set_defaults();
                local_anim.set_xy(-10.0, -47.0);
                local_anim.set_xscale(1.0, 0.005, 1.15);
                local_anim.set_yscale(0.9);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, PORTAL_CIRCLE_CENTER, PLANTHEAD_DEPTH, 0, myObject);



                break;

            case WALLNUT_PLANT:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the wallnut body
                local_anim.set_defaults();
                local_anim.set_x(-22.0, 0.0, 0.0);
                local_anim.set_y(-35.0, 0.0, 0.0);
                local_anim.set_angle(-2.0, 0.1, 2.0, ANCHOR_S);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.001, 0.66);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, WALLNUT_BODY, PLANTHEAD_DEPTH, 0, myObject);


                // children[1] is the shadow
                local_anim.set_defaults();
                local_anim.set_x(-22.0, 0.0, 0.0);
                local_anim.set_y(-50.0, 0.0, 0.0);
                local_anim.set_xscale(0.85, 0.0, 1.0);
                local_anim.set_yscale(0.85, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);

                break;

            case TALLNUT_PLANT:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the wallnut body
                local_anim.set_defaults();
                local_anim.set_x(-22.0, 0.0, 0.0);
                local_anim.set_y(-35.0, 0.0, 0.0);
                local_anim.set_angle(-2.0, 0.1, 2.0, ANCHOR_S);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.001, 0.66);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, TALLNUT_BODY, PLANTHEAD_DEPTH, 0, myObject);


                // children[1] is the shadow
                local_anim.set_defaults();
                local_anim.set_x(-22.0, 0.0, 0.0);
                local_anim.set_y(-50.0, 0.0, 0.0);
                local_anim.set_xscale(0.85, 0.0, 1.0);
                local_anim.set_yscale(0.85, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);



                break;


            case CHOMPER_PLANT:
                 /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the bottom part of the stalk
                local_anim.set_defaults();
                local_anim.set_x(5.0, 0.0, 0.0);
                local_anim.set_y(-20.0, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_STEM_3, STEM_DEPTH, 5, myObject);

                // children[0]->children[0] is the middle part of the stem
                local_anim.set_defaults();
                local_anim.set_xy(-17.0, 4.75);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(-10.0, 0.25, 10.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_yscale(0.8, 0.15, 2.3);local_anim.set_angle(0.0, -9.0, -90.0, ANCHOR_S);anim.push_back(local_anim);
                local_anim.set_xy(-17.0, 4.75);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(-10.0, 0.25, 10.0, ANCHOR_S);
                anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_STEM_2, STEM_DEPTH, 1, myObject->children[0]);

                   // children[0]->children[0]->children[0] is the top of the stem
                local_object = myObject->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(0.0, 18.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(-10.0, 0.25, 10.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_x(0.0, 2.0, 20.0);local_anim.set_y(18.0, 2.0, 38.0);local_anim.set_angle(0.0, 9.0, 90.0, ANCHOR_S);anim.push_back(local_anim);
                local_anim.set_xy(0.0, 18.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(-10.0, 0.25, 10.0, ANCHOR_S);anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_STEM_1, STEM_DEPTH, 6, local_object);

                // children[0]->children[0]->children[0]->children[0] is lowest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(3.0, 0.0, 0.0);
                local_anim.set_y(7.5, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                local_anim.set_angle(-10.0, 0.75, 10.0, ANCHOR_E);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_4, HEADLEAF_2_4_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[1] is the middle headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(-8.0, 0.0, 0.0);
                local_anim.set_y(12.0, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                local_anim.set_angle(-5.0, 0.375, 5.0, ANCHOR_E);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_1, HEADLEAF_1_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[2] is the second highest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(0.0, 0.0, 0.0);
                local_anim.set_y(11.25, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                local_anim.set_angle(5.0, -0.375, -5.0, ANCHOR_E);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_2, HEADLEAF_2_4_DEPTH, 0, local_object);


                // children[0]->children[0]->children[0]->children[3] is the highest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(6.0, 0.0, 0.0);
                local_anim.set_y(22.75, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                local_anim.set_angle(10.0, -0.75, -10.0, ANCHOR_E);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_3, HEADLEAF_3, 0, local_object);


                // children[0]->children[0]->children[0]->children[4] is the underjaw
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(6.5, -36.5);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(20.0, 0.75, 30.0, ANCHOR_NW);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(20.0, -2.0, 0.0, ANCHOR_NW);anim.push_back(local_anim);
                local_anim.set_x(3.5, 0.2, 9.5);local_anim.set_angle(42.0, -0.25, 30.0, ANCHOR_NW);anim.push_back(local_anim);
                local_object->children[4] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_UNDERJAW, PLANTHEAD_DEPTH, 3, local_object);

                // children[0]->children[0]->children[0]->children[4]->children[0] is the bottom lip
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(8.75, 0.0, 0.0);
                local_anim.set_y(7.25, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_BOTTOMLIP, PLANTHEAD_DEPTH, 1, local_object);


                // children[0][0][0][4][0][0] is the zombie arm (only visible during eating)
                local_object = myObject->children[0]->children[0]->children[0]->children[4]->children[0];
                local_anim.set_defaults();
                local_anim.set_angle(-24.0, 1.225, -8.0, ANCHOR_NE);
                local_anim.set_xy(2.0, -4.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, BLANK_SPRITE, PLANTHEAD_ACCESSORY_DEPTH, 1, local_object);


                local_anim.set_defaults();
                local_anim.set_xy(2.0,-14);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, BLANK_SPRITE, PLANTHEAD_DEPTH, 0, local_object->children[0]);


                // children[0]->children[0]->children[0]->children[4]->children[1] is the inside of the mouth
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(12, 0.0, 0.0);
                local_anim.set_y(12, 0.0, 0.0);
                local_anim.set_xscale(0.80);local_anim.set_yscale(0.80);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_INSIDEMOUTH, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[4]->children[2] is the tongue
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(10.0, 0.25, 20.0);
                local_anim.set_y(22.0);
                local_anim.set_xscale(0.8);local_anim.set_yscale(0.8);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_xscale(0.8, 0.2, 2.8);anim.push_back(local_anim);
                local_anim.set_xscale(0.8);anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_TONGUE, CHOMPER_TONGUE_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[5] is the top of the head
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(13.5, 9.0);
                local_anim.set_xscale(0.80);local_anim.set_yscale(0.80);
                local_anim.set_angle(0.0, 0.5, 10.0, ANCHOR_W);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_y(9.0, -0.4, 5.0);local_anim.set_angle(10.0, 2.0, 30.0, ANCHOR_W);anim.push_back(local_anim);
                local_anim.set_xy(13.5, 9.0);
                local_anim.set_angle(0.0, -0.125, -5.0, ANCHOR_NW);anim.push_back(local_anim);
                local_object->children[5] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_TOPJAW, PLANTHEAD_DEPTH, 4, local_object);

                // children[0]->children[0]->children[0]->children[5]->children[0] is the 1st spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(5.0, 0.0, 0.0);
                local_anim.set_y(22.0, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(58.0, 0.0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);


                 // children[0]->children[0]->children[0]->children[5]->children[1] is the 2nd spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(9.5, 0.0, 0.0);
                local_anim.set_y(25.0, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                local_anim.set_angle(40, 0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[2] is the 3rd spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(16.0, 0.0, 0.0);
                local_anim.set_y(28.0, 0.0, 0.0);
                local_anim.set_xscale(0.80, 0.0, 1.0);
                local_anim.set_yscale(0.80, 0.0, 1.0);
                local_anim.set_angle(30.0, 0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[3] is the 4th spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(25.0, 0.0, 0.0);
                local_anim.set_y(29.0, 0.0, 0.0);
                local_anim.set_xscale(0.80, 0.0, 1.0);
                local_anim.set_yscale(0.80, 0.0, 1.0);
                local_anim.set_angle(15.0, 0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);


                // children[0]->children[1] is the left backleaf
                local_anim.set_defaults();
                local_anim.set_x(-21.0, 0.0, 0.0);
                local_anim.set_y(-5, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_3, BACKLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[2] is the right backleaf
                local_anim.set_defaults();
                local_anim.set_x(3.5, 0.0, 0.0);
                local_anim.set_y(-5.0, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_4, BACKLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[3] is the left frontleaf
                local_anim.set_defaults();
                local_anim.set_x(-24.0, 0.0, 0.0);
                local_anim.set_y(-14.0, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[3] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_1, FRONTLEAF_DEPTH, 1, myObject->children[0]);

                // children[0]->children[4] is the left frontleaf tip
                local_object = myObject->children[0]->children[3];
                local_anim.set_defaults();
                local_anim.set_x(-11, 0.0, 0.0);
                local_anim.set_y(.75, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_1_TIP, FRONTLEAF_DEPTH, 0, local_object);


                // children[0]->children[4] is the right frontleaf
                local_anim.set_defaults();
                local_anim.set_x(1.0, 0.0, 0.0);
                local_anim.set_y(-12.25, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[4] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_2, CHOMPER_RIGHT_FRONTLEAF_DEPTH, 1, myObject->children[0]);


                // children[0]->children[4] is the right frontleaf tip
                local_object = myObject->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(24.5, 0.0, 0.0);
                local_anim.set_y(3, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_2_TIP, CHOMPER_RIGHT_FRONTLEAF_DEPTH, 0, local_object);

                // children[1] is the shadow
                local_anim.set_defaults();
                local_anim.set_x(-25.0, 0.0, 0.0);
                local_anim.set_y(-50.0, 0.0, 0.0);
                local_anim.set_xscale(0.85, 0.0, 1.0);
                local_anim.set_yscale(0.85, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);

                break;

            case PEASHOOTER_PLANT:
            case REPEATER_PLANT:
            case GATLING_PLANT:
            case SNOW_PLANT:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 4, NULL);

                // children[0] is the bottom part of the stalk
                local_anim.set_defaults();
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, -0.01, 0.25);
                local_anim.set_y(-22.0, 0.0, 0.0);
                local_anim.set_angle(-18.0, 1.8, 18.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_STALK_BOTTOM, STEM_DEPTH, 1, myObject);

                // children[0]->children[0] is the top part of the stalk
                local_anim.set_defaults();
                local_anim.set_x(1.0, 0.0, 0.0);
                local_anim.set_y(10.0, -0.169, 4.23);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_STALK_TOP, STEM_DEPTH, 1, myObject->children[0]);


                // children[1] is the backleaf
                local_anim.set_defaults();
                local_anim.set_x(-11.0, 0.0, 0.0);
                local_anim.set_y(-20.0, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF, BACKLEAF_DEPTH, 2, myObject);

                // children[1]->children[0] is the left tip of the backleaf
                local_object = myObject->children[1];
                local_anim.set_defaults();
                local_anim.set_xy(-3.0, 6.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF_LEFTTIP, BACKLEAF_DEPTH, 0, local_object);

                // children[1]->children[1] is the right tip of the backleaf
                local_object = myObject->children[1];
                local_anim.set_defaults();
                local_anim.set_xy(24.0, 8.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF_RIGHTTIP, BACKLEAF_DEPTH, 0, local_object);

                // children[2] is the frontleaf
                local_anim.set_defaults();
                local_anim.set_xy(-17.0, -35.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[2] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF, FRONTLEAF_DEPTH, 2, myObject);

                // children[2]->children[0] is the left tip of the frontleaf
                local_object = myObject->children[2];
                local_anim.set_defaults();
                local_anim.set_xy(-3.0, -2.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF_LEFTTIP, FRONTLEAF_DEPTH, 0, local_object);

                // children[2]->children[1] is the right tip of the frontleaf
                local_object = myObject->children[2];
                local_anim.set_defaults();
                local_anim.set_xy(39.0, 4.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF_RIGHTTIP, FRONTLEAF_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0] is the head
                if (type == PEASHOOTER_PLANT) {
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-19.0, 6.0);
                    local_anim.set_angle(18.0, -1.8, -18.0, ANCHOR_S);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_xscale(0.65, -0.015, 0.5);
                    local_anim.set_angle(9.0, -1.8, -9.0, ANCHOR_S);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEAD, PLANTHEAD_DEPTH, 2, local_object);

                    // children[0]->children[0]->children[0]->children[0] is the lips
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(39.0, 7.0);
                    local_anim.set_angle(-4.0, 0.2, 4.0, ANCHOR_W);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(39.0, -1.05, 28.5);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_LIPS, PLANTHEAD_DEPTH, 0, local_object);

                    // children[0]->children[0]->children[0]->children[1] is the sprout
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-9.0, 19.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(-10.0, 0.75, 10.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_SPROUT, FRONTLEAF_DEPTH, 0, local_object);
                }

                if (type == GATLING_PLANT) {
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-19.0, 6.0);
                    local_anim.set_angle(18.0, -1.8, -18.0, ANCHOR_S);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_xscale(0.65, -0.015, 0.5);
                    local_anim.set_angle(9.0, -1.8, -9.0, ANCHOR_S);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_HEAD, PLANTHEAD_DEPTH, 7, local_object);

                    // children[0]->children[0]->children[0]->children[0] is the lips
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(37.0, 7.0);
                    local_anim.set_angle(-2.0, 0.1, 2.0, ANCHOR_W);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(37.0, -0.85, 28.5);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_MOUTH, PLANTHEAD_DEPTH, 0, local_object);

                    // children[0]->children[0]->children[0]->children[1] is the sprout
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(37.0, 7.0);
                    local_anim.set_angle(-2.0, 0.1, 2.0, ANCHOR_W);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(37.0, -0.85, 28.5);
                    anim.push_back(local_anim);
                    local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_MOUTH_OVERLAY, PLANTHEAD_OVERLAY_DEPTH, 0, local_object);


                    // children[0]->children[0]->children[0]->children[2] through [5] are the barrels.
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(53.0, 20.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(53.0, -1.0, 43.0);
                    anim.push_back(local_anim);
                    local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_BARREL, PLANTHEAD_ACCESSORY_DEPTH, 0, local_object);

                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(50.0, 12.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(50.0, -1.0, 40.0);
                    anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_BARREL, PLANTHEAD_ACCESSORY_DEPTH, 0, local_object);

                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(50.0, 28.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(50.0, -1.0, 40.0);
                    anim.push_back(local_anim);
                    local_object->children[4] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_BARREL, PLANTHEAD_ACCESSORY_DEPTH, 0, local_object);

                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(47.0, 20.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(47.0, -1.0, 37.0);
                    anim.push_back(local_anim);
                    local_object->children[5] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_BARREL, PLANTHEAD_ACCESSORY_DEPTH, 0, local_object);


                    // children[0]->children[0]->children[0]->children[6] is the helmet
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-10.0, -2.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-9.0, 1.8, 9.0, ANCHOR_S);
                    anim.push_back(local_anim);
                    local_object->children[6] = new Object(anim, anim_count, TEX_PLANTS, GATLINGPEA_HELMET, PLANTHEAD_OVERLAY_DEPTH, 0, local_object);



                }

                // children[0]->children[0]->children[0] is the head
                if (type == REPEATER_PLANT) {
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-19.0, 6.0);
                    local_anim.set_angle(18.0, -1.8, -18.0, ANCHOR_S);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_xscale(0.65, -0.015, 0.5);
                    local_anim.set_angle(9.0, -1.8, -9.0, ANCHOR_S);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEAD, PLANTHEAD_DEPTH, 6, local_object);

                    // children[0]->children[0]->children[0]->children[0] is the lips
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(39.0, 7.0);
                    local_anim.set_angle(-4.0, 0.2, 4.0, ANCHOR_W);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(39.0, -1.05, 28.5);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_LIPS, PLANTHEAD_DEPTH, 0, local_object);

                    // children[0]->children[0]->children[0]->children[1] is the HEADLEAF_FARTHEST
                    local_anim.set_defaults();
                    local_anim.set_xy(-9.0, 13.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(-10.0, 0.75, 10.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEADLEAF_FARTHEST, FRONTLEAF_DEPTH, 0, local_object);

                    // children[0]->children[0]->children[0]->children[2] is the HEADLEAF_2NDFARTHEST
                    local_anim.set_defaults();
                    local_anim.set_xy(-3.0, 25.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(-5.0, 0.375, 5.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEADLEAF_2RDFARTHEST, FRONTLEAF_DEPTH, 1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_xy(-6.0, 7.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2]->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEADLEAF_TIP_TOP, FRONTLEAF_DEPTH, 0, local_object->children[2]);


                    // children[0]->children[0]->children[0]->children[3] is the HEADLEAF_3RDFARTHEST
                    local_anim.set_defaults();
                    local_anim.set_xy(4.0, 32.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(10.0, -0.75, -10.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEADLEAF_3RDFARTHEST, FRONTLEAF_DEPTH, 0, local_object);


                    // children[0]->children[0]->children[0]->children[4] is the HEADLEAF_NEAREST
                    local_anim.set_defaults();
                    local_anim.set_xy(-10.0, 14.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(5.0, -0.375, -5.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[4] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEADLEAF_NEAREST, FRONTLEAF_DEPTH, 1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_xy(-4.0, 3.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[4]->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEADLEAF_TIP_BOTTOM, FRONTLEAF_DEPTH, 0, local_object->children[4]);

                    // children[0]->children[0]->children[0]->children[5] is the eyebrows
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(19.0, 28.0);
                    // The eyebrows are apparently not scaled. Probably a PvZ bug.
                    //local_anim.set_xscale(0.65, 0.0, 1.0);
                    //local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(19.0, -1.05, 8.5);
                    anim.push_back(local_anim);
                    local_object->children[5] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_EYEBROW, PLANTHEAD_ACCESSORY_DEPTH, 0, local_object);



                }

                // children[0]->children[0]->children[0] is the head
                if (type == SNOW_PLANT) {
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-19.0, 6.0);
                    local_anim.set_angle(18.0, -1.8, -18.0, ANCHOR_S);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_xscale(0.65, -0.015, 0.5);
                    local_anim.set_angle(9.0, -1.8, -9.0, ANCHOR_S);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_HEAD, PLANTHEAD_DEPTH, 4, local_object);

                    // children[0]->children[0]->children[0]->children[0] is the lips
                    local_object = myObject->children[0]->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(39.0, 9.0);
                    local_anim.set_angle(-4.0, 0.2, 4.0, ANCHOR_W);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_x(39.0, -1.05, 28.5);
                    anim.push_back(local_anim);
                    local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_MOUTH, PLANTHEAD_DEPTH, 0, local_object);

                    // children[0]->children[0]->children[0]->children[1] is the SNOWPEA_CRYSTALS
                    local_anim.set_defaults();
                    local_anim.set_xy(-9.0, 1.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(-3.0, 0.375*0.4, 3.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_CRYSTALS_1, FRONTLEAF_DEPTH, 0, local_object);

                    local_anim.set_defaults();
                    local_anim.set_xy(-9.0, 20.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(2.5, -0.375/2, -2.5, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_CRYSTALS_2, FRONTLEAF_DEPTH, 0, local_object);

                    local_anim.set_defaults();
                    local_anim.set_xy(-10.0, 3.0);
                    local_anim.set_xscale(0.65, 0.0, 1.0);
                    local_anim.set_yscale(0.65, 0.0, 1.0);
                    local_anim.set_angle(-5.0, 0.375, 5.0, ANCHOR_E);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_CRYSTALS_3, FRONTLEAF_DEPTH, 0, local_object);


                }

                // children[3] is the shadow
                local_anim.set_defaults();
                local_anim.set_xy(-25.0, -50.0);
                local_anim.set_xscale(0.85, 0.0, 1.0);
                local_anim.set_yscale(0.85, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[3] = new Object(anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);

                break;

            default:
                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the bottom part of the stalk
                local_anim.set_defaults();
                local_anim.set_xscale(1.0, 0.025, 2.0);
                local_anim.set_angle(0.0, 1.0, 7200.0, ANCHOR_SW);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, WHITE_PLANT_SPRITE, PLANTHEAD_DEPTH, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_x(108.0, 2.7, 216.0);
                local_anim.set_y(0.0, 0.0, 0.0);
                local_anim.set_angle(-90.0, 2.0, 90.0, ANCHOR_W);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_PLANTS, WHITE_PLANT_SPRITE, PLANTHEAD_DEPTH, 1, myObject->children[0]);


                local_anim.set_defaults();
                local_anim.set_x(54.0, 0.0, 0.0);
                local_anim.set_y(0.0, 0.0, 0.0);
                local_anim.set_angle(45.0, 2.0, 135.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_PLANTS, WHITE_PLANT_SPRITE, PLANTHEAD_DEPTH, 0, myObject->children[0]->children[0]);

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

        /* Don't reset animation counts for the plants */
//        myObject->endDemo();

        /* Put placed plants into GAME mode */
        if (status != PLANT_STATUS_DEFAULT)
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
    * Function: Plant::updateTransition
    * Description: When a transition event occurs, perform some plant-specific
    * update.
    *****************************************************************************/
    void Plant::updateTransition(uint16_t val) {


        /* Transitions are plant-specific, and usually will involve specific sprite swaps */
        switch (type) {
            case WALLNUT_PLANT:
                if (val == plantTransitions[WALLNUT_PLANT][0])
                    myObject->children[0]->updateSprite(WALLNUT_CRACKED_1);
                if (val == plantTransitions[WALLNUT_PLANT][1])
                    myObject->children[0]->updateSprite(WALLNUT_CRACKED_2);
                break;
            case TALLNUT_PLANT:
                if (val == plantTransitions[TALLNUT_PLANT][0])
                    myObject->children[0]->updateSprite(TALLNUT_CRACKED_1);
                if (val == plantTransitions[TALLNUT_PLANT][1])
                    myObject->children[0]->updateSprite(TALLNUT_CRACKED_2);
                break;

            default:
                break;
        }

    }



    /*****************************************************************************
    * Function: Plant::bite
    * Description: Updates the health (and potentially other aspects) when a
    * plant gets bitten
    *****************************************************************************/
    void Plant::bite() {

        health--;

        // WALLNUT_PLANTS and TALLNUT_PLANTS stop moving once bitten
        if ((type == WALLNUT_PLANT) || (type == TALLNUT_PLANT)) {
            myObject->children[0]->anim[OBJECT_STATUS_DEMO].delta_angle = 0.0;
            myObject->children[0]->anim[OBJECT_STATUS_DEMO].delta_yscale = 0.0;
            myObject->setMode(OBJECT_STATUS_DEMO);
        }


    }


    /*****************************************************************************
    * Function: Plant::fire
    * Description: Performs a plant-specific action command
    *****************************************************************************/
    void Plant::fire() {

        // We shouldn't fire a non-placed plant
        if (status != PLANT_STATUS_GAME)
            return;

        fvu::Particle *local_particle;

        switch (type) {
            case PEASHOOTER_PLANT:
            case REPEATER_PLANT:
            case GATLING_PLANT:
            case SNOW_PLANT:
                if (fire_count == 0) {
                    myObject->setMode(OBJECT_STATUS_ACTION);
                    fire_count++;
                }
                else {
                    fire_count++;
                    if (fire_count == 20) {
                        if (type == SNOW_PLANT) {
                            local_particle = new Particle(SNOW_PROJECTILE, this);
                        }
                        else {
                            local_particle = new Particle(PEA_PROJECTILE, this);
                        }
                        myGame->myParticles[team].push_back(*local_particle);
                        myGame->playSound(SFX_THROW, 25);
                        has_fired = true;
                        fire_count = 0;
                        myObject->setMode(OBJECT_STATUS_DEMO);
                    }
                }
                break;
            case CHOMPER_PLANT:
                if (fire_count == 0) {
                    myObject->setMode(OBJECT_STATUS_ACTION);
                    fire_count++;
                }
                else {
                    fire_count++;
                    if (fire_count == 20) {
                        has_fired = true;
                        fire_count = 0;

                        // Did we chomp a zombie? If so, act accordingly
                        has_hit = false;
                        uint16_t i;
                        for (i = 0; i < myGame->myZombies[team].size(); i++) {
                            if ((myGame->myZombies[team][i].getRow() == row) && (myGame->myZombies[team][i].getCol() == col)) {
                                has_hit = true;
                                break;
                            }
                            if ((col != NUM_COLS) && (myGame->myZombies[team][i].getRow() == row) && (myGame->myZombies[team][i].getCol() == col+1)) {
                                has_hit = true;
                                break;
                            }
                        }

                        // We can use the zombie::shoot()
                        if (has_hit == true) {
                            local_particle = new Particle(CHOMP_PROJECTILE, this);
                            myGame->myZombies[team][i].shoot(local_particle);
                            myGame->playSound(SFX_BIGCHOMP, 25);
                            myObject->setMode(OBJECT_STATUS_WINNING);
                            myObject->children[0]->children[0]->children[0]->children[4]->children[0]->children[0]->updateSprite(ZOMBIE_INNERARM_LOWER);
                            myObject->children[0]->children[0]->children[0]->children[4]->children[0]->children[0]->children[0]->updateSprite(ZOMBIE_INNERARM_HAND);
                            action_count = 1000;
                            delete local_particle;
                        }
                        else {
                            myObject->setMode(OBJECT_STATUS_DEMO);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }



    /*****************************************************************************
    * Function: Plant::update
    * Description: Updates each plant
    *****************************************************************************/
    void Plant::update() {


        /* Check all the active plants and if their current health has triggered a transition */
        if (status == PLANT_STATUS_GAME) {

            // If we're in the middle of a fire operation, continue it
            if (fire_count != 0) {
                fire();
            }

            // Otherwise, update our action counter, so that we can act again in the near future
            else if (action_count > 0) {
                action_count--;
                // Special case for CHOMPER plants
                if ((action_count == 0) && (type == CHOMPER_PLANT) && (has_hit == true)) {
                    myObject->children[0]->children[0]->children[0]->children[4]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[0]->children[4]->children[0]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->setMode(OBJECT_STATUS_DEMO);
                }
            }


            /* Check if there are any transitions left */
            while (!transitions.empty()) {

                if (health <= transitions[0]) {
                    updateTransition(transitions[0]);
                    transitions.erase(transitions.begin());
                }
                else {
                    break;
                }
            }

            if (health <= 0) {
                myGame->playSound(SFX_GULP, 50);
                status = PLANT_STATUS_INACTIVE;
                myGame->plantGrid[team][row][col] = false;
            }

        }

        myObject->update();
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
