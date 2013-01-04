/**
 * @file simulator/entity/embodied_entity.h
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
#include <argos3/core/utility/math/ray.h>
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

      CEmbodiedEntity(CEntity* pc_parent) :
         CPositionalEntity(pc_parent),
         m_bCollisionDetected(false),
         m_unNumCollisions(0),
         m_pcCollidingEntity(NULL) {}
      virtual ~CEmbodiedEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      virtual void Update() {}

      inline virtual bool IsCollisionDetected() const {
         return m_bCollisionDetected;
      }

      inline virtual void SetCollisionDetected(CEmbodiedEntity& c_entity) {
         m_pcCollidingEntity = &c_entity;
         m_bCollisionDetected = true;
         ++m_unNumCollisions;
      }

      inline virtual void ClearCollisionDetected() {
         m_bCollisionDetected = false;
         m_pcCollidingEntity  = NULL;
      }

      inline bool IsMovable() const {
         return m_bMovable;
      }

      inline void SetMovable(bool b_movable) {
         m_bMovable = b_movable;
      }

      inline virtual UInt32 GetCollisionNumber() {
         return m_unNumCollisions;
      }

      inline virtual CEmbodiedEntity& GetCollidingEntity() {
         return *m_pcCollidingEntity;
      }

      inline virtual void SetPosition(const CVector3& c_position) {
         CPositionalEntity::SetPosition(c_position);
         m_bBoundingBoxRecalculationNeeded = true;
      }

      inline virtual void SetOrientation(const CQuaternion& c_orientation) {
         CPositionalEntity::SetOrientation(c_orientation);
         m_bBoundingBoxRecalculationNeeded = true;
      }

      inline virtual void UpdateBoundingBox() {
         if(m_bBoundingBoxRecalculationNeeded) {
            CalculateBoundingBox();
            m_bBoundingBoxRecalculationNeeded = false;
         }
      }

      inline SBoundingBox& GetBoundingBox() {
         return m_sBoundingBox;
      }

      inline virtual void AddPhysicsEngine(CPhysicsEngine& c_physics_engine) {
         m_tEngines.push_back(&c_physics_engine);
      }

      inline virtual void RemovePhysicsEngine(CPhysicsEngine& c_physics_engine) {
         CPhysicsEngine::TVector::iterator it = std::find(m_tEngines.begin(),
                                                          m_tEngines.end(),
                                                          &c_physics_engine);
         if(it == m_tEngines.end()) {
            THROW_ARGOSEXCEPTION("Engine \"" << c_physics_engine.GetId() << "\" not found when removing it from entity id = \"" << GetId() << "\"");
         }
         m_tEngines.erase(it);
      }

      inline virtual CPhysicsEngine& GetPhysicsEngine(UInt32 un_index) const {
         if(un_index >= m_tEngines.size()) {
            THROW_ARGOSEXCEPTION("Index out of bound for physics engine query for entity " << GetId() <<
                                 ". Passed index = " << un_index << ", but " <<
                                 m_tEngines.size() << " engines were associated to this entity.");
         }
         return *(m_tEngines[un_index]);
      }

      inline virtual UInt32 GetPhysicsEngineNum() const {
         return m_tEngines.size();
      }

      inline virtual void AddPhysicsEngineEntity(const std::string& str_engine_id,
                                                 CPhysicsEngineEntity& c_physics_entity) {
         m_tPhysicsEngineEntityMap[str_engine_id] = &c_physics_entity;
         m_tPhysicsEngineEntityVector.push_back(&c_physics_entity);
      }

      inline virtual void RemovePhysicsEngineEntity(const std::string& str_engine_id) {
         CPhysicsEngineEntity::TMap::iterator itMap = m_tPhysicsEngineEntityMap.find(str_engine_id);
         if(itMap == m_tPhysicsEngineEntityMap.end()) {
            THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no associated entity in physics engine " << str_engine_id);
         }
         CPhysicsEngineEntity::TVector::iterator itVec = std::find(m_tPhysicsEngineEntityVector.begin(),
                                                                  m_tPhysicsEngineEntityVector.end(),
                                                                  itMap->second);
         m_tPhysicsEngineEntityMap.erase(itMap);
         m_tPhysicsEngineEntityVector.erase(itVec);
      }

      inline virtual const CPhysicsEngineEntity& GetPhysicsEngineEntity(const std::string& str_engine_id) const {
         CPhysicsEngineEntity::TMap::const_iterator it = m_tPhysicsEngineEntityMap.find(str_engine_id);
         if(it == m_tPhysicsEngineEntityMap.end()) {
            THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no associated entity in physics engine \"" << str_engine_id << "\"");
         }
         return *(it->second);
      }

      virtual bool CheckIntersectionWithRay(Real& f_distance,
                                            const CRay& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

   protected:

      virtual void CalculateBoundingBox() = 0;
      
   protected:
      
      CPhysicsEngine::TVector m_tEngines;
      CPhysicsEngineEntity::TMap m_tPhysicsEngineEntityMap;
      CPhysicsEngineEntity::TVector m_tPhysicsEngineEntityVector;
      bool m_bCollisionDetected;
      UInt32 m_unNumCollisions;
      CEmbodiedEntity* m_pcCollidingEntity;
      SBoundingBox m_sBoundingBox;
      bool m_bBoundingBoxRecalculationNeeded;
      bool m_bMovable;

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
