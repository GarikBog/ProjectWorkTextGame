#include "player_behavior.h"

#include <algorithm>

PlayerBehavior::PlayerBehavior(float hostility, float exploration, float mercy,
                               float honor, float enrichment, float honesty)
    : hostility(hostility),
      exploration(exploration),
      mercy(mercy),
      honor(honor),
      enrichment(enrichment),
      honesty(honesty) {}

// hostility
float PlayerBehavior::getHostility() const { return hostility; }

void PlayerBehavior::addHostility(float value) {
  hostility = std::clamp(hostility + value, -100.0f, 100.0f);
}

// exploration
float PlayerBehavior::getExploration() const { return exploration; }

void PlayerBehavior::addExploration(float value) {
  exploration = std::clamp(exploration + value, -100.0f, 100.0f);
}

// mercy
float PlayerBehavior::getMercy() const { return mercy; }

void PlayerBehavior::addMercy(float value) {
  mercy = std::clamp(mercy + value, -100.0f, 100.0f);
}

// honor
float PlayerBehavior::getHonor() const { return honor; }

void PlayerBehavior::addHonor(float value) {
  honor = std::clamp(honor + value, -100.0f, 100.0f);
}

// enrichment
float PlayerBehavior::getEnrichment() const { return enrichment; }

void PlayerBehavior::addEnrichment(float value) {
  enrichment = std::clamp(enrichment + value, -100.0f, 100.0f);
}

// honesty
float PlayerBehavior::getHonesty() const { return honesty; }

void PlayerBehavior::addHonesty(float value) {
  honesty = std::clamp(honesty + value, -100.0f, 100.0f);
}

PlayerBehavior& PlayerBehavior::operator+=(const PlayerBehavior& other) {
  addHostility(other.hostility);
  addExploration(other.exploration);
  addMercy(other.mercy);
  addHonor(other.honor);
  addEnrichment(other.enrichment);
  addHonesty(other.honesty);
  return *this;
}
