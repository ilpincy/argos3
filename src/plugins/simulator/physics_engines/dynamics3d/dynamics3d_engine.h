/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_ENGINE_H
#define DYNAMICS3D_ENGINE_H

namespace argos {
   class CDynamics3DEngine;
   class CDynamics3DModel;
   class CDynamics3DPlugin;
}

#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/utility/math/ray2.h>
#include <argos3/core/utility/math/rng.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#endif

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/btBulletDynamicsCommon.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>

#ifdef __APPLE__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

namespace argos {

   /****************************************/
   /****************************************/

   class CDynamics3DEngine : public CPhysicsEngine {

   public:
      
      CDynamics3DEngine();

      virtual ~CDynamics3DEngine() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

      virtual void Destroy();

      virtual void PostSpaceInit();

      virtual size_t GetNumPhysicsModels();

      virtual bool AddEntity(CEntity& c_entity);

      virtual bool RemoveEntity(CEntity& c_entity);

      virtual void CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                            const CRay3& c_ray) const;

      inline btMultiBodyDynamicsWorld& GetWorld() {
         return m_cWorld;
      }
      
      inline std::map<std::string, CDynamics3DPlugin*>& GetPhysicsPlugins() {
         return m_tPhysicsPlugins;
      }

      inline btCollisionDispatcher& GetDispatcher() {
         return m_cDispatcher;
      }
      
      inline const btCollisionDispatcher& GetDispatcher() const {
         return m_cDispatcher;
      }

      inline btScalar GetDefaultFriction() const {
         return m_fDefaultFriction;
      }
      
      void AddPhysicsModel(const std::string& str_id,
                           CDynamics3DModel& c_model);      
      
      void RemovePhysicsModel(const std::string& str_id);

      void AddPhysicsPlugin(const std::string& str_id,
                            CDynamics3DPlugin& c_plugin);

      void RemovePhysicsPlugin(const std::string& str_id);

   private:
      /* Maps of models and plugins */
      std::map<std::string, CDynamics3DModel*> m_tPhysicsModels;
      std::map<std::string, CDynamics3DPlugin*> m_tPhysicsPlugins;
      /* Random number generation */
      CRandom::CRNG* m_pcRNG;
      CRange<UInt32> m_cRandomSeedRange;
      /* Debug filename */
      std::string m_strDebugFilename;
      /* Default friction */
      btScalar m_fDefaultFriction;
      /* Bullet Physics World Data */
      btDbvtBroadphase                m_cBroadphase;
      btDefaultCollisionConfiguration m_cConfiguration;
      btCollisionDispatcher           m_cDispatcher;
      btMultiBodyConstraintSolver     m_cSolver;
      btMultiBodyDynamicsWorld        m_cWorld;
   };

   /****************************************/
   /****************************************/

   template <typename ACTION>
   class CDynamics3DOperation : public CEntityOperation<ACTION, CDynamics3DEngine, SOperationOutcome> {
   public:
      virtual ~CDynamics3DOperation() {}
   };

   class CDynamics3DOperationAddEntity : public CDynamics3DOperation<CDynamics3DOperationAddEntity> {
   public:
      virtual ~CDynamics3DOperationAddEntity() {}
   };

   class CDynamics3DOperationRemoveEntity : public CDynamics3DOperation<CDynamics3DOperationRemoveEntity> {
   public:
      virtual ~CDynamics3DOperationRemoveEntity() {}
   };

#define REGISTER_DYNAMICS3D_OPERATION(ACTION, OPERATION, ENTITY)        \
   REGISTER_ENTITY_OPERATION(ACTION, CDynamics3DEngine, OPERATION, SOperationOutcome, ENTITY);

#define REGISTER_STANDARD_DYNAMICS3D_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN3D_MODEL) \
   class CDynamics3DOperationAdd ## SPACE_ENTITY : public CDynamics3DOperationAddEntity { \
   public:                                                              \
   CDynamics3DOperationAdd ## SPACE_ENTITY() {}                         \
   virtual ~CDynamics3DOperationAdd ## SPACE_ENTITY() {}                \
   SOperationOutcome ApplyTo(CDynamics3DEngine& c_engine,               \
                SPACE_ENTITY& c_entity) {                               \
      DYN3D_MODEL* pcPhysModel = new DYN3D_MODEL(c_engine,              \
                                                 c_entity);             \
      c_engine.AddPhysicsModel(c_entity.GetId(),                        \
                               *pcPhysModel);                           \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         AddPhysicsModel(c_engine.GetId(), *pcPhysModel);               \
      return SOperationOutcome(true);                                   \
   }                                                                    \
   };                                                                   \
   REGISTER_DYNAMICS3D_OPERATION(CDynamics3DOperationAddEntity,         \
                                 CDynamics3DOperationAdd ## SPACE_ENTITY, \
                                 SPACE_ENTITY);
   
#define REGISTER_STANDARD_DYNAMICS3D_OPERATION_REMOVE_ENTITY(SPACE_ENTITY) \
   class CDynamics3DOperationRemove ## SPACE_ENTITY : public CDynamics3DOperationRemoveEntity { \
   public:                                                              \
   CDynamics3DOperationRemove ## SPACE_ENTITY() {}                      \
   virtual ~CDynamics3DOperationRemove ## SPACE_ENTITY() {}             \
   SOperationOutcome ApplyTo(CDynamics3DEngine& c_engine,               \
                SPACE_ENTITY& c_entity) {                               \
      c_engine.RemovePhysicsModel(c_entity.GetId());                    \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         RemovePhysicsModel(c_engine.GetId());                          \
      return SOperationOutcome(true);                                   \
   }                                                                    \
   };                                                                   \
   REGISTER_DYNAMICS3D_OPERATION(CDynamics3DOperationRemoveEntity,      \
                                 CDynamics3DOperationRemove ## SPACE_ENTITY, \
                                 SPACE_ENTITY);
   
#define REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(SPACE_ENTITY, DYN3D_ENTITY) \
   REGISTER_STANDARD_DYNAMICS3D_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN3D_ENTITY) \
   REGISTER_STANDARD_DYNAMICS3D_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)

   /****************************************/
   /****************************************/

}

#endif
