/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"

#include <iostream>

const glm::vec2 coinSize(50.0f, 50.0f);  // coin size

void GameLevel::Draw(SpriteRenderer &renderer) {
  for (GameObject &zapper : this->Zappers)
    if (!zapper.Destroyed)
      zapper.Draw(renderer);
  for (GameObject &coin : this->Coins)
    if (!coin.Destroyed)
    coin.Draw(renderer);
}

bool GameLevel::IsCompleted() {
  for (GameObject &tile : this->Zappers)
    if (!tile.Destroyed)
      return false;
  return true;
}

void GameLevel::Init(unsigned int level, unsigned int width, unsigned int height, float BackgroundVelocity) {
  this->Coins.clear();
  glm::vec3 coinColor = glm::vec3(1.0f);

  glm::vec2 coinPos;

  float LevelLength = 1.0f + (((level != 1) ? 2 : 1) * ZAPPERS_PER_LEVEL - 1) * ((level != 1) ? (2.0f / 3.0f) : 1.0f);

  std::cout << "LevelLength: " << LevelLength << std::endl;

  for (float i = float(width); i < LevelLength * width; i += 50.f) {
    std::cout << "i: " << i << std::endl;
    if (rand() % 4 != 2) continue;

    int NoOfCoins = rand() % 5 + 3;
    std::cout << "NoOfCoins: " << NoOfCoins << std::endl;
    float coinHeight = ((float)rand() / RAND_MAX) * (height - 200.0f) + 100.0f;
    std::cout << "coinHeight: " << coinHeight << std::endl;
    for (int j = 0; j < NoOfCoins; j++) {
      coinPos = glm::vec2(i + 65.f * j, coinHeight);
      GameObject obj(coinPos, coinSize, ResourceManager::GetTexture("coin"), coinColor, glm::vec2(BackgroundVelocity, 0.0f));
      this->Coins.push_back(obj);
    }
    i += 65.f * NoOfCoins;
  }

  // clear old data
  this->Zappers.clear();
  for (int i = 0; i < ZAPPERS_PER_LEVEL * ((level != 1) ? 2 : 1); i++) {
    float length;
    if (level == 1)
      length = ((float)rand() / RAND_MAX) * (height / 2.0f - 200.0f) + 200.0f;
    else
      length = ((float)rand() / RAND_MAX) * (height / 1.5f - 300.0f) + 300.0f;

    float ycoord = ((float)rand() / RAND_MAX) * (height - length);
    glm::vec2 pos((float)width * i * ((level != 1) ? 2.0f / 3.0f : 1.0f) + width, ycoord);
    glm::vec2 size(50.0f, length);
    glm::vec3 color = glm::vec3(1.0f);
    if (level == 3 && i % 2 == 1) color = glm::vec3(1.0f, 0.5f, 0.5f);  // red zappers

    GameObject obj(pos, size, ResourceManager::GetTexture("zapper"), color, glm::vec2(BackgroundVelocity, 0.0f));
    obj.Rotation = ((float)rand() / RAND_MAX) * 360.0f;
    if (level == 3 && i % 2 == 1) obj.ContRotate = ((float)rand() / RAND_MAX) * 10.0f + 5.0f;
    this->Zappers.push_back(obj);
  }
}