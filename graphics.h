#pragma once
#include "agk.h"

class PixelBuffer
{
  public:
    PixelBuffer();
    PixelBuffer(int width, int height)
    {
      _id = agk::CreateMemblock(12 + width * height * 4);
      _width = width;
      _height = height;
	  agk::SetMemblockInt(_id, 0, _width); //writing starts with offset 0
	  agk::SetMemblockInt(_id, 4, _height); //integer of width took 4 bytes, so offset by 4
	  agk::SetMemblockInt(_id, 8, 32); //bit depth, must be 32. also offset by another 4
      _image = agk::CreateImageFromMemblock(_id);
      _sprite = agk::CreateSprite(_image);
    }

    ~PixelBuffer()
    {
      agk::DeleteSprite(_sprite);
      agk::DeleteImage(_image);
      agk::DeleteMemblock(_id);
    }

    //TODO: GetPixel(x, y)

    int getID() const { return _id; }
    int getImage() const { return _image; }
    int getSprite() const { return _sprite; }
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

    void Draw()
    {
      agk::CreateImageFromMemblock(_image, _id);
      agk::SetSpriteImage(_sprite, _image);
      agk::SetSpritePosition(_sprite, 0, 0);
	  agk::SetSpriteSize(_sprite, _width, _height);
    }

    void Poke(int x, int y, int r, int g, int b)
    {
      int offset = 12 + ((y * _height) + x); // initial offset, the first 12 bytes are metadata
      //TODO: make this into a loop maybe?
      agk::SetMemblockInt(_id, offset, r);
      offset += 4; //offset by 4 bytes for each value
      agk::SetMemblockInt(_id, offset, g);
      offset += 4;
      agk::SetMemblockInt(_id, offset, b);
      offset += 4;
      agk::SetMemblockInt(_id, offset, 255); //Alpha default to 255 for now, maybe change later
    }

    void Poke(int x, int y, int color)
    {
      Poke(x, y, 
          agk::GetColorRed(color),
          agk::GetColorGreen(color),
          agk::GetColorBlue(color));
    }

  private:
    int _id;
    int _image;
    int _sprite;
    int _width;
    int _height;
};
