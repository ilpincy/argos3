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

namespace argos {

   class CSimulatedActuator {

   public:

      virtual ~CSimulatedActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity) = 0;

      virtual void Update() = 0;

   };

}

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
