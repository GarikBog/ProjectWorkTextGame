#include "game_state.h"

#include "SFML/Graphics.hpp"

void GameState::StartGame() {
  while (game_is_run_ && window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      console_.HandleEvent(event);
      if (event.type == sf::Event::Closed) window_.close();
    }

    window_.clear(sf::Color::Black);
    Update();
    Draw();
    window_.display();
  }
}

GameState& GameState::GetGameState() {
  if (!game_state_) game_state_ = new GameState();
  return *game_state_;
}

GameState::GameState()
    : window_(sf::VideoMode(kWindowWidth_, kWindowHeight_), "Dexyan",
              sf::Style::Fullscreen),
      console_(window_, {kWindowWidth_ * 0.5, 0},
               {kWindowWidth_ * 0.5, kWindowHeight_}, kConsoleTexturePath,
               {0, 0, 960, 1080}) {
  sf::View view(sf::FloatRect(0, 0, kWindowWidth_, kWindowHeight_));
  window_.setView(view);
}

void GameState::Update() { console_.Update(); }

void GameState::Draw() { console_.Draw(); }
