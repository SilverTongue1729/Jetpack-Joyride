/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

// Represents the current state of the game
enum GameState {
  GAME_ACTIVE,
  GAME_LOSE,
  GAME_WIN
};

const glm::vec2 PLAYER_SIZE(75.0f, 75.0f);  // Size of the player
const float ACCELERATION(1200.0f);          // Acceleration of the gravity
const float BOOST_ACCELERATION(2.2f);       // ratio of acceleration of boost to gravity
const float BACKGROUND_VELOCITY(100.0f);    // Speed of background

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
 public:
  // game state
  GameState State;
  float BackgroundPosition = 0.0f;
  bool Keys[1024];
  unsigned int Width, Height;
  std::vector<GameLevel> Levels;
  unsigned int Level;
  unsigned int Score;  
  // constructor/destructor
  Game(unsigned int width, unsigned int height);
  ~Game();
  // initialize game state (load all shaders/textures/levels)
  void Init();
  // functions
  void ZapperCollisions();
  void CoinCollisions();
  // game loop
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
};

#endif