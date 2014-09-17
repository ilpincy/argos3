/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_cylinder_model.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "dynamics2d_cylinder_model.h"
#include "dynamics2d_gripping.h"
#include "dynamics2d_engine.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DCylinderModel::CDynamics2DCylinderModel(CDynamics2DEngine& c_engine,
                                                      CCylinderEntity& c_entity) :
      CDynamics2DModel(c_engine, c_entity.GetEmbodiedEntity()),
      m_bMovable(false),
      m_cCylinderEntity(c_entity),
      m_pcGrippable(NULL),
      m_fMass(c_entity.GetMass()),
      m_ptShape(NULL),
      m_ptBody(NULL) {
      /* Set movable flag */
      m_bMovable = c_entity.GetEmbodiedEntity().IsMovable();
      /* Get the radius of the entity */
      Real fRadius = c_entity.GetRadius();
      /* Create a circle object in the physics space */
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      if(m_bMovable) {
         /* The cylinder is movable */
         /* Register the origin anchor update method */
         RegisterAnchorMethod<CDynamics2DCylinderModel>(GetEmbodiedEntity().GetOriginAnchor(),
                                                        &CDynamics2DCylinderModel::UpdateOriginAnchor);
         /* Create the body */
         m_ptBody = cpSpaceAddBody(m_cDyn2DEngine.GetPhysicsSpace(),
                                   cpBodyNew(m_fMass,
                                             cpMomentForCircle(m_fMass,
                                                               0,
                                                               fRadius + fRadius,
                                                               cpvzero)));
         m_ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
         CRadians cXAngle, cYAngle, cZAngle;
         GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
         cpBodySetAngle(m_ptBody, cZAngle.GetValue());
         /* Create the shape */
         m_ptShape = cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
                                     cpCircleShapeNew(m_ptBody, fRadius, cpvzero));
         m_ptShape->e = 0.0; // no elasticity
         m_ptShape->u = 0.7; // lots contact friction to help pushing
         /* The shape is grippable */
         m_pcGrippable = new CDynamics2DGrippable(GetEmbodiedEntity(),
                                                  m_ptShape);
         /* Friction with ground */
         m_ptLinearFriction =
            cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                                 cpPivotJointNew2(m_cDyn2DEngine.GetGroundBody(),
                                                  m_ptBody,
                                                  cpvzero,
                                                  cpvzero));
         m_ptLinearFriction->maxBias = 0.0f; // disable joint correction
         m_ptLinearFriction->maxForce = 1.49f; // emulate linear friction (this is just slightly smaller than FOOTBOT_MAX_FORCE)
         m_ptAngularFriction =
            cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                                 cpGearJointNew(m_cDyn2DEngine.GetGroundBody(),
                                                m_ptBody,
                                                0.0f,
                                                1.0f));
         m_ptAngularFriction->maxBias = 0.0f; // disable joint correction
         m_ptAngularFriction->maxForce = 1.49f; // emulate angular friction (this is just slightly smaller than FOOTBOT_MAX_TORQUE)
      }
      else {
         /* The cylinder is not movable */
         /* Create a static body */
         m_ptBody = cpBodyNewStatic();
         /* Create the shape */
         m_ptShape = cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
                                     cpCircleShapeNew(m_ptBody,
                                                      fRadius,
                                                      cpv(cPosition.GetX(),
                                                          cPosition.GetY())));
         m_ptShape->e = 0.0; // No elasticity
         m_ptShape->u = 0.1; // Little contact friction to help sliding away
         /* This shape is normal (not grippable, not gripper) */
         m_ptShape->collision_type = CDynamics2DEngine::SHAPE_NORMAL;
      }
      /* Associate this model to the body data for ray queries */
      m_ptBody->data = this;
      /* Calculate bounding box */
      GetBoundingBox().MinCorner.SetZ(GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.SetZ(GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + m_cCylinderEntity.GetHeight());
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   CDynamics2DCylinderModel::~CDynamics2DCylinderModel() {
      if(m_bMovable) {
         delete m_pcGrippable;
         cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptLinearFriction);
         cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptAngularFriction);
         cpConstraintFree(m_ptLinearFriction);
         cpConstraintFree(m_ptAngularFriction);
         cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBody);
         cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShape);
         cpShapeFree(m_ptShape);
         cpBodyFree(m_ptBody);
      }
      else {
         cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShape);
         cpShapeFree(m_ptShape);
         cpBodyFree(m_ptBody);
         cpSpaceReindexStatic(m_cDyn2DEngine.GetPhysicsSpace());
      }
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DCylinderModel::MoveTo(const CVector3& c_position,
                                          const CQuaternion& c_orientation,
                                          bool b_check_only) {
      SInt32 nCollision;
      /* Check whether the cylinder is movable or not */
      if(m_bMovable) {
         /* The cylinder is movable */
         /* Save body position and orientation */
         cpVect tOldPos = m_ptBody->p;
         cpFloat fOldA = m_ptBody->a;
         /* Move the body to the desired position */
         m_ptBody->p = cpv(c_position.GetX(), c_position.GetY());
         CRadians cXAngle, cYAngle, cZAngle;
         c_orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
         cpBodySetAngle(m_ptBody, cZAngle.GetValue());
         /* Create a shape sensor to test the movement */
         cpShape* ptTestShape = cpCircleShapeNew(m_ptBody, m_cCylinderEntity.GetRadius(), cpvzero);
         /* Check if there is a collision */
         nCollision = cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), ptTestShape, NULL, NULL);
         /* Dispose of the sensor shape */
         cpShapeFree(ptTestShape);
         if(b_check_only || nCollision) {
            /* Restore old body state if there was a collision or
               it was only a check for movement */
            m_ptBody->p = tOldPos;
            cpBodySetAngle(m_ptBody, fOldA);
         }
         else {
            /* Object moved, release all gripped entities */
            m_pcGrippable->ReleaseAll();
            /* Update the active space hash if the movement is actual */
            cpSpaceReindexShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShape);
            /* Update anchors */
            CalculateAnchors();
            /* Update bounding box */
            CalculateBoundingBox();
         }
      }
      else {
         /* The cylinder is not movable, so you can't move it :-) */
         nCollision = 1;
      }
      /* The movement is allowed if there is no collision */
      return !nCollision;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DCylinderModel::Reset() {
      if(m_bMovable) {
         /* Reset body position */
         const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
         m_ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
         /* Reset body orientation */
         CRadians cXAngle, cYAngle, cZAngle;
         GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
         cpBodySetAngle(m_ptBody, cZAngle.GetValue());
         /* Zero speed and applied forces */
         m_ptBody->v = cpvzero;
         m_ptBody->w = 0.0f;
         cpBodyResetForces(m_ptBody);
         /* Update bounding box */
         cpShapeCacheBB(m_ptShape);
         CalculateBoundingBox();
         /* Release all attached entities */
         m_pcGrippable->ReleaseAll();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DCylinderModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.SetX(m_ptShape->bb.l);
      GetBoundingBox().MinCorner.SetY(m_ptShape->bb.b);
      GetBoundingBox().MaxCorner.SetX(m_ptShape->bb.r);
      GetBoundingBox().MaxCorner.SetY(m_ptShape->bb.t);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DCylinderModel::UpdateEntityStatus() {
      if(m_bMovable) {
         CPhysicsModel::UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DCylinderModel::IsCollidingWithSomething() const {
      return cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShape, NULL, NULL) > 0;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DCylinderModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position.SetX(m_ptBody->p.x);
      s_anchor.Position.SetY(m_ptBody->p.y);
      s_anchor.Orientation.FromAngleAxis(CRadians(m_ptBody->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CCylinderEntity, CDynamics2DCylinderModel);

}
