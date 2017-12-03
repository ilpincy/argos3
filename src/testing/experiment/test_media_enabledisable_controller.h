#ifndef TEST_MEDIA_ENABLEDISABLE_CONTROLLER_H
#define TEST_MEDIA_ENABLEDISABLE_CONTROLLER_H

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_light_sensor.h>

using namespace argos;

class CTestMediaEnableDisableController : public CCI_Controller {

public:

   CTestMediaEnableDisableController();
   virtual ~CTestMediaEnableDisableController();
   virtual void Init(TConfigurationNode& t_node);
   virtual void ControlStep();
   virtual void Reset();
   virtual void Destroy();

private:

   CCI_LEDsActuator* m_pcLEDs;
   CCI_RangeAndBearingActuator*  m_pcRABA;
   CCI_RangeAndBearingSensor* m_pcRABS;
   CCI_FootBotLightSensor* m_pcLight;
};

#endif
