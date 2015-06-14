#include "physics_model.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   SAnchor::SAnchor(const std::string& str_id,
                    UInt32 un_index,
                    const CVector3& c_offset_position,
                    const CQuaternion& c_offset_orientation,
                    const CVector3& c_position,
                    const CQuaternion& c_orientation) :
      Id(str_id),
      Index(un_index),
      OffsetPosition(c_offset_position),
      OffsetOrientation(c_offset_orientation),
      Position(c_position),
      Orientation(c_orientation),
      InUseCount(0) {
   }

   /****************************************/
   /****************************************/

   CPhysicsModel::CPhysicsModel(CPhysicsEngine& c_engine,
                                CEmbodiedEntity& c_entity) :
      m_cEngine(c_engine),
      m_cEmbodiedEntity(c_entity),
      m_sBoundingBox(),
      m_vecAnchorMethodHolders(c_entity.GetAnchors().size(), NULL),
      m_vecThunks(c_entity.GetAnchors().size(), NULL) {}

   /****************************************/
   /****************************************/

   void CPhysicsModel::UpdateEntityStatus() {
      CalculateBoundingBox();
      CalculateAnchors();
      /*
       * Update entity components
       */
      /* Get a reference to the root entity */
      /* NOTE: here the cast is static because we know that an embodied entity MUST have a parent
       * which, by definition, is a composable entity */
      CComposableEntity& cRoot = static_cast<CComposableEntity&>(m_cEmbodiedEntity.GetRootEntity());
      /* Update its components */
      cRoot.UpdateComponents();
      /*
       * Check whether a transfer is necessary
       */
      CPhysicsEngine* pcEngine = m_cEngine.CalculateTransfer(GetEmbodiedEntity().GetOriginAnchor().Position);
      if(!pcEngine) {
         THROW_ARGOSEXCEPTION("Can't transfer entity \"" << cRoot.GetId() << "\" from engine \"" << m_cEngine.GetId() << "\" to position " << GetEmbodiedEntity().GetOriginAnchor().Position << ". It is out of this engine's boundaries, and no other engine contains that point.");
      }
      else if(pcEngine != &m_cEngine) {
         m_cEngine.ScheduleEntityForTransfer(*pcEngine, cRoot);
      }
   }

   /****************************************/
   /****************************************/

   void CPhysicsModel::CalculateAnchors() {
      std::vector<SAnchor*>& vecAnchors = m_cEmbodiedEntity.GetEnabledAnchors();
      for(size_t i = 0; i < vecAnchors.size(); ++i) {
         if(m_vecThunks[vecAnchors[i]->Index] != NULL) {
            TThunk tThunk = m_vecThunks[vecAnchors[i]->Index];
            (this->*tThunk)(*vecAnchors[i]);
         }
      }
   }

   /****************************************/
   /****************************************/

}
