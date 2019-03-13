/**
 * @file <argos3/core/simulator/actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

namespace argos {
   class CComposableEntity;
}

#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /**
    * The basic interface for a simulated actuator.
    * To implement the simulated model of an actuator, you must define a class
    * that inherits from both the control interface and this class.
    * @see CCI_Actuator
    */
   class CSimulatedActuator {

   public:

      /**
       * Class destructor.
       */
      virtual ~CSimulatedActuator() {}

      /**
       * Sets the entity associated to this actuator.
       * When a controller is created, all the sensors and actuators are created, too.
       * This function is called right before the <tt>Init()</tt> method of the actuator
       * is called.
       * @param c_entity The entity to associate to this actuator.
       * @see CComposableEntity
       * @see CControllableEntity::SetController()
       * @see CCI_Actuator::Init()
       */
      virtual void SetRobot(CComposableEntity& c_entity) = 0;

      /**
       * Updates the state of the entity associated to this actuator.
       */
      virtual void Update() = 0;

   };

}

/**
 * Registers a new actuator model inside ARGoS.
 * You must register your actuator model for ARGoS to be able to recognize it.
 * This statement must be included in a .cpp file. It can't be in a header.
 */
#define REGISTER_ACTUATOR(CLASSNAME,                        \
                          LABEL,                            \
                          IMPLEMENTATION,                   \
                          AUTHOR,                           \
                          VERSION,                          \
                          BRIEF_DESCRIPTION,                \
                          LONG_DESCRIPTION,                 \
                          STATUS)                           \
   REGISTER_SYMBOL(CSimulatedActuator,                      \
                   CLASSNAME,                               \
                   LABEL " (" IMPLEMENTATION ")",           \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif
