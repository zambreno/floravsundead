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
        //transitions = std::vector<uint16_t>();
        for (uint8_t i = 0; i < NUM_PLANT_TRANSITIONS; i++) {
            if (plantTransitions[type][i] == 0)
                break;
            transitions.push_back(plantTransitions[type][i]);
        }


        Object *local_object;
        animation_struct demo_anim;

        switch(type) {
            case WALLNUT_PLANT:
                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                demo_anim.set_defaults();
                myObject = new Object(demo_anim, demo_anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the wallnut body
                demo_anim.set_defaults();
                demo_anim.set_x(-22.0, 0.0, 0.0);
                demo_anim.set_y(-35.0, 0.0, 0.0);
                demo_anim.set_angle(-2.0, 0.1, 2.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.001, 0.66);

                myObject->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, WALLNUT_BODY, PLANTHEAD_DEPTH, 0, myObject);
                myObject->children[0]->anchor = ANCHOR_S;


                // children[1] is the shadow
                demo_anim.set_defaults();
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -22.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -50.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                demo_anim.set_xscale(0.85, 0.0, 1.0);
                demo_anim.set_yscale(0.85, 0.0, 1.0);

                myObject->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);



                break;

            // This is for testing purposes only. Don't mess with it.
            case GATLING_PLANT:
                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                demo_anim.set_defaults();
                myObject = new Object(demo_anim, demo_anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the bottom part of the stalk
                demo_anim.set_defaults();
                demo_anim.set_angle(0.0, 1.0, 7200.0);
                myObject->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, WHITE_PLANT_SPRITE, PLANTHEAD_DEPTH, 1, myObject);

                demo_anim.set_defaults();
                demo_anim.set_x(108.0, 0.0, 0.0);
                demo_anim.set_y(0.0, 0.0, 0.0);
                demo_anim.set_angle(-90.0, 2.0, 90.0);
                myObject->children[0]->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, WHITE_PLANT_SPRITE, PLANTHEAD_DEPTH, 1, myObject->children[0]);
                myObject->children[0]->children[0]->anchor = ANCHOR_W;


                demo_anim.set_defaults();
                demo_anim.set_x(54.0, 0.0, 0.0);
                demo_anim.set_y(0.0, 0.0, 0.0);
                demo_anim.set_angle(45.0, 2.0, 135.0);
                myObject->children[0]->children[0]->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, WHITE_PLANT_SPRITE, PLANTHEAD_DEPTH, 0, myObject->children[0]->children[0]);
                myObject->children[0]->children[0]->children[0]->anchor = ANCHOR_S;



                break;

            case PEASHOOTER_PLANT:
            case CHOMPER_PLANT:
                 /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                demo_anim.set_defaults();
                myObject = new Object(demo_anim, demo_anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the bottom part of the stalk
                demo_anim.set_defaults();
                demo_anim.set_x(5.0, 0.0, 0.0);
                demo_anim.set_y(-10.0, 0.0, 0.0);
                demo_anim.set_xscale(0.8, 0.0, 1.0);
                demo_anim.set_yscale(0.8, 0.0, 1.0);
                myObject->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_STEM_3, STEM_DEPTH, 5, myObject);

                // children[0]->children[0] is the middle part of the stem
                demo_anim.set_defaults();
                demo_anim.set_x(-17.0, 0.0, 0.0);
                demo_anim.set_y(4.75, 0.0, 0.0);
                demo_anim.set_xscale(0.8, 0.0, 1.0);
                demo_anim.set_yscale(0.8, 0.0, 1.0);
                myObject->children[0]->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_STEM_2, STEM_DEPTH, 1, myObject->children[0]);

                   // children[0]->children[0]->children[0] is the top of the stem
                local_object = myObject->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(0.0, 0.0, 0.0);
                demo_anim.set_y(20.0, 0.0, 0.0);
                demo_anim.set_xscale(0.8, 0.0, 1.0);
                demo_anim.set_yscale(0.8, 0.0, 1.0);

                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_STEM_1, STEM_DEPTH, 7, local_object);

                // children[0]->children[0]->children[0]->children[0] is lowest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(3.0, 0.0, 0.0);
                demo_anim.set_y(-3.5, 0.0, 0.0);
                demo_anim.set_xscale(1, 0.0, 1.0);
                demo_anim.set_yscale(1, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_4, HEADLEAF_2_4_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[1] is the middle headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(-8.0, 0.0, 0.0);
                demo_anim.set_y(2.0, 0.0, 0.0);
                demo_anim.set_xscale(1, 0.0, 1.0);
                demo_anim.set_yscale(1, 0.0, 1.0);
                local_object->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_1, HEADLEAF_1_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[2] is the second highest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(0.0, 0.0, 0.0);
                demo_anim.set_y(1.25, 0.0, 0.0);
                demo_anim.set_xscale(1, 0.0, 1.0);
                demo_anim.set_yscale(1, 0.0, 1.0);
                local_object->children[2] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_2, HEADLEAF_2_4_DEPTH, 0, local_object);


                // children[0]->children[0]->children[0]->children[3] is the highest headleaf
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(5.0, 0.0, 0.0);
                demo_anim.set_y(6.75, 0.0, 0.0);
                demo_anim.set_xscale(1, 0.0, 1.0);
                demo_anim.set_yscale(1, 0.0, 1.0);
                local_object->children[3] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_HEADLEAF_3, HEADLEAF_3, 0, local_object);


 // children[0]->children[0]->children[0]->children[4] is the underjaw
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(10.0, 0.0, 0.0);
                demo_anim.set_y(-25.0, 0.0, 0.0);
                demo_anim.set_angle(0.0, 0.0, 0.0);
                demo_anim.set_xscale(0.6, 0.0, 1.0);
                demo_anim.set_yscale(0.6, 0.0, 1.0);
                local_object->children[4] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_UNDERJAW, PLANTHEAD_DEPTH, 1, local_object);

 // children[0]->children[0]->children[0]->children[4]->children[0] is the bottom lip
                local_object = myObject->children[0]->children[0]->children[0]->children[4];
                demo_anim.set_defaults();
                demo_anim.set_x(8.75, 0.0, 0.0);
                demo_anim.set_y(7.25, 0.0, 0.0);
                demo_anim.set_xscale(0.6, 0.0, 1.0);
                demo_anim.set_yscale(0.6, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_BOTTOMLIP, PLANTHEAD_DEPTH, 0, local_object);


 // children[0]->children[0]->children[0]->children[5] is the top of the head
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(14.5, 0.0, 0.0);
                demo_anim.set_y(7.3, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[5] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_TOPJAW, PLANTHEAD_DEPTH, 4, local_object);
 // children[0]->children[0]->children[0]->children[5]->children[0] is the 1st spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                demo_anim.set_defaults();
                demo_anim.set_x(14.5, 0.0, 0.0);
                demo_anim.set_y(7.3, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[1] is the 2nd spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                demo_anim.set_defaults();
                demo_anim.set_x(14.5, 0.0, 0.0);
                demo_anim.set_y(7.3, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[2] is the 3rd spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                demo_anim.set_defaults();
                demo_anim.set_x(14.5, 0.0, 0.0);
                demo_anim.set_y(7.3, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[2] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

                 // children[0]->children[0]->children[0]->children[5]->children[3] is the 4th spike from the left
                local_object = myObject->children[0]->children[0]->children[0]->children[5];
                demo_anim.set_defaults();
                demo_anim.set_x(14.5, 0.0, 0.0);
                demo_anim.set_y(7.3, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[3] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_SPIKE, CHOMPER_INSIDEMOUTH_DEPTH, 0, local_object);

// children[0]->children[0]->children[0]->children[6] is the inside of the mouth
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(19.5, 0.0, 0.0);
                demo_anim.set_y(-12, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[6] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_INSIDEMOUTH, CHOMPER_INSIDEMOUTH_DEPTH, 1, local_object);

// children[0]->children[0]->children[0]->children[6]->children[0] is the tongue
                local_object = myObject->children[0]->children[0]->children[0]->children[6];
                demo_anim.set_defaults();
                demo_anim.set_x(0, 0.0, 0.0);
                demo_anim.set_y(7.3, 0.0, 0.0);
                demo_anim.set_xscale(0.60, 0.0, 1.0);
                demo_anim.set_yscale(0.60, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_TONGUE, CHOMPER_TONGUE_DEPTH, 0, local_object);


                // children[0]->children[1] is the left backleaf
                demo_anim.set_defaults();
                demo_anim.set_x(-18.0, 0.0, 0.0);
                demo_anim.set_y(-80.75, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_3, BACKLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[2] is the right backleaf
                demo_anim.set_defaults();
                demo_anim.set_x(-25.0, 0.0, 0.0);
                demo_anim.set_y(-80.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[2] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_4, BACKLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[3] is the left frontleaf
                demo_anim.set_defaults();
                demo_anim.set_x(-25.0, 0.0, 0.0);
                demo_anim.set_y(-80.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[3] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_1, FRONTLEAF_DEPTH, 0, myObject->children[0]);

                // children[0]->children[4] is the right frontleaf
                demo_anim.set_defaults();
                demo_anim.set_x(0.0, 0.0, 0.0);
                demo_anim.set_y(-20.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[4] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, CHOMPER_GROUNDLEAF_2, CHOMPER_RIGHT_FRONTLEAF_DEPTH, 0, myObject->children[0]);


                // children[1] is the shadow
                demo_anim.set_defaults();
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -25.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -50.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                demo_anim.set_xscale(0.85, 0.0, 1.0);
                demo_anim.set_yscale(0.85, 0.0, 1.0);
                myObject->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);

                break;
            default:

                /* The object structure starts at the x/y location of the plant head, and moves out in all directions */
                demo_anim.set_defaults();
                myObject = new Object(demo_anim, demo_anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the bottom part of the stalk
                demo_anim.set_defaults();
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_STALK_BOTTOM, STEM_DEPTH, 3, myObject);

                // children[0]->children[0] is the top part of the stalk
                demo_anim.set_defaults();
                demo_anim.set_x(1.0, 0.0, 0.0);
                demo_anim.set_y(11.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_STALK_TOP, STEM_DEPTH, 1, myObject->children[0]);


                // children[0]->children[1] is the backleaf
                demo_anim.set_defaults();
                demo_anim.set_x(-11.0, 0.0, 0.0);
                demo_anim.set_y(2.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF, BACKLEAF_DEPTH, 2, myObject->children[0]);

                // children[0]->children[1]->children[0] is the left tip of the backleaf
                local_object = myObject->children[0]->children[1];
                demo_anim.set_defaults();
                demo_anim.set_x(-3.0, 0.0, 0.0);
                demo_anim.set_y(6.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF_LEFTTIP, BACKLEAF_DEPTH, 0, local_object);

                // children[0]->children[1]->children[1] is the right tip of the backleaf
                local_object = myObject->children[0]->children[1];
                demo_anim.set_defaults();
                demo_anim.set_x(24.0, 0.0, 0.0);
                demo_anim.set_y(8.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_BACKLEAF_RIGHTTIP, BACKLEAF_DEPTH, 0, local_object);

                // children[0]->children[2] is the frontleaf
                demo_anim.set_defaults();
                demo_anim.set_x(-17.0, 0.0, 0.0);
                demo_anim.set_y(-15.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                myObject->children[0]->children[2] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF, FRONTLEAF_DEPTH, 2, myObject->children[0]);

                // children[0]->children[2]->children[0] is the left tip of the frontleaf
                local_object = myObject->children[0]->children[2];
                demo_anim.set_defaults();
                demo_anim.set_x(-3.0, 0.0, 0.0);
                demo_anim.set_y(-2.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF_LEFTTIP, FRONTLEAF_DEPTH, 0, local_object);

                // children[0]->children[2]->children[1] is the right tip of the frontleaf
                local_object = myObject->children[0]->children[2];
                demo_anim.set_defaults();
                demo_anim.set_x(39.0, 0.0, 0.0);
                demo_anim.set_y(4.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_FRONTLEAF_RIGHTTIP, FRONTLEAF_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0] is the head
                local_object = myObject->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(-19.0, 0.0, 0.0);
                demo_anim.set_y(6.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_HEAD, PLANTHEAD_DEPTH, 2, local_object);

                // children[0]->children[0]->children[0]->children[0] is the lips
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(39.0, 0.0, 0.0);
                demo_anim.set_y(7.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[0] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_LIPS, PLANTHEAD_DEPTH, 0, local_object);

                // children[0]->children[0]->children[0]->children[1] is the sprout
                local_object = myObject->children[0]->children[0]->children[0];
                demo_anim.set_defaults();
                demo_anim.set_x(-9.0, 0.0, 0.0);
                demo_anim.set_y(19.0, 0.0, 0.0);
                demo_anim.set_xscale(0.65, 0.0, 1.0);
                demo_anim.set_yscale(0.65, 0.0, 1.0);
                local_object->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, PEASHOOTER_SPROUT, FRONTLEAF_DEPTH, 0, local_object);

                // children[1] is the shadow
                demo_anim.set_defaults();
                demo_anim.set_x(-25.0, 0.0, 0.0);
                demo_anim.set_y(-30.0, 0.0, 0.0);
                demo_anim.set_xscale(0.85, 0.0, 1.0);
                demo_anim.set_yscale(0.85, 0.0, 1.0);
                myObject->children[1] = new Object(demo_anim, demo_anim, anim_count, TEX_PLANTS, POT_SHADOW, PLANT_SHADOW_DEPTH, 0, myObject);

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
            myObject->children[0]->demo_anim.set_defaults();
            myObject->children[0]->angle = 0.0;
        }

    }



    /*****************************************************************************
    * Function: Plant::update
    * Description: Updates each plant
    *****************************************************************************/
    void Plant::update() {


        /* Check all the active plants and if their current health has triggered a transition */
        if (status == PLANT_STATUS_GAME) {


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
