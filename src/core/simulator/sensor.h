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

   class CAbstractSimulatedSensor : virtual public CCI_Sensor {

   public:

      virtual ~CAbstractSimulatedSensor() {}
      virtual CEntity& GetEntity() = 0;
      virtual void SetEntity(CEntity& c_entity) = 0;
      virtual void Update() = 0;
   };

   template<class ENTITY>
   class CSimulatedSensor : public CAbstractSimulatedSensor {

   public:

      CSimulatedSensor() :
         m_pcEntity(NULL) {}

      virtual ~CSimulatedSensor() {}

      virtual ENTITY& GetEntity() {
         return *m_pcEntity;
      }

      virtual void SetEntity(CEntity& c_entity) {
         m_pcEntity = dynamic_cast<ENTITY*>(&c_entity);
         if(m_pcEntity == NULL) {
            THROW_ARGOSEXCEPTION("Cannot associate an sensor to a robot of type \"" << c_entity.GetTypeDescription() << "\"");
         }
      }

   private:

      ENTITY* m_pcEntity;

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
   REGISTER_SYMBOL(CAbstractSimulatedSensor,                \
                   CLASSNAME,                               \
                   LABEL " (" IMPLEMENTATION ")",           \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif
