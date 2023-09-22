#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   CVector3 GlobalToLocal(const CVector3& c_global,
                          const CVector3& c_translation,
                          const CQuaternion& c_orientation);

   CVector3 LocalToGlobal(const CVector3& c_local,
                          const CVector3& c_translation,
                          const CQuaternion& c_orientation);

}

#endif // TRANSFORMS_H
