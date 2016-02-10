#include "dynamics2d_multi_body_object_model.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DMultiBodyObjectModel::CDynamics2DMultiBodyObjectModel(CDynamics2DEngine& c_engine,
                                                                    CComposableEntity& c_entity) :
      CDynamics2DModel(c_engine, c_entity.GetComponent<CEmbodiedEntity>("body")),
      m_cEntity(c_entity) {}

   /****************************************/
   /****************************************/

   CDynamics2DMultiBodyObjectModel::~CDynamics2DMultiBodyObjectModel() {
      /* Dispose of shapes and bodies */
      while(!m_vecBodies.empty()) {
         /* Get pointer to body */
         cpBody* ptBody = m_vecBodies.back().Body;
         /* Remove all of its shapes */
         cpShape* ptCurShape = ptBody->shapeList;
         cpShape* ptNextShape;
         while(ptCurShape) {
            ptNextShape = ptCurShape->next;
            cpSpaceRemoveShape(GetDynamics2DEngine().GetPhysicsSpace(), ptCurShape);
            cpShapeFree(ptCurShape);
            ptCurShape = ptNextShape;
         }
         /* Dispose of body */
         cpSpaceRemoveBody(GetDynamics2DEngine().GetPhysicsSpace(), ptBody);
         cpBodyFree(ptBody);
         /* Dispose of struct */
         m_vecBodies.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DMultiBodyObjectModel::MoveTo(const CVector3& c_position,
                                                const CQuaternion& c_orientation) {
      /* Set target position and orientation */
      cpVect tBodyPos = cpv(c_position.GetX(), c_position.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      c_orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpFloat tBodyOrient = cZAngle.GetValue();
      /* For each body: */
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         /* Set body orientation at anchor */
         cpBodySetAngle(m_vecBodies[i].Body,
                        tBodyOrient + m_vecBodies[i].OffsetOrient);
         /* Set body position at anchor */
         cpBodySetPos(m_vecBodies[i].Body,
                      cpvadd(tBodyPos,
                             cpvrotate(m_vecBodies[i].OffsetPos,
                                       m_vecBodies[i].Body->rot)));
         
         /* Update shape index */
         cpSpaceReindexShapesForBody(GetDynamics2DEngine().GetPhysicsSpace(),
                                     m_vecBodies[i].Body);
      }
      /* Update ARGoS entity state */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DMultiBodyObjectModel::Reset() {
      /* Reset body position */
      MoveTo(GetEmbodiedEntity().GetOriginAnchor().Position,
             GetEmbodiedEntity().GetOriginAnchor().Orientation);
      /* For each body: */
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         /* Zero the speeds */
         m_vecBodies[i].Body->v = cpvzero;
         m_vecBodies[i].Body->w = 0.0f;
         /* Zero forces and torques */
         cpBodyResetForces(m_vecBodies[i].Body);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DMultiBodyObjectModel::CalculateBoundingBox() {
      if(m_vecBodies.empty()) return;
      cpBB tBoundingBox;
      Real fMaxHeight;
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         tBoundingBox = cpShapeGetBB(m_vecBodies[i].Body->shapeList);
         for(cpShape* pt_shape = m_vecBodies[i].Body->shapeList->next;
             pt_shape != NULL;
             pt_shape = pt_shape->next) {
            cpBB* ptBB = &pt_shape->bb;
            if(ptBB->l < tBoundingBox.l) tBoundingBox.l = ptBB->l;
            if(ptBB->b < tBoundingBox.b) tBoundingBox.b = ptBB->b;
            if(ptBB->r > tBoundingBox.r) tBoundingBox.r = ptBB->r;
            if(ptBB->t > tBoundingBox.t) tBoundingBox.t = ptBB->t;
         }
         fMaxHeight = Max(fMaxHeight, m_vecBodies[i].Height);
      }      
      GetBoundingBox().MinCorner.SetX(tBoundingBox.l);
      GetBoundingBox().MinCorner.SetY(tBoundingBox.b);
      GetBoundingBox().MinCorner.SetZ(GetDynamics2DEngine().GetElevation());
      GetBoundingBox().MaxCorner.SetX(tBoundingBox.r);
      GetBoundingBox().MaxCorner.SetY(tBoundingBox.t);
      GetBoundingBox().MaxCorner.SetZ(GetDynamics2DEngine().GetElevation() + fMaxHeight);
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DMultiBodyObjectModel::IsCollidingWithSomething() const {
      if(m_vecBodies.empty()) return false;
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         for(cpShape* pt_shape = m_vecBodies[i].Body->shapeList;
             pt_shape != NULL;
             pt_shape = pt_shape->next) {
            if(cpSpaceShapeQuery(
                  const_cast<CDynamics2DMultiBodyObjectModel*>(this)->
                  GetDynamics2DEngine().GetPhysicsSpace(),
                  pt_shape, NULL, NULL) > 0) {
               return true;
            }
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DMultiBodyObjectModel::AddBody(cpBody* pt_body,
                                                 const cpVect& t_offset_pos,
                                                 cpFloat t_offset_orient,
                                                 Real f_height) {
      /* Set the body and its data field for ray queries */
      pt_body->data = this;
      /* Add body to list */
      m_vecBodies.push_back(SBody(pt_body,
                                  t_offset_pos,
                                  t_offset_orient,
                                  f_height));
      /* Calculate the bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   CDynamics2DMultiBodyObjectModel::SBody::SBody(cpBody* pt_body,
                                                 const cpVect& t_offset_pos,
                                                 cpFloat t_offset_orient,
                                                 Real f_height) :
      Body(pt_body),
      OffsetPos(t_offset_pos),
      OffsetOrient(t_offset_orient),
      Height(f_height) {}

   /****************************************/
   /****************************************/

}
