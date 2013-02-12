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

   class CSimulatedSensor {

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
