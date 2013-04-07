/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * particles.hpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the pellets and particles class.
 *
 *
 * NOTES:
 * 3/28/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"


/* Particle speeds.*/
float particleSpeeds[NUM_PARTICLE_TYPE] = {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0};

namespace fvu {

    /*****************************************************************************
    * Function: Particle::draw
    * Description: Particle drawing function. Sets the initial modeling position
    * and draws each Object recursively.
    *****************************************************************************/
    void Particle::draw(uint16_t index) {

        float x, y, z;


        // Frozen particles have their color modulated appropriately
        if (frozen == true) {
            glColor3ub(1, 145, 182);
        }
        else {
            glColor3ub(255, 255, 255);
        }


            x = game_x;
            y = game_y;
            z = index*5;

            // Particles should be in front of plants and zombies
            // Put objects on a different depth level if they're on the bottom
            if (team % 2 == 1) {
                switch (row) {
                    case 0:
                    default:
                        z += PARTICLE_BOTTOMROW_1_DEPTH;
                        break;
                    case 1:
                        z += PARTICLE_BOTTOMROW_2_DEPTH;
                        break;
                    case 2:
                        z += PARTICLE_BOTTOMROW_3_DEPTH;
                        break;
                    case 3:
                        z += PARTICLE_BOTTOMROW_4_DEPTH;
                         break;
                    case 4:
                        z += PARTICLE_BOTTOMROW_5_DEPTH;
                        break;
                }
            }
            else {
                switch (row) {
                    case 0:
                    default:
                        z += PARTICLE_TOPROW_1_DEPTH;
                        break;
                    case 1:
                        z += PARTICLE_TOPROW_2_DEPTH;
                        break;
                    case 2:
                        z += PARTICLE_TOPROW_3_DEPTH;
                        break;
                    case 3:
                        z += PARTICLE_TOPROW_4_DEPTH;
                         break;
                    case 4:
                        z += PARTICLE_TOPROW_5_DEPTH;
                        break;
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
    Particle::Particle(uint8_t mytype, fvu::Plant *myPlant) {

        uint32_t anim_count = 0;
        uint8_t particle_rand;
        float local_x, local_y;

        status = PARTICLE_STATUS_ACTIVE;
        type = mytype;
        this->myPlant = myPlant;
        row = myPlant->getRow();col = myPlant->getCol();
        team = myPlant->getTeam();
        offscreen = false;
        plant_particle = true;
        frozen = false;
        live_count = 0;

        // game_x and game_y are a function of the parent plant (tweaked as needed)
        dir = myPlant->getDir();
        game_x = myPlant->getGameX()+dir*25;
        game_y = myPlant->getGameY();

        /* Initialize particle information here so we can leave the rest of this function as object assembly */
        speed = particleSpeeds[type];

        std::vector<animation_struct> anim;
        animation_struct local_anim;

        switch(type) {
            case PEA_PROJECTILE:
            case SNOW_PROJECTILE:

                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the main projectile
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                if (type == PEA_PROJECTILE) {
                    myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PROJECTILEPEA, 0, 0, myObject);
                }
                else if (type == SNOW_PROJECTILE) {
                    myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PROJECTILE_SNOW_PEA, 0, 0, myObject);
                }

                break;

            case SHOVEL_PARTICLE:

                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the main shovel
                game_y -= 25.0;
                local_anim.set_defaults();
                local_anim.set_xscale(-0.65);
                local_anim.set_yscale(0.65);
                local_anim.set_angle(-40.0, 4.0, 40.0, ANCHOR_NW);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, SHOVEL_HI_RES, 0, 0, myObject);

                break;


            case PLANTING_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 0, NULL);
                break;

            case WALLNUT_BIG_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 0, NULL);
                break;

            case WALLNUT_LITTLE_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0-7] are the eating particles.
                // It originally is located at children[0] which is the body of the wallnut/tallnut
                particle_rand = rand() % 15;
                local_x = myPlant->getObject()->children[0]->get_abs_x()+50.0;
                local_y = myPlant->getObject()->children[0]->get_abs_y()+70.0;
                for (uint8_t i = 0; i < 1; i++) {
                    local_anim.set_defaults();
                    local_anim.set_x(local_x+(1.0*particle_rand-7.5));
                    local_anim.set_y(local_y+(1.0*particle_rand-7.5));
                    anim.clear();anim.push_back(local_anim);
                    myObject->children[i] = new Object(anim, anim_count, TEX_PLANTS, WALLNUT_PARTICLES_SMALL_1+particle_rand%5, 0, 0, myObject);
                    particle_rand+=5;
                }

                break;


            default:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 0, NULL);
                break;

        }

    }


   /*****************************************************************************
    * Function: Particle::Particle
    * Description: Class constructor. Uses an enum type to set particle-specific
    * parameters
    *****************************************************************************/
    Particle::Particle(uint8_t mytype, fvu::Zombie *myZombie) {

        uint32_t anim_count = 0;
        float local_x, local_y;

        status = PARTICLE_STATUS_ACTIVE;
        type = mytype;
        this->myZombie = myZombie;
        frozen = myZombie->getFrozen();
        row = myZombie->getRow();col = myZombie->getCol();
        team = myZombie->getTeam();
        plant_particle = false;
        offscreen = false;
        live_count = 0;

        // game_x and game_y are a function of the parent plant (tweaked as needed)
        dir = myZombie->getDir();
        game_x = myZombie->getGameX();
        game_y = myZombie->getGameY();

        /* Initialize particle information here so we can leave the rest of this function as object assembly */
        speed = particleSpeeds[type];

        std::vector<animation_struct> anim;
        animation_struct local_anim;

        static uint8_t peasplat_offset = 0;
        static uint8_t snowsplat_offset = 0;

        switch(type) {



            case PEA_PARTICLE:
                plant_particle = true;
                game_x += 10.0;
                game_y += 30.0;
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 6, NULL);

                // children[0] is the pea splat
                local_anim.set_defaults();
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PEA_SPLATS_1+peasplat_offset, 0, 0, myObject);

                // children[1] is the first pea particle
                local_anim.set_defaults();
                local_anim.set_xy(25.0, 25.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, PEA_PARTICLES_1, 0, 0, myObject);

                // children[2] is the second pea particle
                local_anim.set_defaults();
                local_anim.set_xy(0.0, 25.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[2] = new Object(anim, anim_count, TEX_PLANTS, PEA_PARTICLES_2, 0, 0, myObject);

                // children[3] is the third pea particle
                local_anim.set_defaults();
                local_anim.set_xy(-10.0, 25.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[3] = new Object(anim, anim_count, TEX_PLANTS, PEA_PARTICLES_3, 0, 0, myObject);

                // children[4] is the fourth pea particle
                local_anim.set_defaults();
                local_anim.set_xy(25.0, -10.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[4] = new Object(anim, anim_count, TEX_PLANTS, PEA_PARTICLES_2, 0, 0, myObject);


                // children[5] is the fifth pea particle
                local_anim.set_defaults();
                local_anim.set_xy(-10.0, -10.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[5] = new Object(anim, anim_count, TEX_PLANTS, PEA_PARTICLES_1, 0, 0, myObject);

                peasplat_offset++;peasplat_offset %= 4;

                break;


            case SNOW_PARTICLE:
                plant_particle = true;
                game_x += 10.0;
                game_y += 30.0;
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 6, NULL);

                // children[0] is the pea splat
                local_anim.set_defaults();
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_SPLATS_1+snowsplat_offset, 0, 0, myObject);

                // children[1] is the first pea particle
                local_anim.set_defaults();
                local_anim.set_xy(25.0, 25.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[1] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_PARTICLES_1, 0, 0, myObject);

                // children[2] is the second pea particle
                local_anim.set_defaults();
                local_anim.set_xy(0.0, 25.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[2] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_PARTICLES_2, 0, 0, myObject);

                // children[3] is the third pea particle
                local_anim.set_defaults();
                local_anim.set_xy(-10.0, 25.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[3] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_PARTICLES_3, 0, 0, myObject);

                // children[4] is the fourth pea particle
                local_anim.set_defaults();
                local_anim.set_xy(25.0, -10.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[4] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_PARTICLES_2, 0, 0, myObject);


                // children[5] is the fifth pea particle
                local_anim.set_defaults();
                local_anim.set_xy(-10.0, -10.0);
                local_anim.set_xscale(1.0, 0.025, 1.25);
                local_anim.set_yscale(1.0, 0.025, 1.25);
                anim.clear();anim.push_back(local_anim);
                myObject->children[5] = new Object(anim, anim_count, TEX_PLANTS, SNOWPEA_PARTICLES_1, 0, 0, myObject);

                snowsplat_offset++;snowsplat_offset %= 4;

                break;


            case REGULAR_ARM_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the zombie arm
                // It originally is located at children[0][0][4][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[4]->children[0]->get_abs_x();
                local_y = myZombie->getObject()->children[0]->children[0]->children[4]->children[0]->get_abs_y();
                local_anim.set_x(local_x);
                local_anim.set_y(local_y, -2.0, local_y-40.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_LOWER, 0, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(-4.0, -22.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND, 1, 0, myObject->children[0]);

                break;

            case POLE_ARM_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the pole arm
                // It originally is located at children[0][0][0][2][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[0]->children[2]->children[0]->get_abs_x();
                local_y = myZombie->getObject()->children[0]->children[0]->children[0]->children[2]->children[0]->get_abs_y();
                local_anim.set_x(local_x);
                local_anim.set_y(local_y, -1.8, local_y-36.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_OUTERARM_LOWER, 0, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(-4.0, -22.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND, 1, 0, myObject->children[0]);

                break;


            case NEWS_ARM_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the news arm
                // It originally is located at children[0][1][1][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[1]->children[1]->children[0]->get_abs_x();
                local_y = myZombie->getObject()->children[0]->children[1]->children[1]->children[0]->get_abs_y();
                local_anim.set_x(local_x);
                local_anim.set_y(local_y, -1.8, local_y-36.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_PAPER_LEFTARM_LOWER, 0, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(-70.0, -18.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_PAPER_HANDS3, 0, 0, myObject->children[0]);

                break;


            case FOOTBALL_ARM_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the football arm
                // It originally is located at children[0][2][0][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[2]->children[0]->children[0]->get_abs_x()-20.0;
                local_y = myZombie->getObject()->children[0]->children[2]->children[0]->children[0]->get_abs_y();
                local_anim.set_x(local_x);
                local_anim.set_y(local_y, -2.0, local_y-40.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTARM_LOWER, 0, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(-29, -1);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_FOOTBALL_LEFTARM_HAND, 1, 0, myObject->children[0]);

                break;



            case ZOMBIE_CONE_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the damaged zombie cone
                // It originally is located at children[0][0][2][2]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[2]->children[2]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[0]->children[2]->children[2]->get_abs_y();
                local_anim.set_x(local_x, 2.7, local_x+81.0);
                local_anim.set_y(local_y, -2.7, local_y-81.0);
                local_anim.set_angle(0.0, -3.0, -90.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_CONE_3, 0, 0, myObject);
                break;


            case ZOMBIE_SCREEN_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the damaged zombie screen
                // It originally is located at children[0][0][4][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[4]->children[0]->get_abs_x();
                local_y = myZombie->getObject()->children[0]->children[0]->children[4]->children[0]->get_abs_y();
                local_anim.set_x(local_x);
                local_anim.set_y(local_y);
                local_anim.set_angle(0.0, 3.0, 90.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_SCREENDOOR_3, 0, 0, myObject);
                break;

            case ZOMBIE_NEWS_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the damaged zombie newspaper
                // It originally is located at children[0][1][0][0][0][0][1]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[1]->children[0]->children[0]->children[0]->children[1]->get_abs_x();
                local_y = myZombie->getObject()->children[0]->children[1]->children[0]->children[0]->children[0]->children[1]->get_abs_y();
                local_anim.set_x(local_x);
                local_anim.set_y(local_y);
                local_anim.set_angle(0.0, -3.0, -90.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_PAPER_PAPER3, 0, 0, myObject);

                break;

            case ZOMBIE_QUESTION_PARTICLE:
                frozen = false;

                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the question mark
                // It originally is located above the head at children[0][1][2]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[1]->children[2]->get_abs_x() + 50.0;
                local_y = myZombie->getObject()->children[0]->children[1]->children[2]->get_abs_y() + 50.0;
                local_anim.set_x(local_x);
                local_anim.set_y(local_y);
                if (myZombie->getDir() < 0.0) {
                    local_anim.set_xscale(-1.0);
                }
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_QUESTIONMARK, 0, 0, myObject);
                break;



            case ZOMBIE_BUCKET_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the damaged zombie bucket
                // It originally is located at children[0][0][2][2]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[2]->children[2]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[0]->children[2]->children[2]->get_abs_y();
                local_anim.set_x(local_x, 2.7, local_x+81.0);
                local_anim.set_y(local_y, -2.7, local_y-81.0);
                local_anim.set_angle(0.0, -3.0, -90.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_BUCKET_3, 0, 0, myObject);
                break;


            case ZOMBIE_FOOTBALL_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the damaged zombie football helmet
                // It originally is located at children[0][2][1][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[2]->children[1]->children[0]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[2]->children[1]->children[0]->get_abs_y();
                local_anim.set_x(local_x, 2.7, local_x+81.0);
                local_anim.set_y(local_y, -2.7, local_y-81.0);
                local_anim.set_angle(0.0, -3.0, -90.0, ANCHOR_S);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_HELMET_3, 0, 0, myObject);
                break;





            case POLE_HEAD_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the pole zombie head
                // It originally is located at [0][0][0][0]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[0]->children[0]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[0]->children[0]->children[0]->get_abs_y();
                local_anim.set_x(local_x, 2.0, local_x+60.0);
                local_anim.set_y(local_y, -4.0, local_y-120.0);
                local_anim.set_angle(0.0, -6.0, -180.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_x(local_x+60.0, 2.0, local_x+80.0);
                local_anim.set_y(local_y-120.0);
                local_anim.set_angle(-180.0, -6.0, -240.0, ANCHOR_CENTER);
                anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD, 0, 2, myObject);


                local_anim.set_defaults();
                local_anim.set_angle(-2.0, 0.0, 0.0, ANCHOR_CENTER);
                local_anim.set_x(-7.0);
                local_anim.set_y(14.0, -0.05, 12.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_POLEVAULTER_HAIR, 0, 0, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(10.2, -9.5);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(4.0, -1.0, -12.0, ANCHOR_NE);anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, 0, 0, myObject->children[0]);

                break;



            case FOOTBALL_HEAD_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the pole zombie head
                // It originally is located at children[0][2][1]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[2]->children[1]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[2]->children[1]->get_abs_y();
                local_anim.set_x(local_x, 2.0, local_x+60.0);
                local_anim.set_y(local_y, -4.0, local_y-120.0);
                local_anim.set_angle(0.0, -6.0, -180.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_x(local_x+60.0, 2.0, local_x+80.0);
                local_anim.set_y(local_y-120.0);
                local_anim.set_angle(-180.0, -6.0, -240.0, ANCHOR_CENTER);
                anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_FOOTBALL_HEAD, 0, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(7.2, -7.5);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, 0, 0, myObject->children[0]);

                break;




            case NEWS_HEAD_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the news zombie head
                // It originally is located at children[0][1][2]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[1]->children[2]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[1]->children[2]->get_abs_y();
                local_anim.set_x(local_x, 2.0, local_x+60.0);
                local_anim.set_y(local_y, -4.0, local_y-120.0);
                local_anim.set_angle(0.0, -6.0, -180.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_x(local_x+60.0, 2.0, local_x+80.0);
                local_anim.set_y(local_y-120.0);
                local_anim.set_angle(-180.0, -6.0, -240.0, ANCHOR_CENTER);
                anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_PAPER_MADHEAD, 0, 3, myObject);


                local_anim.set_defaults();
                local_anim.set_xy(4.0, 30.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_PAPER_HAIRPIECE, 0, 0, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(10.0, -9.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, 0, 0, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_xy(-1.0, 5.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[2] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_PAPER_GLASSES, 0, 0, myObject->children[0]);

                break;

            case REGULAR_HEAD_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the regular zombie head
                // It originally is located at children[0][0][2]
                local_anim.set_defaults();
                local_x = myZombie->getObject()->children[0]->children[0]->children[2]->get_abs_x()+20.0;
                local_y = myZombie->getObject()->children[0]->children[0]->children[2]->get_abs_y();
                local_anim.set_x(local_x, 2.0, local_x+60.0);
                local_anim.set_y(local_y, -4.0, local_y-120.0);
                local_anim.set_angle(0.0, -6.0, -180.0, ANCHOR_CENTER);
                anim.clear();anim.push_back(local_anim);
                local_anim.set_x(local_x+60.0, 2.0, local_x+80.0);
                local_anim.set_y(local_y-120.0);
                local_anim.set_angle(-180.0, -6.0, -240.0, ANCHOR_CENTER);
                anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD, 0, 2, myObject);


                local_anim.set_defaults();
                local_anim.set_angle(-2.0, 0.25, 3.0, ANCHOR_CENTER);
                local_anim.set_xy(-5, 20.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HAIR, 0, 0, myObject->children[0]);

                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(10.2, -9.5);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, 0, 0, myObject->children[0]);

                break;

            default:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 0, NULL);
                break;


        }

    }


    /*****************************************************************************
    * Function: Particle::update
    * Description: Updates each particle
    *****************************************************************************/
    void Particle::update() {


        fvu::Particle *local_particle;

        myObject->update();

        // Calculate any zombie impacts here
        switch(type) {
            case PEA_PROJECTILE:
            case SNOW_PROJECTILE:
                game_x += speed*dir;

                if ((game_x > 1050.0) || (game_x < -1050.0)) {
                    offscreen = true;
                    myPlant->has_hit = false;
                    myPlant->has_killed = false;
                }

                // We iterate through the loop forwards to get the closest zombie that is in hit range
                for (uint16_t j = 0; j < myGame->myZombies[team].size(); j++) {
                    uint8_t status = myGame->myZombies[team][j].getStatus();
                    if (((status == ZOMBIE_STATUS_ACTIVE) || (status == ZOMBIE_STATUS_EATING)) &&
                        (myGame->myZombies[team][j].getRow() == row) &&
                        (fabs(myGame->myZombies[team][j].getGameX() - game_x) < 15.0)) {
                        myGame->myZombies[team][j].shoot(this);
                        myPlant->has_hit = true;
                        if (type == PEA_PROJECTILE) {
                            local_particle = new Particle(PEA_PARTICLE, &myGame->myZombies[team][j]);
                        }
                        else {
                            local_particle = new Particle(SNOW_PARTICLE, &myGame->myZombies[team][j]);
                        }
                        myGame->myParticles[team].push_back(*local_particle);
                        delete local_particle;
                        if (myGame->myZombies[team][j].getHealth() == 0) {
                            myPlant->has_killed = true;
                        }
                        else {
                            myPlant->has_killed = false;
                        }
                        offscreen = true;
                        break;
                    }
                }
                break;
            case SHOVEL_PARTICLE:
                live_count++;
                if (live_count == 30)
                    offscreen = true;
                break;

            case WALLNUT_BIG_PARTICLE:
            case WALLNUT_LITTLE_PARTICLE:
            case PLANTING_PARTICLE:
            case PEA_PARTICLE:
            case SNOW_PARTICLE:
                live_count++;
                if (live_count == 10)
                    offscreen = true;
                break;


            case REGULAR_ARM_PARTICLE:
            case POLE_ARM_PARTICLE:
            case FOOTBALL_ARM_PARTICLE:
            case NEWS_ARM_PARTICLE:
                live_count++;
                if (live_count == 20)
                    offscreen = true;

                break;
            case ZOMBIE_CONE_PARTICLE:
            case ZOMBIE_BUCKET_PARTICLE:
            case ZOMBIE_FOOTBALL_PARTICLE:
            case ZOMBIE_SCREEN_PARTICLE:
            case ZOMBIE_NEWS_PARTICLE:
            case ZOMBIE_QUESTION_PARTICLE:
                live_count++;
                if (live_count == 30) {
                    offscreen = true;

                    if (myZombie->getType() == NEWS_ZOMBIE) {
                        myZombie->setSpeed(zombieSpeeds[NEWS_ZOMBIE]*3.0);
                    }

                }
                break;
            case REGULAR_HEAD_PARTICLE:
            case POLE_HEAD_PARTICLE:
            case FOOTBALL_HEAD_PARTICLE:
            case NEWS_HEAD_PARTICLE:
                live_count++;
                if (live_count == 30)
                    myObject->setMode(OBJECT_STATUS_GAME);
                if (live_count == 40)
                    offscreen = true;
                break;
            default:
                offscreen = true;
                break;
        }


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
