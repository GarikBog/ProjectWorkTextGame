#include "object_fabric.h"

Object* ObjectFabric::GetObjectByChar(const char ch) {
  switch (ch) {
    case ('s'):
      return new Object(ASSETS_PATH "/textures/objects/pudge.png",
                        {0, 0, 800, 800});
    default:
      return new Object(ASSETS_PATH "/textures/objects/ground.jpg",
                        {0, 0, 800, 800});
      ;
  }
}
