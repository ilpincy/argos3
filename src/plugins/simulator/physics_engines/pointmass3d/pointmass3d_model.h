/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef POINTMASS3D_ENTITY_H
#define POINTMASS3D_ENTITY_H

namespace argos {
   class CPointMass3DEngine;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CPointMass3DModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CPointMass3DModel*> TMap;

   public:

      CPointMass3DModel(CPointMass3DEngine& c_engine,
                        CEmbodiedEntity& c_entity) :
         CPhysicsModel(c_engine, c_entity),
         m_cPM3DEngine(c_engine) {}

      virtual ~CPointMass3DModel() {}

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) {
         return false;
      }

      virtual void Reset() {}

      virtual void UpdateEntityStatus() = 0;
      virtual void Step() = 0;
      virtual void UpdateFromEntityStatus() = 0;

      virtual bool IsCollidingWithSomething() const {
         return false;
      }

   protected:

      CPointMass3DEngine& m_cPM3DEngine;
      CVector3 m_cVelocity;
      CVector3 m_cTotalForce;

   };

}

#endif
