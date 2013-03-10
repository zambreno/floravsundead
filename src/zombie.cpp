/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * floravsundead.cpp - FloraVsUndead application. Takes as input move files
 * and an optional zombie file and runs a Plants vs Zombies-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"


/* Texture files */
std::string zombieNames[NUM_ZOMBIE_TYPE][NUM_ZOMBIE_SPELLINGS] = {
    {"zombie", "regular"},
    {"conehead", "cone"},
    {"flag", "fast"},
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
    * Function: Object::Object
    * Description: Object class constructor. Creates a set of linked lists
    * between parent and child.
    *****************************************************************************/
    Object::Object(float x, float y, float angle, ZOMBIE_SPRITE_ENUM sprite, uint32_t depth, uint8_t num_children, Object *parent) {
        this->x = x;
        this->y = y;
        this->angle = angle;
        this->sprite = sprite;
        this->depth = depth;
        this->num_children = num_children;
        this->parent = parent;

        children = NULL;
        if (num_children > 0) {
            children = new Object *[num_children];
        }
    }



    /*****************************************************************************
    * Function: Object::draw
    * Description: Object drawing function. Recursively draws each object.
    *****************************************************************************/
    void Object::draw() {

        float texCoords[6];

        /* Draw the current object */
        myGame->getTexCoords(TEX_ZOMBIES, sprite, texCoords);
        glTranslatef(x, y, 0);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(0.0, 0.0, depth);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(texCoords[4], 0.0, depth);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(texCoords[4], texCoords[5], depth);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(0.0, texCoords[5], depth);
        glEnd();

        /* Draw the children */
        for (uint8_t i = 0; i < num_children; i++) {
            glPushMatrix();
            children[i]->draw();
            glPopMatrix();
        }
    }



    /*****************************************************************************
    * Function: Zombie::draw
    * Description: Zombie drawing function. Sets the initial modeling position
    * and draws each Object recursively.
    *****************************************************************************/
    void Zombie::draw(uint16_t index) {

        float x = demo_x;
        float y = demo_y;
        float z = index*ZOMBIE_DEPTH_RANGE+OBJECT_DEPTH;
        glPushMatrix();
        if (status > ZOMBIE_STATUS_DEMO) {
            x = game_x;
            y = game_y;
        }
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
    Zombie::Zombie(ZOMBIE_TYPE mytype) {

        status = ZOMBIE_STATUS_DEFAULT;

        switch(mytype) {
            case REGULAR_ZOMBIE:
            default:
                float local_transitions[] = {5.0, 0.0};
                type = mytype;
                health = 10;
                speed = 0.25;
                transitions.insert(transitions.begin(), local_transitions, local_transitions+2);

                /* The object structure starts at the torso, and moves out in all directions */
                myObject = new Object(0.0,0.0,0.0,ZOMBIEBODY,ZOMBIEBODY_DEPTH,5, NULL);

                // children[0] is the outer leg
                myObject->children[0] = new Object(29.5,-16.5,0.0,ZOMBIE_OUTERLEG_UPPER,ZOMBIE_OUTERLEG_UPPER_DEPTH,1, myObject);
                myObject->children[0]->children[0] = new Object(10.5,-19.75,0.0,ZOMBIE_OUTERLEG_LOWER,ZOMBIE_OUTERLEG_LOWER_DEPTH,1, myObject->children[0]);
                myObject->children[0]->children[0]->children[0] = new Object(-14.75,-12.0,0.0,ZOMBIE_OUTERLEG_FOOT,ZOMBIE_OUTERLEG_FOOT_DEPTH,0, myObject->children[0]->children[0]);

                // children[1] is the inner leg
                myObject->children[1] = new Object(19.5,-16.5,0.0,ZOMBIE_INNERLEG_UPPER,ZOMBIE_INNERLEG_UPPER_DEPTH,1, myObject);
                myObject->children[1]->children[0] = new Object(-14.0,-26.0,0.0,ZOMBIE_INNERLEG_LOWER,ZOMBIE_INNERLEG_LOWER_DEPTH,1, myObject->children[1]);
                myObject->children[1]->children[0]->children[0] = new Object(3.0,-7.0,0.0,ZOMBIE_INNERLEG_FOOT,ZOMBIE_INNERLEG_FOOT_DEPTH,0, myObject->children[1]->children[0]);

                // children[2] is the inner arm
                myObject->children[2] = new Object(0.0,15.5,0.0,ZOMBIE_INNERARM_UPPER,ZOMBIE_INNERARM_UPPER_DEPTH,1, myObject);
                myObject->children[2]->children[0] = new Object(-3.0,-20.0,0.0,ZOMBIE_INNERARM_LOWER,ZOMBIE_INNERARM_LOWER_DEPTH,1, myObject->children[2]);
                myObject->children[2]->children[0]->children[0] = new Object(0.0,-15.0,0.0,ZOMBIE_INNERARM_HAND,ZOMBIE_INNERARM_HAND_DEPTH,0, myObject->children[2]->children[0]);

                // children[3] is the outer arm
                myObject->children[3] = new Object(29.5,10.5,0.0,ZOMBIE_OUTERARM_UPPER,ZOMBIE_OUTERARM_UPPER_DEPTH,1, myObject);
                myObject->children[3]->children[0] = new Object(-10.0,-20.0,0.0,ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, myObject->children[3]);
                myObject->children[3]->children[0]->children[0] = new Object(-4.0,-25.0,0.0,ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, myObject->children[3]->children[0]);

                // children[4] is the head
                myObject->children[4] = new Object(-10.5,35.5,0.0,ZOMBIE_HEAD_GROSSOUT,ZOMBIE_HEAD_DEPTH,0, myObject);

                // children[5] is the body accessories


                break;
        }
    }

    /*****************************************************************************
    * Function: Zombie::place
    * Description: Places the zombie (both for demo mode and regular game mode)
    * based on the location and delay values
    *****************************************************************************/
    void Zombie::place(int16_t location, int16_t delay, uint8_t team) {

        /* Teams 0 and 1 are on the left, 2 and 3 are on the right.
         * Teams 0 and 2 are on the top, 1 and 3 on the bottom.
         * Placement may need to be sprite-specific. */
        switch (team) {
            case 0:
            default:
                game_x = -700.0;
                game_y = gridHeights[location]-50.0;
                demo_x = (rand()%200)-1200.0;
                demo_y = 1.0*(rand()%450);
                dir = -1.0;
                break;
            case 1:
                game_x = -700.0;
                game_y = gridHeights[location]-581.0;
                demo_x = (rand()%200)-1200.0;
                demo_y = -1.0*(rand()%450);
                dir = -1.0;
                break;
            case 2:
                game_x = 700.0;
                game_y = gridHeights[location]-50.0;
                demo_x = (rand()%200)+1000.0;
                demo_y = 1.0*(rand()%450);
                printf("[%f, %f], [%f, %f]\n", game_x, game_y, demo_x, demo_y);
                dir = 1.0;
                break;
            case 3:
                game_x = 700.0;
                game_y = gridHeights[location]-581.0;
                demo_x = (rand()%200)+1000.0;
                demo_y = -1.0*(rand()%450);
                printf("[%f, %f], [%f, %f]\n", game_x, game_y, demo_x, demo_y);
                dir = 1.0;
                break;
        }
        this->team = team;
        this->delay = delay;
        status = ZOMBIE_STATUS_PLACED;

    }


    /*****************************************************************************
    * Function: Zombie::endDemo
    * Description: Places the zombie in game mode
    *****************************************************************************/
    void Zombie::endDemo() {

        /* Set all the angles to 0.0 */

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

    }


    /*****************************************************************************
    * Function: Zombie::update
    * Description: Updates each zombie
    *****************************************************************************/
    void Zombie::update() {

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
                return (fabsf(demo_x) >= fabsf(rhs.demo_x));
            }
            else return (demo_y < rhs.demo_y);
        }
        else {
            if (game_y == rhs.game_y) {
                return (fabsf(game_x) >= fabsf(rhs.game_x));
            }
            else return (game_y < rhs.game_y);
        }
    }




} // namespace fvu
