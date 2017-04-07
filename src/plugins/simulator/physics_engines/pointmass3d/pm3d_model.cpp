#include "pm3d_model.h"
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPM3DModel::CPM3DModel(CPM3DEngine& c_engine,
                                        CEmbodiedEntity& c_entity) :
      CPhysicsModel(c_engine, c_entity),
      m_cPM3DEngine(c_engine) {
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPM3DModel::UpdateOriginAnchor);
      /* Set initial position */
      m_cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      /* Set initial orientation */
      m_cOrientation = GetEmbodiedEntity().GetOriginAnchor().Orientation;
   }
   
   /****************************************/
   /****************************************/

   void CPM3DModel::Reset() {
      m_cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CPM3DModel::MoveTo(const CVector3& c_position,
                                  const CQuaternion& c_orientation) {
      m_cPosition = c_position;
      m_cOrientation = c_orientation;
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   bool CPM3DModel::IsCollidingWithSomething() const {
      /* Go through other objects and check if the BB intersect */
      for(std::map<std::string, CPM3DModel*>::const_iterator it = GetPM3DEngine().GetPhysicsModels().begin();
          it != GetPM3DEngine().GetPhysicsModels().end(); ++it) {
         if((it->second != this) &&
            GetBoundingBox().Intersects(it->second->GetBoundingBox()))
            return true;
      }
      return false;
   }

   /****************************************/
   /****************************************/

   void CPM3DModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position = m_cPosition;
      s_anchor.Orientation = m_cOrientation;
   }

   /****************************************/
   /****************************************/

}
