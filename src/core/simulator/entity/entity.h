/**
 * @file <argos3/core/simulator/entity/entity.h>
 *
 * @brief This file contains the definition of an entity.
 *
 * This file contains the class definition of an
 * entity, that is, the basic class that provides the interface for the
 * simulation of all the objects in the environment.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ENTITY_H
#define ENTITY_H

namespace argos {
   class CEntity;
   class CComposableEntity;
   class CSpace;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/utility/plugins/vtable.h>

#include <vector>
#include <map>
#include <string>

#if defined(__apple_build_version__ )
// We are on Apple, check compiler version
# if __clang_major__ >= 5
  // XCode 5.0.0 or later
#  include <unordered_map>
using std::unordered_map;
# else
  // XCode 4.6.3 or earlier
#  include <tr1/unordered_map>
using std::tr1::unordered_map;
# endif
#else
// We are on Linux, use c++03 standard
# include <tr1/unordered_map>
using std::tr1::unordered_map;
#endif

namespace argos {

   /**
    * The basic entity type.
    * <p>
    * The simulation data is organized into basic items referred to as
    * <em>entities</em> ARGoS natively offers several entity types, and the
    * user can customize them or add new ones if necessary. Each type of entity
    * stores information about a specific aspect of the simulation.
    * </p>
    * <p>
    * For instance, to store the complete state of a wheeled robot, a
    * CComposableEntity is used. CComposableEntity are
    * logical containers that are used to group other
    * entities. CComposableEntity can be nested to form trees of
    * arbitrary complexity. The CControllableEntity is a component
    * that stores a reference to the user-defined control code and to the
    * robot's sensors and actuators. The CEmbodiedEntity component
    * stores the position, orientation and 3D bounding box of the robot. The
    * current wheel speed is stored into the CWheeledEntity
    * component. If the robot is equipped with colored LEDs, their state is
    * stored in a component called CLEDEquippedEntity.
    * </p>
    * <p>
    * Entity types are organized in hierarchies. For instance, the
    * CEmbodiedEntity is an extension of the simpler CPositionalEntity,
    * which contains just the position and orientation of an
    * object, but not its bounding box. These design choices (entity
    * composition and extension) ensure flexibility, enhance code reuse
    * and diminish information redundancy.
    * </p>
    * <p>
    * Entity types are indexed in efficient data structures optimized for
    * access speed. In this way, the performance of the plug-ins that access
    * the simulated 3D space is enhanced.  For example, positional entities
    * and their extensions are indexed in several type-specific space
    * hashes.
    * @see CSpace
    * @see CSpaceHash
    */
   class CEntity : public CBaseConfigurableResource,
                   public EnableVTableFor<CEntity> {

   public:

      ENABLE_VTABLE();

      /** A vector of entities */
      typedef std::vector<CEntity*> TVector;

      /** A map of entities */
      typedef unordered_map<std::string, CEntity*> TMap;

      /** A multi-map of entities */
      typedef std::multimap<std::string, CEntity*> TMultiMap;

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method.
       * @param pc_parent The parent of this entity.
       */
      CEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone.
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen.
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       */
      CEntity(CComposableEntity* pc_parent,
              const std::string& str_id);

      /**
       * Class destructor.
       */
      virtual ~CEntity() {}

      /**
       * Initializes the state of the entity from the XML configuration tree.
       * If the id of the entity has not been set yet, this method sets an id for
       * the entity. If the entity has no parent, this method parses the
       * passed XML tree and looks for the <tt>id</tt> attribute, setting its value
       * as id. If, instead, this entity has a parent, the id is set as
       * <tt>GetParent().GetId() + "." + GetTypeDescription()</tt>.
       * @throws CARGoSException if a parse error occurred
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * Resets the state of the entity to whatever it was after Init() or the standalone constructor was called.
       * The default implementation of this method does nothing.
       */
      virtual void Reset() {}

      /**
       * Destroys the entity, undoing whatever was done by Init() or by the standalone constructor.
       * The default implementation of this method does nothing.
       */
      virtual void Destroy() {}

      /**
       * Returns the id of this entity.
       * @return The id of this entity.
       */
      inline const std::string& GetId() const {
         return m_strId;
      }

      /**
       * Returns the context of this entity.
       * @return The context of this entity.
       */
      std::string GetContext() const;

      /**
       * Returns <tt>true</tt> if this entity has a parent.
       * @return <tt>true</tt> if this entity has a parent.
       */
      inline bool HasParent() const {
         return (m_pcParent != NULL);
      }

      /**
       * Returns the root entity containing this entity.
       * @return The root entity containing this entity.
       */
      CEntity& GetRootEntity();

      /**
       * Returns the root entity containing this entity.
       * @return The root entity containing this entity.
       */
      const CEntity& GetRootEntity() const;

      /**
       * Returns this entity's parent.
       * @return This entity's parent.
       * @throws CARGoSException if this entity has no parent.
       */
      CComposableEntity& GetParent();

      /**
       * Returns this entity's parent.
       * @return This entity's parent.
       * @throws CARGoSException if this entity has no parent.
       */
      const CComposableEntity& GetParent() const;

      /**
       * Sets this entity's parent.
       * @param c_parent The new parent.
       */
      inline void SetParent(CComposableEntity& c_parent) {
         m_pcParent = &c_parent;
      }

      /**
       * Returns a string label for this class.
       * @return A string label for this class.
       */
      virtual std::string GetTypeDescription() const {
         return "entity";
      }

      /**
       * Updates the state of this entity.
       * The default implementation of this method does nothing.
       */
      virtual void Update() {}

      /**
       * Returns the entity index.
       * 
       * The entity index is used to order entities globally when
       * necessary to ensure determinism.
       *
       * When an entity index is <0, the index is considered unset and
       * it should not be used.
       *
       * @return The entity index.
       */
      ssize_t GetIndex() const {
         return m_nIndex;
      }

      /**
       * Sets the entity index.
       * The entity index is used to order entities globally when
       * necessary to ensure determinism.
       * Never call this function in your code.
       * @param un_idx The entity index.
       */
      void SetIndex(ssize_t n_idx) {
         m_nIndex = n_idx;
      }

      /**
       * Returns <tt>true</tt> if the entity is enabled.
       * @return <tt>true</tt> if the entity is enabled.
       * @see m_bEnabled
       * @see m_bCanBeEnabledIfDisabled
       */
      bool IsEnabled() const {
         return m_bEnabled;
      }

      /**
       * Enables the entity.
       * @see m_bEnabled
       * @see m_bCanBeEnabledIfDisabled
       * @see SetEnabled()
       */
      inline void Enable() {
         SetEnabled(true);
      }

      /**
       * Disables the entity.
       * @see m_bEnabled
       * @see m_bCanBeEnabledIfDisabled
       * @see SetEnabled()
       */
      inline void Disable() {
         SetEnabled(false);
      }

      /**
       * Enables or disables an entity.
       * @param b_enabled <tt>true</tt> if the entity is enabled, <tt>false</tt> otherwise
       * @see m_bEnabled
       * @see m_bCanBeEnabledIfDisabled
       */
      virtual void SetEnabled(bool b_enabled);

      /**
       * Returns a pointer to the configuration node that was used to create this entity.
       * The pointer is NULL if no configuration node was used.
       * @return A pointer to the configuration node that was used to create this entity.
       */
      TConfigurationNode* GetConfigurationNode() {
         return m_ptConfNode;
      }

   private:

      /** The parent of this entity */
      CComposableEntity* m_pcParent;

      /** The id of this entity */
      std::string m_strId;

      /** The unique index of this entity, used for deterministic ordering */
      ssize_t m_nIndex;

      /** When <tt>true</tt>, this entity is updated; when <tt>false</tt>, this entity is not updated */
      bool m_bEnabled;

      /** The XML tag used to configure this entity, if any */
      TConfigurationNode* m_ptConfNode;

   };

   /**
    * A generic entity comparator, used in containers that must be ordered deterministically.
    */
   struct SEntityComparator {
      bool operator()(const CEntity* pc_a, const CEntity* pc_b) const {
         return pc_a->GetIndex() < pc_b->GetIndex();
      }
      bool operator()(const CEntity& c_a, const CEntity& c_b) const {
         return c_a.GetIndex() < c_b.GetIndex();
      }
   };

   /**
    * The basic operation to be stored in the vtable
    */
   template <typename LABEL, typename PLUGIN, typename RETURN_TYPE>
   class CEntityOperation {
   public:
      template <typename DERIVED, typename OPERATION_IMPL>
      RETURN_TYPE Hook(PLUGIN& t_plugin, CEntity& c_entity) {
         return Dispatch<DERIVED, OPERATION_IMPL>(t_plugin, c_entity);
      }
   protected:
      template <typename DERIVED, typename OPERATION_IMPL>
      RETURN_TYPE Dispatch(PLUGIN& t_plugin, CEntity& c_entity) {
         /* First dispatch: cast this operation into the specific operation */
         OPERATION_IMPL& tOperation = static_cast<OPERATION_IMPL&>(*this);
         /* Second dispatch: cast t_base to DERIVED */
         DERIVED& tDerived = static_cast<DERIVED&>(c_entity);
         /* Perform visit */
         return tOperation.ApplyTo(t_plugin, tDerived);
      }
   };

   /**
    * Type to use as return value for operation outcome
    */
   struct SOperationOutcome {
      bool Value;
      SOperationOutcome() : Value(false) {}
      SOperationOutcome(bool b_value) : Value(b_value) {}
      bool operator()() const { return Value; }
   };

   /**
    * @cond HIDDEN_SYMBOLS
    */
   template <typename LABEL, typename PLUGIN, typename RETURN_TYPE>
   class CEntityOperationInstanceHolder {
   public:
      ~CEntityOperationInstanceHolder() {
         while(!m_vecOperationInstances.empty()) {
            if(m_vecOperationInstances.back() != NULL) {
               delete m_vecOperationInstances.back();
            }
            m_vecOperationInstances.pop_back();
         }
      }
      template <typename DERIVED>
      void Add(CEntityOperation<LABEL, PLUGIN, RETURN_TYPE>* pc_operation) {
         /* Find the slot */
         size_t unIndex = GetTag<DERIVED, CEntity>();
         /* Does the holder have a slot for this index? */
         if(unIndex >= m_vecOperationInstances.size()) {
            /* No, new slots must be created
             * Fill the slots with NULL
             */
            /* Create new slots up to index+1 and fill them with tDefaultFunction */
            m_vecOperationInstances.resize(unIndex+1, NULL);
         }
         m_vecOperationInstances[unIndex] = pc_operation;
      }
      CEntityOperation<LABEL, PLUGIN, RETURN_TYPE>* operator[](size_t un_index) const {
         if(un_index >= m_vecOperationInstances.size()) {
            return NULL;
         }
         return m_vecOperationInstances[un_index];
      }
   private:
      std::vector<CEntityOperation<LABEL, PLUGIN, RETURN_TYPE>*> m_vecOperationInstances;
   };
   /**
    * @endcond
    */

   /**
    * @cond HIDDEN_SYMBOLS
    * Function that returns a reference to the static operation instance holder
    */
   template <typename LABEL, typename PLUGIN, typename RETURN_VALUE>
   CEntityOperationInstanceHolder<LABEL, PLUGIN, RETURN_VALUE>& GetEntityOperationInstanceHolder() {
      static CEntityOperationInstanceHolder<LABEL, PLUGIN, RETURN_VALUE> cHolder;
      return cHolder;
   }
   /**
    * @endcond
    */

   /**
    * Calls the operation corresponding to the given context and operand
    * Skips the function call if the operation is missing in the vtable
    */
   template<typename LABEL, typename PLUGIN, typename RETURN_VALUE>
   RETURN_VALUE CallEntityOperation(PLUGIN& t_plugin, CEntity& c_entity) {
      typedef RETURN_VALUE (CEntityOperation<LABEL, PLUGIN, RETURN_VALUE>::*TFunction)(PLUGIN& t_plugin, CEntity&);
      TFunction tFunction = GetVTable<LABEL, CEntity, TFunction>()[c_entity.GetTag()];
      if(tFunction != NULL) {
         CEntityOperation<LABEL, PLUGIN, RETURN_VALUE>* pcOperation = GetEntityOperationInstanceHolder<LABEL, PLUGIN, RETURN_VALUE>()[c_entity.GetTag()];
         if(pcOperation != NULL) {
            return (pcOperation->*tFunction)(t_plugin, c_entity);
         }
      }
      return RETURN_VALUE();
   }

}

#define REGISTER_ENTITY(CLASSNAME,                  \
                        LABEL,                      \
                        AUTHOR,                     \
                        VERSION,                    \
                        BRIEF_DESCRIPTION,          \
                        LONG_DESCRIPTION,           \
                        STATUS)                     \
   REGISTER_SYMBOL(CEntity,                         \
                   CLASSNAME,                       \
                   LABEL,                           \
                   AUTHOR,                          \
                   VERSION,                         \
                   BRIEF_DESCRIPTION,               \
                   LONG_DESCRIPTION,                \
                   STATUS)

/**
 * Convenience macro to register vtable entity operations
 */
#define REGISTER_ENTITY_OPERATION(LABEL, PLUGIN, OPERATION, RETURN_VALUE, DERIVED)                         \
   class C ## LABEL ## PLUGIN ## OPERATION ## RETURN_VALUE ## DERIVED {                                    \
      typedef RETURN_VALUE (CEntityOperation<LABEL, PLUGIN, RETURN_VALUE>::*TFunction)(PLUGIN&, CEntity&); \
   public:                                                                                                 \
      C ## LABEL ## PLUGIN ## OPERATION ## RETURN_VALUE ## DERIVED() {                                     \
         GetVTable<LABEL, CEntity, TFunction>().Add<DERIVED>(&OPERATION::Hook<DERIVED, OPERATION>);        \
         GetEntityOperationInstanceHolder<LABEL, PLUGIN, RETURN_VALUE>().Add<DERIVED>(new OPERATION());    \
      }                                                                                                    \
   } c ## LABEL ## PLUGIN ## OPERATION ## RETURN_VALUE ## DERIVED;

#endif
