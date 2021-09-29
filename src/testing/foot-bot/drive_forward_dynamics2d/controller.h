/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>

namespace argos {

   class CTestController : public CCI_Controller {

   public:

      CTestController() {}

      virtual ~CTestController() {}

      virtual void Init(TConfigurationNode& t_tree);

   private:

   };
}
