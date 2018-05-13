#ifndef _H_AGKTEMPLATE_
#define _H_AGKTEMPLATE_

// Link to AGK files
#include "agk.h"

// libraries
#include <string>
#include <vector>
#include <iostream>

// other includes
#include "graphics.h"

#define DEVICE_WIDTH 1024
#define DEVICE_HEIGHT 576
#define FULLSCREEN false

// used to make a more unique folder for the write path
#define COMPANY_NAME "Feesh Games"

// macros
//
// TODO: EVENTUALLY a game class should take this stuff as arguments so its not constant
#define screenWidth 512
#define screenHeight 288
// TODO: Find a way to not need this to be constant later
#define texWidth 64
#define texHeight 64
// TODO: Make this into a map class
#define mapWidth 24
#define mapHeight 24

// Global values for the app
class app
{
	public:

		// constructor
		app() { memset ( this, 0, sizeof(app)); }

		// main app functions
		void Begin( void );
		int Loop( void );
		void End( void );

    float posX;
    float posY;
    float dirX;
    float dirY;
    float planeX;
    float planeY;

    float time;
    float oldTime;

    std::vector<int> texture[8];
    std::vector<int> loadedTextures;
};

extern app App;

#endif

// Allow us to use the LoadImage function name
#ifdef LoadImage
 #undef LoadImage
#endif
