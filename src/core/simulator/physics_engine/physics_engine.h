/**
 * @file core/simulator/physics_engine/physics_engine.h
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

namespace argos {
   class CPhysicsEngine;
   class CPhysicsEngineEntity;
   class CEntity;
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

      virtual UInt32 GetNumPhysicsEngineEntities() = 0;
      virtual void AddEntity(CEntity& c_entity) = 0;
      virtual void RemoveEntity(CEntity& c_entity) = 0;
      inline virtual bool IsEntityTransferNeeded() const {
         return false;
      }
      virtual void TransferEntities() {}

      static Real GetSimulationClockTick();
      static Real GetInverseSimulationClockTick();
      static void SetSimulationClockTick(Real f_simulation_clock_tick);

      inline void SetPhysicsEngineClock(Real f_physics_engine_clock) {
         m_fPhysicsEngineClock = f_physics_engine_clock;
      }
      inline Real GetPhysicsEngineClock() const {
         return m_fPhysicsEngineClock;
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
