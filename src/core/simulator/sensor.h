/**
 * @file <argos3/core/simulator/sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SENSOR_H
#define SENSOR_H

namespace argos {
   class CComposableEntity;
}

namespace argos {

   /**
    * The basic interface for a simulated sensor.
    * To implement the simulated model of an sensor, you must define a class
    * that inherits from both the control interface and this class.
    * @see CCI_Sensor
    */
   class CSimulatedSensor {

   public:

      /**
       * Class destructor.
       */
      virtual ~CSimulatedSensor() {}

      /**
       * Sets the entity associated to this sensor.
       * @param c_entity The entity to associate to this sensor.
       * @see CComposableEntity
       */
      virtual void SetRobot(CComposableEntity& c_entity) = 0;

      /**
       * Updates the state of the entity associated to this sensor.
       */
      virtual void Update() = 0;

   };

   /****************************************/
   /****************************************/

}

/**
 * Registers a new sensor model inside ARGoS.
 * You must register your sensor model for ARGoS to be able to recognize it.
 * This statement must be included in a .cpp file. It can't be in a header.
 */
#define REGISTER_SENSOR(CLASSNAME,                          \
                        LABEL,                              \
                        IMPLEMENTATION,                     \
                        AUTHOR,                             \
                        VERSION,                            \
                        BRIEF_DESCRIPTION,                  \
                        LONG_DESCRIPTION,                   \
                        STATUS)                             \
   REGISTER_SYMBOL(CSimulatedSensor,                        \
                   CLASSNAME,                               \
                   LABEL " (" IMPLEMENTATION ")",           \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif
