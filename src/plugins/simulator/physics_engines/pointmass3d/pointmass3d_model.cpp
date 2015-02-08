#include "pointmass3d_model.h"
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPointMass3DModel::CPointMass3DModel(CPointMass3DEngine& c_engine,
                                        CEmbodiedEntity& c_entity) :
      CPhysicsModel(c_engine, c_entity),
      m_cPM3DEngine(c_engine) {
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPointMass3DModel::UpdateOriginAnchor);
      /* Calculate the bounding box */
      Reset();
   }
   
   /****************************************/
   /****************************************/

   void CPointMass3DModel::Reset() {
      m_cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      m_cVelocity = CVector3();
      m_cAcceleration = CVector3();
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DModel::MoveTo(const CVector3& c_position,
                                  const CQuaternion& c_orientation) {
      CalculateBoundingBox();
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position = m_cPosition;
   }

   /****************************************/
   /****************************************/

}
