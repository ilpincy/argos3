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

      struct SBoundingBox {
         CVector3 MinCorner;
         CVector3 MaxCorner;

         inline bool Intersects(const SBoundingBox& s_bb) const {
            return
               (MinCorner.GetX() < s_bb.MaxCorner.GetX()) && (MaxCorner.GetX() > s_bb.MinCorner.GetX()) &&
               (MinCorner.GetY() < s_bb.MaxCorner.GetY()) && (MaxCorner.GetY() > s_bb.MinCorner.GetY()) &&
               (MinCorner.GetZ() < s_bb.MaxCorner.GetZ()) && (MaxCorner.GetZ() > s_bb.MinCorner.GetZ());
         }
      };

   public:

      ENABLE_VTABLE();

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

      inline void SetPosition(const CVector3& c_position) {
         CPositionalEntity::SetPosition(c_position);
         m_bBoundingBoxRecalculationNeeded = true;
      }

      inline void SetOrientation(const CQuaternion& c_orientation) {
         CPositionalEntity::SetOrientation(c_orientation);
         m_bBoundingBoxRecalculationNeeded = true;
      }

      inline SBoundingBox& GetBoundingBox() {
         return m_sBoundingBox;
      }

      void AddPhysicsEngine(CPhysicsEngine& c_physics_engine);

      void RemovePhysicsEngine(CPhysicsEngine& c_physics_engine);

      CPhysicsEngine& GetPhysicsEngine(UInt32 un_index) const;

      UInt32 GetPhysicsEngineNum() const;

      virtual void AddPhysicsEngineEntity(const std::string& str_engine_id,
                                          CPhysicsEngineEntity& c_physics_entity);

      void RemovePhysicsEngineEntity(const std::string& str_engine_id);

      const CPhysicsEngineEntity& GetPhysicsEngineEntity(const std::string& str_engine_id) const;

      virtual bool CheckIntersectionWithRay(Real& f_distance,
                                            const CRay3& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Update();

      virtual std::string GetTypeDescription() const {
         return "body";
      }

   protected:

      virtual void CalculateBoundingBox() = 0;
      
   protected:
      
      bool m_bMovable;
      CPhysicsEngine::TVector m_tEngines;
      CPhysicsEngineEntity::TMap m_tPhysicsEngineEntityMap;
      CPhysicsEngineEntity::TVector m_tPhysicsEngineEntityVector;
      SBoundingBox m_sBoundingBox;
      bool m_bBoundingBoxRecalculationNeeded;

   };

   /****************************************/
   /****************************************/

   typedef std::vector<CEmbodiedEntity*> TEmbodiedEntityVector;
   typedef std::map<std::string, CEmbodiedEntity*> TEmbodiedEntityMap;
   typedef std::tr1::unordered_set<CEmbodiedEntity*> TEmbodiedEntitySet;

   /****************************************/
   /****************************************/

   void CalculateBoundingBoxFromHalfSize(CEmbodiedEntity::SBoundingBox& s_bounding_box,
                                         const CVector3& c_half_size,
                                         const CVector3& c_center_pos,
                                         const CRotationMatrix3& c_orientation);

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
