#include <argos3/core/control_interface/ci_controller.h>

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

      virtual void LoadScript(const std::string& str_filename);

      void DumpGlobals();

   protected:

      virtual void CreateLuaVariables() = 0;

      virtual void SensorReadingsToLuaVariables() = 0;

      virtual void LuaVariablesToActuatorSettings() = 0;

      void CallLuaFunction(const std::string& str_function);

   private:

      lua_State* m_ptLuaState;

   };

}
