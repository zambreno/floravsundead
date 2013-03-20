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
        animation_struct demo_anim, game_anim, zero_anim;

        /* The zero_anim is useful if we don't care about initial positioning of animation */
        zero_anim.start_angle = 0.0;zero_anim.delta_angle = 0.0;zero_anim.end_angle = 0.0;
        zero_anim.start_x     = 0.0;zero_anim.delta_x     = 0.0;zero_anim.end_x     = 0.0;
        zero_anim.start_y     = 0.0;zero_anim.delta_y     = 0.0;zero_anim.end_y     = 0.0;

        switch(type) {
            case REGULAR_ZOMBIE:
            case CONE_ZOMBIE:
            case BUCKET_ZOMBIE:
            case FLAG_ZOMBIE:
            default:

                speed = 1.0;

                /* The object structure starts at the x/y location of the torso, and moves out in all directions */
                myObject = new Object(zero_anim, zero_anim, anim_count, 0, 0, 0, 6, NULL);

                // children[0] is the body
                demo_anim.start_angle = 10.0;demo_anim.delta_angle = -0.2;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 0.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -3.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIEBODY, ZOMBIEBODY_DEPTH, 3, myObject);

                // children[0][1] is the tie
                demo_anim.start_angle = 10.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 5.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 15.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0]->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_TIE, ZOMBIE_ACCESSORY_DEPTH, 0, myObject->children[0]);

                // children[0][2] is the zombie_underbody
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 15.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 15.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[0]->children[2] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_UNDERBODY, ZOMBIE_ACCESSORY_DEPTH, 0, myObject->children[0]);


                // children[0][0] is the head
                // ZOMBIES have a tongue 50% of the time
                uint8_t num_head_children = 2;
                if ((type == CONE_ZOMBIE) || (type == BUCKET_ZOMBIE)) {
                    num_head_children++;
                }
                else {
                    num_head_children += ((rand()%2) == 0);
                }

                local_object = myObject->children[0];
                demo_anim.start_angle = -2.0;demo_anim.delta_angle = 0.25;demo_anim.end_angle = 3.0;
                demo_anim.start_x     = -24.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 51.75;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD,ZOMBIE_HEAD_DEPTH, num_head_children, local_object);

                // children[0][0][0] is the hair
                demo_anim.start_angle = -4.0;demo_anim.delta_angle = 0.1;demo_anim.end_angle = -3.0;
                demo_anim.start_x     = -5.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 23.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_HAIR, ZOMBIE_HEAD_DEPTH, 0, local_object->children[0]);

                // children[0][0][1] is the jaw
                demo_anim.start_angle = 8.0;demo_anim.delta_angle = -0.2;demo_anim.end_angle = -8.0;
                demo_anim.start_x     = 10.2;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -10.5;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                local_object->children[0]->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, ZOMBIE_HEAD_DEPTH,0, local_object->children[0]);

                // children[0][0][2] is the cone/bucket
                if (type == CONE_ZOMBIE) {
                    demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                    demo_anim.start_x     = 2.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                    demo_anim.start_y     = 22.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                    game_anim = demo_anim;
                    local_object->children[0]->children[2] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_CONE_1, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[0]);
                }
                else if (type == BUCKET_ZOMBIE) {
                    demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                    demo_anim.start_x     = -7.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                    demo_anim.start_y     = 1.75;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                    game_anim = demo_anim;
                    local_object->children[0]->children[2] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_BUCKET_1, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[0]);
                }

                // otherwise, children[0][0][2] is the tongue
                else if (num_head_children > 2) {
                    demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                    demo_anim.start_x     = 20.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                    demo_anim.start_y     = -10.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                    game_anim = demo_anim;
                    local_object->children[0]->children[2] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_TONGUE, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, local_object->children[0]);
                }

                // children[1] is the outer leg
                demo_anim.start_angle = 1.0;demo_anim.delta_angle = 0.2;demo_anim.end_angle = 9.0;
                demo_anim.start_x     = 29.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -15.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[1] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_UPPER,ZOMBIE_OUTERLEG_UPPER_DEPTH,1, myObject);

                demo_anim.start_angle = -1.0;demo_anim.delta_angle = -0.2;demo_anim.end_angle = -9.0;
                demo_anim.start_x     = 8.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -19.75;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[1]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_LOWER,ZOMBIE_OUTERLEG_LOWER_DEPTH,1, myObject->children[1]);

                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -14.75;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -12.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[1]->children[0]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERLEG_FOOT,ZOMBIE_OUTERLEG_FOOT_DEPTH,0, myObject->children[1]->children[0]);

                // children[2] is the inner leg
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 19.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -16.5;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[2] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERLEG_UPPER,ZOMBIE_INNERLEG_UPPER_DEPTH,1, myObject);

                demo_anim.start_angle = 2.0;demo_anim.delta_angle = 0.2;demo_anim.end_angle = 11.0;
                demo_anim.start_x     = -14.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -23.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[2]->children[0] = new Object(demo_anim, game_anim, anim_count,  TEX_ZOMBIES, ZOMBIE_INNERLEG_LOWER,ZOMBIE_INNERLEG_LOWER_DEPTH,1, myObject->children[2]);

                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 4.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -3.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[2]->children[0]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERLEG_FOOT,ZOMBIE_INNERLEG_FOOT_DEPTH,0, myObject->children[2]->children[0]);

                // children[3] is the inner arm
                demo_anim.start_angle = -10.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 0.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 23.5;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[3] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_UPPER,ZOMBIE_INNERARM_UPPER_DEPTH,1, myObject);

                demo_anim.start_angle = 5.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -2.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -19.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[3]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_LOWER,ZOMBIE_INNERARM_LOWER_DEPTH,1, myObject->children[3]);

                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 0.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -15.25;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[3]->children[0]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_INNERARM_HAND,ZOMBIE_INNERARM_HAND_DEPTH,0, myObject->children[3]->children[0]);

                // children[4] is the outer arm
                demo_anim.start_angle = 8.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 23.5;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = 23.5;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[4] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_UPPER,ZOMBIE_OUTERARM_UPPER_DEPTH,1, myObject);

                demo_anim.start_angle = -10.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -15.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -15.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[4]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, myObject->children[4]);

                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = -8.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -25.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[4]->children[0]->children[0] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, myObject->children[4]->children[0]);


                // children[5] is the shadow
                demo_anim.start_angle = 0.0;demo_anim.delta_angle = 0.0;demo_anim.end_angle = 0.0;
                demo_anim.start_x     = 0.0;demo_anim.delta_x     = 0.0;demo_anim.end_x     = 0.0;
                demo_anim.start_y     = -50.0;demo_anim.delta_y     = 0.0;demo_anim.end_y     = 0.0;
                game_anim = demo_anim;
                myObject->children[5] = new Object(demo_anim, game_anim, anim_count, TEX_ZOMBIES, ZOMBIE_SHADOW, ZOMBIE_SHADOW_DEPTH, 0, myObject);

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

        /* Teams 0 and 1 are on the left, 2 and 3 are on the right.
         * Teams 0 and 2 are on the top, 1 and 3 on the bottom.
         * Placement may need to be sprite-specific. */
        switch (team) {
            case 0:
            default:
                game_x = -700.0;
                game_y = gridHeights[location]-35.0;
                demo_x = 1.5*(rand()%200)-1375.0;
                demo_y = 1.5*(rand()%300);
                dir = -1.0;
                break;
            case 1:
                game_x = -700.0;
                game_y = gridHeights[NUM_ROWS-1-location]-581.0;
                demo_x = 1.5*(rand()%200)-1375.0;
                demo_y = -1.5*(rand()%300);
                dir = -1.0;
                break;
            case 2:
                game_x = 700.0;
                game_y = gridHeights[location]-35.0;
                demo_x = 1.5*(rand()%200)+1100.0;
                demo_y = 1.5*(rand()%300);
                dir = 1.0;
                break;
            case 3:
                game_x = 700.0;
                game_y = gridHeights[NUM_ROWS-1-location]-581.0;
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
            switch(team) {
                case 0:
                case 1:
                    game_x += speed;
                    break;
                case 2:
                case 3:
                default:
                    game_x -= speed;
                    break;
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
