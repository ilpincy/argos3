#include "transforms.h"

namespace argos {

   /****************************************/
   /****************************************/

   CVector3 GlobalToLocal(CVector3 c_global,
                          CVector3 c_translation,
                          CQuaternion c_orientation) {
      CVector3 cLocal = c_global - c_translation;
      cLocal.Rotate(c_orientation.Inverse());
      return cLocal;
   }

   /****************************************/
   /****************************************/

   CVector3 LocalToGlobal(CVector3 c_local,
                          CVector3 c_translation,
                          CQuaternion c_orientation) {
      CVector3 cGlobal = c_local.Rotate(c_orientation);
      cGlobal += c_translation;
      return cGlobal;
   }

   /****************************************/
   /****************************************/

}
