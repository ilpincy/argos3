/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_gripping.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_gripping.h"
#include "dynamics2d_engine.h"

#include <argos3/core/simulator/entity/embodied_entity.h>

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DGripper::CDynamics2DGripper(CDynamics2DEngine& c_engine,
                                          CGripperEquippedEntity& c_gripper_entity,
                                          cpShape* pt_gripper_shape) :
      m_cEngine(c_engine),
      m_cGripperEntity(c_gripper_entity),
      m_ptGripperShape(pt_gripper_shape),
      m_pcGrippee(NULL),
      m_tConstraint(NULL) {
      m_ptGripperShape->sensor = 1;
      m_ptGripperShape->collision_type = CDynamics2DEngine::SHAPE_GRIPPER;
      m_ptGripperShape->data = this;
      m_tAnchor = cpvzero;
   }

   /****************************************/
   /****************************************/

   CDynamics2DGripper::~CDynamics2DGripper() {
      Release();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DGripper::CalculateAnchor(cpArbiter* pt_arb) {
      /* Calculate the anchor point on the grippable body
         as the centroid of the contact points */
      m_tAnchor = cpvzero;
      for(SInt32 i = 0; i < cpArbiterGetCount(pt_arb); ++i) {
         m_tAnchor = cpvadd(m_tAnchor, cpArbiterGetPoint(pt_arb, i));
      }
      m_tAnchor = cpvmult(m_tAnchor, 1.0f / cpArbiterGetCount(pt_arb));
   }

   /****************************************/
   /****************************************/
   
   void CDynamics2DGripper::Grip(CDynamics2DGrippable* pc_grippee) {
      m_tConstraint =
         cpSpaceAddConstraint(m_cEngine.GetPhysicsSpace(),
                              cpPivotJointNew(
                                 m_ptGripperShape->body,
                                 pc_grippee->GetShape()->body,
                                 m_tAnchor));
      m_tConstraint->maxBias = 0.95f;     // Correct overlap
      m_tConstraint->maxForce = 10000.0f; // Max correction speed
      m_cGripperEntity.SetGrippedEntity(pc_grippee->GetEmbodiedEntity());
      m_pcGrippee = pc_grippee;
      m_pcGrippee->Attach(*this);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DGripper::Release() {
      if(IsGripping()) {
         cpSpaceRemoveConstraint(m_cEngine.GetPhysicsSpace(), m_tConstraint);
         cpConstraintFree(m_tConstraint);
         m_tConstraint = NULL;
         m_cGripperEntity.ClearGrippedEntity();
         m_pcGrippee->Remove(*this);
         m_pcGrippee = NULL;
      }
   }

   /****************************************/
   /****************************************/

   CDynamics2DGrippable::CDynamics2DGrippable(CEmbodiedEntity& c_entity,
                                              cpShape* pt_shape) :
      m_cEmbodiedEntity(c_entity),
      m_ptShape(pt_shape) {
      m_ptShape->collision_type = CDynamics2DEngine::SHAPE_GRIPPABLE;
      m_ptShape->data = this;
   }

   /****************************************/
   /****************************************/

   CDynamics2DGrippable::~CDynamics2DGrippable() {
      ReleaseAll();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DGrippable::Attach(CDynamics2DGripper& c_gripper) {
      m_listGrippers.push_back(&c_gripper);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DGrippable::Remove(CDynamics2DGripper& c_gripper) {
      CDynamics2DGripper::TList::iterator it =
         std::find(m_listGrippers.begin(), m_listGrippers.end(), &c_gripper);
      if(it != m_listGrippers.end()) {
         m_listGrippers.erase(it);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DGrippable::Release(CDynamics2DGripper& c_gripper) {
      CDynamics2DGripper::TList::iterator it =
         std::find(m_listGrippers.begin(), m_listGrippers.end(), &c_gripper);
      if(it != m_listGrippers.end()) {
         (*it)->Release();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DGrippable::ReleaseAll() {
      while(!m_listGrippers.empty()) {
         m_listGrippers.back()->Release();
      }
   }

   /****************************************/
   /****************************************/

   int BeginCollisionBetweenGripperAndGrippable(cpArbiter* pt_arb,
                                                cpSpace* pt_space,
                                                void* p_data) {
      /* Get the shapes involved */
      CP_ARBITER_GET_SHAPES(pt_arb, ptGripperShape, ptGrippableShape);
      /* Get a reference to the gripper data */
      CDynamics2DGripper* pcGripper = reinterpret_cast<CDynamics2DGripper*>(ptGripperShape->data);
      /* Get a reference to the grippable entity */
      CDynamics2DGrippable* pcGrippable = reinterpret_cast<CDynamics2DGrippable*>(ptGrippableShape->data);
      /* If the entities match, ignore the collision forever */
      return (&(pcGripper->GetGripperEntity().GetParent()) != &(pcGrippable->GetEmbodiedEntity().GetParent()));
   }

   /****************************************/
   /****************************************/

   int ManageCollisionBetweenGripperAndGrippable(cpArbiter* pt_arb,
                                                 cpSpace* pt_space,
                                                 void* p_data) {
      /* Get the shapes involved */
      CP_ARBITER_GET_SHAPES(pt_arb, ptGripperShape, ptGrippableShape);
      /* Get a reference to the gripper data */
      CDynamics2DGripper* pcGripper = reinterpret_cast<CDynamics2DGripper*>(ptGripperShape->data);
      /*
       * When to process gripping:
       * 1. when the robot was gripping and it just unlocked the gripper
       * 2. when the robot was not gripping and it just locked the gripper
       *    in this case, also precalculate the anchor point
       * Otherwise ignore it
       */
      if(pcGripper->IsGripping() && !pcGripper->IsLocked()) {
         /* Instruct the engine to remove the constraint in a post-step callback */
         cpSpaceAddPostStepCallback(
            pt_space,
            RemoveConstraintBetweenGripperAndGrippable,
            pcGripper,
            ptGrippableShape->data);
         return false;
      }
      if(!pcGripper->IsGripping() && pcGripper->IsLocked()) {
         pcGripper->CalculateAnchor(pt_arb);
         /* Instruct the engine to add the constraint in a post-step callback */
         cpSpaceAddPostStepCallback(
            pt_space,
            AddConstraintBetweenGripperAndGrippable,
            pcGripper,
            reinterpret_cast<CDynamics2DGrippable*>(ptGrippableShape->data));
         return false;
      }
      /* Always return false, anyway the gripper is a sensor shape */
      return false;
   }
   
   /****************************************/
   /****************************************/

   void AddConstraintBetweenGripperAndGrippable(cpSpace* pt_space,
                                                void* p_obj,
                                                void* p_data) {
      /* Get the objects involved */
      CDynamics2DGripper*   pcGripper   = reinterpret_cast<CDynamics2DGripper*>  (p_obj);
      CDynamics2DGrippable* pcGrippable = reinterpret_cast<CDynamics2DGrippable*>(p_data);
      /* Connect the objects */
      pcGripper->Grip(pcGrippable);
   }
   
   /****************************************/
   /****************************************/

   void RemoveConstraintBetweenGripperAndGrippable(cpSpace* pt_space,
                                                   void* p_obj,
                                                   void* p_data) {
      /* Get the gripper objects */
      CDynamics2DGripper* pcGripper   = reinterpret_cast<CDynamics2DGripper*>  (p_obj);
      /* Disconnect the objects */
      pcGripper->Release();
   }
   
   /****************************************/
   /****************************************/

}
