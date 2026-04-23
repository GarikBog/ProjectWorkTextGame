#ifndef OBJECT_FABRIC
#define OBJECT_FABRIC

#include "../FightSystem/body_part.h"
#include "../FightSystem/object.h"

class ObjectFabric {
 public:
  static Object* GetObjectByChar(const char ch);

 private:
  static std::vector<BodyPart> GetBodyPartsFromFile(std::string file_path);
};

#endif  // !OBJECT_FABRIC
