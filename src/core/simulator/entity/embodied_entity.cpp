
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
      CEntity(pc_parent),
      m_bMovable(true),
      m_sBoundingBox(NULL),
      m_psOriginAnchor(NULL) {}

   /****************************************/
   /****************************************/

   CEmbodiedEntity::CEmbodiedEntity(CComposableEntity* pc_parent,
                                    const std::string& str_id,
                                    const CVector3& c_position,
                                    const CQuaternion& c_orientation,
                                    bool b_movable) :
      CEntity(pc_parent, str_id),
      m_bMovable(b_movable),
      m_sBoundingBox(NULL),
      m_psOriginAnchor(new SAnchor(*this,
                                   "origin",
                                   0,
                                   CVector3(),
                                   CQuaternion(),
                                   c_position,
                                   c_orientation)),
      m_cInitOriginPosition(c_position),
      m_cInitOriginOrientation(c_orientation) {
      /* Add anchor to map and enable it */
      m_mapAnchors[m_psOriginAnchor->Id] = m_psOriginAnchor;
      EnableAnchor("origin");
   }

   /****************************************/
   /****************************************/

   CEmbodiedEntity::~CEmbodiedEntity() {
      if(!m_bMovable && m_sBoundingBox != NULL) {
         delete m_sBoundingBox;
      }
      for(std::map<std::string, SAnchor*>::iterator it = m_mapAnchors.begin();
          it != m_mapAnchors.end(); ++it) {
         /* it->second points to the current anchor */
         delete it->second;
      }
      m_mapAnchors.clear();
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize base entity */
         CEntity::Init(t_tree);
         /* Get the position of the entity */
         GetNodeAttributeOrDefault(t_tree, "position", m_cInitOriginPosition, CVector3());
         /* Get the orientation of the entity */
         GetNodeAttributeOrDefault(t_tree, "orientation", m_cInitOriginOrientation, CQuaternion());
         /* Create origin anchor */
         m_psOriginAnchor = new SAnchor(*this,
                                        "origin",
                                        0,
                                        CVector3(),
                                        CQuaternion(),
                                        m_cInitOriginPosition,
                                        m_cInitOriginOrientation);
         /* Add anchor to map and enable it */
         m_mapAnchors[m_psOriginAnchor->Id] = m_psOriginAnchor;
         EnableAnchor("origin");
         /* Embodied entities are movable by default */
         m_bMovable = true;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize embodied entity \"" << GetContext() << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Reset() {
      /* Reset origin anchor first */
      m_psOriginAnchor->Position = m_cInitOriginPosition;
      m_psOriginAnchor->Orientation = m_cInitOriginOrientation;
      /* Reset other anchors */
      SAnchor* psAnchor;
      for(std::map<std::string, SAnchor*>::iterator it = m_mapAnchors.begin();
          it != m_mapAnchors.end(); ++it) {
         /* it->second points to the current anchor */
         psAnchor = it->second;
         if(psAnchor->Index > 0) {
            /* Calculate global position and orientation */
            psAnchor->Position = psAnchor->OffsetPosition;
            psAnchor->Position.Rotate(m_cInitOriginOrientation);
            psAnchor->Position += m_cInitOriginPosition;
            psAnchor->Orientation = m_cInitOriginOrientation * psAnchor->OffsetOrientation;
         }
      }
   }

   /****************************************/
   /****************************************/

   SAnchor& CEmbodiedEntity::AddAnchor(const std::string& str_id,
                                       const CVector3& c_offset_position,
                                       const CQuaternion& c_offset_orientation) {
      /* Make sure the anchor id is unique */
      if(m_mapAnchors.count(str_id) > 0 ) {
         THROW_ARGOSEXCEPTION("Embodied entity \"" << GetContext() + GetId() << "\" already has an anchor with id " << str_id);
      }
      /* Calculate anchor position */
      CVector3 cPos = c_offset_position;
      cPos.Rotate(m_psOriginAnchor->Orientation);
      cPos += m_psOriginAnchor->Position;
      /* Calculate anchor orientation */
      CQuaternion cOrient = m_psOriginAnchor->Orientation * c_offset_orientation;
      /* Create anchor */
      SAnchor* psAnchor = new SAnchor(*this,
                                      str_id,
                                      m_mapAnchors.size(),
                                      c_offset_position,
                                      c_offset_orientation,
                                      cPos,
                                      cOrient);
      /* Add anchor to map */
      m_mapAnchors[str_id] = psAnchor;
      return *psAnchor;
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::EnableAnchor(const std::string& str_id) {
      /* Lookup the anchor id */
      std::map<std::string, SAnchor*>::iterator it = m_mapAnchors.find(str_id);
      /* Found? */
      if(it == m_mapAnchors.end()) {
         THROW_ARGOSEXCEPTION("Embodied entity \"" << GetContext() + GetId() << "\" has no anchor with id " << str_id);
      }
      /* Now it->second points to the requested anchor */
      /* Increase the in-use count */
      ++(it->second->InUseCount);
      /* Add to vector of enabled anchors if necessary */
      if(it->second->InUseCount == 1) {
         m_vecEnabledAnchors.push_back(it->second);
      }
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::DisableAnchor(const std::string& str_id) {
      /* Cannot disable the origin anchor */
      if(str_id == "origin") return;
      /* Lookup the anchor id */
      std::vector<SAnchor*>::iterator it = std::find(m_vecEnabledAnchors.begin(),
                                                     m_vecEnabledAnchors.end(),
                                                     str_id);
      /* Found? */
      if(it == m_vecEnabledAnchors.end()) return;
      /* Now *it points to the requested anchor */
      /* Decrease the in-use count */
      --((*it)->InUseCount);
      /* Remove from vector of enabled anchors if necessary */
      if((*it)->InUseCount == 0) {
         m_vecEnabledAnchors.erase(it);
      }
   }

   /****************************************/
   /****************************************/

   const SAnchor& CEmbodiedEntity::GetAnchor(const std::string& str_id) const {
      /* Lookup the anchor id */
      std::map<std::string, SAnchor*>::const_iterator it = m_mapAnchors.find(str_id);
      /* Found? */
      if(it == m_mapAnchors.end()) {
         THROW_ARGOSEXCEPTION("Embodied entity \"" << GetContext() + GetId() << "\" has no anchor with id " << str_id);
      }
      /* Now it->second points to the requested anchor */
      return *(it->second);
   }

   /****************************************/
   /****************************************/

   SAnchor& CEmbodiedEntity::GetAnchor(const std::string& str_id) {
      /* Lookup the anchor id */
      std::map<std::string, SAnchor*>::iterator it = m_mapAnchors.find(str_id);
      /* Found? */
      if(it == m_mapAnchors.end()) {
         THROW_ARGOSEXCEPTION("Embodied entity \"" << GetContext() + GetId() << "\" has no anchor with id " << str_id);
      }
      /* Now it->second points to the requested anchor */
      return *(it->second);
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::IsAnchorEnabled(const std::string& str_id) {
      /* Lookup the anchor id */
      std::map<std::string, SAnchor*>::const_iterator it = m_mapAnchors.find(str_id);
      /* Found? */
      if(it == m_mapAnchors.end()) {
         THROW_ARGOSEXCEPTION("Embodied entity \"" << GetContext() + GetId() << "\" has no anchor with id " << str_id);
      }
      /* Now it->second points to the requested anchor */
      return (it->second->InUseCount > 0);
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
         THROW_ARGOSEXCEPTION(GetContext() << GetId() << " is a movable embodied entity and can't have more than 1 physics engine entity associated");
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

   bool CEmbodiedEntity::MoveTo(const CVector3& c_position,
                                const CQuaternion& c_orientation,
                                bool b_check_only) {
      /* Can't move an entity with no model associated */
      if(GetPhysicsModelsNum() == 0) return false;
      /* Save current position and orientation */
      CVector3    cOriginalPosition    = m_psOriginAnchor->Position;
      CQuaternion cOriginalOrientation = m_psOriginAnchor->Orientation;
      /* Treat specially the case of movable entity */
      if(m_bMovable) {
         /* Move entity and check for collisions */
         m_tPhysicsModelVector[0]->MoveTo(c_position, c_orientation);
         bool bNoCollision = ! m_tPhysicsModelVector[0]->IsCollidingWithSomething();
         /* Depending on the presence of collisions... */
         if(bNoCollision && !b_check_only) {
            /* No collision and not a simple check */
            /* Tell the caller that we managed to move the entity */
            return true;
         }
         else {
            /* Collision or just a check, undo changes */
            m_tPhysicsModelVector[0]->MoveTo(cOriginalPosition, cOriginalOrientation);
            /* Tell the caller about collisions */
            return bNoCollision;
         }
      }
      else {
         /* The entity is not movable, go through all the models */
         size_t i;
         bool bNoCollision = true;
         for(i = 0; i < m_tPhysicsModelVector.size() && bNoCollision; ++i) {
            m_tPhysicsModelVector[i]->MoveTo(c_position, c_orientation);
            bNoCollision = !m_tPhysicsModelVector[i]->IsCollidingWithSomething();
         }
         if(bNoCollision && !b_check_only) {
            /* No collision and not a simple check */
            CalculateBoundingBox();
            /* Tell the caller that we managed to move the entity */
            return true;
         }
         else {
            /* No collision or just a check, undo changes */
            for(size_t j = 0; j < i; ++j) {
               m_tPhysicsModelVector[j]->MoveTo(cOriginalPosition, cOriginalOrientation);
            }
            /* Tell the caller about collisions */
            return bNoCollision;
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

   bool operator==(const SAnchor* ps_anchor,
                   const std::string& str_id) {
      return (ps_anchor->Id == str_id);
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
