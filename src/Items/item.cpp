#include "item.h"

#include <fstream>
#include <sstream>

#include "../Basic/game_state.h"

// ITEM
Item::Item(std::string asset_file_path) {
  type_ = kDefaultItem;
  std::ifstream file(asset_file_path);

  if (!file.is_open()) {
    file.open(ASSETS_PATH + asset_file_path);
    if (!file.is_open()) {
      throw std::runtime_error("Can't open item asset file: " +
                               asset_file_path);
    }
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string key;
    iss >> key;

    if (key == "NAME") {
      std::getline(iss, name_);
      if (!name_.empty() && name_[0] == ' ') {
        name_ = name_.substr(1);
      }
    } else if (key == "TEXTURE") {
      std::string texture_file;
      iss >> texture_file;
      icon_.SetTexture(texture_file);
    } else if (key == "TEXTURE_RECT") {
      int x, y, w, h;
      if (iss >> x >> y >> w >> h) {
        icon_.SetTextureRect(sf::IntRect(x, y, w, h));
      } else {
        throw std::runtime_error("Invalid TEXTURE_RECT format in: " +
                                 asset_file_path);
      }
    }
  }
}

Item::~Item() {}

void Item::ActiveItem() {}

void Item::DeactiveItem() {}

ItemTypes Item::GetType() const { return type_; }

std::string Item::GetName() const { return name_; }

Icon& Item::GetIcon() { return icon_; }

// ITEM

// DEFENSIVE_ITEM
DefensiveItem::DefensiveItem(std::string asset_file_path, ItemTypes type)
    : Item(asset_file_path), defensive_power_(0) {
  type_ = type;

  std::ifstream file(asset_file_path);

  if (!file.is_open()) {
    file.open(ASSETS_PATH + asset_file_path);
    if (!file.is_open()) {
      throw std::runtime_error("Can't open item asset file: " +
                               asset_file_path);
    }
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string key;
    iss >> key;

    if (key == "DEFENSE") {
      iss >> defensive_power_;
    }
  }
}

DefensiveItem::~DefensiveItem() {}

void DefensiveItem::ActiveItem() {
  GameState::GetGameState().GetPlayer().ChangeDefensive(defensive_power_);
}

void DefensiveItem::DeactiveItem() {
  GameState::GetGameState().GetPlayer().ChangeDefensive(-defensive_power_);
}
// DEFENSIVE_ITEM

// HAND_ITEM
HandItem::HandItem(std::string asset_file_path)
    : Item(asset_file_path), damage_(0) {
  type_ = kHandItem;

  std::ifstream file(asset_file_path);

  if (!file.is_open()) {
    file.open(ASSETS_PATH + asset_file_path);
    if (!file.is_open()) {
      throw std::runtime_error("Can't open item asset file: " +
                               asset_file_path);
    }
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string key;
    iss >> key;

    if (key == "DAMAGE") {
      iss >> damage_;
    }
  }
}

void HandItem::ActiveItem() {
  GameState::GetGameState().GetPlayer().ChangeDamage(damage_);
}
void HandItem::DeactiveItem() {
  GameState::GetGameState().GetPlayer().ChangeDamage(-damage_);
}

// HAND_ITEM

// BELT_ITEM
BeltItem::BeltItem(std::string asset_file_path) : Item(asset_file_path) {
  type_ = kBeltItem;
}

StatItem::StatItem(std::string asset_file_path)
    : BeltItem(asset_file_path), battle_stats_diff_(0, 0, 0, 0, 0, 0, 0) {
  std::ifstream file(asset_file_path);

  if (!file.is_open()) {
    file.open(ASSETS_PATH + asset_file_path);
    if (!file.is_open()) {
      throw std::runtime_error("Can't open item asset file: " +
                               asset_file_path);
    }
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string key;
    iss >> key;

    if (key == "DAMAGE") {
      iss >> battle_stats_diff_.damage;
    } else if (key == "DEFENSE") {
      iss >> battle_stats_diff_.defensive;
    } else if (key == "HEALTH") {
      iss >> battle_stats_diff_.max_health;
    } else if (key == "STAMINA") {
      iss >> battle_stats_diff_.stamina;
    } else if (key == "MAX_ATTACK_RANGE") {
      iss >> battle_stats_diff_.max_attack_range;
    } else if (key == "MIN_ATTACK_RANGE") {
      iss >> battle_stats_diff_.min_attack_range;
    } else if (key == "ATTACK_COUNT") {
      iss >> battle_stats_diff_.max_attack_count;
    }
  }
}

void StatItem::ActiveItem() {
  GameState::GetGameState().GetPlayer().GetBattleStats() += battle_stats_diff_;
}

void StatItem::DeactiveItem() {
  GameState::GetGameState().GetPlayer().GetBattleStats() -= battle_stats_diff_;
}
