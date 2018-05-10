// Includes
#include "template.h"

// Namespace
using namespace AGK;

app App;

void app::Begin(void)
{
	agk::SetVirtualResolution (1024, 576);
  agk::SetWindowAllowResize(0);
  agk::SetClearColor( 0,0,0 ); // light blue
	agk::SetSyncRate(60,0);
}

int app::Loop (void)
{
	agk::Print((int)std::ceil(agk::ScreenFPS()));
	agk::Sync();

	return 0; // return 1 to close app
}


void app::End (void)
{

}
