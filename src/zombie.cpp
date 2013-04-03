/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * zombies.cpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main zombie class.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"


/* Zombie names */
std::string zombieNames[NUM_ZOMBIE_TYPE][NUM_ZOMBIE_SPELLINGS] = {
    {"zombie", "regular"},
    {"flag", "fast"},
    {"conehead", "cone"},
    {"polevault", "pole"},
    {"buckethead", "bucket"},
    {"newspaper", "news"},
    {"screendoor", "screen"},
    {"football", "foot"},
    {"dancing", "dance"},
    {"yeti", "bigfoot"}
    };

/* Zombie healths */
int16_t zombieHealths[NUM_ZOMBIE_TYPE] = {10, 10, 28, 17, 65, 16, 65, 80, 17, 46};
/* Plant speeds. This variable is zombie-specific */
float zombieSpeeds[NUM_ZOMBIE_TYPE] = {0.25, 0.35, 0.25, 0.5, 0.25, 0.25, 0.25, 0.75, 0.25, 0.25};
/* Plant transitions */
uint16_t zombieTransitions[NUM_ZOMBIE_TYPE][NUM_ZOMBIE_TRANSITIONS] = {
    {5, 0, 0, 0},
    {5, 0, 0, 0},
    {21, 15, 10, 5},
    {8, 0, 0, 0},
    {46, 28, 10, 5},
    {13, 10, 8, 4},
    {46, 28, 10, 5},
    {56, 33, 10, 5},
    {8, 0, 0, 0},
    {23, 0, 0, 0}
    };


namespace fvu {

    /*****************************************************************************
    * Function: Zombie::draw
    * Description: Zombie drawing function. Sets the initial modeling position
    * and draws each Object recursively.
    *****************************************************************************/
    void Zombie::draw(uint16_t index) {

        float x, y, z;

        // Frozen zombies have their color modulated appropriately
        if (frozen == true) {
            glColor3ub(1, 145, 182);
        }
        else {
            glColor3ub(255, 255, 255);
        }


        // In DEMO mode, we can do a pure depth sort of all the zombies
        if (status == ZOMBIE_STATUS_DEMO) {
            x = demo_x;
            y = demo_y;
            z = index*ZOMBIE_DEPTH_RANGE;

            // Put objects on a different depth level if they're on the bottom
            if (team % 2 == 1) {
                z += OBJECT_BOTTOM_DEPTH;
            }
            else {
                z += OBJECT_TOP_DEPTH;
            }

        }

        // In WINNING/GAMEOVER status, the Zombies will leave their rows so we can do the full sort again.
        else if ((status == ZOMBIE_STATUS_WINNING) || (status == ZOMBIE_STATUS_WINNING)) {
            x = game_x;
            y = game_y;
            z = index*ZOMBIE_DEPTH_RANGE + OBJECT_BOTTOM_DEPTH;

        }

        // In GAME/ACTIVE mode, we need to be row-aware with our sorting
        else {
            x = game_x;
            y = game_y;
            z = index*ZOMBIE_DEPTH_RANGE;

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
    * Function: Zombie::Zombie
    * Description: Class constructor. Uses an enum type to set zombie-specific
    * parameters
    *****************************************************************************/
    Zombie::Zombie(uint8_t mytype, uint16_t myindex) {

        uint32_t anim_count = rand();

        status = ZOMBIE_STATUS_DEFAULT;
        type = mytype;
        index = myindex;
        action_count = 0;
        special_count = 0;
        special_done = false;
        death_count = 0;
        has_item = true;
        frozen_count = 0;
        frozen = false;

        Object *local_object;

        /* Initialize zombie information here so we can leave the rest of this function as object assembly */
        health = zombieHealths[type];
        speed = zombieSpeeds[type];
        for (uint8_t i = 0; i < NUM_ZOMBIE_TRANSITIONS; i++) {
            if (zombieTransitions[type][i] == 0)
                break;
            transitions.push_back(zombieTransitions[type][i]);
        }

        std::vector<animation_struct> anim;
        animation_struct local_anim;

        switch(type) {

            case FOOTBALL_ZOMBIE:
                /* The object structure starts at the x/y location of the outer leg, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();
                anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 2, NULL);

                // children[0] is the lower body
                local_anim.set_defaults();
                local_anim.set_angle(5.0, -0.25, -5.0, ANCHOR_S);
                local_anim.set_y(25.0, -0.25, 15.0);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_angle(10.0, -0.75, -10.0, ANCHOR_S);
                local_anim.set_y(25.0, -0.25, 15.0);anim.push_back(local_anim);
                local_anim.set_defaults();local_anim.set_y(25.0);local_anim.set_angle(10.0, 0.0, 0.0, ANCHOR_S);anim.push_back(local_anim);
                local_anim.set_angle(5.0, -0.25, -5.0, ANCHOR_S);local_anim.set_y(25.0, -0.25, 15.0);anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LOWERBODY, ZOMBIEBODY_DEPTH, 4, myObject);

                // children[0][0] is the left leg
                local_anim.set_defaults();
                local_anim.set_xy(-16, -23);
                local_anim.set_angle(-5.0, 0.25, 5.0, ANCHOR_N);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_angle(60.0, -2.667, -40.0, ANCHOR_N);local_anim.set_xy(-11.0, -18.0);anim.push_back(local_anim);
                local_anim.set_xy(-16, -23);local_anim.set_angle(0.0);anim.push_back(local_anim);
                local_anim.set_xy(-16, -23);local_anim.set_angle(-5.0, 0.25, 5.0, ANCHOR_N);anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTLEG_UPPER, ZOMBIE_INNERLEG_UPPER_DEPTH, 1, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(5, -17);
                local_anim.set_angle(-5.0, 0.25, 5.0, ANCHOR_N);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTLEG_LOWER, ZOMBIE_INNERLEG_LOWER_DEPTH, 1, myObject->children[0]->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-19, -17);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTLEG_FOOT, ZOMBIE_INNERLEG_FOOT_DEPTH, 0, myObject->children[0]->children[0]->children[0]);


                // children[0][1] is the right leg
                local_anim.set_defaults();
                local_anim.set_xy(22.0, -30.0);
                local_anim.set_angle(-7.0, 0.35, 7.0, ANCHOR_N);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_angle(-40.0, 2.667, 60.0, ANCHOR_N);local_anim.set_xy(18.0, -27.0);anim.push_back(local_anim);
                local_anim.set_xy(22.0, -30.0);local_anim.set_angle(0.0);anim.push_back(local_anim);
                local_anim.set_angle(-7.0, 0.35, 7.0, ANCHOR_N);anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_RIGHTLEG_UPPER, ZOMBIE_OUTERLEG_UPPER_DEPTH, 1, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(17, -20);
                local_anim.set_angle(0.0, 0.25, 10.0, ANCHOR_N);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_RIGHTLEG_LOWER, ZOMBIE_OUTERLEG_LOWER_DEPTH, 1, myObject->children[0]->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(-27, -19);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_RIGHTLEG_FOOT, ZOMBIE_OUTERLEG_FOOT_DEPTH, 0, myObject->children[0]->children[1]->children[0]);


                // children[0][2] is the left upper body
                local_anim.set_defaults();
                local_anim.set_xy(-53, 29.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_UPPERBODY_2, ZOMBIEBODY_DEPTH, 2, myObject->children[0]);

                local_object = myObject->children[0]->children[2];
                local_anim.set_defaults();
                local_anim.set_xy(18, -5);
                local_anim.set_angle(4.0, -0.35, -4.0, ANCHOR_N);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_xy(14.0, 8.0);
                local_anim.set_angle(-60.0, 0.0, 0.0, ANCHOR_N);anim.push_back(local_anim);
                local_anim.set_xy(18, -5);
                local_anim.set_angle(40.0, -3.5, -40.0, ANCHOR_N);anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTARM_UPPER, ZOMBIE_INNERARM_UPPER_DEPTH, 1, local_object);

                local_anim.set_defaults();
                local_anim.set_xy(-23, -1);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_angle(60.0, 0.0, 0.0, ANCHOR_E);anim.push_back(local_anim);
                local_anim.set_angle(0.0, 0.875, 20.0, ANCHOR_E);anim.push_back(local_anim);
                local_anim.set_angle(0.0, 3.0, 40.0, ANCHOR_N);anim.push_back(local_anim);
                local_object->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTARM_LOWER, ZOMBIE_INNERARM_LOWER_DEPTH, 1, local_object->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-29, -1);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTARM_HAND, ZOMBIE_INNERARM_HAND_DEPTH, 0, local_object->children[0]->children[0]);


                // children[0][2][1] is the head body
                local_object = myObject->children[0]->children[2];
                local_anim.set_defaults();
                local_anim.set_angle(-6.0, 0.15, 5.0, ANCHOR_S);
                local_anim.set_xy(20.0, 27.0);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_angle(-12.0, 0.8, 12.0, ANCHOR_S);anim.push_back(local_anim);
                local_anim.set_angle(7.0, 0.0, 0.0, ANCHOR_S);anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_HEAD, ZOMBIE_HEADFRONT_DEPTH, 2, local_object);

                local_anim.set_defaults();
                local_anim.set_xy(-2.0, -8.0);
                local_anim.set_angle(-2.0, 0.059, 2.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_HELMET, ZOMBIE_ACCESSORY_DEPTH, 0, local_object->children[1]);

                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(7.2, -7.5);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(4.0, -1.0, -12.0, ANCHOR_NE);anim.push_back(local_anim);
                local_object->children[1]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, ZOMBIE_HEADFRONT_DEPTH, 0, local_object->children[1]);


                // children[0][3] is the right upper body
                local_anim.set_defaults();
                local_anim.set_xy(-53, 7.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[3] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_UPPERBODY, ZOMBIEBODY_DEPTH, 2, myObject->children[0]);

                // Upperbody overlay
                local_object = myObject->children[0]->children[3];
                local_anim.set_defaults();
                local_anim.set_xy(57.0, 22.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_UPPERBODY_3, ZOMBIE_OUTERARM_LOWER_DEPTH, 0, local_object);

                local_anim.set_defaults();
                local_anim.set_xy(85, 10);
                local_anim.set_angle(-4.0, 0.35, 4.0, ANCHOR_N);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_angle(-40.0, 3.0, 40.0, ANCHOR_N);anim.push_back(local_anim);
                local_anim.set_angle(-40.0, 3.0, 40.0, ANCHOR_N);anim.push_back(local_anim);
                local_anim.set_angle(0.0, 3.0, 40.0, ANCHOR_N);anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_RIGHTARM_UPPER, ZOMBIE_OUTERARM_UPPER_DEPTH, 1, local_object);

                local_anim.set_defaults();
                local_anim.set_xy(-18, -8);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(-10.0, 0.375, 10.0, ANCHOR_N);anim.push_back(local_anim);
                local_object->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_RIGHTARM_LOWER, ZOMBIE_OUTERARM_LOWER_DEPTH, 1, local_object->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(-32, -15);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_RIGHTARM_HAND, ZOMBIE_OUTERARM_HAND_DEPTH, 0, local_object->children[1]->children[0]);


                // children[1] is the shadow
                local_anim.set_defaults();
                local_anim.set_xy(-15.0, -52.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_SHADOW, ZOMBIE_SHADOW_DEPTH, 0, myObject);

                break;


            case POLE_ZOMBIE:
                /* The object structure starts at the x/y location of the outer leg, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();
                anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 3, NULL);


                // children[0] is the outer leg, starting at the upper part. It connects to the rest of the leg and the body
                local_anim.set_defaults();
                local_anim.set_angle(1.0, 0.2, 9.0, ANCHOR_N);
                local_anim.set_xy(39.0, -2.0);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(9.0, -3.3, -90.0, ANCHOR_N);anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_OUTERLEG_UPPER,ZOMBIE_OUTERLEG_UPPER_DEPTH, 2, myObject);

                local_anim.set_defaults();
                local_anim.set_angle(-1.0, -0.2, -9.0, ANCHOR_N);
                local_anim.set_xy(0, -23.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_OUTERLEG_LOWER, ZOMBIE_OUTERLEG_LOWER_DEPTH,1, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-6.0, -16.5);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_OUTERLEG_FOOT, ZOMBIE_OUTERLEG_FOOT_DEPTH, 1, myObject->children[0]->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-11.0, 0.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_OUTERLEG_TOE, ZOMBIE_OUTERLEG_FOOT_DEPTH, 0, myObject->children[0]->children[1]->children[0]);


                // children[1] is the inner leg, starting at the foot. It connects to the rest of the leg
                local_anim.set_defaults();
                local_anim.set_xy(14.0, -34.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERLEG_TOE, ZOMBIE_INNERLEG_TOE_DEPTH, 2, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(-10.0, 2.0);
                local_anim.set_angle(0.0, 0.25, 10.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERLEG_FOOT, ZOMBIE_INNERLEG_FOOT_DEPTH, 0, myObject->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(-3.0, 9.0);
                local_anim.set_angle(0.0, 0.25, 10.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[1] = new Object(anim, anim_count,  TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERLEG_LOWER, ZOMBIE_INNERLEG_LOWER_DEPTH,1, myObject->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(12.0, 21.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERLEG_UPPER, ZOMBIE_INNERLEG_UPPER_DEPTH, 0, myObject->children[1]->children[1]);


                // children[0][0] is the underbody. It connects to the upperbody
                local_anim.set_defaults();
                local_anim.set_angle(5.0, -0.1, -5.0, ANCHOR_SE);
                local_anim.set_xy(-22.0, 4.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_BODY2, ZOMBIEBODY_DEPTH, 1, myObject->children[0]);

                // children[0][0][0] is the upper body
                local_anim.set_defaults();
                local_anim.set_xy(-16.0, 15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_BODY1, ZOMBIEBODY_DEPTH, 3, myObject->children[0]->children[0]);

                // children[0][0][0][0] is the head
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_angle(-12.0, 0.25, 5.0, ANCHOR_S);
                local_anim.set_xy(-24.5, 51.75);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(7.0, 0.0, 0.0, ANCHOR_S);anim.push_back(local_anim);
                local_object->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD, ZOMBIE_HEADFRONT_DEPTH, 2, local_object);

                // children[0][0][0][0][0] is the hair
                local_anim.set_defaults();
                local_anim.set_angle(-2.0, 0.0, 0.0, ANCHOR_CENTER);
                local_anim.set_x(-7.0);
                local_anim.set_y(14.0, -0.05, 12.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_HAIR, ZOMBIE_HEADFRONT_DEPTH, 0, local_object->children[0]);

                // children[0][0][0][0][1] is the jaw
                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(10.2, -9.5);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(4.0, -1.0, -12.0, ANCHOR_NE);anim.push_back(local_anim);
                local_object->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, ZOMBIE_HEADFRONT_DEPTH, 0, local_object->children[0]);


                // children[0][0][0][1] is the inner arm. It connects to the rest of the arm. Note that these 2 sprites are backwards.
                local_object = myObject->children[0]->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_xy(-18.0, 40.0);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(90.0, 0.0, 0.0, ANCHOR_N);anim.push_back(local_anim);
                local_object->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERARM_LOWER, ZOMBIE_INNERARM_UPPER_DEPTH,1, local_object);

                local_anim.set_defaults();
                local_anim.set_xy(-15.0, -1.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERARM_UPPER,ZOMBIE_INNERARM_LOWER_DEPTH,1, local_object->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(-2.0, 33.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_INNERHAND, ZOMBIE_INNERARM_HAND_DEPTH, 2, local_object->children[1]->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-72.0, 12.0);
                local_anim.set_angle(0.0, -0.1, -1.5, ANCHOR_W);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1]->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_POLE, ZOMBIE_INNERARM_UPPER_DEPTH, 0, local_object->children[1]->children[0]->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-72.0,12.0);
                local_anim.set_angle(0.0, 0.1, 1.5, ANCHOR_E);
                anim.clear();anim.push_back(local_anim);
                local_object->children[1]->children[0]->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_POLE2, ZOMBIE_INNERARM_UPPER_DEPTH, 0, local_object->children[1]->children[0]->children[0]);



                // children[0][0][0][2] is the outer arm. It connects to the rest of the arm.
                local_anim.set_defaults();
                local_anim.set_angle(8.0, 0.05, 13.0, ANCHOR_NE);
                local_anim.set_xy(26.5, 20.5);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(13.0, -1.85, -25.0, ANCHOR_N);anim.push_back(local_anim);
                local_anim.set_angle(13.0, -2.0, -47.0, ANCHOR_N);anim.push_back(local_anim);
                local_anim.set_angle(-90.0, 0.0, 0.0, ANCHOR_N);anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_OUTERARM_UPPER, ZOMBIE_OUTERARM_UPPER_DEPTH,1, local_object);

                local_anim.set_defaults();
                local_anim.set_xy(-15.0, -30.0);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(-17.0, -1.15, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                local_anim.set_angle(-80.0, 0.0, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                local_object->children[2]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES,ZOMBIE_POLEVAULTER_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, local_object->children[2]);

                local_anim.set_defaults();
                local_anim.set_xy(-4.0, -22.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, local_object->children[2]->children[0]);


                // children[2] is the shadow
                local_anim.set_defaults();
                local_anim.set_xy(0.0, -50.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_SHADOW, ZOMBIE_SHADOW_DEPTH, 0, myObject);

                break;


            case REGULAR_ZOMBIE:
            case CONE_ZOMBIE:
            case BUCKET_ZOMBIE:
            case FLAG_ZOMBIE:
            case SCREEN_ZOMBIE:
            default:

                /* The object structure starts at the x/y location of the outer leg, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();
                anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 3, NULL);


                // children[0] is the outer leg, starting at the upper part. It connects to the rest of the leg and the body
                local_anim.set_defaults();
                if (type == FLAG_ZOMBIE) {
                    local_anim.set_angle(1.0, 0.05, 4.0, ANCHOR_N);
                }
                else {
                    local_anim.set_angle(1.0, 0.2, 9.0, ANCHOR_N);
                }
                local_anim.set_xy(27.0, -15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_UPPER,ZOMBIE_OUTERLEG_UPPER_DEPTH, 2, myObject);

                local_anim.set_defaults();
                local_anim.set_angle(-1.0, -0.2, -9.0, ANCHOR_N);
                local_anim.set_xy(10, -20);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_LOWER,ZOMBIE_OUTERLEG_LOWER_DEPTH,1, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-18, -10);
                local_anim.set_angle(1.0, 0.2, 9.0, ANCHOR_SE);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_FOOT,ZOMBIE_OUTERLEG_FOOT_DEPTH,0, myObject->children[0]->children[0]);


                // children[1] is the inner leg, starting at the foot. It connects to the rest of the leg
                local_anim.set_defaults();
                local_anim.set_xy(7.0, -34.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERLEG_FOOT,ZOMBIE_INNERLEG_FOOT_DEPTH, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(0.0, 0.0);
                local_anim.set_angle(0.0, 0.25, 10.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[0] = new Object(anim, anim_count,  TEX_ZOMBIES, ZOMBIE_INNERLEG_LOWER,ZOMBIE_INNERLEG_LOWER_DEPTH,1, myObject->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(16, 27);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERLEG_UPPER,ZOMBIE_INNERLEG_UPPER_DEPTH, 0, myObject->children[1]->children[0]);


                // children[0][0] is the body. It connects to the tie, the underbody, the arms, and the head
                local_anim.set_defaults();
                if ((type == FLAG_ZOMBIE) || (type == SCREEN_ZOMBIE)) {
                    local_anim.set_angle(2.0, -0.04, -2.0, ANCHOR_SE);
                }
                else {
                    local_anim.set_angle(5.0, -0.1, -5.0, ANCHOR_SE);
                }
                local_anim.set_xy(-25, 20);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIEBODY, ZOMBIEBODY_DEPTH, 5, myObject->children[0]);

                // children[0][0][0] is the tie
                local_anim.set_defaults();
                local_anim.set_angle(12.0, -0.3, -8.0, ANCHOR_N);
                local_anim.set_xy(0.0, 15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_TIE, ZOMBIE_ACCESSORY_DEPTH, 0, myObject->children[0]->children[0]);

                // children[0][0][1] is the zombie_underbody
                local_anim.set_defaults();
                local_anim.set_xy(15.0, 15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, BLANK_SPRITE, ZOMBIEBODY_DEPTH, 0, myObject->children[0]->children[0]);

                // CONE and BUCKET zombies have a second object on their head
                // ZOMBIES have a tongue 50% of the time
                uint8_t num_head_children = 2;
                uint8_t has_tongue=0;

                if (type == REGULAR_ZOMBIE) {
                    has_tongue = rand()%2;
                }
                if ((type == CONE_ZOMBIE) || (type == BUCKET_ZOMBIE)) {
                   num_head_children++;
                }


                // children[0][0][2] is the head
                local_object = myObject->children[0]->children[0];
                local_anim.set_defaults();
                local_anim.set_angle(-12.0, 0.25, 5.0, ANCHOR_S);
                local_anim.set_xy(-24.5, 51.75);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(7.0, 0.0, 0.0, ANCHOR_S);anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD,ZOMBIE_HEAD_DEPTH, num_head_children, local_object);

                // children[0][0][2][0] is the hair
                local_anim.set_defaults();
                local_anim.set_angle(-2.0, 0.25, 3.0, ANCHOR_CENTER);
                local_anim.set_xy(-5, 20.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HAIR, ZOMBIE_HEAD_DEPTH, 0, local_object->children[2]);

                // children[0][0][2][1] is the jaw
                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(10.2, -9.5);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(4.0, -1.0, -12.0, ANCHOR_NE);anim.push_back(local_anim);
                local_object->children[2]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, ZOMBIE_HEAD_DEPTH, has_tongue, local_object->children[2]);

                // children[0][0][2][2] is the cone/bucket
                if (type == CONE_ZOMBIE) {
                    local_anim.set_defaults();
                    local_anim.set_xy(2.5, 22.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2]->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_CONE_1, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[2]);
                }
                else if (type == BUCKET_ZOMBIE) {
                    local_anim.set_defaults();
                    local_anim.set_xy(-7.0, 1.75);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2]->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_BUCKET_1, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[2]);
                }


                // children[0][0][2][1][0] is the tongue (optional)
                if (has_tongue == 1) {
                    local_anim.set_defaults();
                    local_anim.set_xy(8.0, -5.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2]->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_TONGUE, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[2]->children[1]);
                }

                // The FLAG_ZOMBIE has a slightly different inner arm
                if ((type == FLAG_ZOMBIE) || (type == SCREEN_ZOMBIE)) {
                    // children[0][0][3] is the inner arm. It connects to the rest of the arm.
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_xy(-32.0, 23.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_SCREENDOOR,ZOMBIE_INNERARM_LOWER_DEPTH, 1, local_object);

                    if (type == FLAG_ZOMBIE) {
                        local_anim.set_defaults();
                        local_anim.set_xy(-9.0, 3.0);
                        local_anim.set_angle(45.0, 0.0, 0.0, ANCHOR_CENTER);
                        anim.clear();anim.push_back(local_anim);
                        local_object->children[3]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FLAGHAND, ZOMBIE_INNERARM_HAND_DEPTH, 1, local_object->children[3]);

                        local_anim.set_defaults();
                        local_anim.set_xy(0.0, 0.0);
                        anim.clear();anim.push_back(local_anim);
                        local_object->children[3]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FLAGPOLE, ZOMBIE_INNERARM_HAND_DEPTH, 1, local_object->children[3]->children[0]);

                        local_anim.set_defaults();
                        local_anim.set_xy(21.0, 27.0);
                        anim.clear();anim.push_back(local_anim);
                        local_object->children[3]->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FLAG_1, ZOMBIE_INNERARM_HAND_DEPTH, 0, local_object->children[3]->children[0]->children[0]);
                    }
                    else {
                        local_anim.set_defaults();
                        local_anim.set_xy(-9.0, 3.0);
                        //local_anim.set_angle(45.0, 0.0, 0.0, ANCHOR_CENTER);
                        anim.clear();anim.push_back(local_anim);
                        local_object->children[3]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_SCREENDOOR_HAND, ZOMBIE_INNERARM_HAND_DEPTH, 0, local_object->children[3]);
                    }
                }
                else {
                    // children[0][0][3] is the inner arm. It connects to the rest of the arm.
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    local_anim.set_angle(-24.0, 0.225, -8.0, ANCHOR_NE);
                    local_anim.set_xy(3, 23.5);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-8.0, -1.6, -32.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_anim.set_angle(-17.0, -2.0, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_UPPER,ZOMBIE_INNERARM_UPPER_DEPTH,1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_angle(5.0, 0.0, 0.0, ANCHOR_NE);
                    local_anim.set_xy(-2.5, -19.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-8.0, -3.2, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_anim.set_angle(-60.0, 0.0, -60.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_object->children[3]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_LOWER,ZOMBIE_INNERARM_LOWER_DEPTH,1, local_object->children[3]);

                    local_anim.set_defaults();
                    local_anim.set_xy(2.0,-14);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_HAND,ZOMBIE_INNERARM_HAND_DEPTH,0, local_object->children[3]->children[0]);
                }

                // children[0][0][4] is the outer arm. SCREEN_ZOMBIES have a different outer arm
                if (type == SCREEN_ZOMBIE) {
                    local_anim.set_defaults();
                    local_anim.set_angle(8.0, 0.05, 13.0, ANCHOR_NE);
                    local_anim.set_xy(5.5, 5.5);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[4] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_SCREENDOOR, ZOMBIE_OUTERARM_UPPER_DEPTH, 1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_angle(-8.0, -0.05, -13.0, ANCHOR_NE);
                    local_anim.set_xy(-40.5, -55.5);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[4]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_SCREENDOOR_1, ZOMBIE_ACCESSORY_DEPTH, 0, local_object);
                }
                else {
                    // children[0][0][4] is the outer arm. It connects to the rest of the arm.
                    local_anim.set_defaults();
                    local_anim.set_angle(8.0, 0.05, 13.0, ANCHOR_NE);
                    local_anim.set_xy(23.5, 23.5);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(13.0, -1.85, -25.0, ANCHOR_N);anim.push_back(local_anim);
                    local_anim.set_angle(13.0, -2.0, -47.0, ANCHOR_N);anim.push_back(local_anim);
                    local_object->children[4] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_UPPER,ZOMBIE_OUTERARM_UPPER_DEPTH,1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_xy(-12.0, -25.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-17.0, -1.15, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_anim.set_angle(-80.0, 0.0, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_object->children[4]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, local_object->children[4]);

                    local_anim.set_defaults();
                    local_anim.set_xy(-4.0, -22.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[4]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, local_object->children[4]->children[0]);

                }

                // children[2] is the shadow
                local_anim.set_defaults();
                local_anim.set_xy(0.0, -50.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_SHADOW, ZOMBIE_SHADOW_DEPTH, 0, myObject);

                break;

        }

    }

    /*****************************************************************************
    * Function: Zombie::place
    * Description: Places the zombie (both for demo mode and regular game mode)
    * based on the location and delay values
    *****************************************************************************/
    void Zombie::place(int16_t location, int16_t delay, uint8_t team) {

        static uint16_t place_count = 0;
        row = location;
        col = NUM_COLS-1;

        /* Teams 0 and 1 are on the left, 2 and 3 are on the right.
         * Teams 0 and 2 are on the top, 1 and 3 on the bottom.
         * Placement may need to be sprite-specific. */
        switch (team) {
            case 0:
            default:
                game_x = left_gridWidths[NUM_COLS-1];
                if (type == POLE_ZOMBIE)
                    game_x -= 35.0;
                if (type == FOOTBALL_ZOMBIE)
                    game_x -= 25.0;
                game_y = top_gridHeights[location];
                demo_x = 1.5*(rand()%200)-1375.0;
                demo_y = 1.5*(rand()%300);
                dir = -1.0;
                break;
            case 1:
                game_x = left_gridWidths[NUM_COLS-1];
                if (type == POLE_ZOMBIE)
                    game_x -= 35.0;
                if (type == FOOTBALL_ZOMBIE)
                    game_x -= 25.0;
                game_y = bottom_gridHeights[location];
                demo_x = 1.5*(rand()%200)-1375.0;
                demo_y = -1.5*(rand()%300);
                dir = -1.0;
                break;
            case 2:
                game_x = right_gridWidths[NUM_COLS-1];
                if (type == POLE_ZOMBIE)
                    game_x += 35.0;
                if (type == FOOTBALL_ZOMBIE)
                    game_x += 25.0;
                game_y = top_gridHeights[location];
                demo_x = 1.5*(rand()%200)+1100.0;
                demo_y = 1.5*(rand()%300);
                dir = 1.0;
                break;
            case 3:
                game_x = right_gridWidths[NUM_COLS-1];
                if (type == POLE_ZOMBIE)
                    game_x += 35.0;
                if (type == FOOTBALL_ZOMBIE)
                    game_x += 25.0;
                game_y = bottom_gridHeights[location];
                demo_x = 1.5*(rand()%200)+1100.0;
                demo_y = -1.5*(rand()%300);
                dir = 1.0;
                break;
        }
        this->team = team;
        this->delay = delay;
        status = ZOMBIE_STATUS_PLACED;
        if (++place_count > DEMO_ZOMBIE_COUNT) {
            status = ZOMBIE_STATUS_SKIP;
        }

    }


    /*****************************************************************************
    * Function: Zombie::endDemo
    * Description: Places the zombie in game mode
    *****************************************************************************/
    void Zombie::endDemo() {

        /* Reset all animation counts and angles */
        myObject->setMode(OBJECT_STATUS_GAME);

        /* Point x,y to game_x and game_y */
        status = ZOMBIE_STATUS_GAME;
    }




    /*****************************************************************************
    * Function: Zombie::updateDemo
    * Description: Updates each zombie in demo mode
    *****************************************************************************/
    void Zombie::updateDemo() {

        if (status == ZOMBIE_STATUS_PLACED) {
            status = ZOMBIE_STATUS_DEMO;
        }

        myObject->update();
    }



    /*****************************************************************************
    * Function: Zombie::shoot
    * Description: Updates the health (and potentially other aspects) when a
    * zombie gets hit by a particle
    *****************************************************************************/
    void Zombie::shoot(fvu::Particle *myParticle) {

        // A chomp is an invisible particle specifically to destroy a zombie
        if (myParticle->getType() == CHOMP_PROJECTILE) {
            health = 0;
            status = ZOMBIE_STATUS_INACTIVE;
            return;
        }


        health--;
        switch (type) {
            case REGULAR_ZOMBIE:
            case FLAG_ZOMBIE:
            default:
                myGame->playSound(SFX_SPLAT, 25);
                break;
            case BUCKET_ZOMBIE:
                if (has_item) {
                    myGame->playSound(SFX_SHIELDHIT, 25);
                }
                else {
                    myGame->playSound(SFX_SPLAT, 25);
                }
                break;
            case CONE_ZOMBIE:
                if (has_item) {
                    myGame->playSound(SFX_SPLAT2, 25);
                }
                else {
                    myGame->playSound(SFX_SPLAT, 25);
                }
                break;
            case SCREEN_ZOMBIE:
                if (has_item) {
                    myGame->playSound(SFX_SHIELDHIT2, 25);
                }
                else {
                    myGame->playSound(SFX_SPLAT, 25);
                }
                break;
        }

        // Were we hit by a SNOW_PROJECTILE? If we still have our screen door it doesn't matter.
        if ((myParticle->getType() == SNOW_PROJECTILE) && ((type != SCREEN_ZOMBIE) || (has_item == false))) {
            if (frozen == false) {
                frozen = true;
                speed /= 2.0;
                myGame->playSound(SFX_FROZEN, 25);
            }
            frozen_count = FREEZE_LENGTH;
        }

    }



    /*****************************************************************************
    * Function: Zombie::special
    * Description: Performs a zombie-specific action command
    *****************************************************************************/
    void Zombie::special() {

        switch (type) {
            // POLE_ZOMBIE jumps over a plant for its special move. Straight forward x/y update, followed by some manual sprite removal.
            case POLE_ZOMBIE:
                if (special_count == 0) {
                    myObject->setMode(OBJECT_STATUS_SPECIAL);
                    myGame->playSound(SFX_POLEVAULT, 25);
                    special_count++;
                }
                else {
                    // For now, we skip the conventional update in special mode. I'm not sure now if special() even needs an additional function.
                    myObject->update();
                    special_count++;
                    if ((team == 0) || (team == 1)) {
                        game_x += (left_gridWidths[col-1]-left_gridWidths[col])/60.0;
                    }
                    else {
                        game_x += (right_gridWidths[col-1]-right_gridWidths[col])/60.0;
                    }
                    if (special_count > 30) {
                        game_y -= 3.0;
                    }
                    else {
                        game_y += 3.0;
                    }
                    if (special_count == 60) {
                        special_count = 0;
                        special_done = true;
                        speed = zombieSpeeds[0];
                        myObject->children[0]->children[0]->children[0]->children[1]->children[0]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                        myObject->children[0]->children[0]->children[0]->children[1]->children[0]->children[0]->children[1]->updateSprite(BLANK_SPRITE);
                        myObject->children[0]->children[0]->children[0]->children[1]->anim[OBJECT_STATUS_GAME].set_angle(130.0, -0.5, 115.0, ANCHOR_N);
                        myObject->children[0]->children[0]->children[0]->children[1]->anim[OBJECT_STATUS_GAME].set_x(-12.0);
                        myObject->children[0]->children[0]->children[0]->children[1]->anim[OBJECT_STATUS_ACTION].set_angle(130.0, -2.0, 100.0, ANCHOR_N);
                        myObject->children[0]->children[0]->children[0]->children[1]->anim[OBJECT_STATUS_ACTION].set_x(-12.0);
                        myObject->setMode(OBJECT_STATUS_GAME);
                        //col--;
                    }
                }
                break;
            default:
                break;
        }
    }


    /*****************************************************************************
    * Function: Zombie::updateTransition
    * Description: When a transition event occurs, perform some zombie-specific
    * update.
    *****************************************************************************/
    void Zombie::updateTransition(uint16_t val) {


        fvu::Particle *local_particle;


        /* Transitions are zombie-specific, and usually will involve specific sprite swaps */
        switch (type) {
            case REGULAR_ZOMBIE:
            case FLAG_ZOMBIE:
                if (val == zombieTransitions[REGULAR_ZOMBIE][0]) {
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myObject->children[0]->children[0]->children[4]->updateSprite(ZOMBIE_OUTERARM_UPPER_2);
                    myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[4]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(REGULAR_ARM_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                }
                break;
            case POLE_ZOMBIE:
                if (val == zombieTransitions[POLE_ZOMBIE][0]) {
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myObject->children[0]->children[0]->children[0]->children[2]->updateSprite(ZOMBIE_POLEVAULTER_OUTERARM_UPPER2);
                    myObject->children[0]->children[0]->children[0]->children[2]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[0]->children[2]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(POLE_ARM_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                }
                break;
            case BUCKET_ZOMBIE:
                if (val == zombieTransitions[BUCKET_ZOMBIE][0]) {
                    myObject->children[0]->children[0]->children[2]->children[2]->updateSprite(ZOMBIE_BUCKET_2);
                }
                if (val == zombieTransitions[BUCKET_ZOMBIE][1]) {
                    myObject->children[0]->children[0]->children[2]->children[2]->updateSprite(ZOMBIE_BUCKET_3);
                }
                if (val == zombieTransitions[BUCKET_ZOMBIE][2]) {
                    myObject->children[0]->children[0]->children[2]->children[2]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(ZOMBIE_BUCKET_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                    has_item = false;
                }
                if (val == zombieTransitions[BUCKET_ZOMBIE][3]) {
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myObject->children[0]->children[0]->children[4]->updateSprite(ZOMBIE_OUTERARM_UPPER_2);
                    myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[4]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(REGULAR_ARM_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                }
                break;
            case FOOTBALL_ZOMBIE:
                if (val == zombieTransitions[FOOTBALL_ZOMBIE][0]) {
                    myObject->children[0]->children[2]->children[1]->children[0]->updateSprite(ZOMBIE_FOOTBALL_HELMET_2);
                }
                if (val == zombieTransitions[FOOTBALL_ZOMBIE][1]) {
                    myObject->children[0]->children[2]->children[1]->children[0]->updateSprite(ZOMBIE_FOOTBALL_HELMET_3);
                }
                if (val == zombieTransitions[FOOTBALL_ZOMBIE][2]) {
                    myObject->children[0]->children[2]->children[1]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(ZOMBIE_FOOTBALL_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);

                }
                if (val == zombieTransitions[FOOTBALL_ZOMBIE][3]) {
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myObject->children[0]->children[2]->children[0]->updateSprite(ZOMBIE_FOOTBALL_LEFTARM_UPPER_2);
                    myObject->children[0]->children[2]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[2]->children[0]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(FOOTBALL_ARM_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                }
                break;
            case CONE_ZOMBIE:
                if (val == zombieTransitions[CONE_ZOMBIE][0]) {
                    myObject->children[0]->children[0]->children[2]->children[2]->updateSprite(ZOMBIE_CONE_2);
                }
                if (val == zombieTransitions[CONE_ZOMBIE][1]) {
                    myObject->children[0]->children[0]->children[2]->children[2]->updateSprite(ZOMBIE_CONE_3);
                }
                if (val == zombieTransitions[CONE_ZOMBIE][2]) {
                    myObject->children[0]->children[0]->children[2]->children[2]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(ZOMBIE_CONE_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                    has_item = false;
                }
                if (val == zombieTransitions[CONE_ZOMBIE][3]) {
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myObject->children[0]->children[0]->children[4]->updateSprite(ZOMBIE_OUTERARM_UPPER_2);
                    myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[4]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(REGULAR_ARM_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                }
                break;
            case SCREEN_ZOMBIE:
                if (val == zombieTransitions[SCREEN_ZOMBIE][0]) {
                    myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(ZOMBIE_SCREENDOOR_2);
                }
                if (val == zombieTransitions[SCREEN_ZOMBIE][1]) {
                    myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(ZOMBIE_SCREENDOOR_3);
                }
                // When we lose the screen, essentially translate the SCREEN_ZOMBIE into a REGULAR_ZOMBIE
                if (val == zombieTransitions[SCREEN_ZOMBIE][2]) {
                    //myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(BLANK_SPRITE);

                    // children[0][0][4] is the outer arm. It connects to the rest of the arm.
                    std::vector<animation_struct> anim;
                    animation_struct local_anim;
                    fvu::Object *local_object = myObject->children[0]->children[0];


                    local_particle = new Particle(ZOMBIE_SCREEN_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);


                    delete local_object->children[4]->children[0];
                    delete local_object->children[4];
                    local_anim.set_defaults();
                    local_anim.set_angle(8.0, 0.05, 13.0, ANCHOR_NE);
                    local_anim.set_xy(23.5, 23.5);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(13.0, -1.85, -25.0, ANCHOR_N);anim.push_back(local_anim);
                    local_anim.set_angle(13.0, -2.0, -47.0, ANCHOR_N);anim.push_back(local_anim);
                    local_object->children[4] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_OUTERARM_UPPER,ZOMBIE_OUTERARM_UPPER_DEPTH,1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_xy(-12.0, -25.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-17.0, -1.15, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_anim.set_angle(-80.0, 0.0, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_object->children[4]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, local_object->children[4]);

                    local_anim.set_defaults();
                    local_anim.set_xy(-4.0, -22.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[4]->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, local_object->children[4]->children[0]);


                    // children[0][0][3] is the inner arm. It connects to the rest of the arm.
                    delete local_object->children[3]->children[0];
                    delete local_object->children[3];
                    local_anim.set_defaults();
                    local_anim.set_angle(-24.0, 0.225, -8.0, ANCHOR_NE);
                    local_anim.set_xy(3, 23.5);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-8.0, -1.6, -32.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_anim.set_angle(-17.0, -2.0, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_INNERARM_UPPER,ZOMBIE_INNERARM_UPPER_DEPTH,1, local_object);

                    local_anim.set_defaults();
                    local_anim.set_angle(5.0, 0.0, 0.0, ANCHOR_NE);
                    local_anim.set_xy(-2.5, -19.0);
                    anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                    local_anim.set_angle(-8.0, -3.2, -80.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_anim.set_angle(-60.0, 0.0, -60.0, ANCHOR_NE);anim.push_back(local_anim);
                    local_object->children[3]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_INNERARM_LOWER,ZOMBIE_INNERARM_LOWER_DEPTH,1, local_object->children[3]);

                    local_anim.set_defaults();
                    local_anim.set_xy(2.0,-14);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3]->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_INNERARM_HAND,ZOMBIE_INNERARM_HAND_DEPTH,0, local_object->children[3]->children[0]);


                    has_item = false;
                }
                if (val == zombieTransitions[SCREEN_ZOMBIE][3]) {
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myObject->children[0]->children[0]->children[4]->updateSprite(ZOMBIE_OUTERARM_UPPER_2);
                    myObject->children[0]->children[0]->children[4]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[4]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    local_particle = new Particle(REGULAR_ARM_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);
                }
                break;
            default:
                break;

        }


    }


    /*****************************************************************************
    * Function: Zombie::die
    * Description: When a transition event occurs, perform some zombie-specific
    * update.
    *****************************************************************************/
    void Zombie::die() {

        fvu::Particle *local_particle;

        /* Deaths are zombie-specific, and usually will involve specific sprite swaps */
        switch (type) {
            case REGULAR_ZOMBIE:
            case FLAG_ZOMBIE:
            case SCREEN_ZOMBIE:
            case BUCKET_ZOMBIE:
            case CONE_ZOMBIE:
                if (death_count == 0) {
                    death_count++;
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myGame->myStatus.scores[team] += KILL_SCORE;
                    myObject->children[0]->children[0]->children[2]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[2]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[2]->children[1]->updateSprite(BLANK_SPRITE);
                    if (myObject->children[0]->children[0]->children[2]->children[1]->num_children == 1)
                        myObject->children[0]->children[0]->children[2]->children[1]->children[0]->updateSprite(BLANK_SPRITE);

                    local_particle = new Particle(REGULAR_HEAD_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);

                }
                else {
                    // For now, we skip the conventional update in death mode
                    myObject->update();
                    death_count++;
                    if (death_count == 31) {
                        status = ZOMBIE_STATUS_INACTIVE;
                        row = 25;
                    }
                }

                break;
            case POLE_ZOMBIE:
                if (death_count == 0) {
                    death_count++;
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myGame->myStatus.scores[team] += KILL_SCORE;
                    myObject->children[0]->children[0]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[0]->children[0]->children[0]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[0]->children[0]->children[0]->children[1]->updateSprite(BLANK_SPRITE);

                    local_particle = new Particle(POLE_HEAD_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);

                }
                else {
                    // For now, we skip the conventional update in death mode
                    myObject->update();
                    death_count++;
                    if (death_count == 31) {
                        status = ZOMBIE_STATUS_INACTIVE;
                        row = 25;
                    }
                }

                break;
            case FOOTBALL_ZOMBIE:
                if (death_count == 0) {
                    death_count++;
                    myGame->playSound(SFX_LIMBS_POP, 25);
                    myGame->myStatus.scores[team] += KILL_SCORE;
                    myObject->children[0]->children[2]->children[1]->updateSprite(BLANK_SPRITE);
                    myObject->children[0]->children[2]->children[1]->children[1]->updateSprite(BLANK_SPRITE);

                    local_particle = new Particle(FOOTBALL_HEAD_PARTICLE, this);
                    myGame->myParticles[team].push_back(*local_particle);

                }
                else {
                    // For now, we skip the conventional update in death mode
                    myObject->update();
                    death_count++;
                    if (death_count == 31) {
                        status = ZOMBIE_STATUS_INACTIVE;
                        row = 25;
                    }
                }

                break;
            case NEWS_ZOMBIE:
                break;
            case DANCING_ZOMBIE:
                break;
            case YETI_ZOMBIE:
                break;

        }

    }

    /*****************************************************************************
    * Function: Zombie::update
    * Description: Updates each zombie
    *****************************************************************************/
    void Zombie::update() {

        /* Check all the active zombies. Move them forward (based on speed),
         * and determine if they're within eating range or if they've made it past
         * all the plant grid locations */
        if (status == ZOMBIE_STATUS_ACTIVE) {

            // If we're in the middle of a special operation, continue it
            if (special_count != 0) {
                special();
                return;
            }

            // If we're in the middle of dying, continue
            if (death_count != 0) {
                die();
                return;
            }

            // Are we still frozen? If so decrease the count appropriately
            if (frozen_count != 0) {
                frozen_count--;
                if (frozen_count == 0) {
                    frozen = false;
                    speed *= 2.0;
                }
            }


            /* First, have we been hit recently? If so, check transitions / death conditions */
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
                die();
            }


            switch(team) {
                case 0:
                case 1:
                    // Check if we've moved locations and have won or are now within range of a plant
                    if (col == 0) {
                        status = ZOMBIE_STATUS_WINNING;
                        myObject->setMode(OBJECT_STATUS_WINNING);
                        myGame->myStatus.scores[team] += ZOMBIE_SCORE;
                        final_x = -173.0 + rand()%292;
                        final_y = -333.0 + rand()%620;
                    }

                    else if (game_x >= (left_gridWidths[col-1]-60.0)) {
                        col--;
                        // We've entered a grid in which there is a plant
                        if (myGame->plantGrid[team][row][col] == true) {
                            bool do_eat = true;
                            if ((type == POLE_ZOMBIE) && (special_done == false)) {
                                for (uint16_t i = 0; i < myGame->myPlants[team].size(); i++) {
                                    if ((myGame->myPlants[team][i].getRow() == row) && (myGame->myPlants[team][i].getCol() == col)) {
                                        if ((myGame->myPlants[team][i].getType() != TALLNUT_PLANT) && (myGame->myPlants[team][i].getType() != PORTAL_PLANT)){
                                            do_eat = false;
                                        }
                                        break;
                                    }
                                }
                            }

                            if (do_eat == true) {
                                status = ZOMBIE_STATUS_EATING;
                                myObject->setMode(OBJECT_STATUS_ACTION);
                            }
                            else {
                                special();
                            }
                        }
                    }

                    // Keep walking for this iteration
                    game_x += speed;
                    break;
                case 2:
                case 3:
                default:
                    // Check if we've moved locations and have won or are now within range of a plant
                    if (col == 0) {
                        status = ZOMBIE_STATUS_WINNING;
                        myObject->setMode(OBJECT_STATUS_WINNING);
                        myGame->myStatus.scores[team] += ZOMBIE_SCORE;
                        final_x = -173.0 + rand()%292;
                        final_y = -333.0 + rand()%620;
                    }
                    else if (game_x <= (right_gridWidths[col-1]+60.0)) {
                        col--;
                        // We've entered a grid in which there is a plant
                        if (myGame->plantGrid[team][row][col] == true) {
                            bool do_eat = true;
                            if ((type == POLE_ZOMBIE) && (special_done == false)) {
                                for (uint16_t i = 0; i < myGame->myPlants[team].size(); i++) {
                                    if ((myGame->myPlants[team][i].getRow() == row) && (myGame->myPlants[team][i].getCol() == col)) {
                                        if (myGame->myPlants[team][i].getType() != TALLNUT_PLANT) {
                                            do_eat = false;
                                        }
                                        break;
                                    }
                                }
                            }

                            if (do_eat == true) {
                                status = ZOMBIE_STATUS_EATING;
                                myObject->setMode(OBJECT_STATUS_ACTION);
                            }
                            else {
                                special();
                            }
                        }
                    }

                    // Keep walking for this iteration
                    game_x -= speed;
                    break;
            }
        }
        else if (status == ZOMBIE_STATUS_EATING) {

            // If we're in the middle of dying, continue
            if (death_count != 0) {
                die();
                return;
            }


            /* First, have we been hit recently? If so, check transitions / death conditions */
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
                die();
            }


            // Is the plant gone/moved?
            if (myGame->plantGrid[team][row][col] == false) {
                status = ZOMBIE_STATUS_ACTIVE;
                myObject->setMode(OBJECT_STATUS_GAME);
            }

            // Otherwise find the appropriate plant (every BITE_FRAMES) and take a byte
            static uint16_t chomp_counter = 0;
            action_count++;
            if (action_count > BITE_FRAMES) {
                for (uint16_t i = 0; i < myGame->myPlants[team].size(); i++) {
                    if ((myGame->myPlants[team][i].getRow() == row) && (myGame->myPlants[team][i].getCol() == col)) {
                                                myGame->myPlants[team][i].bite();
                        myGame->playSound(SFX_CHOMP+chomp_counter, 25);
                        chomp_counter++;
                        chomp_counter %= 3;
                        action_count = 0;
                        break;
                    }
                }
            }

        }
        else if (status == ZOMBIE_STATUS_WINNING) {

            bool gameover = true;
            if (fabs(game_x - final_x) >= speed) {
                if (game_x > final_x) {
                    game_x -= speed;
                }
                else {
                    game_x += speed;
                }
                gameover = false;
            }

            if (fabs(game_y - final_y) >= speed) {
                if (game_y > final_y) {
                    game_y -= speed;
                }
                else {
                    game_y += speed;
                }
                gameover = false;
            }

            if (gameover == true) {
                status = ZOMBIE_STATUS_GAMEOVER;
            }

        }


        myObject->update();
    }



    /*****************************************************************************
    * Function: Zombie::move
    * Description: Moves the zombie around (for testing purposes)
    *****************************************************************************/
    void Zombie::move(float delta_x, float delta_y) {

        game_x += delta_x;
        game_y += delta_y;

    }

    /*****************************************************************************
    * Function: Zombie::operator <
    * Description: Overloads the < operator for comparison / sort purposes
    *****************************************************************************/
    bool Zombie::operator< (const Zombie &rhs) const {

        /* We want pseudo-3D, so lower = closer. Otherwise, closer to the sides
         * should be closer to the screen. */
        if (status == ZOMBIE_STATUS_DEMO) {
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
