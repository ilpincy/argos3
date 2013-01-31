
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
      m_bMovable(true) {}

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
      m_bMovable(b_movable) {}

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CPositionalEntity::Init(t_tree);
         m_bBoundingBoxRecalculationNeeded = true;
         m_bMovable = true;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize embodied entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::AddPhysicsEngine(CPhysicsEngine& c_physics_engine) {
      m_tEngines.push_back(&c_physics_engine);
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::RemovePhysicsEngine(CPhysicsEngine& c_physics_engine) {
      CPhysicsEngine::TVector::iterator it = std::find(m_tEngines.begin(),
                                                       m_tEngines.end(),
                                                       &c_physics_engine);
      if(it == m_tEngines.end()) {
         THROW_ARGOSEXCEPTION("Engine \"" << c_physics_engine.GetId() << "\" not found when removing it from entity id = \"" << GetId() << "\"");
      }
      m_tEngines.erase(it);
   }

   /****************************************/
   /****************************************/

   CPhysicsEngine& CEmbodiedEntity::GetPhysicsEngine(UInt32 un_index) const {
      if(un_index >= m_tEngines.size()) {
         THROW_ARGOSEXCEPTION("Index out of bound for physics engine query for entity " << GetId() <<
                              ". Passed index = " << un_index << ", but " <<
                              m_tEngines.size() << " engines were associated to this entity.");
      }
      return *(m_tEngines[un_index]);
   }

   /****************************************/
   /****************************************/

   UInt32 CEmbodiedEntity::GetPhysicsEngineNum() const {
      return m_tEngines.size();
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::AddPhysicsEngineEntity(const std::string& str_engine_id,
                                                CPhysicsEngineEntity& c_physics_entity) {
      m_tPhysicsEngineEntityMap[str_engine_id] = &c_physics_entity;
      m_tPhysicsEngineEntityVector.push_back(&c_physics_entity);
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

   bool CEmbodiedEntity::CheckIntersectionWithRay(Real& f_distance,
                                                  const CRay& c_ray) const {
      for(UInt32 i = 0; i < m_tPhysicsEngineEntityVector.size(); ++i)
         if(m_tPhysicsEngineEntityVector[i]->CheckIntersectionWithRay(f_distance, c_ray)) {
            return true;
         }
      return false;
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

   void CEmbodiedEntity::Update() {
      if(m_bBoundingBoxRecalculationNeeded) {
         CalculateBoundingBox();
         m_bBoundingBoxRecalculationNeeded = false;
      }
   }

   /****************************************/
   /****************************************/

   void CalculateBoundingBoxFromHalfSize(CEmbodiedEntity::SBoundingBox& s_bounding_box,
                                         const CVector3& c_half_size,
                                         const CVector3& c_center_pos,
                                         const CRotationMatrix3& c_orientation) {
      CVector3 cExtent(Abs(c_orientation(0) * c_half_size.GetX()) +
                       Abs(c_orientation(1) * c_half_size.GetY()) +
                       Abs(c_orientation(2) * c_half_size.GetZ()),
                       Abs(c_orientation(3) * c_half_size.GetX()) +
                       Abs(c_orientation(4) * c_half_size.GetY()) +
                       Abs(c_orientation(5) * c_half_size.GetZ()),
                       Abs(c_orientation(6) * c_half_size.GetX()) +
                       Abs(c_orientation(7) * c_half_size.GetY()) +
                       Abs(c_orientation(8) * c_half_size.GetZ()));
      s_bounding_box.MinCorner =  c_center_pos;
      s_bounding_box.MinCorner -= cExtent;
      s_bounding_box.MaxCorner =  c_center_pos;
      s_bounding_box.MaxCorner += cExtent;
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CEmbodiedEntity>& c_space_hash,
                                                    CEmbodiedEntity& c_element) {
      /* Update the bounding box */
      c_element.Update();
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

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CEmbodiedEntity);

   /****************************************/
   /****************************************/

}
