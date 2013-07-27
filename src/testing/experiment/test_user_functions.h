#ifndef TEST_USER_FUNCTIONS_H
#define TEST_USER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>

using namespace argos;

class CTestUserFunctions : public CQTOpenGLUserFunctions {

public:

   CTestUserFunctions();
   virtual ~CTestUserFunctions();

   virtual void DrawInWorld();

};

#endif
