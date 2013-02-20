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
    * Function: Game::drawWorld
    * Description: Draws the various static background objects.
    *****************************************************************************/
    void Game::drawWorld() {

        glClear(GL_DEPTH_BUFFER_BIT);

        /* Move everything by the pan amount */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(myStatus.pan, 0.0, 0.0);//+0.375, 0.375, 0.0);


        /* Draw the court texture */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_COURT].texHandle);
        glBegin(GL_QUADS);
            glTexCoord2d(1.0, 0.0);
            glVertex3f(210.0, 395.0, COURT_DEPTH);
            glTexCoord2d(0.0, 0.0);
            glVertex3f(-210.0, 395.0, COURT_DEPTH);
            glTexCoord2d(0.0, 1.0);
            glVertex3f(-210.0, -395.0, COURT_DEPTH);
            glTexCoord2d(1.0, 1.0);
            glVertex3f(210.0, -395.0, COURT_DEPTH);
        glEnd();

        /* Draw the background textures */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_BACKGROUND].texHandle);
        glBegin(GL_QUADS);
            glTexCoord2d(1.0, 0.0);
            glVertex3f(1385.0, 581.0, BACKGROUND_DEPTH);
            glTexCoord2d(0.0, 0.0);
            glVertex3f(0.0, 581.0, BACKGROUND_DEPTH);
            glTexCoord2d(0.0, 1.0);
            glVertex3f(0.0, -581.0, BACKGROUND_DEPTH);
            glTexCoord2d(1.0, 1.0);
            glVertex3f(1385.0, -581.0, BACKGROUND_DEPTH);

            glTexCoord2d(1.0, 0.0);
            glVertex3f(-1385.0, 581.0, BACKGROUND_DEPTH);
            glTexCoord2d(0.0, 0.0);
            glVertex3f(0.0, 581.0, BACKGROUND_DEPTH);
            glTexCoord2d(0.0, 1.0);
            glVertex3f(0.0, -581.0, BACKGROUND_DEPTH);
            glTexCoord2d(1.0, 1.0);
            glVertex3f(-1385.0, -581.0, BACKGROUND_DEPTH);
        glEnd();



    }

    /*****************************************************************************
    * Function: Game::drawScoreboard
    * Description: Draws the various dynamic in-game objects.
    *****************************************************************************/
    void Game::drawScoreboard() {

        /* Draw the bottom scoreboard */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_SCOREBOARD_BOTTOM].texHandle);
        glBegin(GL_QUADS);
            glTexCoord2d(1.0, 0.0);
            glVertex3f(249.5, -395.0, SCOREBOARD_DEPTH);
            glTexCoord2d(0.0, 0.0);
            glVertex3f(-249.5, -395.0, SCOREBOARD_DEPTH);
            glTexCoord2d(0.0, 1.0);
            glVertex3f(-249.5, -578.0, SCOREBOARD_DEPTH);
            glTexCoord2d(1.0, 1.0);
            glVertex3f(249.5, -578.0, SCOREBOARD_DEPTH);
        glEnd();

        /* Draw the top scoreboard */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_SCOREBOARD_TOP].texHandle);
        glBegin(GL_QUADS);
            glTexCoord2d(1.0, 0.0);
            glVertex3f(249.5, 578.0, SCOREBOARD_DEPTH);
            glTexCoord2d(0.0, 0.0);
            glVertex3f(-249.5, 578.0, SCOREBOARD_DEPTH);
            glTexCoord2d(0.0, 1.0);
            glVertex3f(-249.5, 395.0, SCOREBOARD_DEPTH);
            glTexCoord2d(1.0, 1.0);
            glVertex3f(249.5, 395.0, SCOREBOARD_DEPTH);
        glEnd();


    }


    /*****************************************************************************
    * Function: Game::drawMap
    * Description: Draws the various dynamic in-game objects.
    *****************************************************************************/
    void Game::drawMap() {
    }

    /*****************************************************************************
    * Function: Game::processEvents
    * Description: Processes keyboard and other SFML events.
    *****************************************************************************/
    void Game::processEvents() {

        sf::Event event;
        int16_t i = 0;
        while (myWindow.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    myWindow.close();
                }
                else {
                    mySound.setBuffer(mySoundBuffers[i]);
                    mySound.play();
                    i++;
                    i %= NUM_SFX;
                }

            }
        }

    }


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
        GLuint myTextureHandles[NUM_TEXTURES];
        glGenTextures(NUM_TEXTURES, myTextureHandles);
        if (myConfig.debug_level > 3)
            printf("Loading texture data...");


        for (int16_t i = 0; i < NUM_TEXTURES; i++) {
            if (!pixels.loadFromFile(texFiles[i]))
                raise_error(ERR_NOFILE1, texFiles[i].c_str());
            myTextures[i].scale = 1.0;
            myTextures[i].width = pixels.getSize().x;
            myTextures[i].height = pixels.getSize().y;
            myTextures[i].texHandle = myTextureHandles[i];

            glBindTexture(GL_TEXTURE_2D, myTextures[i].texHandle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextures[i].width,
                    myTextures[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getPixelsPtr());
        }

        /* Bind the individual spriteMaps */
        myTextures[GREEN_FONT].spriteMap = green_font_spriteMap;
        printf("Test of spriteMap - [%u, %u, %u, %u]\n", myTextures[GREEN_FONT].spriteMap[0][0],
               myTextures[GREEN_FONT].spriteMap[0][1],myTextures[GREEN_FONT].spriteMap[0][2],myTextures[GREEN_FONT].spriteMap[0][3]);


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

        myWindow.setVerticalSyncEnabled(true);
        //myWindow.setFramerateLimit(FRAME_RATE);


        /* Configure OpenGL default state */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(BACK_DEPTH);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1032.5, 1032.5, -581.0, 581.0, FRONT_DEPTH, -BACK_DEPTH);
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_TEXTURE_2D);

        //glAlphaFunc(GL_GREATER, 0);
        //glEnable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    }


} // namespace fvu
