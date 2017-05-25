/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef POINTMASS3D_ENGINE_H
#define POINTMASS3D_ENGINE_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DModel;
   class CEmbodiedEntity;
}

#include <argos3/core/utility/math/ray2.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

   class CPointMass3DEngine : public CPhysicsEngine {

   public:

      CPointMass3DEngine();
      virtual ~CPointMass3DEngine();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void Update();

      virtual size_t GetNumPhysicsModels();
      virtual bool AddEntity(CEntity& c_entity);
      virtual bool RemoveEntity(CEntity& c_entity);

      virtual bool IsPointContained(const CVector3& c_point);

      virtual bool IsEntityTransferNeeded() const;

      virtual void TransferEntities();

      virtual void CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                            const CRay3& c_ray) const;

      void AddPhysicsModel(const std::string& str_id,
                           CPointMass3DModel& c_model);
      void RemovePhysicsModel(const std::string& str_id);

      std::map<std::string, CPointMass3DModel*>& GetPhysicsModels() {
         return m_tPhysicsModels;
      }

      const std::map<std::string, CPointMass3DModel*>& GetPhysicsModels() const {
         return m_tPhysicsModels;
      }

      inline Real GetGravity() const {
         return m_fGravity;
      }

   private:

      CControllableEntity::TMap m_tControllableEntities;
      std::map<std::string, CPointMass3DModel*> m_tPhysicsModels;
      Real m_fGravity;

   };

   /****************************************/
   /****************************************/

   template <typename ACTION>
   class CPointMass3DOperation : public CEntityOperation<ACTION, CPointMass3DEngine, SOperationOutcome> {
   public:
      virtual ~CPointMass3DOperation() {}
   };

   class CPointMass3DOperationAddEntity : public CPointMass3DOperation<CPointMass3DOperationAddEntity> {
   public:
      virtual ~CPointMass3DOperationAddEntity() {}
   };

   class CPointMass3DOperationRemoveEntity : public CPointMass3DOperation<CPointMass3DOperationRemoveEntity> {
   public:
      virtual ~CPointMass3DOperationRemoveEntity() {}
   };

#define REGISTER_POINTMASS3D_OPERATION(ACTION, OPERATION, ENTITY)       \
   REGISTER_ENTITY_OPERATION(ACTION, CPointMass3DEngine, OPERATION, SOperationOutcome, ENTITY);

#define REGISTER_STANDARD_POINTMASS3D_OPERATION_ADD_ENTITY(SPACE_ENTITY, PM3D_MODEL) \
   class CPointMass3DOperationAdd ## SPACE_ENTITY : public CPointMass3DOperationAddEntity { \
   public:                                                              \
   CPointMass3DOperationAdd ## SPACE_ENTITY() {}                        \
   virtual ~CPointMass3DOperationAdd ## SPACE_ENTITY() {}               \
   SOperationOutcome ApplyTo(CPointMass3DEngine& c_engine,              \
                             SPACE_ENTITY& c_entity) {                  \
      PM3D_MODEL* pcPhysModel = new PM3D_MODEL(c_engine,                \
                                               c_entity);               \
      c_engine.AddPhysicsModel(c_entity.GetId(),                        \
                               *pcPhysModel);                           \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         AddPhysicsModel(c_engine.GetId(), *pcPhysModel);               \
      return SOperationOutcome(true);                                   \
   }                                                                    \
   };                                                                   \
   REGISTER_POINTMASS3D_OPERATION(CPointMass3DOperationAddEntity,       \
                                  CPointMass3DOperationAdd ## SPACE_ENTITY, \
                                  SPACE_ENTITY);

#define REGISTER_STANDARD_POINTMASS3D_OPERATION_REMOVE_ENTITY(SPACE_ENTITY) \
   class CPointMass3DOperationRemove ## SPACE_ENTITY : public CPointMass3DOperationRemoveEntity { \
   public:                                                              \
   CPointMass3DOperationRemove ## SPACE_ENTITY() {}                     \
   virtual ~CPointMass3DOperationRemove ## SPACE_ENTITY() {}            \
   SOperationOutcome ApplyTo(CPointMass3DEngine& c_engine,              \
                SPACE_ENTITY& c_entity) {                               \
      c_engine.RemovePhysicsModel(c_entity.GetId());                    \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         RemovePhysicsModel(c_engine.GetId());                          \
      return SOperationOutcome(true);                                   \
   }                                                                    \
   };                                                                   \
   REGISTER_POINTMASS3D_OPERATION(CPointMass3DOperationRemoveEntity,    \
                                  CPointMass3DOperationRemove ## SPACE_ENTITY, \
                                  SPACE_ENTITY);

#define REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(SPACE_ENTITY, PM3D_ENTITY) \
   REGISTER_STANDARD_POINTMASS3D_OPERATION_ADD_ENTITY(SPACE_ENTITY, PM3D_ENTITY) \
   REGISTER_STANDARD_POINTMASS3D_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)

   /****************************************/
   /****************************************/

}

#endif
