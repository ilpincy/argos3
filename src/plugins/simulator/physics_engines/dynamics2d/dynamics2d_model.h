/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_MODEL_H
#define DYNAMICS2D_MODEL_H

namespace argos {
   class CDynamics2DEngine;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {

   /**
    * The base class for models in the dynamics 2D engine.
    * All the models in the dynamics 2D engine inherit from this class, which
    * provides the additional method GetDynamics2DEngine() over the CPhysicsModel
    * interface.
    * @see CPhysicsModel
    * @see CDynamics2DEngine
    */
   class CDynamics2DModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CDynamics2DModel*> TMap;

   public:

      CDynamics2DModel(CDynamics2DEngine& c_engine,
                       CEmbodiedEntity& c_entity) :
         CPhysicsModel(c_engine, c_entity),
         m_cDyn2DEngine(c_engine) {}

      virtual ~CDynamics2DModel() {}

      virtual void Reset() = 0;

      /**
       * Returns the dynamics 2D engine state.
       * @return The dynamics 2D engine state.
       */
      inline CDynamics2DEngine& GetDynamics2DEngine() {
         return m_cDyn2DEngine;
      }

      /**
       * Returns the dynamics 2D engine state as a const reference.
       * @return The dynamics 2D engine state as a const reference.
       */
      inline const CDynamics2DEngine& GetDynamics2DEngine() const {
         return m_cDyn2DEngine;
      }

   private:

      CDynamics2DEngine& m_cDyn2DEngine;

   };

}

#endif
