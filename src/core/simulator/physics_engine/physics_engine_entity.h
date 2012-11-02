/**
 * @file argos3/core/simulator/physics_engine/physics_engine_entity.h
 *
 * @brief This file provides the definition of the physics engine entity interface.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef PHYSICS_ENGINE_ENTITY_H
#define PHYSICS_ENGINE_ENTITY_H

namespace argos {
   class CPhysicsEngineEntity;
   class CEmbodiedEntity;
   class CRay;
   class CVector3;
   class CQuaternion;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <map>
#include <vector>
#include <string>

namespace argos {

   class CPhysicsEngineEntity {

   public:

      typedef std::map<std::string, CPhysicsEngineEntity*> TMap;
      typedef std::vector<CPhysicsEngineEntity*> TVector;

   public:

      CPhysicsEngineEntity(CEmbodiedEntity& c_entity) :
         m_pcEmbodiedEntity(&c_entity) {}
      virtual ~CPhysicsEngineEntity() {}

      inline CEmbodiedEntity& GetEmbodiedEntity() const {
         return *m_pcEmbodiedEntity;
      }

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay& c_ray) const = 0;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) = 0;

   protected:

      CEmbodiedEntity* m_pcEmbodiedEntity;

   };

}

#endif
