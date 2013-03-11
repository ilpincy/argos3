#ifndef TEST_LOOP_FUNCTIONS_H
#define TEST_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>

using namespace argos;

class CTestLoopFunctions : public CLoopFunctions {

public:

   virtual ~CTestLoopFunctions() {}

   virtual CColor GetFloorColor(const CVector2& c_pos_on_floor);

};

#endif
