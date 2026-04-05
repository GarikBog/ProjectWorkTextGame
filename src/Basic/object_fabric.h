#ifndef OBJECT_FABRIC
#define OBJECT_FABRIC

#include "../FightSystem/object.h"

class ObjectFabric {
 public:
  static Object* GetObjectByChar(const char ch);
};

#endif  // !OBJECT_FABRIC
