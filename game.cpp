/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include "game.h"

#include <iostream>

#include "game_object.h"
#include "include/resource_manager.h"
#include "sprite_renderer.h"

// Game-related State data
SpriteRenderer *Renderer;
GameObject *Player;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {
}

Game::~Game() {
  delete Renderer;
  delete Player;
}

bool CheckCollision(GameObject &one, GameObject &two) {
  // Create the axis vectors for the first rectangle
  std::vector<glm::vec2> axes1(4);
  axes1[0] = glm::vec2(cos(one.Rotation), sin(one.Rotation));
  axes1[1] = glm::vec2(-sin(one.Rotation), cos(one.Rotation));
  axes1[2] = glm::vec2(-cos(one.Rotation), -sin(one.Rotation));
  axes1[3] = glm::vec2(sin(one.Rotation), -cos(one.Rotation));

  // Create the axis vectors for the second rectangle
  std::vector<glm::vec2> axes2(4);
  axes2[0] = glm::vec2(cos(two.Rotation), sin(two.Rotation));
  axes2[1] = glm::vec2(-sin(two.Rotation), cos(two.Rotation));
  axes2[2] = glm::vec2(-cos(two.Rotation), -sin(two.Rotation));
  axes2[3] = glm::vec2(sin(two.Rotation), -cos(two.Rotation));

  // Combine the axes for both rectangles
  std::vector<glm::vec2> all_axes(axes1);
  all_axes.insert(all_axes.end(), axes2.begin(), axes2.end());

  // Check for overlap on each axis
  for (const auto &axis : all_axes) {
    float min1 = std::numeric_limits<float>::max();
    float max1 = -std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max();
    float max2 = -std::numeric_limits<float>::max();

    // Project the vertices of the first rectangle onto the current axis
    for (int i = 0; i < 4; i++) {
      float dot = glm::dot(one.Position + (one.Size * (glm::vec2((i == 1 || i == 2) ? 1.0f : 0.0f, (i == 2 || i == 3) ? 1.0f : 0.0f) - 0.5f)), axis);
      min1 = std::min(min1, dot);
      max1 = std::max(max1, dot);
    }

    // Project the vertices of the second rectangle onto the current axis
    for (int i = 0; i < 4; i++) {
      float dot = glm::dot(two.Position + (two.Size * (glm::vec2((i == 1 || i == 2) ? 1.0f : 0.0f, (i == 2 || i == 3) ? 1.0f : 0.0f) - 0.5f)), axis);
      min2 = std::min(min2, dot);
      max2 = std::max(max2, dot);
    }

    // Check for overlap on this axis
    float len1 = max1 - min1;
    float len2 = max2 - min2;
    float distance = (min2 + len2 / 2.0f) - (min1 + len1 / 2.0f);
    float sum = len1 / 2.0f + len2 / 2.0f;
    if (fabs(distance) > sum) {
      return false;
    }
  }

  return true;
}

void Game::ZapperCollisions() {
  for (GameObject &zapper : this->Levels[this->Level].Zappers) {
    if (!zapper.Destroyed) {
      if (CheckCollision(*Player, zapper)) {
        zapper.Destroyed = true;
        this->State = GAME_LOSE;
        std::cout << "You lose!" << std::endl;
        std::cout << "Score: " << this->Score << std::endl;
        return;
      }
    }
  }
}

void Game::CoinCollisions() {
  for (GameObject &coin : this->Levels[this->Level].Coins) {
    if (!coin.Destroyed) {
      if (CheckCollision(*Player, coin)) {
        coin.Destroyed = true;
        this->Score++;
      }
    }
  }
}

void Game::Init() {
  // load shaders
  ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
  // configure shaders
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  // set render-specific controls
  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
  // load textures
  ResourceManager::LoadTexture("textures/barry.png", true, "barry");
  ResourceManager::LoadTexture("textures/background.png", true, "background");
  ResourceManager::LoadTexture("textures/zapper.png", true, "zapper");
  ResourceManager::LoadTexture("textures/coin.png", true, "coin");
  // load levels
  GameLevel one, two, three;
  one.Init(1, this->Width, this->Height, BACKGROUND_VELOCITY);
  two.Init(2, this->Width, this->Height, BACKGROUND_VELOCITY);
  three.Init(3, this->Width, this->Height, BACKGROUND_VELOCITY);
  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);

  this->Level = 0;
  this->Score = 0;
  glm::vec2 playerPos = glm::vec2(PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
  Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("barry"));
}

void Game::Update(float dt) {
  if (this->State == GAME_ACTIVE) {
    float acceleration = ACCELERATION * dt;         // gravity
    Player->Position.y += Player->Velocity.y * dt;  // move player
    Player->Velocity.y += acceleration;             // apply gravity
    if (Player->Position.y < 0.0f) {                // check if player is above ceiling
      Player->Position.y = 0.0f;
      Player->Velocity.y = 0.0f;
    }
    if (Player->Position.y >= this->Height - PLAYER_SIZE.y) {  // check if player is below ground
      Player->Position.y = this->Height - PLAYER_SIZE.y;
      Player->Velocity.y = 0.0f;
    }

    // std::cout << "Player->Velocity.y: " << Player->Velocity.y << std::endl;
    // std::cout << "Player->Position.y: " << Player->Position.y << std::endl;

    BackgroundPosition -= BACKGROUND_VELOCITY * dt;  // move background
    if (BackgroundPosition < -1.0f * this->Width) {  // reset background
      BackgroundPosition = 0.0f;
    }
    // std::cout << "BackgroundPosition: " << BackgroundPosition << std::endl;

    for (GameObject &zapper : this->Levels[this->Level].Zappers) {  // move zappers
      if (!zapper.Destroyed) {
        zapper.Position.x -= zapper.Velocity.x * dt;
        if (zapper.Position.x < -1.0f * (this->Width) / 2.0f) {
          zapper.Destroyed = true;
        }
        zapper.Rotation += zapper.ContRotate * dt;  // rotate zappers
      }
    }
    for (GameObject &coin : this->Levels[this->Level].Coins) {  // move coins
      if (!coin.Destroyed) {
        coin.Position.x -= coin.Velocity.x * dt;
        if (coin.Position.x < -1.0f * 50.0f) {
          coin.Destroyed = true;
        }
      }
    }

    if (this->Levels[this->Level].IsCompleted() == true) {  // check if level is completed
      this->Level++;
      if (this->Level == 3) {
        this->State = GAME_WIN;
        std::cout << "You win!" << std::endl;
        std::cout << "Score: " << this->Score << std::endl;
      }
      std::cout << "Level: " << this->Level + 1 << std::endl;
    }
  }

  this->ZapperCollisions();
  this->CoinCollisions();
}

void Game::ProcessInput(float dt) {
  if (this->State == GAME_ACTIVE) {
    float acceleration = ACCELERATION * dt;

    // move player
    if (this->Keys[GLFW_KEY_SPACE]) {
      if (Player->Position.y >= 0.0f)
        Player->Velocity.y -= acceleration * BOOST_ACCELERATION;
    }
  }
}

void Game::Render() {
  if (this->State == GAME_ACTIVE) {
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f + BackgroundPosition, 0.0f), glm::vec2(this->Width, this->Height));
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(this->Width + BackgroundPosition, 0.0f), glm::vec2(this->Width, this->Height));
    Player->Draw(*Renderer);
    this->Levels[this->Level].Draw(*Renderer);
  } else if (this->State == GAME_WIN) {
  }
}