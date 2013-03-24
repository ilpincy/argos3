
/**
 * @file <argos3/testing/experiment/test_loop_functions.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "test_loop_functions.h"

/****************************************/
/****************************************/

CColor CTestLoopFunctions::GetFloorColor(const CVector2& c_pos_on_floor) {
   if(Sign(c_pos_on_floor.GetX()) == Sign(c_pos_on_floor.GetY()))
      return CColor::GRAY30;
   else
      return CColor::GRAY70;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_lf");
