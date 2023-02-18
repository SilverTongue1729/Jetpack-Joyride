/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

#include "game_object.h"
#include "include/resource_manager.h"
#include "sprite_renderer.h"

const unsigned int ZAPPERS_PER_LEVEL = 3;  // number of zappers in level 1

// float TOTAL_LENGTH_OF_GAME = 4.5f + (11 * ZAPPERS_PER_LEVEL - 7) / 3.0f;  // total length of game

/// GameLevel holds all Tiles as part of a Breakout level and
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel {
 public:
  // level state
  std::vector<GameObject> Zappers;
  std::vector<GameObject> Coins;
  // constructor
  GameLevel() {}
  // render level
  void Draw(SpriteRenderer &renderer);
  // check if the level is completed (all non-solid tiles are destroyed)
  bool IsCompleted();
  // create level
  void Init(unsigned int level, unsigned int width, unsigned int height, float BackgroundVelocity);
};

#endif