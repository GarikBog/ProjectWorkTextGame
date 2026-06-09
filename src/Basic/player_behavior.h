#ifndef PLAYER_BEHAVIOR
#define PLAYER_BEHAVIOR

struct PlayerBehavior {
 public:
  // Конструкторы и деструктор
  PlayerBehavior(float hostility = 0, float exploration = 0, float mercy = 0,
                 float honor = 0, float enrichment = 0, float honesty = 0);
  ~PlayerBehavior() = default;

  // Копирование
  PlayerBehavior(const PlayerBehavior& other) = default;
  PlayerBehavior& operator=(const PlayerBehavior& other) = default;

  // Перемещение
  PlayerBehavior(PlayerBehavior&& other) noexcept = default;
  PlayerBehavior& operator=(PlayerBehavior&& other) noexcept = default;

  // hostility
  float getHostility() const;
  void addHostility(float value);

  // exploration
  float getExploration() const;
  void addExploration(float value);

  // mercy
  float getMercy() const;
  void addMercy(float value);

  // honor
  float getHonor() const;
  void addHonor(float value);

  // enrichment
  float getEnrichment() const;
  void addEnrichment(float value);

  // honesty
  float getHonesty() const;
  void addHonesty(float value);

  PlayerBehavior& operator+=(const PlayerBehavior& other);

 private:
  float hostility = 0;    // враждебность
  float exploration = 0;  // исследование
  float mercy = 0;        // Милосердие
  float honor = 0;        // Боевая честь
  float enrichment = 0;   // Обогащение
  float honesty = 0;      // Честность
};

#endif  // !PLAYER_BEHAVIOR
