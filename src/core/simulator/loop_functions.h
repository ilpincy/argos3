/**
 * @file <argos3/core/simulator/loop_functions.h>
 *
 * @brief This file provides the definition of the loop functions.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LOOP_FUNCTIONS_H
#define LOOP_FUNCTIONS_H

namespace argos {
   class CLoopFunctions;
   class CFloorEntity;
   class CPositionalEntity;
   class CEmbodiedEntity;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

   /**
    * A set of hook functions to customize an experimental run.
    * <p>
    * It is very difficult to identify a set of features that can cover all
    * the possible use cases of multi-robot systems.  Even though some
    * features, such as robot motion, are almost always necessary, many
    * other features depend on the type of experiment considered. For
    * instance, the metrics against which statistics must be calculated
    * depend on the experiment. Also, if the environment presents custom
    * dynamics, such as objects being added or removed as a result of the
    * actions of the robots, these mechanisms need to be implemented in the
    * simulator. The need for specific and often divergent features renders
    * the design of a generic simulator extremely complex. Furthermore, the
    * approach of trying to add a myriad of features in the attempt to cover
    * every possible use case usually renders the learning curve of a tool
    * much steeper, hindering usability and maintainability.
    * </p>
    * <p>
    * To cope with these issues, we followed the common approach of
    * providing user-defined function hooks in strategic points of the
    * simulation loop. In ARGoS, these hooks are called <em>loop
    * functions</em>.  The user can customize the initialization and the end of
    * an experiment, and add custom functionality executed before and/or
    * after each simulation step. It is also possible to define custom end
    * conditions for an experiment.
    * </p>
    * <p>
    * Loop functions allow one to access and modify the entire
    * simulation. In this way, the user can collect figures and statistics,
    * and store complex data for later analysis. It is also possible to
    * interact with the simulation by moving, adding or removing entities in
    * the environment, or by changing their internal state.
    * </p>
    * <p>
    * Finally, loop functions can be used to prototype new features before
    * they are promoted to the core ARGoS code.
    * </p>
    */
   class CLoopFunctions : public CBaseConfigurableResource {

   public:

      /**
       * Class constructor.
       */
      CLoopFunctions() :
         m_cSimulator(CSimulator::GetInstance()),
         m_cSpace(m_cSimulator.GetSpace()) {
      }

      /**
       * Class destructor.
       */
      virtual ~CLoopFunctions() {}

      /**
       * Executes user-defined initialization logic.
       * The default implementation of this method does nothing.
       * @param t_tree The <tt>&lt;loop_functions&gt;</tt> XML configuration tree.
       * @see Reset()
       * @see Destroy()
       */
      virtual void Init(TConfigurationNode& t_tree) {}

      /**
       * Executes user-defined reset logic.
       * This method should restore the state of the simulation at it was right
       * after Init() was called.
       * The default implementation of this method does nothing.
       * @see Init()
       */
      virtual void Reset() {}

      /**
       * Executes user-defined destruction logic.
       * This method should undo whatever is done in Init().
       * The default implementation of this method does nothing.
       * @see Init()
       */
      virtual void Destroy() {}

      /**
       * Executes user-defined logic right before a control step is executed.
       * This function is executed before the sensors are updated for the current time step.
       * The default implementation of this method does nothing.
       * @see PostStep()
       */
      virtual void PreStep() {}

      /**
       * Executes user-defined logic right after a control step is executed.
       * This function is executed before the actuators are updated for the next time step.
       * The default implementation of this method does nothing.
       * @see PreStep()
       */
      virtual void PostStep() {}

      /**
       * Returns <tt>true</tt> if the experiment is finished.
       * This method allows the user to specify experiment-specific ending conditions.
       * The default implementation of this method returns always <tt>false</tt>.
       * This means that the only ending conditions for an experiment are time limit
       * expiration or GUI shutdown.
       * @return <tt>true</tt> if the experiment is finished.
       * @see CSimulator::IsExperimentFinished()
       */
      virtual bool IsExperimentFinished() {
         return false;
      }

      /**
       * Executes user-defined logic when the experiment finishes.
       * This method is called within CSimulator::IsExperimentFinished()
       * as soon as its return value evaluates to <tt>true</tt>. This
       * method is executed before Destroy().
       * You can use this method to perform final calculations at the
       * end of an experiment.
       * The default implementation of this method does nothing.
       */
      virtual void PostExperiment() {
      }

      /**
       * Returns the color of the floor in the specified point.
       * This function is called if the floor entity was configured to take the loop functions
       * as source. The floor color is used by the ground sensors to calculate their readings,
       * and by the graphical visualization to create a texture to display on the arena floor.
       * @param c_pos_on_floor The position on the floor.
       * @return The color of the floor in the specified point.
       * @see CFloorEntity
       * @see CGroundSensorEquippedEntity
       * @see CGroundRotZOnlySensor
       */
      virtual CColor GetFloorColor(const CVector2& c_pos_on_floor) {
         return CColor::WHITE;
      }

      /**
       * Returns the simulator instance.
       * @return The simulator instance.
       */
      inline CSimulator& GetSimulator() {
         return m_cSimulator;
      }

      /**
       * Returns a reference to the space state.
       * @return A reference to the space state.
       */
      inline CSpace& GetSpace() {
         return m_cSpace;
      }

      /**
       * Moves the entity to the wanted position and orientation.
       * @param c_entity The positional component of the entity to move.
       * @param c_position The wanted position.
       * @param c_orientation The wanted orientation.
       */
      virtual void MoveEntity(CPositionalEntity& c_entity,
                              const CVector3& c_position,
                              const CQuaternion& c_orientation);

      /**
       * Moves the entity to the wanted position and orientation.
       * The movement is allowed only if the
       * object does not collide with anything once in the new position.
       * @param c_entity The positional or embodied component of the entity to move.
       * @param c_position The wanted position.
       * @param c_orientation The wanted orientation.
       * @param b_check_only If <tt>false</tt>, the movement is executed; otherwise, the object is not actually moved.
       * @return <tt>true</tt> if no collisions were detected, <tt>false</tt> otherwise.
       */
      virtual bool MoveEntity(CEmbodiedEntity& c_entity,
                              const CVector3& c_position,
                              const CQuaternion& c_orientation,
                              bool b_check_only = false);

      /**
       * Adds the passed entity to the simulation.
       * Important: the entity must be created with a <tt>new</tt> statement or a CFactory::New() statement.
       * In other words, the entity must be stored in the heap.
       * If this is not the case, memory corruption will occur.
       * @param c_entity A reference to the entity to add.
       * @throws CARGoSException if an error occurs.
       */
      virtual void AddEntity(CEntity& c_entity);

      /**
       * Removes an entity from the simulation.
       * @param str_entity_id The id of the entity to remove.
       * @throws CARGoSException If an entity with the specified id was not found.
       */
      virtual void RemoveEntity(const std::string& str_entity_id);

      /**
       * Removes an entity from the simulation.
       * @param c_entity A reference to the entity to remove.
       */
      virtual void RemoveEntity(CEntity& c_entity);

   private:

      /** A reference to the CSimulator instance */
      CSimulator& m_cSimulator;

      /** A reference to the CSpace instance */
      CSpace& m_cSpace;

   };
}

/**
 * Registers a loop function class inside ARGoS.
 * You must register your loop function class for ARGoS to be able to recognize it.
 * This statement must be included in a .cpp file. It can't be in a header.
 */
#define REGISTER_LOOP_FUNCTIONS(CLASSNAME, LABEL) \
   REGISTER_SYMBOL(CLoopFunctions,                \
                   CLASSNAME,                     \
                   LABEL,                         \
                   "undefined",                   \
                   "undefined",                   \
                   "undefined",                   \
                   "undefined",                   \
                   "undefined")

#endif
