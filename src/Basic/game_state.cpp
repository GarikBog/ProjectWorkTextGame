#include "game_state.h"

#include <iostream>

#include "../FightSystem/battle_field.h"
#include "../FightSystem/movement_controller.h"
#include "../Visual/button.h"
#include "SFML/Graphics.hpp"

void GameState::StartGame() {
  current_battle_ = new BattleField(window_, ASSETS_PATH "/battles/test.txt",
                                    {0, 0}, "", {0, 0, 100, 100});
  movement_controller_ = new MovementController(current_battle_);
  current_battle_->SetMovementController(movement_controller_);

  sf::Clock clock;

  while (game_is_run_ && window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      console_.HandleEvent(event);
      if (event.type == sf::Event::Closed) window_.close();
      if (event.type == sf::Event::MouseButtonReleased) {
        Click();
      }
    }

    world_delta_time_ = clock.restart().asSeconds();

    window_.clear(sf::Color::Black);
    Update();
    Draw();
    window_.display();
  }
}

GameState& GameState::GetGameState() {
  if (!game_state_) {
    game_state_ = new GameState();
    return *game_state_;
  }
  return *game_state_;
}

void GameState::PrintText(std::string text, sf::Color color) {
  console_.Log(text, color);
}

void GameState::ButtonChangeVisibility(Button* button, bool is_visible) {
  if (is_visible)
    AddButton(button);
  else
    RemoveButton(button);
}

void GameState::AddButton(Button* button) {
  visible_buttons_.push_back(button);
}

void GameState::RemoveButton(Button* button) {
  for (int i = 0; i < visible_buttons_.size(); ++i) {
    if (visible_buttons_[i] == button) {
      visible_buttons_.erase(visible_buttons_.begin() + i);
      break;
    }
  }
}

sf::RenderWindow& GameState::GetWindow() { return window_; }

Player& GameState::GetPlayer() { return player_; }

void GameState::Click() {
  sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
  for (Button* button : visible_buttons_) {
    if (button) {
      button->Click(mouse_pos);
    }
  }
}

GameState::GameState()
    : window_(sf::VideoMode(kWindowWidth_, kWindowHeight_), "Dexyan",
              sf::Style::Fullscreen),
      console_(window_, {kWindowWidth_ * 0.5625, kWindowHeight_ * 0.25},
               {kWindowWidth_ * 0.4375, kWindowHeight_ * 0.75},
               ASSETS_PATH "/textures/console.jpg", {0, 0, 960, 1080}),
      movement_controller_(nullptr),
      world_delta_time_(0) {
  sf::View view(sf::FloatRect(0, 0, kWindowWidth_, kWindowHeight_));
  window_.setView(view);
}

GameState::~GameState() {
  delete current_battle_;
  delete movement_controller_;
  visible_buttons_.clear();
}
void GameState::Update() {
  console_.Update();
  if (movement_controller_ != nullptr) {
    movement_controller_->Update(world_delta_time_);
  }
}

void GameState::Draw() {
  if (current_battle_) {
    current_battle_->Draw();
  }
  console_.Draw();
}
