/**
 * @file <argos3/testing/experiment/test_footbot_controller.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <argos3/core/utility/math/vector2.h>

using namespace argos;

class CTestFootBotController : public CCI_Controller {

public:

   CTestFootBotController();
   virtual ~CTestFootBotController();
   virtual void Init(TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual void ControlStep();

private:

   CCI_DifferentialSteeringActuator* m_pcWheels;
   CCI_RangeAndBearingActuator* m_pcRABAct;
   CCI_FootBotProximitySensor* m_pcProximity;
   CCI_RangeAndBearingSensor* m_pcRABSens;

   CVector2 m_cAccumulator;
   CRadians m_cAngle;

   CRange<CRadians> m_cGoStraightRange;
   Real m_fDelta;

};
