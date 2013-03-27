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
float zombieSpeeds[NUM_ZOMBIE_TYPE] = {1.0, 1.5, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0};
/* Plant transitions */
uint16_t zombieTransitions[NUM_ZOMBIE_TYPE][NUM_ZOMBIE_TRANSITIONS] = {
    {5, 0, 0, 0},
    {5, 0, 0, 0},
    {21, 15, 10, 0},
    {8, 0, 0, 0},
    {46, 28, 10, 5},
    {13, 10, 8, 4},
    {46, 28, 10, 5},
    {56, 33, 5, 0},
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

        float x = demo_x;
        float y = demo_y;
        float z;

        // Put objects on a different depth level if they're on the bottom
        if (team % 2 == 1) {
            z = index*ZOMBIE_DEPTH_RANGE+OBJECT_BOTTOM_DEPTH;
        }
        else {
            z = index*ZOMBIE_DEPTH_RANGE+OBJECT_TOP_DEPTH;
        }

        if (status > ZOMBIE_STATUS_DEMO) {
            x = game_x;
            y = game_y;
        }
        else if (status == ZOMBIE_STATUS_SKIP) {
            return;
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
            case REGULAR_ZOMBIE:
            case CONE_ZOMBIE:
            case BUCKET_ZOMBIE:
            case FLAG_ZOMBIE:
            default:

                /* The object structure starts at the x/y location of the outer leg, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();
                anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 3, NULL);


                // children[0] is the outer leg, starting at the upper part. It connects to the rest of the leg and the body
                local_anim.set_defaults();
                //local_anim.set_angle(1.0, 0.2, 9.0, ANCHOR_CENTER);
                local_anim.set_xy(27.0, -15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_UPPER,ZOMBIE_OUTERLEG_UPPER_DEPTH, 2, myObject);

                local_anim.set_defaults();
                //local_anim.set_angle(-1.0, -0.2, -9.0, ANCHOR_CENTER);
                local_anim.set_xy(10, -20);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_LOWER,ZOMBIE_OUTERLEG_LOWER_DEPTH,1, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-18, -10);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_FOOT,ZOMBIE_OUTERLEG_FOOT_DEPTH,0, myObject->children[0]->children[0]);


                // children[1] is the inner leg, starting at the foot. It connects to the rest of the leg
                local_anim.set_defaults();
                local_anim.set_xy(7.0, -34.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERLEG_FOOT,ZOMBIE_INNERLEG_FOOT_DEPTH, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(2, 3);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[0] = new Object(anim, anim_count,  TEX_ZOMBIES, ZOMBIE_INNERLEG_LOWER,ZOMBIE_INNERLEG_LOWER_DEPTH,1, myObject->children[1]);

                local_anim.set_defaults();
                local_anim.set_xy(16, 27);
                //local_anim.set_angle(2.0, 0.2, 11.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERLEG_UPPER,ZOMBIE_INNERLEG_UPPER_DEPTH, 0, myObject->children[1]->children[0]);


                // children[0][0] is the body. It connects to the tie, the underbody, the arms, and the head
                local_anim.set_defaults();
                //local_anim.set_angle(10.0, -0.4, -5.0, ANCHOR_SE);
                local_anim.set_xy(-25, 20);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIEBODY, ZOMBIEBODY_DEPTH, 5, myObject->children[0]);

                // children[0][0][0] is the tie
                local_anim.set_defaults();
                //local_anim.set_angle(10.0, -0.4, -10.0, ANCHOR_N);
                local_anim.set_xy(0.0, 15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_TIE, ZOMBIE_ACCESSORY_DEPTH, 0, myObject->children[0]->children[0]);

                // children[0][0][1] is the zombie_underbody
                local_anim.set_defaults();
                local_anim.set_xy(15.0, 15.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, BLANK_SPRITE, ZOMBIEBODY_DEPTH, 0, myObject->children[0]->children[0]);


                // children[0][0][2] is the head
                // ZOMBIES have a tongue 50% of the time
                uint8_t num_head_children = 2;
                if ((type == CONE_ZOMBIE) || (type == BUCKET_ZOMBIE)) {
                    num_head_children++;
                }
                else {
                    num_head_children += ((rand()%2) == 0);
                }

                local_object = myObject->children[0]->children[0];
                local_anim.set_defaults();
                //local_anim.set_angle(-2.0, 0.25, 3.0, ANCHOR_CENTER);
                local_anim.set_xy(-24.5, 51.75);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD,ZOMBIE_HEAD_DEPTH, num_head_children, local_object);

                // children[0][0][2][0] is the hair
                local_anim.set_defaults();
                //local_anim.set_angle(-2.0, 0.25, 3.0, ANCHOR_CENTER);
                local_anim.set_xy(-5, 20.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HAIR, ZOMBIE_HEAD_DEPTH, 0, local_object->children[2]);

                // children[0][0][2][1] is the jaw
                local_anim.set_defaults();
                //local_anim.set_angle(4.0, -0.2, -8.0, ANCHOR_NE);
                local_anim.set_xy(10.2, -9.5);
                anim.clear();anim.push_back(local_anim);
                local_object->children[2]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, ZOMBIE_HEAD_DEPTH,0, local_object->children[2]);

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

                // otherwise, children[0][0][2][2] is the tongue
                else if (num_head_children > 2) {
                    local_anim.set_defaults();
                    local_anim.set_xy(20.0, -10.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[2]->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_TONGUE, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[2]);
                }

                // The FLAG_ZOMBIE has a slightly different inner arm
                if (type == FLAG_ZOMBIE) {
                    // children[0][0][3] is the inner arm. It connects to the rest of the arm.
                    local_object = myObject->children[0]->children[0];

                    local_anim.set_defaults();
                    local_anim.set_xy(-32.0, 23.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_SCREENDOOR,ZOMBIE_INNERARM_LOWER_DEPTH, 1, local_object);

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
                    // children[0][0][3] is the inner arm. It connects to the rest of the arm.
                    local_object = myObject->children[0]->children[0];
                    local_anim.set_defaults();
                    //local_anim.set_angle(-10.0, -0.1, -8.0, ANCHOR_NE);
                    local_anim.set_xy(3, 23.5);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_UPPER,ZOMBIE_INNERARM_UPPER_DEPTH,1, local_object);

                    local_anim.set_defaults();
                    //local_anim.set_angle(5.0, 0.0, 0.0, ANCHOR_NE);
                    local_anim.set_xy(-2.5, -19.0);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_LOWER,ZOMBIE_INNERARM_LOWER_DEPTH,1, local_object->children[3]);

                    local_anim.set_defaults();
                    local_anim.set_xy(2.0,-14);
                    anim.clear();anim.push_back(local_anim);
                    local_object->children[3]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_HAND,ZOMBIE_INNERARM_HAND_DEPTH,0, local_object->children[3]->children[0]);
                }

                // children[0][0][4] is the outer arm. It connects to the rest of the arm.
                local_anim.set_defaults();
                //local_anim.set_angle(8.0, 1.0, 13.0, ANCHOR_NW);
                local_anim.set_xy(23.5, 23.5);
                anim.clear();anim.push_back(local_anim);
                local_object->children[4] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_UPPER,ZOMBIE_OUTERARM_UPPER_DEPTH,1, local_object);

                local_anim.set_defaults();
                //local_anim.set_angle(-10.0, 0.0, 0.0, ANCHOR_CENTER);
                local_anim.set_xy(-12, -17.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[4]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, local_object->children[4]);

                local_anim.set_defaults();
                local_anim.set_xy(-4.0, -22.0);
                anim.clear();anim.push_back(local_anim);
                local_object->children[4]->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, local_object->children[4]->children[0]);


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
                game_y = top_gridHeights[location];
                demo_x = 1.5*(rand()%200)-1375.0;
                demo_y = 1.5*(rand()%300);
                dir = -1.0;
                break;
            case 1:
                game_x = left_gridWidths[NUM_COLS-1];
                game_y = bottom_gridHeights[location];
                demo_x = 1.5*(rand()%200)-1375.0;
                demo_y = -1.5*(rand()%300);
                dir = -1.0;
                break;
            case 2:
                game_x = right_gridWidths[NUM_COLS-1];
                game_y = top_gridHeights[location];
                demo_x = 1.5*(rand()%200)+1100.0;
                demo_y = 1.5*(rand()%300);
                dir = 1.0;
                break;
            case 3:
                game_x = right_gridWidths[NUM_COLS-1];
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
        myObject->endDemo();

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
    * Function: Zombie::update
    * Description: Updates each zombie
    *****************************************************************************/
    void Zombie::update() {

        /* Check all the active zombies. Move them forward (based on speed),
         * and determine if they're within eating range or if they've made it past
         * all the plant grid locations */
        if (status == ZOMBIE_STATUS_ACTIVE) {


            /* First, have we been hit recently? If so, check transitions / death conditions */
            if (health <= transitions[0]) {

                // This part is a little tricky. How do we know what transition to do in fact here?
                transitions.erase(transitions.begin());

                if (health <= 0) {
                    status = ZOMBIE_STATUS_INACTIVE;
                    return;
                }
            }

            switch(team) {
                case 0:
                case 1:
                    // Check if we've moved locations and are now within range of a plant
                    if (game_x >= (left_gridWidths[col-1]-60.0)) {
                        if (col == 1) {
                            status = ZOMBIE_STATUS_WINNING;
                            myGame->myStatus.scores[team] += ZOMBIE_SCORE;
                        }
                        else {
                            col--;
                            // We've entered a grid in which there is a plant
                            if (myGame->plantGrid[team][row][col] == true) {
                                status = ZOMBIE_STATUS_EATING;
                            }
                        }
                    }

                    // Keep walking for this iteration
                    game_x += speed;
                    break;
                case 2:
                case 3:
                default:
                    // Check if we've moved locations and are now within range of a plant
                    if (game_x <= (right_gridWidths[col-1]+60.0)) {
                        if (col == 1) {
                            status = ZOMBIE_STATUS_WINNING;
                            myGame->myStatus.scores[team] += ZOMBIE_SCORE;
                        }
                        else {
                            col--;
                            // We've entered a grid in which there is a plant
                            if (myGame->plantGrid[team][row][col] == true) {
                                status = ZOMBIE_STATUS_EATING;
                            }
                        }
                    }

                    // Keep walking for this iteration
                    game_x -= speed;
                    break;
            }
        }
        else if (status == ZOMBIE_STATUS_EATING) {

            // Is the plant gone/moved?
            if (myGame->plantGrid[team][row][col] == false) {
                status = ZOMBIE_STATUS_ACTIVE;
            }
            // Otherwise find the appropriate plant and take a byte
            for (uint16_t i = 0; i < myGame->myPlants[team].size(); i++) {
                if ((myGame->myPlants[team][i].getRow() == row) && (myGame->myPlants[team][i].getCol() == col)) {
                    myGame->myPlants[team][i].bite();
                    break;
                }
            }

        }
        else if (status == ZOMBIE_STATUS_WINNING) {

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
