/**
 * @file <argos3/core/simulator/actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

namespace argos {
   class CActuator;
   class CEntity;
}

#include <argos3/core/control_interface/ci_actuator.h>

#include <string>
#include <map>

namespace argos {

   class CActuator : virtual public CCI_Actuator {

   public:

      virtual ~CActuator() {}

      virtual CEntity& GetEntity() = 0;

      virtual void SetEntity(CEntity& c_entity) = 0;

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
   REGISTER_SYMBOL(CActuator,                               \
                   CLASSNAME,                               \
                   LABEL "$$" IMPLEMENTATION,               \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif
