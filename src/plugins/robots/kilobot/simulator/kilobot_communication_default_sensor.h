/**
 * @file <argos3/plugins/robots/kilobot/control_interface/kilobot_communication_default_sensor.h>
 *
 * @brief This file provides the definition of the kilobot communicationdefault sensor.
 *
 * This file provides the definition of the kilobot communicationdefault sensor.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef KILOBOT_COMMUNICATION_DEFAULT_SENSOR_H
#define KILOBOT_COMMUNICATION_DEFAULT_SENSOR_H

namespace argos {
   class CKilobotCommunicationDefaultSensor;
   class CKilobotCommunicationEntity;
   class CKilobotCommunicationMedium;
   class CControllableEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_sensor.h>

namespace argos {

   class CKilobotCommunicationDefaultSensor : public CCI_KilobotCommunicationSensor,
                                              public CSimulatedSensor {

   public:

      CKilobotCommunicationDefaultSensor();
      virtual ~CKilobotCommunicationDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();
      virtual void Destroy();

   private:

      CKilobotCommunicationEntity* m_pcCommEntity;
      CKilobotCommunicationMedium* m_pcMedium;
      CControllableEntity*         m_pcControllableEntity;
      Real                         m_fDistanceNoiseStdDev;
      CRandom::CRNG*               m_pcRNG;
      CSpace&                      m_cSpace;
      bool                         m_bShowRays;
   };

}

#endif
