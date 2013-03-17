/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/

/*****************************************************************************
 * utils.cpp - utility functions for the floravsundead application.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/

#include "floravsundead.hpp"


/*****************************************************************************
 * Function: strlower
 * Description: Overwrites an input string so that each character is lower-case
 *****************************************************************************/
void strlower(char *in) {
    for (int i = 0; in[i]; i++) {
        if (in[i] >= 'A' && in[i] <= 'Z') {
            in[i] = in[i] - 'A' + 'a';
        }
    }
    return;
}


namespace fvu {


    /*****************************************************************************
    * Function: Game::getTexCoords
    * Description: Returns texture coordinates for a specified texture map and
    * object name.
    *****************************************************************************/
    float *Game::getTexCoords(TEXTURE_ENUM texID, uint16_t spriteID, float *texCoords) {

        texCoords[0] = 1.0*myTextures[texID].spriteMap[spriteID][0] / myTextures[texID].width;
        texCoords[3] = 1.0*myTextures[texID].spriteMap[spriteID][1] / myTextures[texID].height;
        texCoords[2] = 1.0*myTextures[texID].spriteMap[spriteID][2] / myTextures[texID].width;
        texCoords[1] = 1.0*myTextures[texID].spriteMap[spriteID][3] / myTextures[texID].height;
        texCoords[4] = 1.0*(myTextures[texID].spriteMap[spriteID][2] - myTextures[texID].spriteMap[spriteID][0]);
        texCoords[5] = 1.0*(myTextures[texID].spriteMap[spriteID][3] - myTextures[texID].spriteMap[spriteID][1]);

        return texCoords;
    }




    /*****************************************************************************
    * Function: Game::printStatus
    * Description: Prints out the game's status, in terms of player and zombie
    * locations, score, etc.
    *****************************************************************************/

    void Game::printStatus() {
        printf("\nPrinting status information\n");
    }

    /*****************************************************************************
    * Function: print_help
    * Description: Prints out the program help message.
    *****************************************************************************/

    void Game::print_help() {
        printf("Usage: %s [options] <file1> [<files> ...]\n\n", EXEC_NAME);
        printf("Main options:\n");

        printf("-W <value>, --width <value>        Screen pixel width ");
        printf("(default is %d)\n", SCREEN_WIDTH_DEFAULT);

        printf("-H <value>, --height <value>       Screen pixel height ");
        printf("(default is %d)\n", SCREEN_HEIGHT_DEFAULT);

        printf("-D <value>, --debug <value>        Debug level ");
        printf("(default is %d)\n", DEBUG_DEFAULT);

        printf("-Z <file>, --zom <file>            Zombie file ");
        printf("(default is %s)\n", ZOM_FNAME_DEFAULT);

        printf("-h, --help                         Print this message\n");

        printf("<file1> <file2> <file3> <file4>    Team files ");
        printf("(default is %s)\n", TEAM_FNAME_DEFAULT);

        exit(0);
    }

    /*****************************************************************************
    * Function: raise_error
    * Description: Prints out an error message determined by
    * the condition and exits the program.
    *****************************************************************************/

    void Game::raise_error(uint32_t error_num, const char *msg) {

        fprintf(stderr, "\n");
        switch(error_num) {
            case ERR_SFML:
                fprintf(stderr, "Error: initSDL failed: %s\n", msg);
                break;
            case ERR_USAGE:
                fprintf(stderr, "Usage: %s [-W <n>] [-H <n>] [-D <n>] ", EXEC_NAME);
                fprintf(stderr, "[-Z <file>] <file1> [<file2> <file3> <file4>]\n");
                break;
            case ERR_NOFILE1:
                fprintf(stderr, "Error: %s texture file not found\n", msg);
                break;
            case ERR_NOFILE2:
                fprintf(stderr, "Error: %s player file not found\n", msg);
                break;
            case ERR_NOFILE3:
                fprintf(stderr, "Error: %s zombie file not found\n", msg);
                break;
            case ERR_BADFILE1:
                fprintf(stderr, "Error: %s texture file is invalid\n", msg);
                break;
            case ERR_BADFILE2:
                fprintf(stderr, "Error: %s player file is invalid\n", msg);
                break;
            case ERR_BADFILE3:
                fprintf(stderr, "Error: %s zombie file is invalid\n", msg);
                break;
            case ERR_BADFILE4:
                fprintf(stderr, "Error: %s audio file is invalid\n", msg);
                break;
            case ERR_AUDIO:
                fprintf(stderr, "Error: audio problem in %s\n", msg);
                break;
            case ERR_NOMEM:
                fprintf(stderr, "Error: memory allocation problem in %s\n", msg);
                break;
            case ERR_OVERBUDGET:
                fprintf(stderr, "Error: team %s is over budget\n", msg);
                break;
            case ERR_UNDEFINED:
                default:
                fprintf(stderr, "Error: undefined error\n");
                break;
        }
        fprintf(stderr, "%s exited with error code %d\n", EXEC_NAME, error_num);
        exit(error_num);
    }


    /*****************************************************************************
    * Function: printConfig
    * Description: Prints the current configuration data.
    *****************************************************************************/

    void Game::printConfig() {

        if (myConfig.debug_level > 5) {
            printf("\nCurrent configuration information:\n");
            printf("\tDebug level:           %d\n", myConfig.debug_level);
            printf("\tScreen pixel width:  %d\n", myConfig.screen_width);
            printf("\tScreen pixel height: %d\n", myConfig.screen_height);

            for (int8_t i = 0; i < 4; i++) {
                printf("\tTeam %d file name: %16s\n", i, myConfig.team_fname[i]);
            }

            printf("\tZombie file name: %16s\n", myConfig.zom_fname);
            printf("\tNumber of zombies: %d\n", myZombies[0].size());
            printf("\n");


        }
    }


} //namespace fvu
