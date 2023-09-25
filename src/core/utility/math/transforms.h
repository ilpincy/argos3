#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   CVector3 GlobalToLocal(CVector3 c_global,
                          CVector3 c_translation,
                          CQuaternion c_orientation);

   CVector3 LocalToGlobal(CVector3 c_local,
                          CVector3 c_translation,
                          CQuaternion c_orientation);

}

#endif // TRANSFORMS_H
