#include "dynamics2d_stretchable_object_model.h"
#include "dynamics2d_gripping.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DStretchableObjectModel::CDynamics2DStretchableObjectModel(CDynamics2DEngine& c_engine,
                                                                        CComposableEntity& c_entity) :
      CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
      m_fMass(0.0f),
      m_pcGrippable(NULL),
      m_ptLinearFriction(NULL),
      m_ptAngularFriction(NULL) {}

   /****************************************/
   /****************************************/

   CDynamics2DStretchableObjectModel::~CDynamics2DStretchableObjectModel() {
      if(m_pcGrippable != NULL) delete m_pcGrippable;
      if(m_ptLinearFriction != NULL) {
         cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptLinearFriction);
         cpConstraintFree(m_ptLinearFriction);
      }
      if(m_ptAngularFriction != NULL) {
         cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptAngularFriction);
         cpConstraintFree(m_ptAngularFriction);
      }
   }
   
   /****************************************/
   /****************************************/

   void CDynamics2DStretchableObjectModel::MoveTo(const CVector3& c_position,
                                                  const CQuaternion& c_orientation) {
      if(m_pcGrippable != NULL) {
         /* Release all attached entities */
         m_pcGrippable->ReleaseAll();
      }
      CDynamics2DSingleBodyObjectModel::MoveTo(c_position, c_orientation);
   }
   
   /****************************************/
   /****************************************/

   void CDynamics2DStretchableObjectModel::Reset() {
      if(m_pcGrippable != NULL) {
         /* Release all attached entities */
         m_pcGrippable->ReleaseAll();
      }
      CDynamics2DSingleBodyObjectModel::Reset();
   }
   
   /****************************************/
   /****************************************/

   void CDynamics2DStretchableObjectModel::SetLinearFriction(Real f_max_bias,
                                                             Real f_max_force) {
         m_ptLinearFriction =
            cpSpaceAddConstraint(GetDynamics2DEngine().GetPhysicsSpace(),
                                 cpPivotJointNew2(GetDynamics2DEngine().GetGroundBody(),
                                                  GetBody(),
                                                  cpvzero,
                                                  cpvzero));
         m_ptLinearFriction->maxBias = f_max_bias;
         m_ptLinearFriction->maxForce = f_max_force; // emulate linear friction
   }
   
   /****************************************/
   /****************************************/

   void CDynamics2DStretchableObjectModel::SetAngularFriction(Real f_max_bias,
                                                              Real f_max_force) {
         m_ptAngularFriction =
            cpSpaceAddConstraint(GetDynamics2DEngine().GetPhysicsSpace(),
                                 cpGearJointNew(GetDynamics2DEngine().GetGroundBody(),
                                                GetBody(),
                                                0.0f,
                                                1.0f));
         m_ptAngularFriction->maxBias = f_max_bias;
         m_ptAngularFriction->maxForce = f_max_force; // emulate angular friction
   }

   /****************************************/
   /****************************************/

}
