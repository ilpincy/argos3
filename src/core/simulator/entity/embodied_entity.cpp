
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

   CEmbodiedEntity::~CEmbodiedEntity() {
      if(!m_bMovable && m_sBoundingBox != NULL) {
         delete m_sBoundingBox;
      }
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CPositionalEntity::Init(t_tree);
         m_bMovable = true;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize embodied entity \"" << GetContext() << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   const SBoundingBox& CEmbodiedEntity::GetBoundingBox() const {
      if(GetPhysicsModelsNum() == 0) {
         /* No engine associated to this entity */
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::GetBoundingBox() : entity \"" << GetContext() << GetId() << "\" is not associated to any engine");
      }
      return *m_sBoundingBox;
   }

   /****************************************/
   /****************************************/

   UInt32 CEmbodiedEntity::GetPhysicsModelsNum() const {
      return m_tPhysicsModelVector.size();
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::AddPhysicsModel(const std::string& str_engine_id,
                                         CPhysicsModel& c_physics_model) {
      if(m_bMovable && GetPhysicsModelsNum() > 0) {
         THROW_ARGOSEXCEPTION(GetContext() << GetId() << " is movable embodied entity and can't have more than 1 physics engine entity associated");
      }
      m_tPhysicsModelMap[str_engine_id] = &c_physics_model;
      m_tPhysicsModelVector.push_back(&c_physics_model);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::RemovePhysicsModel(const std::string& str_engine_id) {
      CPhysicsModel::TMap::iterator itMap = m_tPhysicsModelMap.find(str_engine_id);
      if(itMap == m_tPhysicsModelMap.end()) {
         THROW_ARGOSEXCEPTION("Entity \"" << GetContext() << GetId() << "\" has no associated entity in physics engine " << str_engine_id);
      }
      CPhysicsModel::TVector::iterator itVec = std::find(m_tPhysicsModelVector.begin(),
                                                         m_tPhysicsModelVector.end(),
                                                         itMap->second);
      m_tPhysicsModelMap.erase(itMap);
      m_tPhysicsModelVector.erase(itVec);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   const CPhysicsModel& CEmbodiedEntity::GetPhysicsModel(size_t un_idx) const {
      if(un_idx > m_tPhysicsModelVector.size()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::GetPhysicsModel: entity \"" << GetContext() << GetId() << "\": the passed index " << un_idx << " is out of bounds, the max allowed is " << m_tPhysicsModelVector.size());
      }
      return *m_tPhysicsModelVector[un_idx];
   }

   /****************************************/
   /****************************************/

   CPhysicsModel& CEmbodiedEntity::GetPhysicsModel(size_t un_idx) {
      if(un_idx > m_tPhysicsModelVector.size()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::GetPhysicsModel: entity \"" << GetContext() << GetId() << "\": the passed index " << un_idx << " is out of bounds, the max allowed is " << m_tPhysicsModelVector.size());
      }
      return *m_tPhysicsModelVector[un_idx];
   }

   /****************************************/
   /****************************************/

   const CPhysicsModel& CEmbodiedEntity::GetPhysicsModel(const std::string& str_engine_id) const {
      CPhysicsModel::TMap::const_iterator it = m_tPhysicsModelMap.find(str_engine_id);
      if(it == m_tPhysicsModelMap.end()) {
         THROW_ARGOSEXCEPTION("Entity \"" << GetContext() << GetId() << "\" has no associated entity in physics engine \"" << str_engine_id << "\"");
      }
      return *(it->second);
   }

   /****************************************/
   /****************************************/

   CPhysicsModel& CEmbodiedEntity::GetPhysicsModel(const std::string& str_engine_id) {
      CPhysicsModel::TMap::iterator it = m_tPhysicsModelMap.find(str_engine_id);
      if(it == m_tPhysicsModelMap.end()) {
         THROW_ARGOSEXCEPTION("Entity \"" << GetContext() << GetId() << "\" has no associated entity in physics engine \"" << str_engine_id << "\"");
      }
      return *(it->second);
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                  const CRay3& c_ray) const {
      /* If no model is associated, you can't call this function */
      if(m_tPhysicsModelVector.empty()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::CheckIntersectionWithRay() called on entity \"" << GetContext() << GetId() << "\", but this entity has not been added to any physics engine.");
      }
      /* Special case: if there is only one model, check that directly */
      if(m_tPhysicsModelVector.size() == 1) {
         return m_tPhysicsModelVector[0]->CheckIntersectionWithRay(f_t_on_ray, c_ray);
      }
      /* Multiple associations, go through them and find the closest intersection point */
      else {
         /* Search for the first match */
         UInt32 i = 0;
         bool bFoundFirst = false;
         while(!bFoundFirst && i < m_tPhysicsModelVector.size()) {
            if(m_tPhysicsModelVector[i]->CheckIntersectionWithRay(f_t_on_ray, c_ray)) {
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
            for(size_t j = i+1; j < m_tPhysicsModelVector.size(); ++j) {
               if(m_tPhysicsModelVector[j]->CheckIntersectionWithRay(fTOnRay, c_ray) &&
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
      for(CPhysicsModel::TVector::const_iterator it = m_tPhysicsModelVector.begin();
          it != m_tPhysicsModelVector.end() && bNoCollision; ++it) {
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
         for(CPhysicsModel::TVector::const_iterator it = m_tPhysicsModelVector.begin();
             it != m_tPhysicsModelVector.end(); ++it) {
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

#define CHECK_CORNER(MINMAX, COORD, OP)                                 \
   if(m_sBoundingBox->MINMAX ## Corner.Get ## COORD() OP sBBox.MINMAX ## Corner.Get ## COORD()) { \
      m_sBoundingBox->MINMAX ## Corner.Set ## COORD(sBBox.MINMAX ## Corner.Get ## COORD()); \
   }

   void CEmbodiedEntity::CalculateBoundingBox() {
      if(GetPhysicsModelsNum() > 0) {
         /*
          * There is at least one physics engine entity associated
          */
         if(m_bMovable) {
            /* The bounding box points directly to the associated model bounding box */
            m_sBoundingBox = &m_tPhysicsModelVector[0]->GetBoundingBox();
         }
         else {
            /* The bounding box is obtained taking the extrema of all the bboxes of all the engines */
            if(m_sBoundingBox == NULL) {
               m_sBoundingBox = new SBoundingBox();
            }
            *m_sBoundingBox = m_tPhysicsModelVector[0]->GetBoundingBox();
            for(size_t i = 1; i < GetPhysicsModelsNum(); ++i) {
               const SBoundingBox& sBBox = m_tPhysicsModelVector[0]->GetBoundingBox();
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
      if(m_tPhysicsModelVector.empty()) {
         THROW_ARGOSEXCEPTION("CEmbodiedEntity::IsCollidingWithSomething() called on entity \"" <<
                              GetContext() << GetId() <<
                              "\", but this entity has not been added to any physics engine.");
      }
      /* Special case: if there is only one model, check that directly */
      if(m_tPhysicsModelVector.size() == 1) {
         return m_tPhysicsModelVector[0]->IsCollidingWithSomething();
      }
      /* Multiple associations, go through them */
      else {
         /* Return true at the first detected collision */
         for(size_t i = 0; i < m_tPhysicsModelVector.size(); ++i) {
            if(m_tPhysicsModelVector[i]->IsCollidingWithSomething()) {
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

   CEmbodiedEntityGridUpdater::CEmbodiedEntityGridUpdater(CGrid<CEmbodiedEntity>& c_grid) :
      m_cGrid(c_grid) {}

   bool CEmbodiedEntityGridUpdater::operator()(CEmbodiedEntity& c_entity) {
      try {
         /* Get cell of bb min corner, clamping it if is out of bounds */
         m_cGrid.PositionToCell(m_nMinI, m_nMinJ, m_nMinK, c_entity.GetBoundingBox().MinCorner);
         m_cGrid.ClampCoordinates(m_nMinI, m_nMinJ, m_nMinK);
         /* Get cell of bb max corner, clamping it if is out of bounds */
         m_cGrid.PositionToCell(m_nMaxI, m_nMaxJ, m_nMaxK, c_entity.GetBoundingBox().MaxCorner);
         m_cGrid.ClampCoordinates(m_nMaxI, m_nMaxJ, m_nMaxK);
         /* Go through cells */
         for(SInt32 m_nK = m_nMinK; m_nK <= m_nMaxK; ++m_nK) {
            for(SInt32 m_nJ = m_nMinJ; m_nJ <= m_nMaxJ; ++m_nJ) {
               for(SInt32 m_nI = m_nMinI; m_nI <= m_nMaxI; ++m_nI) {
                  m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
               }
            }
         }
         /* Continue with the other entities */
         return true;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("While updating the embodied entity grid for embodied entity \"" << c_entity.GetContext() << c_entity.GetId() << "\"", ex);
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
         c_space.GetEmbodiedEntityIndex().AddEntity(c_entity);
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
         try {
            while(c_entity.GetPhysicsModelsNum() > 0) {
               c_entity.GetPhysicsModel(0).GetEngine().RemoveEntity(*pcRoot);
            }
         }
         catch(CARGoSException& ex) {
            /*
             * It is safe to ignore errors because they happen only when an entity
             * is completely removed from the space. In this case, the body is
             * first removed from the composable entity, and then the embodied entity
             * is asked to clear up the physics models. In turn, this last operation
             * searches for the body component, which is not there anymore.
             *
             * It is anyway useful to search for the body component because, when robots
             * are transferred from an engine to another, only the physics model is to be
             * removed.
             */
         }
         /* Remove entity from space */
         c_space.GetEmbodiedEntityIndex().RemoveEntity(c_entity);
         c_space.RemoveEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity, CSpaceOperationRemoveEmbodiedEntity, CEmbodiedEntity);
   /**
    * @endcond
    */

   /****************************************/
   /****************************************/

   class CCheckEmbodiedEntitiesForIntersection : public CPositionalIndex<CEmbodiedEntity>::COperation {

   public:

      CCheckEmbodiedEntitiesForIntersection(const CRay3& c_ray) :
         m_fCurTOnRay(2.0f),
         m_fMinTOnRay(2.0f),
         m_cRay(c_ray),
         m_pcClosestEmbodiedEntity(NULL) {}

      Real GetTOnRay() const {
         return m_fMinTOnRay;
      }

      CEmbodiedEntity* GetClosestEmbodiedEntity() {
         return m_pcClosestEmbodiedEntity;
      }

      virtual bool operator()(CEmbodiedEntity& c_entity) {
         if(c_entity.CheckIntersectionWithRay(m_fCurTOnRay, m_cRay) &&
            (m_fCurTOnRay < m_fMinTOnRay)) {
            m_fMinTOnRay = m_fCurTOnRay;
            m_pcClosestEmbodiedEntity = &c_entity;
         }
         /* Continue with the other matches */
         return true;
      }

   protected:

      Real m_fCurTOnRay;
      Real m_fMinTOnRay;
      CRay3 m_cRay;
      CEmbodiedEntity* m_pcClosestEmbodiedEntity;
   };

   /****************************************/
   /****************************************/

   class CCheckEmbodiedEntitiesForIntersectionWIgnore : public CCheckEmbodiedEntitiesForIntersection {

   public:

      CCheckEmbodiedEntitiesForIntersectionWIgnore(const CRay3& c_ray,
                                                   CEmbodiedEntity& c_entity) :
         CCheckEmbodiedEntitiesForIntersection(c_ray),
         m_pcIgnoredEmbodiedEntity(&c_entity) {}

      virtual bool operator()(CEmbodiedEntity& c_entity) {
         if((&c_entity != m_pcIgnoredEmbodiedEntity) &&
            c_entity.CheckIntersectionWithRay(m_fCurTOnRay, m_cRay) &&
            (m_fCurTOnRay < m_fMinTOnRay)) {
            m_fMinTOnRay = m_fCurTOnRay;
            m_pcClosestEmbodiedEntity = &c_entity;
         }
         /* Continue with the other matches */
         return true;
      }

   protected:

      CEmbodiedEntity* m_pcIgnoredEmbodiedEntity;
   };

   /****************************************/
   /****************************************/

   bool GetClosestEmbodiedEntityIntersectedByRay(SEmbodiedEntityIntersectionItem& s_item,
                                                 CPositionalIndex<CEmbodiedEntity>& c_pos_index,
                                                 const CRay3& c_ray) {
      CCheckEmbodiedEntitiesForIntersection cOp(c_ray);
      c_pos_index.ForEntitiesAlongRay(c_ray, cOp, false);
      s_item.IntersectedEntity = cOp.GetClosestEmbodiedEntity();
      s_item.TOnRay = cOp.GetTOnRay();
      return (s_item.IntersectedEntity != NULL);
   }

   /****************************************/
   /****************************************/

   bool GetClosestEmbodiedEntityIntersectedByRay(SEmbodiedEntityIntersectionItem& s_item,
                                                 CPositionalIndex<CEmbodiedEntity>& c_pos_index,
                                                 const CRay3& c_ray,
                                                 CEmbodiedEntity& c_entity) {
      CCheckEmbodiedEntitiesForIntersectionWIgnore cOp(c_ray, c_entity);
      c_pos_index.ForEntitiesAlongRay(c_ray, cOp, false);
      s_item.IntersectedEntity = cOp.GetClosestEmbodiedEntity();
      s_item.TOnRay = cOp.GetTOnRay();
      return (s_item.IntersectedEntity != NULL);
   }

   /****************************************/
   /****************************************/

}
