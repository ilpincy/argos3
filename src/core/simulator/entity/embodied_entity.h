/**
 * @file <argos3/core/simulator/entity/embodied_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef EMBODIED_ENTITY_H
#define EMBODIED_ENTITY_H

namespace argos {
   class CEmbodiedEntity;
   class CRotationMatrix3;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/simulator/space/space_hash.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/physics_engine/physics_engine_entity.h>
#include <algorithm>

namespace argos {

   class CEmbodiedEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

   public:

      CEmbodiedEntity(CComposableEntity* pc_parent);

      CEmbodiedEntity(CComposableEntity* pc_parent,
                      const std::string& str_id,
                      const CVector3& c_position,
                      const CQuaternion& c_orientation,
                      bool b_movable);

      virtual ~CEmbodiedEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline bool IsMovable() const {
         return m_bMovable;
      }

      inline void SetMovable(bool b_movable) {
         m_bMovable = b_movable;
      }

      const SBoundingBox& GetBoundingBox() const;

      UInt32 GetPhysicsEngineEntitiesNum() const;

      virtual void AddPhysicsEngineEntity(const std::string& str_engine_id,
                                          CPhysicsEngineEntity& c_physics_entity);

      void RemovePhysicsEngineEntity(const std::string& str_engine_id);

      const CPhysicsEngineEntity& GetPhysicsEngineEntity(size_t un_idx) const;

      CPhysicsEngineEntity& GetPhysicsEngineEntity(size_t un_idx);

      const CPhysicsEngineEntity& GetPhysicsEngineEntity(const std::string& str_engine_id) const;

      CPhysicsEngineEntity& GetPhysicsEngineEntity(const std::string& str_engine_id);

      virtual bool CheckIntersectionWithRay(Real& f_distance,
                                            const CRay3& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual std::string GetTypeDescription() const {
         return "body";
      }

   protected:

      void CalculateBoundingBox();
      
   protected:
      
      bool m_bMovable;
      CPhysicsEngineEntity::TMap m_tPhysicsEngineEntityMap;
      CPhysicsEngineEntity::TVector m_tPhysicsEngineEntityVector;
      SBoundingBox* m_sBoundingBox;

   };

   /****************************************/
   /****************************************/

   typedef std::vector<CEmbodiedEntity*> TEmbodiedEntityVector;
   typedef std::map<std::string, CEmbodiedEntity*> TEmbodiedEntityMap;
   typedef std::tr1::unordered_set<CEmbodiedEntity*> TEmbodiedEntitySet;

   /****************************************/
   /****************************************/

   class CEmbodiedEntitySpaceHashUpdater : public CSpaceHashUpdater<CEmbodiedEntity> {
      
   public:
      
      virtual void operator()(CAbstractSpaceHash<CEmbodiedEntity>& c_space_hash,
                              CEmbodiedEntity& c_element);

   private:

      SInt32 m_nMinX, m_nMinY, m_nMinZ;
      SInt32 m_nMaxX, m_nMaxY, m_nMaxZ;

   };
   
   /****************************************/
   /****************************************/

}

#endif
