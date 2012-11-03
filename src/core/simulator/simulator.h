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
 * @file argos3/core/simulator/simulator.h
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
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <string>
#include <map>

/**
 * @brief The namespace containing all the ARGoS related code.
 *
 * The namespace containing all the ARGoS related code.
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

      /** A reference to the visualization */
      CVisualization* m_pcVisualization;

      /** The vector of physics engines */
      CPhysicsEngine::TMap m_mapPhysicsEngines;
      CPhysicsEngine::TVector m_vecPhysicsEngines;

      /** The space */
      CSpace* m_pcSpace;

      /** Reference to the loop functions */
      CLoopFunctions* m_pcLoopFunctions;

      /** The file name of the configuration XML file */
      std::string m_strExperimentConfigFileName;

      /** The maximum simulation clock */
      UInt32 m_unMaxSimulationClock;

      /** The random seed used for initialisation */
      UInt32 m_unRandomSeed;

      /** The random number generator */
      CRandom::CRNG* m_pcRNG;

      /** The flag to remember if the random seed was set at the beginning or init with the time of the day  */
      bool m_bWasRandomSeedSet;

      /** XML configuration file */
      ticpp::Document m_tConfiguration;

      /** XML configuration file root node */
      TConfigurationNode m_tConfigurationRoot;

      /** Directory pointed to by ARGOSINSTALLDIR */
      std::string m_strInstallationDirectory;

      /** The number of parallel threads to split up the computation */
      UInt32 m_unThreads;

      /** Profiler class */
      CProfiler* m_pcProfiler;

      /** Profiler format */
      bool m_bHumanReadableProfile;

   private:

      CSimulator();

      CSimulator(const CSimulator&) {}

      CSimulator& operator=(const CSimulator&) {
         return *this;
      }

   public:

      ~CSimulator();

      static CSimulator& GetInstance();

      inline CSpace& GetSpace() const {
         return *m_pcSpace;
      }

      inline CPhysicsEngine& GetPhysicsEngine(const std::string& str_id) const {
         CPhysicsEngine::TMap::const_iterator it = m_mapPhysicsEngines.find(str_id);
         ARGOS_ASSERT(it != m_mapPhysicsEngines.end(), "Physics engine \"" << str_id << "\" not found.")
         return *(it->second);
      }

      inline CPhysicsEngine::TVector& GetPhysicsEngines() {
         return m_vecPhysicsEngines;
      }

      inline CVisualization& GetVisualization() {
         ARGOS_ASSERT(m_pcVisualization != NULL, "No visualization specified in the XML file.");
         return *m_pcVisualization;
      }

      inline TConfigurationNode& GetConfigurationRoot() {
         return m_tConfigurationRoot;
      }

      inline CProfiler& GetProfiler() {
         return *m_pcProfiler;
      }

      inline bool IsProfiling() const {
         return m_pcProfiler != NULL;
      }

      void LoadExperiment();

      void Init();

      void Reset();

      inline void Reset(UInt32 un_new_random_seed) {
         SetRandomSeed(un_new_random_seed);
         Reset();
      }

      void Destroy();

      void Execute();

      void UpdateSpace();

      bool IsExperimentFinished() const;
      
      inline UInt32 GetRandomSeed() const {
         return m_unRandomSeed;
      }

      inline void SetRandomSeed(UInt32 un_random_seed) {
         m_unRandomSeed = un_random_seed;
         m_bWasRandomSeedSet = true;
      }

      inline CRandom::CRNG* GetRNG() {
         return m_pcRNG;
      }

      inline const std::string& GetExperimentFileName() const {
         return m_strExperimentConfigFileName;
      }

      inline void SetExperimentFileName(const std::string& str_file_name) {
         m_strExperimentConfigFileName = str_file_name;
      }

      inline CLoopFunctions& GetLoopFunctions() {
         return *m_pcLoopFunctions;
      }

      inline void SetLoopFunctions(CLoopFunctions& c_loop_functions) {
         m_pcLoopFunctions = &c_loop_functions;
      }

      inline UInt32 GetMaxSimulationClock() const {
         return m_unMaxSimulationClock;
      }

      inline void SetInstallationDirectory(const std::string& str_installation_dir) {
         m_strInstallationDirectory = str_installation_dir;
      }

      inline const std::string& GetInstallationDirectory() const {
         return m_strInstallationDirectory;
      }

      inline UInt32 GetNumThreads() const {
         return m_unThreads;
      }

   private:

      void InitFramework(TConfigurationNode& t_tree);
      void InitSpace(TConfigurationNode& t_tree);
      void InitPhysics(TConfigurationNode& t_tree);
      void InitVisualization(TConfigurationNode& t_tree);
      void InitPhysicsEntitiesMapping(TConfigurationNode& t_tree);
   };

}

#endif
