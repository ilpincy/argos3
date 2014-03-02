/**
 * @mainpage Autonomous Robots GO Swarming
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 *
 * @version  3.0
 * @see      http://iridia.ulb.ac.be/
 * @see      http://www.swarmanoid.org/
 * @see      http://ascens-ist.eu/
 */

/**
 * @file <argos3/core/simulator/simulator.h>
 *
 * @brief This file provides the definition of class CSimulator, the core class of ARGoS.
 *
 * This file provides class CSimulator, the core class of ARGoS.
 * CSimulator organizes the flow of data coming from the modules of ARGoS
 * and provides the main simulation loop.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

namespace argos {
   class CSimulator;
   class CLoopFunctions;
   class CVisualization;
   class CPhysicsEngine;
   class CMedium;
   class CSpace;
   class CProfiler;
}

#include <argos3/core/config.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/medium/medium.h>
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
       * Returns a reference to a medium.
       * @param str_id The id of the wanted medium.
       * @return A reference to the wanted medium.
       */
      template <typename T>
      T& GetMedium(const std::string& str_id) {
         CMedium::TMap::const_iterator it = m_mapMedia.find(str_id);
         if(it != m_mapMedia.end()) {
            T* pcMedium = dynamic_cast<T*>(it->second);
            if(pcMedium != NULL) {
               return *pcMedium;
            }
            else {
               THROW_ARGOSEXCEPTION("Medium \"" << str_id << "\" can't be converted to the wanted type");
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Medium \"" << str_id << "\" not found.");
         }
      }

      /**
       * Returns the list of currently existing media.
       * @return The list of currently existing media.
       */
      inline CMedium::TVector& GetMedia() {
         return m_vecMedia;
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
       * Returns <tt>true</tt> if the clock tick follows the real time.
       * By default, this flag is <tt>false</tt>.
       */
      inline bool IsRealTimeClock() const {
         return m_bRealTimeClock;
      }

      /**
       * When passed <tt>true</tt>, the clock tick follows the real time.
       * @param b_real_time <tt>true</tt> to make the clock tick follow the real time; <tt>false</tt> otherwise.
       */
      inline void SetRealTimeClock(bool b_real_time) {
         m_bRealTimeClock = b_real_time;
      }

      /**
       * Puts an end to the simulation.
       */
      inline void Terminate() {
         m_bTerminated = true;
      }

      /**
       * Returns the base directory in which the ARGoS core was installed.
       * Usually it is <tt>/usr</tt> or <tt>/usr/local</tt>.
       * @return The directory in which the ARGoS core was installed.
       */
      inline std::string GetInstallationDirectory() const {
         return ARGOS_INSTALL_PREFIX;
      }

      /**
       * Returns the XML portion relative to the controller with the given ID
       */
      TConfigurationNode& GetConfigForController(const std::string& str_id);

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
       * The experiment is considered finished when at least one of these conditions
       * holds true:
       * <ul>
       * <li> a termination was scheduled through CSimulator::Terminate();
       * <li> the simulation clock exceeds the maximum value set in the XML;
       * <li> when CLoopFunctions::IsExperimentFinished() returns <tt>true</tt>.
       * </ul>
       * @return <tt>true</tt> if the experiment has finished.       
       * @see GetMaxSimulationClock()
       * @see Terminate()
       * @see CLoopFunctions::IsExperimentFinished()
       */
      bool IsExperimentFinished() const;
      
   private:

      void InitFramework(TConfigurationNode& t_tree);
      void InitLoopFunctions(TConfigurationNode& t_tree);
      void InitControllers(TConfigurationNode& t_tree);
      void InitSpace(TConfigurationNode& t_tree);
      void InitPhysics(TConfigurationNode& t_tree);
      void InitPhysics2();
      void InitMedia(TConfigurationNode& t_tree);
      void InitMedia2();
      void InitVisualization(TConfigurationNode& t_tree);

   private:

      typedef std::map<std::string, TConfigurationNode*> TControllerConfigurationMap;

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
       * The map <id, reference> of active media.
       */
      CMedium::TMap m_mapMedia;

      /**
       * The vector of active media.
       */
      CMedium::TVector m_vecMedia;

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

      /**
       * <tt>true</tt> when ARGoS must run in real-time; <tt>false</tt> otherwise.
       */
      bool m_bRealTimeClock;

      /**
       * <tt>true</tt> if the simulation must be terminated now.
       */
      bool m_bTerminated;

   };

}

#endif
