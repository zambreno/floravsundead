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
    Object::Object(float x, float y, float angle, ZOMBIE_SPRITE_ENUM sprite, DEPTH_ENUM depth, uint8_t num_children, Object *parent) {
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
    void Zombie::draw() {

        //glTranslatef(game_x, game_y, 0);
        glTranslatef(game_x, game_y, 0.0);
        myObject->draw();
    }


    /*****************************************************************************
    * Function: Zombie::Zombie
    * Description: Class constructor. Uses an enum type to set zombie-specific
    * parameters
    *****************************************************************************/
    Zombie::Zombie(ZOMBIE_TYPE mytype) {


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

                // children[0] is the inner leg
                myObject->children[0] = new Object(29.5,-16.5,0.0,ZOMBIE_OUTERLEG_UPPER,ZOMBIE_OUTERLEG_UPPER_DEPTH,1, myObject);
                myObject->children[0]->children[0] = new Object(10.5,-20.0,0.0,ZOMBIE_OUTERLEG_LOWER,ZOMBIE_OUTERLEG_LOWER_DEPTH,1, myObject->children[0]);
                myObject->children[0]->children[0]->children[0] = new Object(-15.0,-20.0,0.0,ZOMBIE_OUTERLEG_FOOT,ZOMBIE_OUTERLEG_FOOT_DEPTH,0, myObject->children[0]->children[0]);

                // children[1] is the outer leg
                myObject->children[1] = new Object(40.0,-17.0,0.0,ZOMBIE_INNERLEG_UPPER,ZOMBIE_INNERLEG_UPPER_DEPTH,1, myObject);
                myObject->children[1]->children[0] = new Object(-14.0,-26.0,0.0,ZOMBIE_INNERLEG_LOWER,ZOMBIE_INNERLEG_LOWER_DEPTH,1, myObject->children[1]);
                myObject->children[1]->children[0]->children[0] = new Object(3.0,-7.0,0.0,ZOMBIE_INNERLEG_FOOT,ZOMBIE_INNERLEG_FOOT_DEPTH,0, myObject->children[1]->children[0]);

                // children[2] is the inner arm
                myObject->children[2] = new Object(0.0,-15.5,0.0,ZOMBIE_INNERARM_UPPER,ZOMBIE_INNERARM_UPPER_DEPTH,1, myObject);
                myObject->children[2]->children[0] = new Object(-3.0,-20.0,0.0,ZOMBIE_INNERARM_LOWER,ZOMBIE_INNERARM_LOWER_DEPTH,1, myObject->children[2]);
                myObject->children[2]->children[0]->children[0] = new Object(0.0,-15.0,0.0,ZOMBIE_INNERARM_HAND,ZOMBIE_INNERARM_HAND_DEPTH,0, myObject->children[2]->children[0]);

                // children[3] is the outer arm
                myObject->children[3] = new Object(50.5,10.5,0.0,ZOMBIE_OUTERARM_UPPER,ZOMBIE_OUTERARM_UPPER_DEPTH,1, myObject);
                myObject->children[3]->children[0] = new Object(-10.0,-20.0,0.0,ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH,1, myObject->children[3]);
                myObject->children[3]->children[0]->children[0] = new Object(-4.0,-25.0,0.0,ZOMBIE_OUTERARM_HAND,ZOMBIE_OUTERARM_HAND_DEPTH,0, myObject->children[3]->children[0]);

                // children[4] is the head
                myObject->children[4] = new Object(10.5,35.5,0.0,ZOMBIE_HEAD_GROSSOUT,ZOMBIE_HEAD_DEPTH,0, myObject);

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

        // TODO: this should be team-specific
        game_x = 1.0*location;
        game_y = -1.0;
        team = team;

    }

    /*****************************************************************************
    * Function: Zombie::move
    * Description: Moves the zombie around (for testing purposes)
    *****************************************************************************/
    void Zombie::move(float delta_x, float delta_y) {

        game_x += delta_x;
        game_y += delta_y;

    }





} // namespace fvu
