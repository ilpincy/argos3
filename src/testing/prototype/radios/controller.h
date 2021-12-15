/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */

namespace argos {
   class CCI_SimpleRadiosActuator;
}

#include <argos3/core/control_interface/ci_controller.h>

namespace argos {

   class CTestController : public CCI_Controller {

   public:

      CTestController() :
         m_pcSimpleRadioActuator(nullptr) {}

      virtual ~CTestController() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void ControlStep() override;

   private:

      CCI_SimpleRadiosActuator* m_pcSimpleRadioActuator;

   };
}
