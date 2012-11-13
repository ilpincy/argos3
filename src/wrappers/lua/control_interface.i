%module argos3_control_interface
%{
#include <argos3/core/control_interface/ci_robot.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/control_interface/ci_actuator.h>
%}

namespace argos {
   class CCI_Sensor : public CBaseConfigurableResource,
                      public CMemento {
   public:
      typedef std::map<std::string, CCI_Sensor*, std::less<std::string> > TMap;
   public:
      virtual ~CCI_Sensor() = 0;
   };

   class CCI_Actuator : public CBaseConfigurableResource,
                        public CMemento {
   public:
      typedef std::map<std::string, CCI_Actuator*, std::less<std::string> > TMap;
   public:
      virtual ~CCI_Actuator() = 0;
   };

   class CCI_Robot {
   public:
      virtual ~CCI_Robot();
      template<typename ACTUATOR_IMPL>
      ACTUATOR_IMPL* GetActuator(const std::string& str_actuator_type);
      template<typename SENSOR_IMPL>
      SENSOR_IMPL* GetSensor(const std::string& str_sensor_type);
      inline const std::string& GetId() const;
      inline void SetId(const std::string& str_id);
   };

   class CCI_Controller : public CBaseConfigurableResource,
                          public CMemento {
   public:
      virtual ~CCI_Controller() {
         if (m_pcRobot != NULL) delete m_pcRobot;
      }
      virtual void Init(argos::TConfigurationNode& t_node) {}
      virtual void ControlStep() {}
      virtual void Reset() {}
      virtual void Destroy() {}

      inline CCI_Robot& GetRobot() {
         return *m_pcRobot;
      }
   };
}
