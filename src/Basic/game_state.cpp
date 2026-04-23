#include "game_state.h"

#include <iostream>

#include "../FightSystem/battle_field.h"
#include "../FightSystem/movement_controller.h"
#include "../Visual/button.h"
#include "SFML/Graphics.hpp"

std::wstring StringToWString(const std::string& str) {
  if (str.empty()) return L"";

  int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(),
                                 static_cast<int>(str.size()), NULL, 0);
  std::wstring result(size, 0);
  MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()),
                      &result[0], size);
  return result;
}

std::string WStringToString(const std::wstring& wstr) {
  if (wstr.empty()) return "";

  int size =
      WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
                          static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
  std::string result(size, 0);
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()),
                      &result[0], size, NULL, NULL);
  return result;
}

void GameState::StartGame() {
  backgrounds_images_["BattleFullSwing"] =
      new Icon(ASSETS_PATH "/textures/UI/BattleFullSwing.png",
               sf::IntRect(0, 0, 900, 1020));
  backgrounds_images_["BattleWon"] = new Icon(
      ASSETS_PATH "/textures/UI/BattleWon.png", sf::IntRect(0, 0, 900, 1020));
  backgrounds_images_["PlayerDead"] = new Icon(
      ASSETS_PATH "/textures/UI/YouDead.png", sf::IntRect(0, 0, 900, 1020));

  ui_inventory_ = new UIInventory(window_, {150, 90},
                                  ASSETS_PATH "/textures/ui/inventory.png",
                                  sf::IntRect(0, 0, 900, 900));

  current_battle_ = new BattleField(
      window_, ASSETS_PATH "/battles/test.txt", std::pair(195, 31),
      ASSETS_PATH "/textures/UI/battle.png", sf::IntRect(0, 0, 810, 810));
  movement_controller_ = new MovementController(current_battle_);
  current_battle_->SetMovementController(movement_controller_);

  ui_inventory_->SetVisibility(false);

  current_battle_button_ = new Button(
      this, window_, std::pair(kBattleButtonX_, kBattleButtonY_),
      std::pair(kSwitchModesButtonW_, kSwitchModesButtonH_),
      ASSETS_PATH "/textures/buttons/Battle.png", sf::IntRect(0, 0, 100, 100));
  ui_inventory_button_ = new Button(
      this, window_, std::pair(kInventoryButtonX_, kInventoryButtonY_),
      std::pair(kSwitchModesButtonW_, kSwitchModesButtonH_),
      ASSETS_PATH "/textures/buttons/Inventory.png",
      sf::IntRect(0, 0, 100, 100));
  background_button_ = new Button(
      this, window_, std::pair(kBackgroundButtonX_, kBackgroundButtonY_),
      std::pair(kSwitchModesButtonW_, kSwitchModesButtonH_),
      ASSETS_PATH "/textures/buttons/Background.png",
      sf::IntRect(0, 0, 100, 100));

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

Console& GameState::GetConsole() { return console_; }

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

BattleField* GameState::GetCurrentBattle() { return current_battle_; }

MovementController* GameState::GetMovementController() {
  return movement_controller_;
}

void GameState::Click() {
  sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
  for (int i = visible_buttons_.size() - 1; i >= 0; --i) {
    if (visible_buttons_[i]) {
      if (visible_buttons_[i]->Click(mouse_pos)) break;
    }
  }
}

GameState::GameState()
    : window_(sf::VideoMode(kWindowWidth_, kWindowHeight_), "Dexyan",
              sf::Style::Fullscreen),  // change to fullscreen
      console_(window_, {kWindowWidth_ * 0.5625, 0},
               {kWindowWidth_ * 0.4375, kWindowHeight_},
               ASSETS_PATH "/textures/UI/console.jpg", {0, 0, 960, 1080}),
      movement_controller_(nullptr),
      world_delta_time_(0),
      ui_inventory_(nullptr),
      current_battle_button_(nullptr),
      ui_inventory_button_(nullptr),
      background_button_(nullptr),
      background_(window_, std::pair(kBackgroundX_, kBackgroundY_),
                  std::pair(kBackgroundW_, kBackgroundH_),
                  ASSETS_PATH "/textures/UI/BattleFullSwing.png",
                  sf::IntRect(0, 0, 900, 1020)) {
  sf::View view(sf::FloatRect(0, 0, kWindowWidth_, kWindowHeight_));
  window_.setView(view);
}

void GameState::UpdateUIInvSlots() { ui_inventory_->UpdateSlots(); }

GameState::~GameState() {
  delete current_battle_;
  delete movement_controller_;
  visible_buttons_.clear();
}
void GameState::ButtonIsPressed(Button* button) {
  if (button == current_battle_button_) {
    HideAll();
    current_battle_->SetVisibility(true);
  } else if (button == ui_inventory_button_) {
    ui_inventory_->UpdateSlots();
    HideAll();
    ui_inventory_->SetVisibility(true);
  } else if (button == background_button_) {
    HideAll();
    background_.SetVisibility(true);
  }
}
void GameState::PlayerDieInBattle() { current_battle_->EndBattle(); }

bool GameState::IsPlayerAlive() const { return player_.IsAlive(); }

void GameState::LoadBackground(std::string name) {
  try {
    Icon* icon = backgrounds_images_.at(name);

    background_.SetIcon(icon);
  } catch (const std::out_of_range& e) {
    std::cout << "Ключ не найден: " << e.what() << std::endl;
  }
}

void GameState::Update() {
  console_.Update();
  if (movement_controller_ != nullptr) {
    movement_controller_->Update(world_delta_time_);
  }
  if (current_battle_) {
    current_battle_->Update();
    if (current_battle_->IsBattleEnded()) {
      CleanupBattle();
    }
  }
}

void GameState::Draw() {
  if (current_battle_) {
    current_battle_->Draw();
    current_battle_button_->Draw();
  }

  if (ui_inventory_) {
    ui_inventory_->Draw();
    ui_inventory_button_->Draw();
  }

  background_button_->Draw();
  background_.Draw();

  console_.Draw();
}

void GameState::HideAll() {
  if (current_battle_) current_battle_->SetVisibility(false);
  if (ui_inventory_) ui_inventory_->SetVisibility(false);
  background_.SetVisibility(false);
}

void GameState::RefreshTurn() {
  if (movement_controller_ != nullptr) {
    movement_controller_->RefreshTurn();
  }
}

void GameState::EndTurn() {
  if (movement_controller_ != nullptr) {
    movement_controller_->EndTurn();
  }
}

void GameState::CleanupBattle() {
  if (player_.IsAlive()) {
    LoadBackground("BattleWon");
  } else {
    LoadBackground("PlayerDead");
  }
  if (current_battle_) {
    delete current_battle_;
    current_battle_ = nullptr;
  }
  if (movement_controller_) {
    delete movement_controller_;
    movement_controller_ = nullptr;
  }
  HideAll();
  background_.SetVisibility(true);
}
