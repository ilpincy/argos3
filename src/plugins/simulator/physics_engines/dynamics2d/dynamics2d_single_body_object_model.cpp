#include "dynamics2d_single_body_object_model.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DSingleBodyObjectModel::CDynamics2DSingleBodyObjectModel(CDynamics2DEngine& c_engine,
                                                                      CComposableEntity& c_entity) :
      CDynamics2DModel(c_engine, c_entity.GetComponent<CEmbodiedEntity>("body")),
      m_cEntity(c_entity),
      m_ptBody(NULL) {}

   /****************************************/
   /****************************************/

   CDynamics2DSingleBodyObjectModel::~CDynamics2DSingleBodyObjectModel() {
      bool bIsStatic = cpBodyIsStatic(m_ptBody);
      /* Dispose of shapes */
      cpShape* ptCurShape = m_ptBody->shapeList;
      cpShape* ptNextShape;
      while(ptCurShape) {
         ptNextShape = ptCurShape->next;
         cpSpaceRemoveShape(GetDynamics2DEngine().GetPhysicsSpace(), ptCurShape);
         cpShapeFree(ptCurShape);
         ptCurShape = ptNextShape;
      }
      /* Dispose of body */
      if(! bIsStatic)
         cpSpaceRemoveBody(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBody);
      cpBodyFree(m_ptBody);
      /* Reindex space */
      if(bIsStatic) cpSpaceReindexStatic(GetDynamics2DEngine().GetPhysicsSpace());
   }

   /****************************************/
   /****************************************/

   void CDynamics2DSingleBodyObjectModel::MoveTo(const CVector3& c_position,
                                                 const CQuaternion& c_orientation) {
      /* Move the body to the desired position */
      m_ptBody->p = cpv(c_position.GetX(), c_position.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      c_orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptBody, cZAngle.GetValue());
      /* Update shape index */
      if(cpBodyIsStatic(m_ptBody)) {
         cpBB tBoundingBox = cpShapeGetBB(m_ptBody->shapeList);
         cpSpaceReindexStatic(GetDynamics2DEngine().GetPhysicsSpace());
         tBoundingBox = cpShapeGetBB(m_ptBody->shapeList);
      }
      else {
         cpSpaceReindexShapesForBody(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBody);
      }
      /* Update ARGoS entity state */
      CDynamics2DModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DSingleBodyObjectModel::Reset() {
      /* Nothing to do for a static body */
      if(cpBodyIsStatic(m_ptBody)) return;
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
      cpSpaceReindexShapesForBody(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBody);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DSingleBodyObjectModel::CalculateBoundingBox() {
      cpBB tBoundingBox = cpShapeGetBB(m_ptBody->shapeList);
      for(cpShape* pt_shape = m_ptBody->shapeList->next;
          pt_shape != NULL;
          pt_shape = pt_shape->next) {
         cpBB* ptBB = &pt_shape->bb;
         if(ptBB->l < tBoundingBox.l) tBoundingBox.l = ptBB->l;
         if(ptBB->b < tBoundingBox.b) tBoundingBox.b = ptBB->b;
         if(ptBB->r > tBoundingBox.r) tBoundingBox.r = ptBB->r;
         if(ptBB->t > tBoundingBox.t) tBoundingBox.t = ptBB->t;
      }
      GetBoundingBox().MinCorner.SetX(tBoundingBox.l);
      GetBoundingBox().MinCorner.SetY(tBoundingBox.b);
      GetBoundingBox().MaxCorner.SetX(tBoundingBox.r);
      GetBoundingBox().MaxCorner.SetY(tBoundingBox.t);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DSingleBodyObjectModel::UpdateEntityStatus() {
      /* Nothing to do for a static body */
      if(!cpBodyIsStatic(m_ptBody)) {
         CDynamics2DModel::UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DSingleBodyObjectModel::IsCollidingWithSomething() const {
      for(cpShape* pt_shape = m_ptBody->shapeList;
          pt_shape != NULL;
          pt_shape = pt_shape->next) {
         if(cpSpaceShapeQuery(
               const_cast<CDynamics2DSingleBodyObjectModel*>(this)->
               GetDynamics2DEngine().GetPhysicsSpace(),
               pt_shape, NULL, NULL) > 0) {
            return true;
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DSingleBodyObjectModel::SetBody(cpBody* pt_body,
                                                  Real f_height) {
      /* Set the body and its data field for ray queries */
      m_ptBody = pt_body;
      m_ptBody->data = this;
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CDynamics2DSingleBodyObjectModel::UpdateOriginAnchor);
      /* Calculate the bounding box */
      GetBoundingBox().MinCorner.SetZ(GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.SetZ(GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + f_height);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DSingleBodyObjectModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position.SetX(m_ptBody->p.x);
      s_anchor.Position.SetY(m_ptBody->p.y);
      s_anchor.Orientation.FromAngleAxis(CRadians(m_ptBody->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

}
