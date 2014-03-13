#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>

using namespace argos;

class CTestEyeBotController : public CCI_Controller {

public:

   CTestEyeBotController();
   virtual ~CTestEyeBotController();
   virtual void Init(TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual void ControlStep();

private:

   CCI_RangeAndBearingActuator* m_pcRABAct;
   CCI_RangeAndBearingSensor* m_pcRABSens;

};
