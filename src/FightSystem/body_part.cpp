#include "body_part.h"

BodyPart::BodyPart(std::string name, int max_health,
                   std::string texture_file_path, sf::IntRect texture_rectangle)
    : body_icon_(texture_file_path, texture_rectangle),
      name_(name),
      max_health_(max_health) {
  health_ = max_health_;
}

BodyPart::BodyPart(BodyPart&& other) noexcept {
  this->body_icon_ = other.body_icon_;
  this->max_health_ = other.max_health_;
  this->health_ = other.health_;
  this->name_ = other.name_;
}

BodyPart::BodyPart(BodyPart& other) noexcept {
  this->body_icon_ = other.body_icon_;
  this->max_health_ = other.max_health_;
  this->health_ = other.health_;
  this->name_ = other.name_;
}

BodyPart& BodyPart::operator=(BodyPart& other) noexcept {
  this->body_icon_ = other.body_icon_;
  this->max_health_ = other.max_health_;
  this->health_ = other.health_;
  this->name_ = other.name_;
  return *this;
}

BodyPart& BodyPart::operator=(BodyPart&& other) noexcept {
  this->body_icon_ = other.body_icon_;
  this->max_health_ = other.max_health_;
  this->health_ = other.health_;
  this->name_ = other.name_;
  return *this;
}

std::string BodyPart::GetName() const { return name_; }

std::string BodyPart::GetHealthInString() const {
  return std::to_string(max_health_) + "/" + std::to_string(health_);
}

Icon& BodyPart::GetIcon() { return body_icon_; }

void BodyPart::GetDamage(int damage) {
  health_ -= damage;
  last_damage = damage;
  if (health_ <= 0) {
    health_ = 0;
  }
}

int BodyPart::GetHealth() const { return health_; }

float BodyPart::GetHealthPercent() const { return health_ / max_health_; }

int BodyPart::GetLostHealth() const { return max_health_ - health_; }

int BodyPart::GetLastDamage() const { return last_damage; }

bool BodyPart::isAlive() const { return health_ > 0; }
