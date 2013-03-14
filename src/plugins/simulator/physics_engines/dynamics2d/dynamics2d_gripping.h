/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_gripping.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_GRIPPING_H
#define DYNAMICS2D_GRIPPING_H

namespace argos {
   class CDynamics2DEngine;
   class CDynamics2DGripper;
   class CDynamics2DGrippable;
   class CGripperEquippedEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>
#include <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
#include <list>

namespace argos {

   /****************************************/
   /****************************************/

   class CDynamics2DGripper {

   public:

      typedef std::list<CDynamics2DGripper*> TList;

   public:

      CDynamics2DGripper(CDynamics2DEngine& c_engine,
                         CGripperEquippedEntity& c_gripper_entity,
                         cpShape* pt_gripper_shape);

      ~CDynamics2DGripper();

      inline bool IsGripping() const {
         return m_pcGrippee != NULL;
      }

      inline bool IsLocked() const {
         return m_cGripperEntity.IsLocked();
      }

      inline CGripperEquippedEntity& GetGripperEntity() {
         return m_cGripperEntity;
      }

      inline cpShape* GripperShape() {
         return m_ptGripperShape;
      }
      
      inline CDynamics2DGrippable* GetGrippee() {
         return m_pcGrippee;
      }

      inline cpConstraint* GetConstraint() {
         return m_tConstraint;
      }

      void CalculateAnchor(cpArbiter* pt_arb);

      void Grip(CDynamics2DGrippable* pc_grippee);

      void Release();

   private:

      CDynamics2DEngine&      m_cEngine;
      CGripperEquippedEntity& m_cGripperEntity;
      cpShape*                m_ptGripperShape;
      CDynamics2DGrippable*   m_pcGrippee;
      cpVect                  m_tAnchor;
      cpConstraint*           m_tConstraint;

   };

   /****************************************/
   /****************************************/

   class CDynamics2DGrippable {

   public:

      CDynamics2DGrippable(CEmbodiedEntity& c_entity,
                           cpShape* pt_shape);

      ~CDynamics2DGrippable();

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return m_cEmbodiedEntity;
      }

      inline cpShape* GetShape() {
         return m_ptShape;
      }

      inline bool HasGrippers() const {
         return !m_listGrippers.empty();
      }

      void Attach(CDynamics2DGripper& c_gripper);

      void Remove(CDynamics2DGripper& c_gripper);

      void Release(CDynamics2DGripper& c_gripper);

      void ReleaseAll();

   private:

      CEmbodiedEntity&          m_cEmbodiedEntity;
      cpShape*                  m_ptShape;
      CDynamics2DGripper::TList m_listGrippers;

   };

   /****************************************/
   /****************************************/

   extern int BeginCollisionBetweenGripperAndGrippable(cpArbiter* pt_arb,
                                                       cpSpace* pt_space,
                                                       void* p_data);

   extern int ManageCollisionBetweenGripperAndGrippable(cpArbiter* pt_arb,
                                                        cpSpace* pt_space,
                                                        void* p_data);
   
   extern void AddConstraintBetweenGripperAndGrippable(cpSpace* pt_space,
                                                       void* p_obj,
                                                       void* p_data);
   
   extern void RemoveConstraintBetweenGripperAndGrippable(cpSpace* pt_space,
                                                          void* p_obj,
                                                          void* p_data);
   
   /****************************************/
   /****************************************/

}

#endif
