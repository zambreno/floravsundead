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


#pragma once

#include "floravsundead.hpp"
#include "object.hpp"

/* Main enum for the plant types. */
typedef enum {PEA_PROJECTILE=0, ICE_PROJECTILE, PEA_PARTICLE, ICE_PARTICLE, WALLNUT_PARTICLE, NUM_PARTICLE_TYPE} PARTICLE_TYPE;
extern float particleSpeeds[NUM_PARTICLE_TYPE];

/* Particle status enum */
typedef enum {PARTICLE_STATUS_INACTIVE = 0, PLANT_STATUS_ACTIVE} PARTICLE_STATUS_ENUM;

namespace fvu {

    /* Main particle class */
    class Particle {
        public:
            Particle(uint8_t type);
            void update();
            void draw(uint16_t index);
            uint8_t getStatus() {return status;}
            uint8_t getType() {return type;}
            uint16_t getRow() {return row;}
            uint16_t getCol() {return col;}
            bool operator< (const Particle &rhs) const;

        private:
            uint8_t type;
            uint8_t status;
            uint16_t row, col;
            float speed;
            float game_x, game_y;
            uint8_t team;
            Object *myObject;
    };

} // namespace fvu

