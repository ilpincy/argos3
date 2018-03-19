/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_cylinder_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_cylinder_model.h"

#include <argos3/plugins/simulator/entities/cylinder_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DCylinderModel::CDynamics3DCylinderModel(CDynamics3DEngine& c_engine,
                                                      CCylinderEntity& c_cylinder) :
      CDynamics3DSingleBodyObjectModel(c_engine, c_cylinder) {
      /* Fetch a collision shape for this model */
      m_pcShape = CDynamics3DShapeManager::RequestCylinder(
         btVector3(c_cylinder.GetRadius(),
                   c_cylinder.GetHeight() * 0.5f, 
                   c_cylinder.GetRadius()));
      /* Get the origin anchor */
      SAnchor& sAnchor = c_cylinder.GetEmbodiedEntity().GetOriginAnchor();
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
         btVector3(0.0f, -c_cylinder.GetHeight() * 0.5f, 0.0f));
      /* Initialize mass and inertia to zero (static object) */
      Real fMass = 0.0f;
      btVector3 cInertia(0.0f, 0.0f, 0.0f);
      /* If the cylinder is movable calculate its mass and inertia */
      if(c_cylinder.GetEmbodiedEntity().IsMovable()) {
         fMass = c_cylinder.GetMass();
         m_pcShape->calculateLocalInertia(fMass, cInertia);
      }
      /* Use the default friction */
      btScalar fFriction = GetEngine().GetDefaultFriction();
      /* Set up the body */
      CBody* pcBody = new CBody(*this,
                                sAnchor,
                                CBody::SData(cStartTransform,
                                             cCenterOfMassOffset,
                                             cInertia,
                                             fMass,
                                             fFriction,
                                             *m_pcShape));
      /* Transfer the body to the base class */
      m_vecBodies.push_back(pcBody);
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }
   
   /****************************************/
   /****************************************/
   
   CDynamics3DCylinderModel::~CDynamics3DCylinderModel() {
      CDynamics3DShapeManager::ReleaseCylinder(m_pcShape);
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CCylinderEntity, CDynamics3DCylinderModel);

   /****************************************/
   /****************************************/

}
