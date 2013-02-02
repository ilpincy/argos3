/**
 * @file <argos3/core/simulator/actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

namespace argos {
   class CAbstractSimulatedActuator;
   class CEntity;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CAbstractSimulatedActuator : virtual public CCI_Actuator {

   public:

      virtual ~CAbstractSimulatedActuator() {}
      virtual CEntity& GetEntity() = 0;
      virtual void SetEntity(CEntity& c_entity) = 0;
      virtual void Update() = 0;
   };

   /****************************************/
   /****************************************/

   template<class ENTITY>
   class CSimulatedActuator : public CAbstractSimulatedActuator {

   public:

      CSimulatedActuator() :
         m_pcEntity(NULL) {}

      virtual ~CSimulatedActuator() {}

      virtual ENTITY& GetEntity() {
         return *m_pcEntity;
      }

      virtual void SetEntity(CEntity& c_entity) {
         m_pcEntity = dynamic_cast<ENTITY*>(&c_entity);
         if(m_pcEntity == NULL) {
            THROW_ARGOSEXCEPTION("Cannot associate an actuator to a robot of type \"" << c_entity.GetTypeDescription() << "\"");
         }
      }

   private:

      ENTITY* m_pcEntity;

   };

   /****************************************/
   /****************************************/

}

#define REGISTER_ACTUATOR(CLASSNAME,                        \
                          LABEL,                            \
                          IMPLEMENTATION,                   \
                          AUTHOR,                           \
                          VERSION,                          \
                          BRIEF_DESCRIPTION,                \
                          LONG_DESCRIPTION,                 \
                          STATUS)                           \
   REGISTER_SYMBOL(CAbstractSimulatedActuator,              \
                   CLASSNAME,                               \
                   LABEL " (" IMPLEMENTATION ")",           \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif
