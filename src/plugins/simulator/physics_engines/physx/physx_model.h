/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_MODEL_H
#define PHYSX_MODEL_H

namespace argos {
   class CPhysXEngine;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_engine.h>

namespace argos {

   /**
    * The base class for models in the PhysX engine.
    * All the models in the PhysX engine inherit from this class, which
    * provides the additional method GetPhysXEngine() over the CPhysicsModel
    * interface.
    * @see CPhysicsModel
    * @see CPhysXEngine
    */
   class CPhysXModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CPhysXModel*> TMap;

   public:

      CPhysXModel(CPhysXEngine& c_engine,
                  CEmbodiedEntity& c_entity) :
         CPhysicsModel(c_engine, c_entity),
         m_cPhysXEngine(c_engine) {}

      virtual ~CPhysXModel() {}

      virtual void Reset() = 0;

      virtual void UpdateFromEntityStatus() = 0;

      /**
       * Returns the PhysX engine state.
       * @return The PhysX engine state.
       */
      inline CPhysXEngine& GetPhysXEngine() {
         return m_cPhysXEngine;
      }

      /**
       * Returns the PhysX engine state as a const reference.
       * @return The PhysX engine state as a const reference.
       */
      inline const CPhysXEngine& GetPhysXEngine() const {
         return m_cPhysXEngine;
      }

   private:

      /** Reference to the PhysX engine state */
      CPhysXEngine& m_cPhysXEngine;

   };

}

#endif
