
/**
 * @file <argos3/testing/experiment/test_loop_functions.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "test_loop_functions.h"
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/simulator/entities/led_entity.h>

/****************************************/
/****************************************/

void CTestLoopFunctions::Init(TConfigurationNode& t_tree) {
   LOG << "CTestLoopFunctions init running!\n";
   CFootBotEntity& fb = dynamic_cast<CFootBotEntity&>(GetSpace().GetEntity("fb"));
   CLEDEntity& cLedA = fb.GetComponent<CLEDEntity>("leds.led[led_2]");
   CLEDEntity& cLedB = fb.GetComponent<CLEDEntity>("leds.led[led_3]");
   CLEDEntity& cLedC = fb.GetComponent<CLEDEntity>("leds.led[led_4]");
   
   cLedA.SetColor(CColor::GREEN);
   cLedB.SetColor(CColor::BLUE);
   cLedC.SetColor(CColor::YELLOW);
}

CColor CTestLoopFunctions::GetFloorColor(const CVector2& c_pos_on_floor) {
   if(Sign(c_pos_on_floor.GetX()) == Sign(c_pos_on_floor.GetY()))
      return CColor::GRAY30;
   else
      return CColor::GRAY70;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_lf");
