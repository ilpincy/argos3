/**
 * @file <argos3/core/simulator/physics_engine/physics_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

namespace argos {
   class CPhysicsEngine;
   class CPhysicsModel;
   class CEntity;
   class CEmbodiedEntity;
   class CVector3;
   class CRay3;
}

#include <map>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/ray2.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /****************************************/
   /****************************************/

   struct SEmbodiedEntityIntersectionItem {
      CEmbodiedEntity* IntersectedEntity;
      Real TOnRay;

      SEmbodiedEntityIntersectionItem() :
         IntersectedEntity(NULL),
         TOnRay(1.0f) {}

      SEmbodiedEntityIntersectionItem(CEmbodiedEntity* pc_entity,
                                      Real f_t_on_ray) :
         IntersectedEntity(pc_entity),
         TOnRay(f_t_on_ray) {}

      SEmbodiedEntityIntersectionItem(const SEmbodiedEntityIntersectionItem& s_item) :
         IntersectedEntity(s_item.IntersectedEntity),
         TOnRay(s_item.TOnRay) {}

      inline bool operator<(const SEmbodiedEntityIntersectionItem& s_item) {
         return TOnRay < s_item.TOnRay;
      }
   };

   typedef std::vector<SEmbodiedEntityIntersectionItem> TEmbodiedEntityIntersectionData;

   /**
    * Checks whether the given ray intersects any entity.
    * The t_data parameter is cleared.
    * @param t_data The list of intersections found.
    * @param c_ray The ray to test for intersections.
    * @return <tt>true</tt> if at least one intersection is found
    */
   extern bool GetEmbodiedEntitiesIntersectedByRay(TEmbodiedEntityIntersectionData& t_data,
                                                   const CRay3& c_ray);

   /**
    * Returns the closest intersection with an embodied entity to the ray start.
    * @param t_data The data of the intersection, if any.
    * @param c_ray The ray to test for intersections.
    * @return <tt>true</tt> if an intersection is found
    */
   extern bool GetClosestEmbodiedEntityIntersectedByRay(SEmbodiedEntityIntersectionItem& s_item,
                                                        const CRay3& c_ray);

   /**
    * Returns the closest intersection with an embodied entity to the ray start.
    * This function allows you to exclude one entity from the list of intersections.
    * @param t_data The data of the intersection, if any.
    * @param c_ray The ray to test for intersections.
    * @param c_entity The entity to exclude from the intersection check.
    * @return <tt>true</tt> if an intersection is found
    */
   extern bool GetClosestEmbodiedEntityIntersectedByRay(SEmbodiedEntityIntersectionItem& s_item,
                                                        const CRay3& c_ray,
                                                        CEmbodiedEntity& c_entity);

   /****************************************/
   /****************************************/

   class CPhysicsEngine : public CBaseConfigurableResource {

   public:

      /**
       * A boundary face for entity transfer among physics engines.
       */
      struct SBoundaryFace {
         std::string EngineId;
      };

      /**
       * A boundary face for top/bottom parts of the volume.
       */
      struct SHorizontalFace : public SBoundaryFace {
         Real Height;
      };

      /**
       * A boundary face for side parts of the volume.
       */
      struct SVerticalFace : public SBoundaryFace {
         CRay2 BaseSegment;
      };

      /**
       * The volume boundaries
       */
      struct SVolume {
         SHorizontalFace*            TopFace;
         SHorizontalFace*            BottomFace;
         std::vector<SVerticalFace*> SideFaces;
         SVolume();
         ~SVolume();
         void Init(TConfigurationNode& t_node);
         bool IsActive() const;
      };
      
   public:

      typedef std::vector<CPhysicsEngine*> TVector;
      typedef std::map<std::string, CPhysicsEngine*, std::less<std::string> > TMap;

   public:

      CPhysicsEngine();
      virtual ~CPhysicsEngine() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset() {}
      virtual void Destroy() {}

      virtual void Update() = 0;

      /**
       * Executes extra initialization activities after the space has been initialized.
       * By default, this method does nothing.
       * The order in which initialization takes place is:
       * <ul>
       * <li>CPhysicsEngine::Init()
       * <li>CSpace::Init()
       * <li>CPhysicsEngine::PostSpaceInit()
       * </ul>
       * @throw CARGoSException if an error occurs
       * @see Init()
       * @see CSpace::Init()
       */
      virtual void PostSpaceInit() {}

      /**
       * Returns <tt>true</tt> if the given point is contained in this physics engine.
       */
      virtual bool IsPointContained(const CVector3& c_point);

      virtual size_t GetNumPhysicsModels() = 0;

      /**
       * Adds an entity to the physics engine.
       * Important: when you implement this function, you must also take care of adding
       * the physics model to the associated embodied entity.
       * @return <tt>true</tt> if the entity was added, <tt>false</tt> otherwise.
       */
      virtual bool AddEntity(CEntity& c_entity) = 0;

      /**
       * Removes an entity from the physics engine.
       * Important: when you implement this function, you must also take care of removing
       * the physics model to the associated embodied entity.
       * @return <tt>true</tt> if the entity was added, <tt>false</tt> otherwise.
       */
      virtual bool RemoveEntity(CEntity& c_entity) = 0;

      /**
       * Returns <tt>true</tt> if this engine has entities that must be transferred to another engine.
       */
      inline bool IsEntityTransferNeeded() const {
         return !m_vecTransferData.empty();
      }

      /**
       * Returns <tt>true</tt> if entity transfer is active for this engine.
       */
      inline bool IsEntityTransferActive() const {
         return m_sVolume.IsActive();
      }

      /**
       * Schedules an entity of transfer.
       * @param c_entity The entity to transfer.
       * @param str_engine_id The id if the destination engine.
       */
      virtual void ScheduleEntityForTransfer(CEmbodiedEntity& c_entity);

      /**
       * Executes the transfer of entities to other engines.
       */
      virtual void TransferEntities();

      /**
       * Returns the boundary faces for the volume associated to this engine.
       */
      inline SVolume& GetVolume() {
         return m_sVolume;
      }

      /**
       * Returns the boundary faces for the volume associated to this engine.
       */
      inline const SVolume& GetVolume() const {
         return m_sVolume;
      }

      /**
       * Check which objects in this engine intersect the given ray.
       * @param t_data The list of entities that intersect the ray.
       * @param c_ray The test ray.
       */
      virtual void CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                            const CRay3& c_ray) const = 0;

      /**
       * Returns the simulation clock tick.
       * The clock tick is the time elapsed between two control steps
       * in a simulation. This value is set in the XML file through
       * the 'ticks_per_second' attribute of the <tt>&lt;experiment&gt;</tt>
       * tag.
       * @returns The simulation clock tick.
       */
      static Real GetSimulationClockTick();

      /**
       * Returns the inverse of GetSimulationClockTick().
       * @return The inverse of GetSimulationClockTick().
       */
      static Real GetInverseSimulationClockTick();

      /**
       * Sets the simulation clock tick.
       * The clock tick is the time elapsed between two control steps
       * in a simulation. This value is set in the XML file through
       * the <tt>ticks_per_second</tt> attribute of the
       * <tt>&lt;experiment&gt;</tt> tag. You should never use this
       * method in your code.
       * @param f_simulation_clock_tick The new simulation clock tick.
       */
      static void SetSimulationClockTick(Real f_simulation_clock_tick);

      /**
       * Returns the number of iterations per simulation clock tick.
       * Physics engines can perform multiple updates for each simulation
       * clock tick, to increase the accuracy of the simulation. This
       * value is set to 1 by default. You can set a different value
       * using the <tt>iterations</tt> attribute available for every
       * physics engine tag.
       * @see GetPhysicsClockTick()
       */
      inline UInt32 GetIterations() const {
         return m_unIterations;
      }

      /**
       * Returns the length of the physics engine tick.
       * This value is calculated as
       * GetSimulationClockTick() / static_cast<Real>(GetIterations()).
       * @see GetIterations()
       */
      inline Real GetPhysicsClockTick() const {
         return m_fPhysicsClockTick;
      }

      /**
       * Returns the id of this physics engine.
       * @return The id of this physics engine.
       */
      inline const std::string& GetId() const {
         return m_strId;
      }

      /**
       * Sets the id of this physics engine.
       * @param str_id The wanted id.
       */
      void SetId(const std::string& str_id) {
         m_strId = str_id;
      }
               
   private:

      /** The number of iterations per simulation time step */
      UInt32 m_unIterations;

      /** The clock tick for this physics engine */
      Real m_fPhysicsClockTick;

      /** The physics engine's id. */
      std::string m_strId;

      /** How long a clock tick lasts (in seconds) */
      static Real m_fSimulationClockTick;

      /** The inverse of m_fSimulationClockTick */
      static Real m_fInverseSimulationClockTick;

      /** Boundary faces of the volume assigned to this engine */
      SVolume m_sVolume;

      /** Entity transfer data */
      std::vector<CEmbodiedEntity*> m_vecTransferData;
   };

}

#define REGISTER_PHYSICS_ENGINE(CLASSNAME,          \
                                LABEL,              \
                                AUTHOR,             \
                                VERSION,            \
                                BRIEF_DESCRIPTION,  \
                                LONG_DESCRIPTION,   \
                                STATUS)             \
   REGISTER_SYMBOL(CPhysicsEngine,                  \
                   CLASSNAME,                       \
                   LABEL,                           \
                   AUTHOR,                          \
                   VERSION,                         \
                   BRIEF_DESCRIPTION,               \
                   LONG_DESCRIPTION,                \
                   STATUS)

#endif
