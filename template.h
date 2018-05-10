#ifndef _H_AGKTEMPLATE_
#define _H_AGKTEMPLATE_

// Link to AGK files
#include "agk.h"

#define DEVICE_WIDTH 1024
#define DEVICE_HEIGHT 576
#define FULLSCREEN false

// used to make a more unique folder for the write path
#define COMPANY_NAME "Feesh Games"

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
};

extern app App;

#endif

// Allow us to use the LoadImage function name
#ifdef LoadImage
 #undef LoadImage
#endif
