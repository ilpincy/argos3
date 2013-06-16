
/**
 * @file <argos3/testing/experiment/test_loop_functions.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef TEST_LOOP_FUNCTIONS_H
#define TEST_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>

using namespace argos;

class CTestLoopFunctions : public CLoopFunctions {

public:

   void Init(TConfigurationNode& t_tree);

   virtual ~CTestLoopFunctions() {}
   
   virtual CColor GetFloorColor(const CVector2& c_pos_on_floor);

};

#endif
