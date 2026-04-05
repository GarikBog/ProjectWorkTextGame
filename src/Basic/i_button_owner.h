#ifndef I_BUTTON_OWNER
#define I_BUTTON_OWNER

class Button;

class IButtonOwner {
 public:
  virtual ~IButtonOwner() = default;
  virtual void ButtonIsPressed(Button* button) = 0;
};

#endif  // !I_BUTTON_OWNER
