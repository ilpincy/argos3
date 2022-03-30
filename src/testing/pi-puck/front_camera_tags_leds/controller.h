/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */

namespace argos {
   class CCI_PiPuckFrontCameraSensor;
}

#include <argos3/core/control_interface/ci_controller.h>

namespace argos {

   class CTestController : public CCI_Controller {

   public:

      CTestController() {}

      virtual ~CTestController() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

   private:
      CCI_PiPuckFrontCameraSensor* m_pcFrontCameraSensor;

   };
}
