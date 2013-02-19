/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/
/*****************************************************************************
 * sfml_utils.cpp - graphics and audio functionality using the SFML-2.0
 * library with OpenGL.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/

#include "floravsundead.hpp"


namespace fvu {


    /*****************************************************************************
    * Function: Game::loadResources
    * Description: Loads the various texture maps and audio files used in the
    * application.
    *****************************************************************************/
    void Game::loadResources() {


        /* Load the sound buffers. These can safely fit in memory */
        if (myConfig.debug_level > 3)
            printf("Loading sound buffers...");

        for (int16_t i = 0; i < NUM_SFX; i++) {
            if (!mySoundBuffers[i].loadFromFile(sfxFiles[i]))
                raise_error(ERR_AUDIO, sfxFiles[i].c_str());
        }

        /* Load the background audio. These should stream from the files */
        for (int16_t i = 0; i < NUM_MUSIC; i++) {
            if (!myMusic[i].openFromFile(musicFiles[i]))
                raise_error(ERR_AUDIO, musicFiles[i].c_str());
            myMusic[i].setLoop(true);
            myMusic[i].setVolume(100);
        }
        if (myConfig.debug_level > 3)
            printf("done.\n");


        /* Load the various textures from the appropriate files */
        sf::Image pixels;
        glGenTextures(NUM_TEXTURES, myTextureHandles);
        if (myConfig.debug_level > 3)
            printf("Loading texture data...");


        for (int16_t i = 0; i < NUM_TEXTURES; i++) {
            if (!pixels.loadFromFile(texFiles[i]))
                raise_error(ERR_NOFILE1, texFiles[i].c_str());
            myTextures[i].scale = 1.0;
            myTextures[i].width = pixels.getSize().x;
            myTextures[i].height = pixels.getSize().y;

            glBindTexture(GL_TEXTURE_2D, myTextureHandles[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextures[i].width,
                    myTextures[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getPixelsPtr());
        }


        if (myConfig.debug_level > 3)
            printf("done.\n");

    }


    /*****************************************************************************
    * Function: Game::initSFML()
    * Description: Initializes the SFML environment for handling graphics and
    * audio.
    *****************************************************************************/
    void Game::initSFML() {

        // Attempt to create the user-specified window
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        bool foundMode = false;

        if (myConfig.debug_level > 10) {
            printf("\nAvailable video modes:\n");
        }
        for (std::size_t i = 0; i < modes.size(); i++) {
            sf::VideoMode mode = modes[i];
            if (myConfig.debug_level > 10) {
                printf("\tMode #%u: [%u,%u] - %ubpp\n", i, mode.width, mode.height, mode.bitsPerPixel);
            }
            if ((mode.width == myConfig.screen_width) && (mode.height == myConfig.screen_height)
                && (mode.bitsPerPixel == myConfig.screen_depth))
                foundMode = true;
        }

        if (foundMode == false) {
            if (myConfig.debug_level > 0) {
                printf("Warning: specified [%u, %u] resolution not available\n",
                       myConfig.screen_width, myConfig.screen_height);
                printf("Substituting with [%u, %u]\n", modes[0].width, modes[0].height);
                myConfig.screen_width = modes[0].width;
                myConfig.screen_height = modes[0].height;
                myConfig.screen_depth = modes[0].bitsPerPixel;
            }
        }

        myWindow.create(sf::VideoMode(myConfig.screen_width, myConfig.screen_height,
                        myConfig.screen_depth), WINDOW_TITLE, sf::Style::Fullscreen);

        mySettings = myWindow.getSettings();
        if (myConfig.debug_level > 3) {
            printf("\n%s Window configuration: \n", WINDOW_TITLE);
            printf("\tDepth bits: %u\n", mySettings.depthBits);
            printf("\tStencil bits: %u\n", mySettings.stencilBits);
            printf("\tAnti-Aliasing level: %u\n", mySettings.antialiasingLevel);
            printf("\tOpenGL v%u.%u\n\n", mySettings.majorVersion, mySettings.minorVersion);
        }

        //myWindow.setVerticalSyncEnabled(true);
        myWindow.setFramerateLimit(FRAME_RATE);


        /* Configure OpenGL default state */
        glClearColor(10.0/255, 19.0/255, 57.0/255, 1.0);
        glClearDepth(BACK_DEPTH);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-2.0, 2.0, -1.5, 1.5, FRONT_DEPTH, -BACK_DEPTH);
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_TEXTURE_2D);

        //glAlphaFunc(GL_GREATER, 0);
        //glEnable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    }


} // namespace fvu
