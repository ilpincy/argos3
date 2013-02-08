/**
 * @file <argos3/core/simulator/sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SENSOR_H
#define SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CAbstractSensor;
   class CEntity;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CSimulatedSensor : virtual public CCI_Sensor {

   public:

      virtual ~CSimulatedSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity) = 0;

      virtual void Update() = 0;

   };

   /****************************************/
   /****************************************/

}

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
