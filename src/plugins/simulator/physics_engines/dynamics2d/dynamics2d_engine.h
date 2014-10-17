/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_ENGINE_H
#define DYNAMICS2D_ENGINE_H

namespace argos {
   class CDynamics2DEngine;
   class CDynamics2DModel;
   class CGripperEquippedEntity;
   class CEmbodiedEntity;
}

#include <argos3/core/utility/math/ray2.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>
#include <pthread.h>

namespace argos {

   /****************************************/
   /****************************************/

   struct SDynamics2DEngineGripperData {
      CDynamics2DEngine& Engine;
      CGripperEquippedEntity& GripperEntity;
      cpVect GripperAnchor;
      cpConstraint* GripConstraint;

      SDynamics2DEngineGripperData(CDynamics2DEngine& c_engine,
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
         SHAPE_GRIPPER
      };

      enum ELayerType {
         LAYER_FROZEN = 0,
         LAYER_NORMAL = CP_ALL_LAYERS
      };

      struct SBoundarySegment {
         CRay2 Segment;
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

      virtual bool IsPointContained(const CVector3& c_point);

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

      virtual CEmbodiedEntity* CheckIntersectionWithRay(Real& f_t_on_ray,
                                                        const CRay3& c_ray) const;

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

      inline const cpSpace* GetPhysicsSpace() const {
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

      void PositionPhysicsToSpace(CVector3& c_new_pos,
                                  const CVector3& c_original_pos,
                                  const cpBody* pt_body);

      void OrientationPhysicsToSpace(CQuaternion& c_new_orient,
                                     cpBody* pt_body);
      
      void AddPhysicsModel(const std::string& str_id,
                            CDynamics2DModel& c_model);
      void RemovePhysicsModel(const std::string& str_id);

   private:

      cpFloat m_fStaticHashCellSize;
      cpFloat m_fActiveHashCellSize;
      SInt32 m_nStaticHashCells;
      SInt32 m_nActiveHashCells;
      cpSpace* m_ptSpace;
      cpBody* m_ptGroundBody;
      Real m_fElevation;
      mutable pthread_mutex_t m_tSpaceQueryMutex;

      std::vector<CVector2> m_vecVertices;
      std::vector<SBoundarySegment> m_vecSegments;
      std::vector<SEntityTransferData> m_vecTransferData;
      bool m_bEntityTransferActive;

      CControllableEntity::TMap m_tControllableEntities;
      std::map<std::string, CDynamics2DModel*> m_tPhysicsModels;

   };

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

#define REGISTER_DYNAMICS2D_OPERATION(ACTION, OPERATION, ENTITY)        \
   REGISTER_ENTITY_OPERATION(ACTION, CDynamics2DEngine, OPERATION, void, ENTITY);

#define REGISTER_STANDARD_DYNAMICS2D_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN2D_MODEL) \
   class CDynamics2DOperationAdd ## SPACE_ENTITY : public CDynamics2DOperationAddEntity { \
   public:                                                              \
   CDynamics2DOperationAdd ## SPACE_ENTITY() {}                         \
   virtual ~CDynamics2DOperationAdd ## SPACE_ENTITY() {}                \
   void ApplyTo(CDynamics2DEngine& c_engine,                            \
                SPACE_ENTITY& c_entity) {                               \
      DYN2D_MODEL* pcPhysModel = new DYN2D_MODEL(c_engine,              \
                                                 c_entity);             \
      c_engine.AddPhysicsModel(c_entity.GetId(),                        \
                               *pcPhysModel);                           \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         AddPhysicsModel(c_engine.GetId(), *pcPhysModel);               \
   }                                                                    \
   };                                                                   \
   REGISTER_DYNAMICS2D_OPERATION(CDynamics2DOperationAddEntity,         \
                                 CDynamics2DOperationAdd ## SPACE_ENTITY, \
                                 SPACE_ENTITY);
   
#define REGISTER_STANDARD_DYNAMICS2D_OPERATION_REMOVE_ENTITY(SPACE_ENTITY) \
   class CDynamics2DOperationRemove ## SPACE_ENTITY : public CDynamics2DOperationRemoveEntity { \
   public:                                                              \
   CDynamics2DOperationRemove ## SPACE_ENTITY() {}                      \
   virtual ~CDynamics2DOperationRemove ## SPACE_ENTITY() {}             \
   void ApplyTo(CDynamics2DEngine& c_engine,                            \
                SPACE_ENTITY& c_entity) {                               \
      c_engine.RemovePhysicsModel(c_entity.GetId());                    \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         RemovePhysicsModel(c_engine.GetId());                          \
   }                                                                    \
   };                                                                   \
   REGISTER_DYNAMICS2D_OPERATION(CDynamics2DOperationRemoveEntity,      \
                                 CDynamics2DOperationRemove ## SPACE_ENTITY, \
                                 SPACE_ENTITY);
   
#define REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(SPACE_ENTITY, DYN2D_ENTITY) \
   REGISTER_STANDARD_DYNAMICS2D_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN2D_ENTITY) \
   REGISTER_STANDARD_DYNAMICS2D_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)

   /****************************************/
   /****************************************/

}

#endif
