#pragma once

#include "Camera.h"
#include "Point.h"
#include "Rect.h"

class Level;
class Player;

class Entity{
    public:
      virtual bool IsAlive()=0;
      virtual Rect GetHitBox()=0;
      virtual void Update(int ms, Level& lvl,Player& player)=0;
      virtual void Draw(const Camera& camera)=0;
};