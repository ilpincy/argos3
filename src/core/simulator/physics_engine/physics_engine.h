/**
 * @file <argos3/core/simulator/physics_engine/physics_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

namespace argos {
   class CPhysicsEngine;
   class CPhysicsModel;
   class CEntity;
   class CVector3;
}

#include <map>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   class CPhysicsEngine : public CBaseConfigurableResource {

   public:

      typedef std::vector<CPhysicsEngine*> TVector;
      typedef std::map<std::string, CPhysicsEngine*, std::less<std::string> > TMap;

   public:

      CPhysicsEngine() :
         m_fPhysicsEngineClock(0.0) {}
      virtual ~CPhysicsEngine() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset() {}
      virtual void Destroy() {}

      virtual void Update() = 0;

      virtual bool IsPointContained(const CVector3& c_point) = 0;

      virtual UInt32 GetNumPhysicsEngineEntities() = 0;

      /**
       * Adds an entity to the physics engine.
       * Important: when you implement this function, you must also take care of adding
       * the physics model to the associated embodied entity.
       */
      virtual void AddEntity(CEntity& c_entity) = 0;

      /**
       * Removes an entity from the physics engine.
       * Important: when you implement this function, you must also take care of removing
       * the physics model to the associated embodied entity.
       */
      virtual void RemoveEntity(CEntity& c_entity) = 0;

      /**
       * Returns <tt>true</tt> if this engine has entities that must be transferred to another engine.
       */
      virtual bool IsEntityTransferNeeded() const  = 0;

      /**
       * Executes the transfer of entities to other engines.
       */
      virtual void TransferEntities()  = 0;

      static Real GetSimulationClockTick();
      static Real GetInverseSimulationClockTick();
      static void SetSimulationClockTick(Real f_simulation_clock_tick);

      inline Real GetPhysicsEngineClock() const {
         return m_fPhysicsEngineClock;
      }

      inline void SetPhysicsEngineClock(Real f_physics_engine_clock) {
         m_fPhysicsEngineClock = f_physics_engine_clock;
      }

      inline const std::string& GetId() const {
         return m_strId;
      }
      void SetId(const std::string& str_id) {
         m_strId = str_id;
      }
               
   protected:

      /** The physics engine's id. */
      std::string m_strId;

      /** How long a clock tick lasts (in seconds) */
      static Real m_fSimulationClockTick;

      /** The inverse of m_fSimulationClockTick */
      static Real m_fInverseSimulationClockTick;

      /** The current clock as seen by this physics engine */
      Real m_fPhysicsEngineClock;

   };

}

#define REGISTER_PHYSICS_ENGINE(CLASSNAME,          \
                                LABEL,              \
                                AUTHOR,             \
                                VERSION,            \
                                BRIEF_DESCRIPTION,  \
                                LONG_DESCRIPTION,   \
                                STATUS)             \
   REGISTER_SYMBOL(CPhysicsEngine,                  \
                   CLASSNAME,                       \
                   LABEL,                           \
                   AUTHOR,                          \
                   VERSION,                         \
                   BRIEF_DESCRIPTION,               \
                   LONG_DESCRIPTION,                \
                   STATUS)

#endif
