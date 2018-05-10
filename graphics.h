#pragma once
#include "agk.h"

class PixelBuffer
{
  public:
    PixelBuffer(int width, int height)
    {
      _id = agk::CreateMemblock(12 + width * height * (8*4));
      _width = width;
      _height = height;
      _image = agk::CreateImageFromMemblock(_id);
      _sprite = agk::CreateSprite(_image);
    }

    ~PixelBuffer()
    {
      agk::DeleteSprite(_sprite);
      agk::DeleteImage(image);
      agk::DeleteMemblock(_id);
    }

    void Draw()
    {
      agk::CreateImageFromMemblock(_image, _id);
      agk::SetSpriteImage(_sprite, _image);
      agk::SetSpritePosition(_sprite, 0, 0);
    }

  private:
    int _id;
    int _image;
    int _sprite;
    int _width;
    int _height;
}
