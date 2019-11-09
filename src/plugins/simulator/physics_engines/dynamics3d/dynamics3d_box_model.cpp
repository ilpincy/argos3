/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_box_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_box_model.h"

#include <argos3/plugins/simulator/entities/box_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DBoxModel::CDynamics3DBoxModel(CDynamics3DEngine& c_engine,
                                            CBoxEntity& c_box) :
      CDynamics3DSingleBodyObjectModel(c_engine, c_box),
      m_ptrBody(nullptr) {
      /* Fetch a collision shape for this model */
      const std::shared_ptr<btCollisionShape>& ptrShape = 
         CDynamics3DShapeManager::RequestBox(
            btVector3(c_box.GetSize().GetX() * 0.5f,
                      c_box.GetSize().GetZ() * 0.5f, 
                      c_box.GetSize().GetY() * 0.5f));
      /* Get the origin anchor */
      SAnchor& sAnchor = c_box.GetEmbodiedEntity().GetOriginAnchor();
      const CQuaternion& cOrientation = sAnchor.Orientation;
      const CVector3& cPosition = sAnchor.Position;
      /* Calculate the start transform */
      const btTransform& cStartTransform = btTransform(
         btQuaternion(cOrientation.GetX(),
                      cOrientation.GetZ(),
                     -cOrientation.GetY(),
                      cOrientation.GetW()),
         btVector3(cPosition.GetX(),
                   cPosition.GetZ(),
                  -cPosition.GetY()));
      /* Calculate the center of mass offset */
      const btTransform& cCenterOfMassOffset = btTransform(
         btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
         btVector3(0.0f, -c_box.GetSize().GetZ() * 0.5f, 0.0f));
      /* Initialize mass and inertia to zero (static object) */
      Real fMass = 0.0f;
      btVector3 cInertia(0.0f, 0.0f, 0.0f);
      /* If the box is movable calculate its mass and inertia */
      if(c_box.GetEmbodiedEntity().IsMovable()) {
         fMass = c_box.GetMass();
         ptrShape->calculateLocalInertia(fMass, cInertia);
      }
      /* Use the default friction */
      btScalar fFriction = GetEngine().GetDefaultFriction();
      /* Set up body data */
      CBody::SData sData(cStartTransform, cCenterOfMassOffset, cInertia, fMass, fFriction);
      /* Create the body */
      m_ptrBody = std::make_shared<CBody>(*this, &sAnchor, ptrShape, sData);
      /* Transfer the body to the base class */
      m_vecBodies.push_back(m_ptrBody);
      /* Finalize model with a reset */
      Reset();
   }
   
   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CBoxEntity, CDynamics3DBoxModel);

   /****************************************/
   /****************************************/

}
