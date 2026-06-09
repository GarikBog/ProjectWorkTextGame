#ifndef OBJECT_FABRIC
#define OBJECT_FABRIC

#include <map>

#include "../FightSystem/body_part.h"
#include "../FightSystem/object.h"
class ObjectFabric {
 public:
  static Object* GetObjectByChar(const char ch);

 private:
  static std::vector<BodyPart> GetBodyPartsFromFile(std::string file_path);
  static inline std::map<std::string, std::string> object_body_paths_ = {
      {"human", "/body/human.txt"},
      {"slime", "/body/slime.txt"},
      {"serafim", "/body/serafim.txt"}};
  static inline std::map<std::string, std::string> object_textures_ = {
      {"pudge", "/textures/objects/pudge.png"},
      {"slime", "/textures/objects/slime.png"},
      {"serafim", "/textures/objects/serafim.png"},
      {"creep", "/textures/objects/creep.png"},
      {"range_creep", "/textures/objects/range_creep.png"},
      {"rock", "/textures/objects/rock.png"},
      {"lava", "/textures/objects/lava.png"},
      {"bricks", "/textures/objects/bricks.jpg"},
      {"chest", "/textures/objects/chest.png"},
      {"ground", "/textures/objects/ground.jpg"}};
};

#endif  // !OBJECT_FABRIC
