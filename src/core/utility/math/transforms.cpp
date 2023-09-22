#include "transforms.h"

using namespace argos;

/****************************************/
/****************************************/

CVector3 GlobalToLocal(const CVector3& c_global,
                       const CVector3& c_translation,
                       const CQuaternion& c_orientation) {
   CVector3 cLocal = c_global - c_translation;
   cLocal.Rotate(c_orientation.Inverse());
   return cLocal;
}

/****************************************/
/****************************************/

CVector3 LocalToGlobal(const CVector3& c_local,
                       const CVector3& c_translation,
                       const CQuaternion& c_orientation) {
   CVector cGlobal = c_local.Rotate(c_orientation);
   cGlobal += c_translation;
   return cGlobal;
}

/****************************************/
/****************************************/

