/**
 * @file <argos3/core/simulator/entity/controllable_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CONTROLLABLE_ENTITY_H
#define CONTROLLABLE_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CSimulatedActuator;
   class CSimulatedSensor;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/ray3.h>

namespace argos {

   /**
    * An entity that contains a pointer to the user-defined controller.
    * @see CComposableEntity
    */
   class CControllableEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      /** A vector of controllable entities */
      typedef std::vector<CControllableEntity*> TVector;

      /** A map of controllable entities, indexed by id */
      typedef std::map<std::string, CControllableEntity*> TMap;

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method. After the
       * execution of this constructor, the controller has not been associated
       * to this entity yet.
       * @param pc_parent The parent of this entity.
       */
      CControllableEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone. 
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen. After the
       * execution of this constructor, a new controller instance must be associated
       * to this entity using SetController().
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       * @throws CARGoSException if an error occurs in the creation of the controller
       * @see SetController()
       */
      CControllableEntity(CComposableEntity* pc_parent,
                          const std::string& str_id);

      /**
       * Class destructor.
       */
      virtual ~CControllableEntity();

      /**
       * Initializes the state of the entity from the XML configuration tree.
       * After the execution of this constructor, a new controller instance is
       * associated to this entity.
       * @throws CARGoSException if an error occurs in the creation of the controller
       * @see CCI_Controller
       * @see SetController()
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * Resets the state of the entity to whatever it was after Init() or one of the standalone constructors was called.
       */
      virtual void Reset();

      /**
       * Destroys the entity, undoing whatever was done by Init() or by the standalone constructor.
       */
      virtual void Destroy();

      /**
       * Returns a reference to the associated controller.
       * @return A reference to the associated controller.
       * @throws CARGoSException If no controller has been associated.
       * @see CCI_Controller
       */
      const CCI_Controller& GetController() const;

      /**
       * Returns a reference to the associated controller.
       * @return A reference to the associated controller.
       * @throws CARGoSException If no controller has been associated.
       * @see CCI_Controller
       */
      CCI_Controller& GetController();

      /**
       * Creates and assigns a controller with the given id.
       * As part of the creation of the controller, all the sensors and actuators are
       * also created and initialized. For each sensor and actuator, this method
       * calls <tt>SetRobot()</tt> and <tt>Init()</tt>, in this order.
       * This method passes to CCI_Controller::Init() the <tt>&lt;params&gt;</tt>
       * section found in the controller section.
       * @param str_controller_id The id of the controller as specified in the XML configuration file
       * @throws CARGoSException if an error occurs in the creation of the controller
       * @see CCI_Controller::Init()
       * @see CSimulatedSensor::SetRobot()
       * @see CSimulatedActuator::SetRobot()
       * @see CCI_Sensor::Init()
       * @see CCI_Actuator::Init()
       */
      void SetController(const std::string& str_controller_id);

      /**
       * Creates and assigns a controller with the given id and the given XML parameters.
       * As part of the creation of the controller, all the sensors and actuators are
       * also created and initialized. For each sensor and actuator, this method
       * calls <tt>SetRobot()</tt> and <tt>Init()</tt>, in this order.
       * This method passes the given XML tree to CCI_Controller::Init()
       * instead of that found in the controller XML section.
       * @param str_controller_id The id of the controller as specified in the XML configuration file
       * @param t_controller_config An XML tree to pass to CCI_Controller::Init()
       * @throws CARGoSException if an error occurs in the creation of the controller
       * @see CCI_Controller::Init()
       * @see CSimulatedSensor::SetRobot()
       * @see CSimulatedActuator::SetRobot()
       * @see CCI_Sensor::Init()
       * @see CCI_Actuator::Init()
       */
      void SetController(const std::string& str_controller_id,
                         TConfigurationNode& t_controller_config);

      /**
       * Executes the CSimulatedSensor::Update() method for all associated sensors.
       * In addition, it clears the list of rays and intersection points.
       * @see CSimulatedSensor
       * @see m_vecCheckedRays;
       * @see m_vecIntersectionPoints;
       */
      virtual void Sense();

      /**
       * Executes CCI_Controller::ControlStep().
       * @throws CARGoSException If no controller has been associated.
       * @see CCI_Controller
       */
      virtual void ControlStep();

      /**
       * Executes the CSimulatedActuator::Update() method for all associated actuators.
       * @see CSimulatedActuator
       */
      virtual void Act();

      virtual std::string GetTypeDescription() const {
         return "controller";
      }

      /**
       * Adds a ray to the list of checked rays.
       * This method is typically called by sensors to show which rays were shot
       * and whether they intersected an object or not. These rays can then be
       * drawn by a visualization (such as the Qt-OpenGL one) or analyzed for
       * debugging purposes.
       * @param b_obstructed <tt>true</tt> if the ray intersects an object.
       * @param c_ray The actual ray.
       * @see CRay3
       * @see AddIntersectionPoint()
       */
      inline void AddCheckedRay(bool b_obstructed,
                                const CRay3& c_ray) {
         m_vecCheckedRays.push_back(std::make_pair(b_obstructed, c_ray));
      }

      /**
       * Adds an intersection point to the list.
       * This method is typically called by sensors to show which rays were shot,
       * whether they intersected an object or not and where the intersection occurred.
       * Rays and intersection points can be drawn by a visualization (such as the Qt-OpenGL one)
       * or analyzed for debugging purposes.
       * @see AddCheckedRay()
       */
      inline void AddIntersectionPoint(const CRay3& c_ray,
                                       Real f_t_on_ray) {
         CVector3 cPoint;
         c_ray.GetPoint(cPoint, f_t_on_ray);
         m_vecIntersectionPoints.push_back(cPoint);
      }

      /**
       * Returns the list of checked rays.
       * @return The list of checked rays.
       * @see AddCheckedRay()
       * @see AddIntersectionPoint()
       */
      inline std::vector<std::pair<bool, CRay3> >& GetCheckedRays() {
         return m_vecCheckedRays;
      }

      /**
       * Returns the list of intersection points.
       * @return The list of intersection points.
       * @see AddCheckedRay()
       * @see AddIntersectionPoint()
       */
      inline std::vector<CVector3>& GetIntersectionPoints() {
         return m_vecIntersectionPoints;
      }

   protected:

      /** The pointer to the associated controller */
      CCI_Controller* m_pcController;

      /** The map of actuators, indexed by actuator type (not implementation!) */
      std::map<std::string, CSimulatedActuator*> m_mapActuators;

      /** The map of sensors, indexed by sensor type (not implementation!) */
      std::map<std::string, CSimulatedSensor*> m_mapSensors;

      /** The list of checked rays */
      std::vector<std::pair<bool, CRay3> > m_vecCheckedRays;

      /** The list of intersection points */
      std::vector<CVector3> m_vecIntersectionPoints;

   };

}

#endif
