/**
 * @file <argos3/core/control_interface/ci_controller.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_CONTROLLER_H
#define CCI_CONTROLLER_H

namespace argos {
   class CCI_Controller;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/plugins/factory.h>

#include <map>
#include <string>
#include <cxxabi.h>
#include <typeinfo>

namespace argos {

   /**
    * The basic interface for a robot controller.
    */
   class CCI_Controller : public CBaseConfigurableResource {

   public:

      /**
       * Class destructor.
       */
      virtual ~CCI_Controller();

      /**
       * Initializes the controller.
       * You should always perform all your memory allocation and configuration in this method,
       * and not in the constructor.
       * The default implementation of this method does nothing.
       * @param t_node The <tt>&lt;params&gt;</tt> XML tree associated to this controller.
       * @see Reset()
       * @see Destroy()
       */
      virtual void Init(TConfigurationNode& t_node) {}

      /**
       * Executes a control step.
       * The logic of your controller goes here.
       * The default implementation does nothing.
       */
      virtual void ControlStep() {}

      /**
       * Resets the state of the controller to what it was right after Init() was executed.
       * Note that the state of the robot, as well as the state of sensors and actuators
       * is automatically reset by ARGoS outside this class.
       * The default implementation of this method does nothing.
       * @see Init()
       * @see Destroy()
       */
      virtual void Reset() {}

      /**
       * The default implementation of this method does nothing.
       * @see Init()
       * @see Reset()
       */
      virtual void Destroy() {}

      /**
       * Returns the id of the robot associated to this controller.
       * @return The id of the robot associated to this controller.
       */
      inline const std::string& GetId() const {
         return m_strId;
      }

      /**
       * Sets the id of the robot associated to this controller.
       * It is used internally by ARGoS to set the robot id. You should
       * never use this function inside your controller.
       * @param str_id The new robot id.
       */
      inline void SetId(const std::string& str_id) {
         m_strId = str_id;
      }

      /**
       * Returns a pointer to the an actuator, given its type.
       * Internally, ARGoS creates an actuator object for each actuator
       * listed in the <tt>&lt;actuators&gt;</tt> section of the XML file.
       * This method allows you to get a pointer to a specific actuator listed in the XML.
       * The type corresponds to the XML tag of an actuator (not its
       * <tt>implementation</tt> attribute!).
       * @param str_actuator_type The wanted actuator type.
       * @return A pointer to the wanted actuator.
       * @throws CARGoSException if the actuator type was not found among those listed in the XML, or if the actual actuator object cannot be cast to the target class
       * @see HasActuator()
       * @see GetSensor()
       */
      template<typename ACTUATOR_IMPL>
      ACTUATOR_IMPL* GetActuator(const std::string& str_actuator_type) {
         CCI_Actuator::TMap::const_iterator it = m_mapActuators.find(str_actuator_type);
         if (it != m_mapActuators.end()) {
            ACTUATOR_IMPL* pcActuator = dynamic_cast<ACTUATOR_IMPL*>(it->second);
            if(pcActuator != NULL) {
               return pcActuator;
            }
            else {
               char* pchDemangledType = abi::__cxa_demangle(typeid(ACTUATOR_IMPL).name(), NULL, NULL, NULL);
               THROW_ARGOSEXCEPTION("Actuator type " << str_actuator_type << " cannot be cast to type " << pchDemangledType);
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown actuator type " << str_actuator_type << " requested in controller. Did you add it to the XML file?");
         }
      }

      /**
       * Returns a pointer to the an sensor, given its type.
       * Internally, ARGoS creates an sensor object for each sensor
       * listed in the <tt>&lt;sensors&gt;</tt> section of the XML file.
       * This method allows you to get a pointer to a specific sensor listed in the XML.
       * The type corresponds to the XML tag of an sensor (not its
       * <tt>implementation</tt> attribute!).
       * @param str_sensor_type The wanted sensor type.
       * @return A pointer to the wanted sensor.
       * @throws CARGoSException if the sensor type was not found among those listed in the XML, or if the actual sensor object cannot be cast to the target class
       * @see HasSensor()
       * @see GetActuator()
       */
      template<typename SENSOR_IMPL>
      SENSOR_IMPL* GetSensor(const std::string& str_sensor_type) {
         CCI_Sensor::TMap::const_iterator it = m_mapSensors.find(str_sensor_type);
         if (it != m_mapSensors.end()) {
            SENSOR_IMPL* pcSensor = dynamic_cast<SENSOR_IMPL*>(it->second);
            if(pcSensor != NULL) {
               return pcSensor;
            }
            else {
               char* pchDemangledType = abi::__cxa_demangle(typeid(SENSOR_IMPL).name(), NULL, NULL, NULL);
               THROW_ARGOSEXCEPTION("Sensor type " << str_sensor_type << " cannot be cast to type " << pchDemangledType);
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown sensor type " << str_sensor_type << " requested in controller. Did you add it to the XML file?");
         }
      }

      /**
       * Returns <tt>true</tt> if an actuator with the passed type is associated to this controller.
       * @param str_actuator_type The type of the wanted actuator.
       * @return <tt>true</tt> if an actuator with the passed type is associated to this controller.
       * @see GetActuator()
       */
      bool HasActuator(const std::string& str_actuator_type) const;

      /**
       * Returns <tt>true</tt> if an sensor with the passed type is associated to this controller.
       * @param str_sensor_type The type of the wanted sensor.
       * @return <tt>true</tt> if an sensor with the passed type is associated to this controller.
       * @see GetSensor()
       */
      bool HasSensor(const std::string& str_sensor_type) const;

      /**
       * Returns a map of the associated actuators.
       * @return A map of the associated actuators.
       */
      inline CCI_Actuator::TMap& GetAllActuators() {
    	  return m_mapActuators;
      }

      /**
       * Returns a map of the associated sensors.
       * @return A map of the associated sensors.
       */
      inline CCI_Sensor::TMap& GetAllSensors() {
    	  return m_mapSensors;
      }

      /**
       * Adds an actuator to this controller.
       * This method is called internally by ARGoS. You should never call it in your controllers.
       * @param str_actuator_type The type of the actuator to add.
       * @param pc_actuator A pointer to the actuator to add.
       * @see CCI_Actuator
       */
      inline void AddActuator(const std::string& str_actuator_type,
                              CCI_Actuator* pc_actuator) {
         m_mapActuators[str_actuator_type] = pc_actuator;
      }

      /**
       * Adds an sensor to this controller.
       * This method is called internally by ARGoS. You should never call it in your controllers.
       * @param str_sensor_type The type of the sensor to add.
       * @param pc_sensor A pointer to the sensor to add.
       * @see CCI_Sensor
       */
      inline void AddSensor(const std::string& str_sensor_type,
                            CCI_Sensor* pc_sensor) {
         m_mapSensors[str_sensor_type] = pc_sensor;
      }

   protected:

      /** A map containing all the actuators associated to this controller */
      CCI_Actuator::TMap m_mapActuators;

      /** A map containing all the sensors associated to this controller */
      CCI_Sensor::TMap m_mapSensors;

      /** The id of the robot associated to this controller  */
      std::string m_strId;

   };

}

/**
 * Registers a new controller inside ARGoS.
 * You must register your controller for ARGoS to be able to recognize it.
 * This statement must be included in a .cpp file. It can't be in a header.
 */
#ifdef ARGOS_DYNAMIC_LIBRARY_LOADING

#define REGISTER_CONTROLLER(CLASSNAME, LABEL) \
   REGISTER_SYMBOL(CCI_Controller,            \
                   CLASSNAME,                 \
                   LABEL,                     \
                   "undefined",               \
                   "undefined",               \
                   "undefined",               \
                   "undefined",               \
                   "undefined")

#else

extern "C" {
   extern argos::CCI_Controller* ControllerMaker(const std::string& str_label);
}

#define REGISTER_CONTROLLER(CLASSNAME, LABEL)                           \
   extern "C" {                                                         \
      argos::CCI_Controller* ControllerMaker(const std::string& str_label) { \
         if(str_label != LABEL) {                                       \
            THROW_ARGOSEXCEPTION("Controller label \"" <<               \
                                 str_label <<                           \
                                 "\" does not match the registered one: \"" << \
                                 LABEL << "\"");                        \
         }                                                              \
         return new CLASSNAME;                                          \
      }                                                                 \
   }

#endif

#endif
