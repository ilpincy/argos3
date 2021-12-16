/*
 * @file <argos3/plugins/robots/generic/simulator/simple_radios_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef SIMPLE_RADIOS_DEFAULT_ACTUATOR_H
#define SIMPLE_RADIOS_DEFAULT_ACTUATOR_H

namespace argos {
   class CSimpleRadiosDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_simple_radios_actuator.h>
#include <argos3/plugins/simulator/entities/simple_radio_equipped_entity.h>
#include <argos3/plugins/simulator/media/simple_radio_medium.h>

namespace argos {

   class CSimpleRadiosDefaultActuator : public CSimulatedActuator,
                                        public CCI_SimpleRadiosActuator {

   public:

      CSimpleRadiosDefaultActuator();

      virtual ~CSimpleRadiosDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CSimpleRadioEquippedEntity* m_pcSimpleRadioEquippedEntity;

      class CSendOperation : public CPositionalIndex<CSimpleRadioEntity>::COperation {
      public:

         CSendOperation(const CSimpleRadioEntity& c_radio,
                        const std::vector<CByteArray>& c_messages) :
            m_cRadio(c_radio),
            m_cMessages(c_messages) {}

         virtual bool operator()(CSimpleRadioEntity& c_radio);

      private:

         const CSimpleRadioEntity& m_cRadio;
         const std::vector<CByteArray>& m_cMessages;
      };
   };
}

#endif
