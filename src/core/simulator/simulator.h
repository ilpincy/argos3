/**
 * @mainpage Autonomous Robots GO Swarming
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 *
 * @version  3.0
 * @see      http://iridia.ulb.ac.be/
 * @see      http://www.swarmanoid.org/
 * @see      http://ascens-ist.eu/
 */

/**
 * @file core/simulator/simulator.h
 *
 * @brief This file provides the definition of class CSimulator, the core class of ARGoS.
 *
 * This file provides class CSimulator, the core class of ARGoS.
 * CSimulator organizes the flow of data coming from the modules of ARGoS
 * and provides the main simulation loop.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

namespace argos {
   class CSimulator;
   class CLoopFunctions;
   class CVisualization;
   class CSpace;
   class CProfiler;
   class CCI_Controller;
   class CControllableEntity;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/control_interface/ci_robot.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/sensor.h>
#include <string>
#include <map>

/**
 * @brief The namespace containing all the ARGoS related code.
 *
 * The namespace containing all the ARGoS-related code.
 */
namespace argos {

   /**
    * @class CSimulator
    *
    * @brief The core class of ARGOS.
    *
    * Class CSimulator is the core class of ARGOS. CSimulator organises the flow
    * of data coming from the modules of ARGoS and provides the main simulation
    * loop.
    *
    */
   class CSimulator {

   private:

      /**
       * Class constructor.
       */
      CSimulator();

      /**
       * Class copy constructor.
       * Left empty because CSimulator is a singleton.
       */
      CSimulator(const CSimulator&) {}

      /**
       * Assignment operator.
       * Left empty because CSimulator is a singleton.
       */
      CSimulator& operator=(const CSimulator&) {
         return *this;
      }

   public:

      /**
       * Class destructor.
       */
      ~CSimulator();

      /**
       * Returns the instance to the CSimulator class.
       * Since CSimulator is a singleton, the instance is created at the first
       * call of this function and returned thereafter.
       * @return The instance to the CSimulator class.
       */
      static CSimulator& GetInstance();

      /**
       * Returns a reference to the simulated space.
       * @return A reference to the simulated space.
       */
      inline CSpace& GetSpace() const {
         return *m_pcSpace;
      }

      /**
       * Returns a reference to a physics engine.
       * @param str_id The id of the wanted physics engine.
       * @return A reference to the wanted physics engine.
       */
      CPhysicsEngine& GetPhysicsEngine(const std::string& str_id) const;

      /**
       * Returns the list of currently existing physics engines.
       * @return The list of currently existing physics engines.
       */
      inline CPhysicsEngine::TVector& GetPhysicsEngines() {
         return m_vecPhysicsEngines;
      }

      /**
       * Returns a reference to the visualization.
       * @return A reference to the visualization.
       */
      inline CVisualization& GetVisualization() {
         ARGOS_ASSERT(m_pcVisualization != NULL, "No visualization specified in the XML file.");
         return *m_pcVisualization;
      }

      /**
       * Returns a reference to the root node of the parsed XML configuration file.
       * @return A reference to the root node of the parsed XML configuration file.
       */
      inline TConfigurationNode& GetConfigurationRoot() {
         return m_tConfigurationRoot;
      }

      /**
       * Returns a reference to the profiler.
       * @return A reference to the profiler.
       */
      inline CProfiler& GetProfiler() {
         return *m_pcProfiler;
      }

      /**
       * Returns <tt>true</tt> if ARGoS is being profiled.
       * @return <tt>true</tt> if ARGoS is being profiled.
       */
      inline bool IsProfiling() const {
         return m_pcProfiler != NULL;
      }

      /**
       * Returns the random seed of the "argos" category of the random seed.
       * @return the random seed of the "argos" category of the random seed.
       * @see CRandom
       */
      inline UInt32 GetRandomSeed() const {
         return m_unRandomSeed;
      }

      /**
       * Sets the random seed of the "argos" category of the random seed.
       * @param un_random_seed sets random seed of the "argos" category of the random seed.
       * @see CRandom
       */
      inline void SetRandomSeed(UInt32 un_random_seed) {
         m_unRandomSeed = un_random_seed;
         m_bWasRandomSeedSet = true;
      }

      /**
       * Returns the random generator of the "argos" category.
       * @return the random generator of the "argos" category.
       * @see CRandom
       */
      inline CRandom::CRNG* GetRNG() {
         return m_pcRNG;
      }

      /**
       * Returns the name of the XML configuration file parsed by Load().
       * @return The name of the XML configuration file parsed by Load().
       * @see Load
       */
      inline const std::string& GetExperimentFileName() const {
         return m_strExperimentConfigFileName;
      }

      /**
       * Sets the name of the XML configuration file parsed by Load().
       * @param str_file_name The name of the XML configuration file parsed by Load().
       * @see Load
       */
      inline void SetExperimentFileName(const std::string& str_file_name) {
         m_strExperimentConfigFileName = str_file_name;
      }

      /**
       * Returns a reference to the loop functions associated to the current experiment.
       * @return A reference to the loop functions associated to the current experiment.
       */
      inline CLoopFunctions& GetLoopFunctions() {
         return *m_pcLoopFunctions;
      }

      /**
       * Asociates loop functions to the current experiment.
       * @param c_loop_functions A reference to the loop functions to associate to the current experiment.
       */
      inline void SetLoopFunctions(CLoopFunctions& c_loop_functions) {
         m_pcLoopFunctions = &c_loop_functions;
      }

      /**
       * Returns the time limit on this experiment.
       * @return The time limit on this experiment.
       */
      inline UInt32 GetMaxSimulationClock() const {
         return m_unMaxSimulationClock;
      }

      /**
       * Returns the number of threads used during the experiment.
       * @return The number of threads used during the experiment.
       */
      inline UInt32 GetNumThreads() const {
         return m_unThreads;
      }

      /**
       * Returns the base directory in which the ARGoS core was installed.
       * Usually it is <tt>/usr</tt> or <tt>/usr/local</tt>.
       * @return The directory in which the ARGoS core was installed.
       */
      inline std::string GetInstallationDirectory() const {
         return INSTALL_PREFIX;
      }

      /**
       * Loads the XML configuration file.
       * The XML configuration file is parsed by this function.
       * The variable m_tConfigurationRoot is set here.
       */
      void LoadExperiment();

      /**
       * Initializes the experiment.
       * It works on the assumption that method LoadExperiment() has been called already.
       */
      void Init();

      /**
       * Resets the experiment.
       * Restores the state of the experiment right after Init() and before any step is executed.
       */
      void Reset();

      /**
       * Resets the experiment.
       * Restores the state of the experiment right after Init() and before any step is executed.
       * This variant changes the random seed to allow for resetting the experiment to a different state.
       * This method is useful, for instance, when embedding ARGoS in an optimization algorithm, and
       * multiple, different runs of an experiment must be executed. This method is faster than destroying
       * and recreating the experiment with a different seed, but it is functionally equivalent.
       * @param un_new_random_seed The new random seed.
       */
      inline void Reset(UInt32 un_new_random_seed) {
         SetRandomSeed(un_new_random_seed);
         Reset();
      }

      /**
       * Undoes whatever was done by Init().
       */
      void Destroy();

      /**
       * Executes the simulation loop.
       */
      void Execute();

      /**
       * Performs an update step of the space.
       */
      void UpdateSpace();

      /**
       * Returns <tt>true</tt> if the experiment has finished.
       * Internally it checks whether the time limit has been reached or
       * the loop functions' IsExperimentFinished() method returned
       * <tt>true</tt>.
       * @return <tt>true</tt> if the experiment has finished.       
       * @see GetMaxSimulationClock()
       * @see CLoopFunctions::IsExperimentFinished()
       */
      bool IsExperimentFinished() const;

      /**
       * Assigns a new controller to an entity.
       * The entity is assumed to be composable, and must have a controllable entity component.
       * @param str_controller_id The id of the controller to create.
       * @param c_entity The entity to which the controller will be assigned.
       */
      template<class ENTITY>
      void AssignController(const std::string& str_controller_id,
                            ENTITY& c_entity) {
         /* Look in the map for the parsed XML configuration of the wanted controller */
         TControllerConfigurationMap::iterator it = m_mapControllerConfig.find(str_controller_id);
         if(it == m_mapControllerConfig.end()) {
            THROW_ARGOSEXCEPTION("Controller id \"" << str_controller_id << "\" not found");
         }
         AssignController(str_controller_id, it->second, c_entity);
      }
      
      /**
       * Assigns a new controller to an entity.
       * The entity is assumed to be composable, and must have a controllable entity component.
       * @param str_controller_id The id of the controller to create.
       * @param t_parameters A parsed XML tree to configure this controller
       * @param c_entity The entity to which the controller will be assigned.
       */
      template<class ENTITY>
      void AssignController(const std::string& str_controller_id,
                            TConfigurationNode& t_parameters,
                            ENTITY& c_entity) {
         /* Look in the map for the parsed XML configuration of the wanted controller */
         TControllerConfigurationMap::iterator itController = m_mapControllerConfig.find(str_controller_id);
         if(itController == m_mapControllerConfig.end()) {
            THROW_ARGOSEXCEPTION("Controller id \"" << str_controller_id << "\" not found");
         }
         /* Now itController->second points to the base section of the wanted controller */
         std::string strImpl;
         /* Create a robot class to contain the sensors and actuators */
         CCI_Robot* pcRobot = new CCI_Robot();
         pcRobot->SetId(c_entity.GetId());
         /* Go through actuators */
         TConfigurationNode& tActuators = GetNode(itController->second, "actuators");
         for(TConfigurationNodeIterator itAct = itAct.begin(&tActuators);
             itAct != itAct.end();
             ++itAct) {
            /* itAct->Value() is the name of the current actuator */
            GetNodeAttribute(*itAct, "implementation", strImpl);
            CCI_Actuator* pcCIAct = CFactory<CCI_Actuator>::New(itAct->Value() + "$$" + strImpl);
            CActuator* pcAct = dynamic_cast<CActuator*>(pcCIAct);
            if(pcAct == NULL) {
               THROW_ARGOSEXCEPTION("[BUG] Actuator <\"" << itAct->Value() << "\", \"" << strImpl << "\"> does not inherit from CActuator");
            }
            pcAct->SetEntity(c_entity);
            pcCIAct->Init(t_parameters);
            pcRobot->AddActuator(itAct->Value(), pcCIAct);
         }
         /* Go through sensors */
         TConfigurationNode& tSensors = GetNode(itController->second, "sensors");
         for(TConfigurationNodeIterator itSens = itSens.begin(&tSensors);
             itSens != itSens.end();
             ++itSens) {
            /* itSens->Value() is the name of the current actuator */
            GetNodeAttribute(*itSens, "implementation", strImpl);
            CCI_Sensor* pcCISens = CFactory<CCI_Sensor>::New(itSens->Value() + "$$" + strImpl);
            CSensor* pcSens = dynamic_cast<CSensor*>(pcCISens);
            if(pcSens == NULL) {
               THROW_ARGOSEXCEPTION("[BUG] Sensor <\"" << itSens->Value() << "\", \"" << strImpl << "\"> does not inherit from CSensor");
            }
            pcSens->SetEntity(c_entity);
            pcCISens->Init(t_parameters);
            pcRobot->AddSensor(itSens->Value(), pcCISens);
         }
         /* Create and configure the controller */
         CCI_Controller* pcController = CFactory<CCI_Controller>::New(itController->second.Value());
         pcController->SetRobot(*pcRobot);
         pcController->SetId(str_controller_id);
         pcController->Init(t_parameters);
         /* Assign the controller to the controllable entity associated to this entity */
         if(!c_entity.HasComponent("controllable_entity")) {
            THROW_ARGOSEXCEPTION("[BUG] Entity \"" << c_entity.GetId() << "\" does not have a controllable entity");
         }
         CControllableEntity& cControllable = c_entity.template GetComponent<CControllableEntity>("controllable_entity");
         cControllable.SetController(*pcController);
      }
      
   private:

      void InitFramework(TConfigurationNode& t_tree);
      void InitLoopFunctions(TConfigurationNode& t_tree);
      void InitControllers(TConfigurationNode& t_tree);
      void InitSpace(TConfigurationNode& t_tree);
      void InitPhysics(TConfigurationNode& t_tree);
      void InitVisualization(TConfigurationNode& t_tree);
      void InitPhysicsEntitiesMapping(TConfigurationNode& t_tree);

   private:

      typedef std::map<std::string, TConfigurationNode&> TControllerConfigurationMap;

   private:

      /**
       * A map <id, XML config> for the controllers.
       */
      TControllerConfigurationMap m_mapControllerConfig;

      /**
       * A reference to the visualization.
       */
      CVisualization* m_pcVisualization;

      /**
       * The map <id, reference> of active physics engines.
       */
      CPhysicsEngine::TMap m_mapPhysicsEngines;

      /**
       * The vector of active physics engines.
       */
      CPhysicsEngine::TVector m_vecPhysicsEngines;

      /**
       * A reference to the simulated space.
       */
      CSpace* m_pcSpace;

      /**
       * A reference to the loop functions.
       */
      CLoopFunctions* m_pcLoopFunctions;

      /**
       * The file name of the configuration XML file.
       */
      std::string m_strExperimentConfigFileName;

      /**
       * The maximum simulation clock.
       */
      UInt32 m_unMaxSimulationClock;

      /**
       * The random seed used for the "argos" CRandom category.
       */
      UInt32 m_unRandomSeed;

      /**
       * The random number generator.
       */
      CRandom::CRNG* m_pcRNG;

      /**
       * Flag to store whether the random seed was set or not.
       * Used in Reset() to know whether the random number generator should be
       * reset to the initial seed or to the user-defined one.
       */
      bool m_bWasRandomSeedSet;

      /**
       * The XML configuration file.
       */
      ticpp::Document m_tConfiguration;

      /**
       * The parsed XML configuration tree.
       */
      TConfigurationNode m_tConfigurationRoot;

      /**
       * The number of parallel threads used for the experiment.
       */
      UInt32 m_unThreads;

      /**
       * Pointer to the profiler class (NULL when profiling is off).
       */
      CProfiler* m_pcProfiler;

      /**
       * Profiler output format: <tt>true</tt> human readable, <tt>false</tt> table format.
       */
      bool m_bHumanReadableProfile;

   };

}

#endif
