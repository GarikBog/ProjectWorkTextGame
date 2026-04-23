#include "lootable_object.h"

#include "../Basic/game_state.h"
#include "../FightSystem/battle_field.h"
#include "../Items/item_fabric.h"

void LootableObject::Check() {
  int dist =
      GameState::GetGameState()
          .GetCurrentBattle()
          ->GetLineBetweenTwoObjects(
              this, GameState::GetGameState().GetPlayer().GetPlayerCharacter())
          .size();

  if (dist > 2) {
    GameState::GetGameState().GetConsole().Log("Dexyan хочет открыть" + name_ +
                                               " но он слишком далеко");
    return;
  }
  GameState::GetGameState().GetConsole().Log("Dexyan обыскивает " + name_ +
                                             " и находит там:");
  auto size = items_in_.size();
  for (int i = 0; i < size; ++i) {
    GameState::GetGameState().GetPlayer().GetInventory().AddItemtoInventory(
        items_in_[items_in_.size() - 1]);
    GameState::GetGameState().GetConsole().Log(
        "- " + items_in_[items_in_.size() - 1]->GetName());
    items_in_.pop_back();
  }
}

LootableObject::LootableObject(std::string texture_file,
                               sf::IntRect texture_rect,
                               std::string asset_file_path, std::string name,
                               BattleStats& battle_stats)
    : Object(texture_file, texture_rect, name, battle_stats, false, true) {
  for (int i = 0; i < 3; ++i) {
    items_in_.push_back(ItemFabric::GetItem(Items(rand() % 5)));
  }
}

LootableObject::~LootableObject() {
  for (Item* item : items_in_) {
    delete item;
  }
}
