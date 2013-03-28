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
int16_t plantHealths[NUM_PLANT_TYPE] = {9, 9, 9, 9, 9, 72};
/* Plant speeds. This variable is plant-specific */
float plantSpeeds[NUM_PLANT_TYPE] = {FRAME_RATE*2.25, FRAME_RATE*2.25/1.5, FRAME_RATE*2.25/3, FRAME_RATE*2.25, 300.0, FRAME_RATE*2.25};
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

            // This is for testing purposes only. Don't mess with it.
            case GATLING_PLANT:
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

            case CHOMPER_PLANT:
                 /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the bottom part of the stalk
                local_anim.set_defaults();
                local_anim.set_x(5.0, 0.0, 0.0);
                local_anim.set_y(-10.0, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_STEM_3, STEM_DEPTH, 5, myObject);

                // children[0]->children[0] is the middle part of the stem
                local_anim.set_defaults();
                local_anim.set_x(-17.0, 0.0, 0.0);
                local_anim.set_y(4.75, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_STEM_2, STEM_DEPTH, 1, myObject->children[0]);

                   // children[0]->children[0]->children[0] is the top of the stem
                local_object = myObject->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(0.0, 0.0, 0.0);
                local_anim.set_y(20.0, 0.0, 0.0);
                local_anim.set_xscale(0.8, 0.0, 1.0);
                local_anim.set_yscale(0.8, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_STEM_1, STEM_DEPTH, 6, local_object);

                // children[0]->children[0]->children[0]->children[0] is lowest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(3.0, 0.0, 0.0);
                local_anim.set_y(-3.5, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_4, HEADLEAF_2_4_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[1] is the middle headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(-8.0, 0.0, 0.0);
                local_anim.set_y(2.0, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_1, HEADLEAF_1_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[2] is the second highest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(0.0, 0.0, 0.0);
                local_anim.set_y(1.25, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_2, HEADLEAF_2_4_DEPTH, 0, local_object);


                // children[0]->children[0]->children[0]->children[3] is the highest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(5.0, 0.0, 0.0);
                local_anim.set_y(6.75, 0.0, 0.0);
                local_anim.set_xscale(1, 0.0, 1.0);
                local_anim.set_yscale(1, 0.0, 1.0);

                anim.clear();anim.push_back(local_anim);
                local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_3, HEADLEAF_3, 0, local_object);


 // children[0]->children[0]->children[0]->children[4] is the underjaw
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(8.5, 0.0, 0.0);
                local_anim.set_y(-26.5, 0.0, 0.0);
                local_anim.set_xscale(0.6, 0.0, 1.0);
                local_anim.set_yscale(0.6, 0.0, 1.0);
                local_anim.set_angle(15.0, 0.0, 0, ANCHOR_NW);
                anim.clear();anim.push_back(local_anim);
                local_object->children[4] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_UNDERJAW, PLANTHEAD_DEPTH, 3, local_object);

 // children[0]->children[0]->children[0]->children[4]->children[0] is the bottom lip
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(8.75, 0.0, 0.0);
                local_anim.set_y(7.25, 0.0, 0.0);
                local_anim.set_xscale(0.6, 0.0, 1.0);
                local_anim.set_yscale(0.6, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_BOTTOMLIP, PLANTHEAD_DEPTH, 0, local_object);

// children[0]->children[0]->children[0]->children[4]->children[1] is the inside of the mouth
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(19.5, 0.0, 0.0);
                local_anim.set_y(-6, 0.0, 0.0);
                local_anim.set_xscale(0.60, 0.0, 1.0);
                local_anim.set_yscale(0.60, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_INSIDEMOUTH, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

// children[0]->children[0]->children[0]->children[6]->children[0] is the tongue
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                local_anim.set_defaults();
                local_anim.set_x(0, 0.0, 0.0);
                local_anim.set_y(7.3, 0.0, 0.0);
                local_anim.set_xscale(0.60, 0.0, 1.0);
                local_anim.set_yscale(0.60, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_TONGUE, CHOMPER_TONGUE_DEPTH, 0, local_object);



 // children[0]->children[0]->children[0]->children[5] is the top of the head
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_x(14.5, 0.0, 0.0);
                local_anim.set_y(27.3, 0.0, 0.0);
                local_anim.set_xscale(0.60, 0.0, 1.0);
                local_anim.set_yscale(0.60, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[5] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_TOPJAW, PLANTHEAD_DEPTH, 4, local_object);

 // children[0]->children[0]->children[0]->children[5]->children[0] is the 1st spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(0, 0.0, 0.0);
                local_anim.set_y(12, 0.0, 0.0);
                local_anim.set_xscale(.7, 0.0, 1.0);
                local_anim.set_yscale(.7, 0.0, 1.0);
                local_anim.set_angle(58.0, 0.0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);


                 // children[0]->children[0]->children[0]->children[5]->children[1] is the 2nd spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(6.5, 0.0, 0.0);
                local_anim.set_y(17, 0.0, 0.0);
                local_anim.set_xscale(0.7, 0.0, 1.0);
                local_anim.set_yscale(0.7, 0.0, 1.0);
                local_anim.set_angle(40, 0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[2] is the 3rd spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(12, 0.0, 0.0);
                local_anim.set_y(21, 0.0, 0.0);
                local_anim.set_xscale(0.70, 0.0, 1.0);
                local_anim.set_yscale(0.70, 0.0, 1.0);
                local_anim.set_angle(30.0, 0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[3] is the 4th spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                local_anim.set_defaults();
                local_anim.set_x(20, 0.0, 0.0);
                local_anim.set_y(21.75, 0.0, 0.0);
                local_anim.set_xscale(0.70, 0.0, 1.0);
                local_anim.set_yscale(0.70, 0.0, 1.0);
                local_anim.set_angle(15.0, 0, 0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                local_object->children[3] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);


                // children[0]->children[1] is the left backleaf
                local_anim.set_defaults();
                local_anim.set_x(-18.0, 0.0, 0.0);
                local_anim.set_y(-80.75, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_3, BACKLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[2] is the right backleaf
                local_anim.set_defaults();
                local_anim.set_x(-25.0, 0.0, 0.0);
                local_anim.set_y(-80.0, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[2] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_4, BACKLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[3] is the left frontleaf
                local_anim.set_defaults();
                local_anim.set_x(-25.0, 0.0, 0.0);
                local_anim.set_y(-55.0, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[3] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_1, FRONTLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[4] is the right frontleaf
                local_anim.set_defaults();
                local_anim.set_x(0.0, 0.0, 0.0);
                local_anim.set_y(-20.0, 0.0, 0.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[4] = new Object(anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_2, CHOMPER_RIGHT_FRONTLEAF_DEPTH, 0, myObject->children[0]);


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
            default:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 4, NULL);

                // children[0] is the bottom part of the stalk
                local_anim.set_defaults();
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, -0.01, 0.25);
                local_anim.set_y(-22.0, 0.0, 0.0);
                local_anim.set_angle(-18.0, 0.9, 18.0, ANCHOR_S);
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
                local_object = myObject->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(-19.0, 6.0);
                local_anim.set_angle(18.0, -0.9, -18.0, ANCHOR_S);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_HEAD, PLANTHEAD_DEPTH, 2, local_object);

                // children[0]->children[0]->children[0]->children[0] is the lips
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(39.0, 7.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_LIPS, PLANTHEAD_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[1] is the sprout
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(-9.0, 19.0);
                local_anim.set_xscale(0.65, 0.0, 1.0);
                local_anim.set_yscale(0.65, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEASHOOTER_SPROUT, FRONTLEAF_DEPTH, 0, local_object);

                // children[3] is the shadow
                local_anim.set_defaults();
                local_anim.set_xy(-25.0, -50.0);
                local_anim.set_xscale(0.85, 0.0, 1.0);
                local_anim.set_yscale(0.85, 0.0, 1.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[3] = new Object(anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);

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

        // WALLNUT_PLANTS stop moving once bitten
        if (type == WALLNUT_PLANT) {
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
                local_particle = new Particle(PEA_PROJECTILE, this);
                myGame->myParticles[team].push_back(*local_particle);
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

            // Update our action counter, so that we can act again in the near future
            if (action_count > 0)
                action_count--;


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
                myGame->playSound(SFX_GULP);
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
