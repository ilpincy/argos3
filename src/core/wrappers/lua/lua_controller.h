/**
 * @file <argos3/core/wrappers/lua/lua_controller.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/rng.h>

extern "C" {
#include <lua.h>
}

namespace argos {

   class CLuaController : public CCI_Controller {

   public:

      CLuaController();

      virtual ~CLuaController();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void ControlStep();

      virtual void Reset();

      virtual void Destroy();

      inline lua_State* GetLuaState() {
         return m_ptLuaState;
      }

      virtual void SetLuaScript(const std::string& str_script,
                                TConfigurationNode& t_tree);

      virtual void SetLuaScript(const std::string& str_script);

      virtual void CreateLuaState();

      virtual void SensorReadingsToLuaState();

      virtual void ParametersToLuaState(TConfigurationNode& t_tree);

      inline bool IsOK() const {
         return m_bIsOK;
      }

      std::string GetErrorMessage();

   private:

      lua_State* m_ptLuaState;
      std::string m_strScriptFileName;
      bool m_bScriptActive;
      bool m_bIsOK;
      CRandom::CRNG* m_pcRNG;

   };

}
