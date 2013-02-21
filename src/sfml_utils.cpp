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

        float texCoords[4];
        glClear(GL_DEPTH_BUFFER_BIT);

        /* Move everything by the pan amount */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(myStatus.pan, 0.0, 0.0);


        /* Draw the court texture */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_COURT].texHandle);
        getTexCoords(TEX_COURT, COURT_ALL, texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-210.0, -395.0, COURT_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(210.0, -395.0, COURT_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(210.0, 395.0, COURT_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-210.0, 395.0, COURT_DEPTH);
        glEnd();

        /* Draw the background textures */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_BACKGROUND].texHandle);
        getTexCoords(TEX_BACKGROUND, BACKGROUND_ALL, texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(0.0, -581.0, BACKGROUND_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(1385.0, -581.0, BACKGROUND_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(1385.0, 581.0, BACKGROUND_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(0.0, 581.0, BACKGROUND_DEPTH);

            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(0.0, -581.0, BACKGROUND_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(-1385.0, -581.0, BACKGROUND_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(-1385.0, 581.0, BACKGROUND_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(0.0, 581.0, BACKGROUND_DEPTH);
        glEnd();



    }

    /*****************************************************************************
    * Function: Game::drawScoreboard
    * Description: Draws the various dynamic in-game objects.
    *****************************************************************************/
    void Game::drawScoreboard() {

        float texCoords[4];

        /* Draw the bottom scoreboard */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_SCOREBOARD_BOTTOM].texHandle);
        getTexCoords(TEX_SCOREBOARD_BOTTOM, SCOREBOARD_BOTTOM_ALL, texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-249.5, -578.0, SCOREBOARD_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(249.5, -578.0, SCOREBOARD_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(249.5, -395.0, SCOREBOARD_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-249.5, -395.0, SCOREBOARD_DEPTH);
        glEnd();

        /* Draw the top scoreboard */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_SCOREBOARD_TOP].texHandle);
        getTexCoords(TEX_SCOREBOARD_TOP, SCOREBOARD_TOP_ALL, texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-249.5, 395.0, SCOREBOARD_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(249.5, 395.0, SCOREBOARD_DEPTH);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(249.5, 578.0, SCOREBOARD_DEPTH);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-249.5, 578.0, SCOREBOARD_DEPTH);
        glEnd();


        /* Draw the scores */
        glBindTexture(GL_TEXTURE_2D, myTextures[GREEN_FONT].texHandle);
        float baseX = -152.0, baseY = 435.0;
        int16_t score;
        char digits[4];
        glBegin(GL_QUADS);
            for (uint8_t i = 0; i < 4; i++) {
                // Limit the score to be displayed
                score = myStatus.scores[i];
                if (score > 999)
                    score = 999;
                if (score < -99)
                    score = -99;

                sprintf(digits,"%3d",score);

                for (uint8_t j = 0; j < 3; j++) {
                    if (digits[j] == '-') {
                        getTexCoords(GREEN_FONT, GREEN_NEG, texCoords);
                    }
                    else if (digits[j] != ' ') {
                        getTexCoords(GREEN_FONT, digits[j] - '0' + GREEN_ZERO, texCoords);
                    }
                    if (digits[j] != ' ') {
                        glTexCoord2d(texCoords[0], texCoords[1]);
                        glVertex3f(baseX+36.0*j, baseY, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[1]);
                        glVertex3f(baseX+36.0*j+30.0, baseY, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[3]);
                        glVertex3f(baseX+36.0*j+30.0, baseY+44.0, FONT_DEPTH);
                        glTexCoord2d(texCoords[0], texCoords[3]);
                        glVertex3f(baseX+36.0*j, baseY+44.0, FONT_DEPTH);
                    }

                }

                if (i == 1) {
                    baseX = -152.0;
                    baseY = -479.0;
                }
                else {
                    baseX += 206.0;
                }


            }
        glEnd();

        /* Draw the time remaining */
        int32_t time_ms;
        if (myStatus.time_ms >= 600000) {
            time_ms = 599999;
        }
        else {
            time_ms = myStatus.time_ms;
        }
        uint32_t fullmins = time_ms / 60000;
        uint32_t mins = (time_ms / 6000) % 10;
        uint32_t fullsecs = ((time_ms / 100) % 60)/10;
        uint32_t secs = (time_ms / 1000) % 10;
        uint32_t tenths = (time_ms % 10);
        printf("%u%u:%u%u.%u time reamining\n", fullmins, mins, fullsecs, secs, tenths);

        glBindTexture(GL_TEXTURE_2D, myTextures[RED_FONT].texHandle);
        baseX = -152.0; baseY = 535.0;
        glBegin(GL_QUADS);
        for (uint8_t i = 0; i < 2; i++) {

            if (fullmins != 0) {
                getTexCoords(RED_FONT, RED_ZERO + fullmins, texCoords);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(baseX, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(baseX+25.0, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(baseX+25.0, baseY+36.0, FONT_DEPTH);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(baseX, baseY+36.0, FONT_DEPTH);
            }
            baseX += 27.0;
            if ((fullmins > 0) || (mins > 0)) {
                getTexCoords(RED_FONT, RED_ZERO + mins, texCoords);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(baseX, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(baseX+25.0, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(baseX+25.0, baseY+36.0, FONT_DEPTH);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(baseX, baseY+36.0, FONT_DEPTH);
            }
            baseX += 27.0;
            if ((fullmins != 0) || (mins != 0)) {
                getTexCoords(RED_FONT, RED_ZERO + fullsecs, texCoords);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(baseX, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(baseX+25.0, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(baseX+25.0, baseY+36.0, FONT_DEPTH);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(baseX, baseY+36.0, FONT_DEPTH);

                baseX += 27.0;

                getTexCoords(RED_FONT, RED_ZERO + secs, texCoords);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(baseX, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(baseX+25.0, baseY, FONT_DEPTH);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(baseX+25.0, baseY+36.0, FONT_DEPTH);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(baseX, baseY+36.0, FONT_DEPTH);
            }

            baseX = -152.0;
            baseY = -420.0;
        }
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
                switch (event.key.code) {
                    case sf::Keyboard::Num1:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[0]--;
                        else
                            myStatus.scores[0]++;
                        break;
                    case sf::Keyboard::Num2:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[1]--;
                        else
                            myStatus.scores[1]++;
                        break;
                    case sf::Keyboard::Num3:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[2]--;
                        else
                            myStatus.scores[2]++;
                        break;
                    case sf::Keyboard::Num4:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[3]--;
                        else
                            myStatus.scores[3]++;
                        break;
                    case sf::Keyboard::Escape:
                    case sf::Keyboard::Q:
                        myWindow.close();
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Return:
                        if (myStatus.mode == DEMO_MID)
                            myStatus.mode = DEMO_END;
                        break;
                    default:
                        mySound.setBuffer(mySoundBuffers[i]);
                        mySound.play();
                        i++;
                        i %= NUM_SFX;
                        break;
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
        myTextures[TEX_BACKGROUND].spriteMap = tex_background_spriteMap;
        myTextures[TEX_COURT].spriteMap = tex_court_spriteMap;
        myTextures[TEX_SCOREBOARD_BOTTOM].spriteMap = tex_scoreboard_bottom_spriteMap;
        myTextures[TEX_SCOREBOARD_TOP].spriteMap = tex_scoreboard_top_spriteMap;
        myTextures[GREEN_FONT].spriteMap = green_font_spriteMap;
        myTextures[RED_FONT].spriteMap = red_font_spriteMap;


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
