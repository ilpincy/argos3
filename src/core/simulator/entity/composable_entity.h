/**
 * @file <argos3/core/simulator/entity/composable_entity.h>
 *
 * @brief This file contains the definition of an entity.
 *
 * This file contains the class definition of an
 * entity, that is, the basic class that provides the interface for the
 * simulation of all the objects in the environment.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef COMPOSABLE_ENTITY_H
#define COMPOSABLE_ENTITY_H

namespace argos {
   class CComposableEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /**
    * Basic class for an entity that contains other entities.
    * Robots, as well as other complex objects, must extend this class.
    * When you add robots, make sure to add the robot body (CEmbodiedEntity class) <em>first</em>,
    * and its controllable component (CControllableEntity class) <em>last</em>.
    * @see CEntity
    */
   class CComposableEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method.
       * @param pc_parent The parent of this entity.
       */
      CComposableEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone.
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen.
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       */
      CComposableEntity(CComposableEntity* pc_parent,
                        const std::string& str_id);

      /**
       * Class destructor.
       */
      virtual ~CComposableEntity() {}

      /**
       * Resets the state of the entity to whatever it was after Init() or the standalone constructor was called.
       * Internally calls Reset() for all the component entities.
       */
      virtual void Reset();

      /**
       * Updates the status of this entity.
       * Internally calls UpdateComponents(). If you plan to overload this method, don't forget to call
       * CComposableEntity::Update() or UpdateComponents() in your code.
       * @see UpdateComponents()
       */
      virtual void Update();

      virtual std::string GetTypeDescription() const {
         return "composite";
      }

      /**
       * Enables or disables an entity
       * @param b_enabled <tt>true</tt> if the entity is enabled, <tt>false</tt> otherwise
       * @see CEntity::m_bEnabled
       * @see CEntity::m_bCanBeEnabledIfDisabled
       */
      virtual void SetEnabled(bool b_enabled);

      /**
       * Calls the Update() method on all the components.
       * @see Update()
       */
      virtual void UpdateComponents();

      /**
       * Adds a component to this composable entity.
       * @param c_component The component to add.
       */
      void AddComponent(CEntity& c_component);

      /**
       * Removes a component from this composable entity.
       * The format of the label can be either <tt>label</tt> or <tt>label[label_N]</tt> to get the
       * <tt>N+1</tt>-th component with the wanted string label.
       * @param str_component The string label of the component to remove.
       * @see GetTypeDescription()
       * @throws CARGoSException if the component was not found.
       */
      CEntity& RemoveComponent(const std::string& str_component);

      /**
       * Returns the component with the passed string label.
       * The format of the label can be either <tt>label</tt>, <tt>label1.label2</tt> or
       * <tt>label1.label2[label2_N]</tt> to get the <tt>N+1</tt>-th component with the wanted string
       * label. The dot syntax <tt>label1.label2</tt> can be nested further, to create combinations
       * such as <tt>label1.label2.label3[label3_N]</tt>.
       * @param str_component The string label of the component to return.
       * @see GetTypeDescription()
       * @throws CARGoSException if the component was not found.
       */
      CEntity& GetComponent(const std::string& str_component);

      /**
       * Returns the component with the passed string label.
       * This method internally performs a <tt>dynamic_cast</tt> and returns
       * directly the desired type instead of CEntity.
       * The format of the label can be either <tt>label</tt>, <tt>label1.label2</tt> or
       * <tt>label1.label2[label2_N]</tt> to get the <tt>N+1</tt>-th component with the wanted string
       * label. The dot syntax <tt>label1.label2</tt> can be nested further, to create combinations
       * such as <tt>label1.label2.label3[label3_N]</tt>.
       * @param str_component The string label of the component to return.
       * @see GetTypeDescription()
       * @throws CARGoSException if the component was not found or can't be cast to the target type.
       */
      template <class E>
      E& GetComponent(const std::string& str_component) {
         E* pcComponent = dynamic_cast<E*>(&GetComponent(str_component));
         if(pcComponent != NULL) {
            return *pcComponent;
         }
         else {
            THROW_ARGOSEXCEPTION("Type conversion failed for component type \"" << str_component << "\" of entity \"" << GetId());
         }
      }

      /**
       * Returns <tt>true</tt> if this composable entity has a component with the given string label.
       * The format of the label can be either <tt>label</tt>, <tt>label1.label2</tt> or
       * <tt>label1.label2[label2_N]</tt> to get the <tt>N+1</tt>-th component with the wanted string
       * label. The dot syntax <tt>label1.label2</tt> can be nested further, to create combinations
       * such as <tt>label1.label2.label3[label3_N]</tt>.
       * @param str_component The string label of the component to check.
       * @return <tt>true</tt> if this composable entity has a component with the given string label.
       * @see GetTypeDescription()
       */
      bool HasComponent(const std::string& str_component);

      /**
       * Searches for a component with the given string label.
       * The format of the label can be either <tt>label</tt> or <tt>label[label_N]</tt> to get the
       * <tt>N+1</tt>-th component with the wanted string label.
       * @param str_component The string label of the component to find.
       * @return An iterator to the component found.
       * @throws CARGoSException if the component was not found.
       */
      CEntity::TMultiMap::iterator FindComponent(const std::string& str_component);

      /**
       * Returns the map of all the components.
       * @return The map of all the components.
       */
      inline CEntity::TMultiMap& GetComponentMap() {
         return m_mapComponents;
      }

      /**
       * Returns the vector of all the components.
       * The elements in this vector are stored in the same order as they were inserted.
       * @return The vector of all the components.
       */
      inline CEntity::TVector& GetComponentVector() {
         return m_vecComponents;
      }

   private:

      CEntity::TMultiMap m_mapComponents;
      CEntity::TVector m_vecComponents;

   };

/**
 * @cond HIDDEN_SYMBOLS
 */
#define SPACE_OPERATION_ADD_COMPOSABLE_ENTITY(ENTITY)                                                               \
   class CSpaceOperationAdd ## ENTITY : public CSpaceOperationAddEntity {                                           \
   public:                                                                                                          \
   void ApplyTo(CSpace& c_space, ENTITY& c_entity) {                                                                \
      c_space.AddEntity(c_entity);                                                                                  \
      for(size_t i = 0; i < c_entity.GetComponentVector().size(); ++i) {                                         \
         CallEntityOperation<CSpaceOperationAddEntity, CSpace, void>(c_space, *(c_entity.GetComponentVector()[i])); \
      }                                                                                                             \
   }                                                                                                                \
   };

#define SPACE_OPERATION_REMOVE_COMPOSABLE_ENTITY(ENTITY)                \
   class CSpaceOperationRemove ## ENTITY : public CSpaceOperationRemoveEntity { \
   public:                                                              \
   void ApplyTo(CSpace& c_space, ENTITY& c_entity) {                    \
      CEntity* pcToRemove;                                              \
      while(!c_entity.GetComponentVector().empty()) {                   \
         pcToRemove = c_entity.GetComponentVector().back();             \
         c_entity.RemoveComponent(pcToRemove->GetTypeDescription() + "[" + pcToRemove->GetId() + "]"); \
         CallEntityOperation<CSpaceOperationRemoveEntity, CSpace, void>(c_space, *pcToRemove); \
      }                                                                 \
      c_space.RemoveEntity(c_entity);                                   \
   }                                                                    \
   };

#define REGISTER_STANDARD_SPACE_OPERATION_ADD_COMPOSABLE(ENTITY)        \
   SPACE_OPERATION_ADD_COMPOSABLE_ENTITY(ENTITY)                        \
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,                   \
                            CSpaceOperationAdd ## ENTITY,               \
                            ENTITY);

#define REGISTER_STANDARD_SPACE_OPERATION_REMOVE_COMPOSABLE(ENTITY)        \
   SPACE_OPERATION_REMOVE_COMPOSABLE_ENTITY(ENTITY)                        \
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,                   \
                            CSpaceOperationRemove ## ENTITY,               \
                            ENTITY);

#define REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(ENTITY)  \
   REGISTER_STANDARD_SPACE_OPERATION_ADD_COMPOSABLE(ENTITY)       \
   REGISTER_STANDARD_SPACE_OPERATION_REMOVE_COMPOSABLE(ENTITY)

/**
 * @endcond
 */

}

#endif
