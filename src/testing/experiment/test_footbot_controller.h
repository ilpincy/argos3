#include <argos3/core/control_interface/ci_controller.h>

using namespace argos;

class CTestFootBotController : public CCI_Controller {

public:

   CTestFootBotController();
   virtual ~CTestFootBotController();
   virtual void Init(TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual void ControlStep();

};
