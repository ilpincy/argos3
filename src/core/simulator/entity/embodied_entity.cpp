
/**
 * @file <argos3/core/simulator/entity/embodied_entity.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "embodied_entity.h"
#include "composable_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/math/matrix/rotationmatrix3.h>

namespace argos {

   /****************************************/
   /****************************************/

   CEmbodiedEntity::CEmbodiedEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_bMovable(true),
      m_sBoundingBox(NULL) {}

   /****************************************/
   /****************************************/

   CEmbodiedEntity::CEmbodiedEntity(CComposableEntity* pc_parent,
                                    const std::string& str_id,
                                    const CVector3& c_position,
                                    const CQuaternion& c_orientation,
                                    bool b_movable) :
      CPositionalEntity(pc_parent,
                        str_id,
                        c_position,
                        c_orientation),
      m_bMovable(b_movable),
      m_sBoundingBox(NULL) {}

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CPositionalEntity::Init(t_tree);
         m_bMovable = true;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize embodied entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   const SBoundingBox& CEmbodiedEntity::GetBoundingBox() const {
      if(GetPhysicsEngineEntitiesNum() == 0) {
         /* No engine associated to this entity */
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::GetBoundingBox() : entity \"" << GetId() << "\" is not associated to any engine");
      }
      return *m_sBoundingBox;
   }

   /****************************************/
   /****************************************/

   UInt32 CEmbodiedEntity::GetPhysicsEngineEntitiesNum() const {
      return m_tPhysicsEngineEntityVector.size();
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::AddPhysicsEngineEntity(const std::string& str_engine_id,
                                                CPhysicsEngineEntity& c_physics_entity) {
      if(m_bMovable && GetPhysicsEngineEntitiesNum() > 0) {
         THROW_ARGOSEXCEPTION(GetId() << " is movable embodied entity and can't have more than 1 physics engine entity associated");
      }
      m_tPhysicsEngineEntityMap[str_engine_id] = &c_physics_entity;
      m_tPhysicsEngineEntityVector.push_back(&c_physics_entity);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::RemovePhysicsEngineEntity(const std::string& str_engine_id) {
      CPhysicsEngineEntity::TMap::iterator itMap = m_tPhysicsEngineEntityMap.find(str_engine_id);
      if(itMap == m_tPhysicsEngineEntityMap.end()) {
         THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no associated entity in physics engine " << str_engine_id);
      }
      CPhysicsEngineEntity::TVector::iterator itVec = std::find(m_tPhysicsEngineEntityVector.begin(),
                                                                m_tPhysicsEngineEntityVector.end(),
                                                                itMap->second);
      m_tPhysicsEngineEntityMap.erase(itMap);
      m_tPhysicsEngineEntityVector.erase(itVec);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   const CPhysicsEngineEntity& CEmbodiedEntity::GetPhysicsEngineEntity(size_t un_idx) const {
      if(un_idx > m_tPhysicsEngineEntityVector.size()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::GetPhysicsEngineEntity: entity \"" << GetId() << "\": the passed index " << un_idx << " is out of bounds, the max allowed is " << m_tPhysicsEngineEntityVector.size());
      }
      return *m_tPhysicsEngineEntityVector[un_idx];
   }

   /****************************************/
   /****************************************/

   CPhysicsEngineEntity& CEmbodiedEntity::GetPhysicsEngineEntity(size_t un_idx) {
      if(un_idx > m_tPhysicsEngineEntityVector.size()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::GetPhysicsEngineEntity: entity \"" << GetId() << "\": the passed index " << un_idx << " is out of bounds, the max allowed is " << m_tPhysicsEngineEntityVector.size());
      }
      return *m_tPhysicsEngineEntityVector[un_idx];
   }

   /****************************************/
   /****************************************/

   const CPhysicsEngineEntity& CEmbodiedEntity::GetPhysicsEngineEntity(const std::string& str_engine_id) const {
      CPhysicsEngineEntity::TMap::const_iterator it = m_tPhysicsEngineEntityMap.find(str_engine_id);
      if(it == m_tPhysicsEngineEntityMap.end()) {
         THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no associated entity in physics engine \"" << str_engine_id << "\"");
      }
      return *(it->second);
   }

   /****************************************/
   /****************************************/

   CPhysicsEngineEntity& CEmbodiedEntity::GetPhysicsEngineEntity(const std::string& str_engine_id) {
      CPhysicsEngineEntity::TMap::iterator it = m_tPhysicsEngineEntityMap.find(str_engine_id);
      if(it == m_tPhysicsEngineEntityMap.end()) {
         THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no associated entity in physics engine \"" << str_engine_id << "\"");
      }
      return *(it->second);
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                  const CRay3& c_ray) const {
      /* If no model is associated, you can't call this function */
      if(m_tPhysicsEngineEntityVector.empty()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::CheckIntersectionWithRay() called on entity \"" << GetId() << "\", but this entity has not been added to any physics engine.");
      }
      /* Special case: if there is only one model, check that directly */
      if(m_tPhysicsEngineEntityVector.size() == 1) {
         return m_tPhysicsEngineEntityVector[0]->CheckIntersectionWithRay(f_t_on_ray, c_ray);
      }
      /* Multiple associations, go through them and find the closest intersection point */
      else {
         /* Search for the first match */
         UInt32 i = 0;
         bool bFoundFirst = false;
         while(!bFoundFirst && i < m_tPhysicsEngineEntityVector.size()) {
            if(m_tPhysicsEngineEntityVector[i]->CheckIntersectionWithRay(f_t_on_ray, c_ray)) {
               bFoundFirst = true;
            }
            else {
               ++i;
            }
         }
         /* Did we find an intersection? */
         if(!bFoundFirst) {
            /* No, return false */
            return false;
         }
         else {
            /*
             * Yes, we did
             * Now, go through the remaining models and check if there is a closer match
             */
            Real fTOnRay;
            for(size_t j = i+1; j < m_tPhysicsEngineEntityVector.size(); ++j) {
               if(m_tPhysicsEngineEntityVector[j]->CheckIntersectionWithRay(fTOnRay, c_ray) &&
                  fTOnRay < f_t_on_ray) {
                  f_t_on_ray = fTOnRay;
               }
            }
            return true;
         }
      }
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::MoveTo(const CVector3& c_position,
                                const CQuaternion& c_orientation,
                                bool b_check_only) {
      bool bNoCollision = true;
      for(CPhysicsEngineEntity::TVector::const_iterator it = m_tPhysicsEngineEntityVector.begin();
          it != m_tPhysicsEngineEntityVector.end() && bNoCollision; ++it) {
         if(! (*it)->MoveTo(c_position, c_orientation, b_check_only)) {
            bNoCollision = false;
         }
      }
      if(bNoCollision && !b_check_only) {
         /* Update space position */
         SetPosition(c_position);
         SetOrientation(c_orientation);
         if( HasParent() ) {
            CComposableEntity* pcEntity = dynamic_cast<CComposableEntity*>(&GetParent());
            if( pcEntity != NULL ) {
               pcEntity->Update();
            }
         }
         return true;
      }
      else {
         /* No Collision or check only, undo changes */
         for(CPhysicsEngineEntity::TVector::const_iterator it = m_tPhysicsEngineEntityVector.begin();
             it != m_tPhysicsEngineEntityVector.end(); ++it) {
            (*it)->MoveTo(GetPosition(), GetOrientation());
         }
         if(!bNoCollision) {
            /* Collision */
            return false;
         }
         else {
            /* No collision, it was a check-only */
            return true;
         }
      }
   }

   /****************************************/
   /****************************************/

#define CHECK_CORNER(MINMAX, COORD, OP)                                    \
   if(m_sBoundingBox->MINMAX ## Corner.Get ## COORD() OP sBBox.MINMAX ## Corner.Get ## COORD()) { \
      m_sBoundingBox->MINMAX ## Corner.Set ## COORD(sBBox.MINMAX ## Corner.Get ## COORD());      \
   }

   void CEmbodiedEntity::CalculateBoundingBox() {
      if(GetPhysicsEngineEntitiesNum() > 0) {
         /*
          * There is at least one physics engine entity associated
          */
         if(m_bMovable) {
            /* The bounding box points directly to the associated model bounding box */
            m_sBoundingBox = &m_tPhysicsEngineEntityVector[0]->GetBoundingBox();
         }
         else {
            /* The bounding box is obtained taking the extrema of all the bboxes of all the engines */
            if(m_sBoundingBox == NULL) {
               m_sBoundingBox = new SBoundingBox();
            }
            *m_sBoundingBox = m_tPhysicsEngineEntityVector[0]->GetBoundingBox();
            for(size_t i = 1; i < GetPhysicsEngineEntitiesNum(); ++i) {
               const SBoundingBox& sBBox = m_tPhysicsEngineEntityVector[0]->GetBoundingBox();
               CHECK_CORNER(Min, X, >);
               CHECK_CORNER(Min, Y, >);
               CHECK_CORNER(Min, Z, >);
               CHECK_CORNER(Max, X, <);
               CHECK_CORNER(Max, Y, <);
               CHECK_CORNER(Max, Z, <);
            }
         }
      }
      else {
         /*
          * No physics engine entity associated
          */
         if(! m_bMovable && m_sBoundingBox != NULL) {
            /* A non-movable entity has its own bounding box, delete it */
            delete m_sBoundingBox;
         }
         m_sBoundingBox = NULL;
      }
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::IsCollidingWithSomething() const {
      /* If no model is associated, you can't call this function */
      if(m_tPhysicsEngineEntityVector.empty()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::IsCollidingWithSomething() called on entity \"" << GetId() << "\", but this entity has not been added to any physics engine.");
      }
      /* Special case: if there is only one model, check that directly */
      if(m_tPhysicsEngineEntityVector.size() == 1) {
         return m_tPhysicsEngineEntityVector[0]->IsCollidingWithSomething();
      }
      /* Multiple associations, go through them */
      else {
         /* Return true at the first detected collision */
         for(size_t i = 0; i < m_tPhysicsEngineEntityVector.size(); ++i) {
            if(m_tPhysicsEngineEntityVector[i]->IsCollidingWithSomething()) {
               return true;
            }
         }
         /* If you get here it's because there are collisions */
         return false;
      }
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CEmbodiedEntity>& c_space_hash,
                                                    CEmbodiedEntity& c_element) {
      /* Translate the min corner of the bounding box into the map's coordinate */
      c_space_hash.SpaceToHashTable(m_nMinX, m_nMinY, m_nMinZ, c_element.GetBoundingBox().MinCorner);
      /* Translate the max corner of the bounding box into the map's coordinate */                                     
      c_space_hash.SpaceToHashTable(m_nMaxX, m_nMaxY, m_nMaxZ, c_element.GetBoundingBox().MaxCorner);
      /* Finally, go through the affected cells and update them */
      for(SInt32 nK = m_nMinZ; nK <= m_nMaxZ; ++nK) {
         for(SInt32 nJ = m_nMinY; nJ <= m_nMaxY; ++nJ) {
            for(SInt32 nI = m_nMinX; nI <= m_nMaxX; ++nI) {
               c_space_hash.UpdateCell(nI, nJ, nK, c_element);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   /**
    * @cond HIDDEN_SYMBOLS
    */
   class CSpaceOperationAddEmbodiedEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CEmbodiedEntity& c_entity) {
         /* Add entity to space */
         c_space.AddEntity(c_entity);
         if(c_space.IsUsingSpaceHash()) {
            c_space.GetEmbodiedEntitiesSpaceHash().AddElement(c_entity);
         }
         /* Try to add entity to physics engine(s) */
         c_space.AddEntityToPhysicsEngine(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity, CSpaceOperationAddEmbodiedEntity, CEmbodiedEntity);
   
   class CSpaceOperationRemoveEmbodiedEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CEmbodiedEntity& c_entity) {
         /* Get a reference to the root entity */
         CEntity* pcRoot = &c_entity;
         while(pcRoot->HasParent()) {
            pcRoot = &pcRoot->GetParent();
         }
         /* Remove entity from all physics engines */
         while(c_entity.GetPhysicsEngineEntitiesNum() > 0) {
            c_entity.GetPhysicsEngineEntity(0).GetEngine().RemoveEntity(*pcRoot);
         }
         /* Remove entity from space */
         if(c_space.IsUsingSpaceHash()) {
            c_space.GetEmbodiedEntitiesSpaceHash().RemoveElement(c_entity);
         }
         c_space.RemoveEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity, CSpaceOperationRemoveEmbodiedEntity, CEmbodiedEntity);
   /**
    * @endcond
    */

   /****************************************/
   /****************************************/

}
