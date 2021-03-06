// Includes
#include "template.h"

// Namespace
using namespace AGK;

app App;

//TODO: Make a game class for ALL of this once it works. Make the big refactor


int worldMap[mapWidth][mapHeight] =
{
	{ 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7 },
{ 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7 },
{ 4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7 },
{ 4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7 },
{ 4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7 },
{ 4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7 },
{ 4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1 },
{ 4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,0 },
{ 4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1 },
{ 4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8 },
{ 4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1 },
{ 4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1 },
{ 6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6 },
{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4 },
{ 6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6 },
{ 4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3 },
{ 4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2 },
{ 4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2 },
{ 4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2 },
{ 4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2 },
{ 4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2 },
{ 4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2 },
{ 4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2 },
{ 4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3 }
};

void app::Begin(void)
{
	agk::SetVirtualResolution(screenWidth, screenHeight);
	//agk::SetWindowAllowResize(0);
	agk::SetClearColor(0, 0, 0); // black
	agk::SetSyncRate(60, 0); // target, gamers won't accept anything less
	agk::SetFolder("media");
	agk::SetDefaultMagFilter(0);

	//buffer = PixelBuffer(screenWidth, screenHeight);

	posX = 22, posY = 11.5;  //x and y start position
	dirX = -1, dirY = 0; //initial direction vector
	planeX = 0, planeY = 0.60; //the 2d raycaster version of camera plane

	time = 0; //time of current frame
	oldTime = 0; //time of previous frame
	;
	//for (int i = 0; i < 8; i++) texture[i].resize(texWidth * texHeight);

	loadedTextures.push_back(agk::LoadImage("eagle.png"));
	loadedTextures.push_back(agk::LoadImage("redbrick.png"));
	loadedTextures.push_back(agk::LoadImage("purplestone.png"));
	loadedTextures.push_back(agk::LoadImage("greystone.png"));
	loadedTextures.push_back(agk::LoadImage("bluestone.png"));
	loadedTextures.push_back(agk::LoadImage("mossy.png"));
	loadedTextures.push_back(agk::LoadImage("wood.png"));
	loadedTextures.push_back(agk::LoadImage("colorstone.png"));

	for (int i = 0; i < loadedTextures.size(); i++)
	{
		int memblock = agk::CreateMemblockFromImage(loadedTextures[i]);
		for (int j = 12; j < agk::GetMemblockSize(memblock) - 1; j += 4)
		{
			texture[i].push_back(agk::MakeColor(agk::GetMemblockInt(memblock, j), agk::GetMemblockInt(memblock, j + 1), agk::GetMemblockInt(memblock, j + 2)));
		}
	}
}

int app::Loop(void)
{
	agk::Print((int)std::ceil(agk::ScreenFPS()));

	PixelBuffer buffer(screenWidth, screenHeight);

	for (int x = 0; x < screenWidth; x++)
	{
		// calculate ray position and direction
		float cameraX = 2.0f * x / (float)screenWidth - 1; // x-coordinate in camera space
		float rayDirX = dirX + planeX * cameraX;
		float rayDirY = dirY + planeY * cameraX;

		// which box of the map we're in
		int mapX = (int)posX;
		int mapY = (int)posY;

		// length of ray from current position to next x or y-side
		float sideDistX;
		float sideDistY;

		// length of ray from one x or y-side to next x or y-side
		float deltaDistX = std::abs(1 / rayDirX);
		float deltaDistY = std::abs(1 / rayDirY);
		float perpWallDist;

		// what direction to step in (+1 or -1)
		int stepX;
		int stepY;

		int hit = 0; // was the wall hit?
		int side; // was a NS or a EW wall hit?
		// calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - posY) * deltaDistY;
		}

		bool clear = false;

		// perform DDA
		while (hit == 0)
		{
			// jump to next map square, OR in x direction, OR in y direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			// Check if ray has hit a wall
			if (worldMap[mapX][mapY] > 0) hit = 1;
			if ((mapY * mapWidth + mapX) >= (mapWidth * mapHeight))
			{
				clear = true;
				hit = 1;
			}
		}

		// Calculate distance projected on camera direction (Euclidean distance will give fisheye)
		if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
		else perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

		// Calculate height of line to draw on screen
		int lineHeight = (int)(screenHeight / perpWallDist);

		// Calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + screenHeight / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + screenHeight / 2;
		if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

		// texturing calculations
		int texNum = worldMap[mapX][mapY] - 1; // 1 subtracted from it so that texture 0 can be used

		if (clear)
			texNum = worldMap[mapWidth - 1][mapHeight - 1] - 1; // eventually set a "clear" texture to call here

		// calculate value of wallX
		float wallX; // where exactly the wall was hit
		if (side == 0) wallX = posY + perpWallDist * rayDirY;
		else
						wallX = posX + perpWallDist * rayDirX;
		wallX -= std::floor((wallX));

		//x coordinate on the texture
		int texX = (wallX * (float)texWidth);
		if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

		for (int y = drawStart; y < drawEnd; y++)
		{
			int d = y * 256 - screenHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
			// TODO: avoid the division to speed this up
			int texY = ((d * texHeight) / lineHeight) / 256;
			int color;
			if (texY < 0 || texY >= texHeight)
				color = texture[texNum][texHeight + texX];
			else if(texX < 0 || texX >= texWidth)
				color = texture[texNum][texWidth * texY + texWidth];
			else
				color = texture[texNum][texWidth * texY + texX];
			//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (side == 1) color = (color >> 1) & 8355711;
			if (y >= 0 && y < screenHeight)
				buffer.Poke(x, y, color);
			else
				buffer.Poke(x, screenHeight - 1, color);
		}
	}
	//agk::SaveImage(buffer.getImage(), "C:\frame.jpg");

	buffer.Draw();

	agk::Sync();

	//speed modifiers
	double moveSpeed = agk::GetFrameTime() * 3.0; //the constant value is in squares/second
	double rotSpeed = agk::GetFrameTime() * 3.0; //the constant value is in radians/second

	//move forward if no wall in front of you
	if (agk::GetRawKeyState(87))
	{
		if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
		if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
	}
	//move backwards if no wall behind you
	if (agk::GetRawKeyState(83))
	{
		if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
		if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
	}
	//rotate to the right
	if (agk::GetRawKeyState(68))
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = dirX;
		dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
		dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
		double oldPlaneX = planeX;
		planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
		planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
	}
	//rotate to the left
	if (agk::GetRawKeyState(65))
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = dirX;
		dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
		dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
		double oldPlaneX = planeX;
		planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
		planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
	}

	return 0; // return 1 to close app
}


void app::End(void)
{

}
