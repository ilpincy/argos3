/*
 * @file <argos3/plugins/robots/generic/simulator/magnets_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef MAGNETS_DEFAULT_ACTUATOR_H
#define MAGNETS_DEFAULT_ACTUATOR_H

namespace argos {
   class CMagnetsDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_magnets_actuator.h>
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>

namespace argos {

   class CMagnetsDefaultActuator : public CSimulatedActuator,
                                   public CCI_MagnetsActuator {

   public:

      CMagnetsDefaultActuator();

      virtual ~CMagnetsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      CMagnetEquippedEntity* m_pcMagnetEquippedEntity;

   };
}

#endif
