/**
 * @file <argos3/plugins/simulator/physx_engines/physx/physx_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_ENGINE_H
#define PHYSX_ENGINE_H

namespace argos {
   class CPhysXEngine;
   class CPhysXModel;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/Include/PxPhysicsAPI.h>

namespace argos {

   class CPhysXEngine : public CPhysicsEngine {

   public:

      CPhysXEngine();
      virtual ~CPhysXEngine();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void Update();

      virtual bool IsPointContained(const CVector3& c_point);

      virtual UInt32 GetNumPhysicsEngineEntities();

      virtual void AddEntity(CEntity& c_entity);

      virtual void RemoveEntity(CEntity& c_entity);

      virtual bool IsEntityTransferNeeded() const;

      virtual void TransferEntities();

      void AddPhysicsModel(const std::string& str_id,
                           CPhysXModel& c_model);
      void RemovePhysicsModel(const std::string& str_id);

   private:

      class CPhysXEngineAllocatorCallback : public physx::PxAllocatorCallback {
      public:
         virtual void* allocate(size_t size, const char*, const char*, int);
         virtual void	deallocate(void* ptr);
      };

      class CPhysXEngineErrorCallback : public physx::PxErrorCallback {
      public:
         CPhysXEngineErrorCallback(CPhysXEngine& c_engine);
         virtual ~CPhysXEngineErrorCallback() {}
         virtual void reportError(physx::PxErrorCode::Enum e_code,
                                  const char* pch_message,
                                  const char* pch_file,
                                  int n_line);
      private:
         CPhysXEngine& m_cEngine;
      };

   private:

      /** List of physics models */
      std::map<std::string, CPhysXModel*> m_tPhysicsModels;

      /** The PhysX memory allocator */
      CPhysXEngineAllocatorCallback m_cAllocatorCallback;
      /** The PhysX error callback */
      CPhysXEngineErrorCallback m_cErrorCallback;

      /** The PhysX foundation class */
      physx::PxFoundation* m_pcFoundation;
      /** The PhysX physics */
      physx::PxPhysics* m_pcPhysics;
      /** The PhysX CPU dispatcher */
      physx::pxtask::CpuDispatcher* m_pcCPUDispatcher;
      /** The PhysX scene descriptor */
      physx::PxSceneDesc* m_pcSceneDesc;
      /** The PhysX scene */
      physx::PxScene* m_pcScene;

   };

/****************************************/
/****************************************/

   template <typename ACTION>
   class CPhysXOperation : public CEntityOperation<ACTION, CPhysXEngine, void> {
   public:
      virtual ~CPhysXOperation() {}
   };

   class CPhysXOperationAddEntity : public CPhysXOperation<CPhysXOperationAddEntity> {
   public:
      virtual ~CPhysXOperationAddEntity() {}
   };

   class CPhysXOperationRemoveEntity : public CPhysXOperation<CPhysXOperationRemoveEntity> {
   public:
      virtual ~CPhysXOperationRemoveEntity() {}
   };

#define REGISTER_PHYSX_OPERATION(ACTION, OPERATION, ENTITY)             \
   REGISTER_ENTITY_OPERATION(ACTION, CPhysXEngine, OPERATION, void, ENTITY);

#define REGISTER_STANDARD_PHYSX_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN2D_MODEL) \
   class CPhysXOperationAdd ## SPACE_ENTITY : public CPhysXOperationAddEntity { \
   public:                                                              \
   CPhysXOperationAdd ## SPACE_ENTITY() {}                              \
   virtual ~CPhysXOperationAdd ## SPACE_ENTITY() {}                     \
   void ApplyTo(CPhysXEngine& c_engine,                                 \
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
   REGISTER_PHYSX_OPERATION(CPhysXOperationAddEntity,                   \
                            CPhysXOperationAdd ## SPACE_ENTITY,         \
                            SPACE_ENTITY);
   
#define REGISTER_STANDARD_PHYSX_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)   \
   class CPhysXOperationRemove ## SPACE_ENTITY : public CPhysXOperationRemoveEntity { \
public:                                                                 \
CPhysXOperationRemove ## SPACE_ENTITY() {}                              \
virtual ~CPhysXOperationRemove ## SPACE_ENTITY() {}                     \
void ApplyTo(CPhysXEngine& c_engine,                                    \
      SPACE_ENTITY& c_entity) {                                         \
   c_engine.RemovePhysicsModel(c_entity.GetId());                       \
   c_entity.                                                            \
   GetComponent<CEmbodiedEntity>("body").                               \
   RemovePhysicsModel(c_engine.GetId());                                \
}                                                                       \
};                                                                      \
   REGISTER_PHYSX_OPERATION(CPhysXOperationRemoveEntity,                \
      CPhysXOperationRemove ## SPACE_ENTITY,                            \
      SPACE_ENTITY);
   
#define REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(SPACE_ENTITY, DYN2D_ENTITY) \
   REGISTER_STANDARD_PHYSX_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN2D_ENTITY) \
   REGISTER_STANDARD_PHYSX_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)

/****************************************/
/****************************************/

}

#endif
