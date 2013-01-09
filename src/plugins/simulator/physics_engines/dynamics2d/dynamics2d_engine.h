/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_ENGINE_H
#define DYNAMICS2D_ENGINE_H

namespace argos {
   class CDynamics2DEngine;
   class CGripperEquippedEntity;
}

#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>

namespace argos {

   /****************************************/
   /****************************************/

   template <typename ACTION>
   class CDynamics2DOperation : public CEntityOperation<ACTION, CDynamics2DEngine, void> {
   public:
      virtual ~CDynamics2DOperation() {}
   };

   class CDynamics2DOperationAddEntity : public CDynamics2DOperation<CDynamics2DOperationAddEntity> {
   public:
      virtual ~CDynamics2DOperationAddEntity() {}
   };
   class CDynamics2DOperationRemoveEntity : public CDynamics2DOperation<CDynamics2DOperationRemoveEntity> {
   public:
      virtual ~CDynamics2DOperationRemoveEntity() {}
   };

   /****************************************/
   /****************************************/

   struct SDynamics2DEngineGripperData {
      cpSpace* Space;
      CGripperEquippedEntity& GripperEntity;
      cpVect GripperAnchor;
      cpConstraint* GripConstraint;

      SDynamics2DEngineGripperData(cpSpace* pt_space,
                                   CGripperEquippedEntity& c_entity,
                                   cpVect t_anchor);
      ~SDynamics2DEngineGripperData();
      void ClearConstraints();
   };

   /****************************************/
   /****************************************/

   class CDynamics2DEngine : public CPhysicsEngine {

   public:

      enum EShapeType {
         SHAPE_NORMAL = 0,
         SHAPE_GRIPPABLE,
         SHAPE_CLAW_GRIPPER,
         SHAPE_MAGNETIC_GRIPPER
      };

      enum ELayerType {
         LAYER_FROZEN = 0,
         LAYER_NORMAL = CP_ALL_LAYERS
      };

      struct SBoundarySegment {
         CSegment Segment;
         enum {
            SEGMENT_TYPE_WALL,
            SEGMENT_TYPE_GATE
         } Type;
         std::string EngineId;

         SBoundarySegment() : 
            Type(SEGMENT_TYPE_WALL) {}
      };

      struct SEntityTransferData {
         std::string EngineId;
         CEntity* Entity;

         SEntityTransferData() :
            Entity(NULL) {}
      };

      CDynamics2DEngine();
      virtual ~CDynamics2DEngine() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Update();
      virtual void Destroy();
      virtual UInt32 GetNumPhysicsEngineEntities();
      virtual void AddEntity(CEntity& c_entity);
      virtual void RemoveEntity(CEntity& c_entity);

      inline virtual bool IsEntityTransferNeeded() const {
         return ! m_vecTransferData.empty();
      }
      virtual void TransferEntities();

      inline virtual bool IsEntityTransferActive() const {
         return m_bEntityTransferActive;
      }
      bool CalculateTransfer(Real f_x, Real f_y,
                             std::string& str_engine_id);
      inline void ScheduleEntityForTransfer(CEntity& c_entity,
                                            const std::string& str_engine_id) {
         m_vecTransferData.push_back(SEntityTransferData());
         m_vecTransferData.back().EngineId = str_engine_id;
         m_vecTransferData.back().Entity = &c_entity;
      }

      inline cpSpace* GetPhysicsSpace() {
         return m_ptSpace;
      }

      inline cpBody* GetGroundBody() {
         return m_ptGroundBody;
      }

      inline Real GetElevation() const {
         return m_fElevation;
      }

      inline Real GetDamping() const {
         return cpSpaceGetDamping(m_ptSpace);
      }

      inline void SetDamping(Real f_damping) {
         cpSpaceSetDamping(m_ptSpace, f_damping);
      }

      inline CVector2 GetGravity() const {
         return CVector2(m_ptSpace->gravity.x, m_ptSpace->gravity.y);
      }

      inline void SetGravity(const CVector2& c_gravity) {
         m_ptSpace->gravity = cpv(c_gravity.GetX(), c_gravity.GetY());
      }

      inline void PositionPhysicsToSpace(CVector3& c_new_pos,
                                         const CVector3& c_original_pos,
                                         const cpBody* pt_body) {
         c_new_pos.SetX(pt_body->p.x);
         c_new_pos.SetY(pt_body->p.y);
         c_new_pos.SetZ(c_original_pos.GetZ());
      }

      inline void OrientationPhysicsToSpace(CQuaternion& c_new_orient,
                                            cpBody* pt_body) {
         c_new_orient.FromAngleAxis(CRadians(pt_body->a), CVector3::Z);
      }

      void AddPhysicsEntity(const std::string& str_id,
                            CDynamics2DEntity& c_entity);
      void AddControllableEntity(CControllableEntity& c_entity);
      void RemovePhysicsEntity(const std::string& str_id);
      void RemoveControllableEntity(const std::string& str_id);

   private:

      SInt32 m_nIterations;
      cpFloat m_fStaticHashCellSize;
      cpFloat m_fActiveHashCellSize;
      SInt32 m_nStaticHashCells;
      SInt32 m_nActiveHashCells;
      cpSpace* m_ptSpace;
      cpBody* m_ptGroundBody;
      Real m_fElevation;

      std::vector<CVector2> m_vecVertices;
      std::vector<SBoundarySegment> m_vecSegments;
      std::vector<SEntityTransferData> m_vecTransferData;
      bool m_bEntityTransferActive;

      CControllableEntity::TMap m_tControllableEntities;
      CDynamics2DEntity::TMap m_tPhysicsEntities;

   };

}

#endif
