/**
 * @file <argos3/core/simulator/entity/embodied_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef EMBODIED_ENTITY_H
#define EMBODIED_ENTITY_H

namespace argos {
   class CEmbodiedEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <algorithm>

namespace argos {

   /**
    * This entity is a link to a body in the physics engine.
    * <p>
    * The embodied entity can either be a root entity (i.e., parentless), or a component.
    * </p>
    * <p>
    * Its main purpose is to serve as an interface to the actual body in the physics engine.
    * Sensors can use embodied entities to know the position and orientations of the objects
    * in the arena, and can perform ray-cast queries to test for occlusions.
    * In addition, the method MoveTo() allows one to move an entity and/or rotate it.
    * </p>
    * <p>
    * Embodied entities can be either <em>movable</em> or <em>non-movable</em>.
    * Movable entities are typically self-propelled objects such as mobile robots,
    * or passive objects that can be grasped and moved around. A movable entity
    * can only be associated to a single physics engine per time. Non-movable
    * embodied entities are typically the structural elements of the environments
    * (walls, columns, etc.) and can be associated to multiple physics engines.
    * </p>
    * @see CPhysicsEngine
    * @see CPhysicsModel
    */
   class CEmbodiedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method.
       * @param pc_parent The parent of this entity.
       */
      CEmbodiedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone.
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen.
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       * @param c_position The initial entity position
       * @param c_orientation The initial entity orientation
       * @param b_movable If set to <tt>true</tt>, this entity is movable.
       */
      CEmbodiedEntity(CComposableEntity* pc_parent,
                      const std::string& str_id,
                      const CVector3& c_position = CVector3(),
                      const CQuaternion& c_orientation = CQuaternion(),
                      bool b_movable = true);

      /**
       * Class destructor.
       */
      virtual ~CEmbodiedEntity();

      /**
       * Initializes the state of the entity from the XML configuration tree.
       * Internally, this entity calls CPositionalEntity::Init() and sets
       * m_bMovable to <tt>true</tt>.
       * @throws CARGoSException if a parse error occurred
       * @see CPositionalEntity::Init()
       * @see m_bMovable
       */
      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      /**
       * Returns <tt>true</tt> if the entity is movable.
       * @return <tt>true</tt> if the entity is movable.
       */
      inline bool IsMovable() const {
         return m_bMovable;
      }

      /**
       * Sets whether this entity is movable or not.
       * This flag cannot be changed during an experiment, because the
       * results are undefined.
       * @param b_movable <tt>true</tt> if the entity is movable.
       */
      inline void SetMovable(bool b_movable) {
         m_bMovable = b_movable;
      }

      /**
       * Returns a const reference to the origin anchor associated to this entity.
       * @returns A const reference to the origin anchor associated to this entity.
       */
      inline const SAnchor& GetOriginAnchor() const {
         return *m_psOriginAnchor;
      }

      /**
       * Returns a non-const reference to the origin anchor associated to this entity.
       * @returns A non-const reference to the origin anchor associated to this entity.
       */
      inline SAnchor& GetOriginAnchor() {
         return *m_psOriginAnchor;
      }

      /**
       * Adds an anchor to the embodied entity.
       * The anchor is initially disabled. To enable it you must call EnableAnchor().
       * @param str_id The id of the anchor.
       * @param c_rel_position The position of the anchor wrt the body coordinate system.
       * @param c_rel_orientation The orientation of the anchor wrt the body coordinate system.
       * @return A reference to the newly created anchor.
       * @throws CARGoSException if an anchor with the passed id already exists in this embodied entity.
       * @see SAnchor
       * @see EnableAnchor
       */
      SAnchor& AddAnchor(const std::string& str_id,
                         const CVector3& c_rel_position = CVector3(),
                         const CQuaternion& c_rel_orientation = CQuaternion());
      
      /**
       * Enables an anchor.
       * @param str_id The id of the anchor.
       * @throws CARGoSException if an anchor with the passed id does not exist in this embodied entity.
       * @see SAnchor
       * @see DisableAnchor
       */
      void EnableAnchor(const std::string& str_id);

      /**
       * Disables an anchor.
       * @param str_id The id of the anchor.
       * @throws CARGoSException if an enabled anchor with the passed id does not exist in this embodied entity.
       * @see SAnchor
       * @see DisableAnchor
       */
      void DisableAnchor(const std::string& str_id);

      /**
       * Returns the wanted anchor as a const reference.
       * @param str_id The id of the anchor.
       * @return The wanted anchor.
       * @throws CARGoSException if the anchor was not found.
       * @see SAnchor
       * @see EnableAnchor
       * @see DisableAnchor
       */
      const SAnchor& GetAnchor(const std::string& str_id) const;

      /**
       * Returns the wanted anchor as a non-const reference.
       * @param str_id The id of the anchor.
       * @return The wanted anchor.
       * @throws CARGoSException if the anchor was not found.
       * @see SAnchor
       * @see EnableAnchor
       * @see DisableAnchor
       */
      SAnchor& GetAnchor(const std::string& str_id);

      /**
       * Returns a map of anchors associated to this embodied entity.
       * The map has the id as key and a pointer to the anchor as value.
       * @return The map of anchors.
       * @see SAnchor
       * @see AddAnchor
       */
      inline std::map<std::string, SAnchor*>& GetAnchors() {
         return m_mapAnchors;
      }

      /**
       * Returns a vector of enabled anchors for fast looping.
       * This method is mainly intended for use in a physics engine
       * when anchors are updated.
       * @return The vector of currently enabled anchors.
       * @see EnableAnchor
       * @see DisableAnchor
       */
      inline std::vector<SAnchor*>& GetEnabledAnchors() {
         return m_vecEnabledAnchors;
      }

      /**
       * Returns <tt>true</tt> if the given anchor is enabled, <tt>false</tt> otherwise.
       * @return <tt>true</tt> if the given anchor is enabled, <tt>false</tt> otherwise.
       * @see EnableAnchor
       * @see DisableAnchor
       */
      bool IsAnchorEnabled(const std::string& str_id);

      /**
       * Returns the bounding box of this embodied entity.
       * The bounding box is calculated by the physics engine. In case multiple
       * physics engines are associated to this entity, the resulting bounding
       * box is the largest bounding box that includes all the bounding boxes
       * of each physics engine.
       * @return The bounding box of this embodied entity.
       * @see SBoundingBox
       */
      const SBoundingBox& GetBoundingBox() const;

      /**
       * Returns the number of physics models associated to this entity.
       * @return The number of physics models associated to this entity.
       */
      UInt32 GetPhysicsModelsNum() const;

      /**
       * Adds a new physics model to this embodied entity.
       * Internally it calls CalculateBoundingBox().
       * @param str_engine_id The id of the engine to which the physics model belongs.
       * @param c_physics_model The physics model to add.
       * @see CPhysicsModel
       */
      virtual void AddPhysicsModel(const std::string& str_engine_id,
                                   CPhysicsModel& c_physics_model);

      /**
       * Removes a physics model from this embodied entity.
       * Internally it calls CalculateBoundingBox().
       * @param str_engine_id The id of the engine to which the physics model belongs.
       * @throws CARGoSException if the engine id is not found.
       */
      void RemovePhysicsModel(const std::string& str_engine_id);

      /**
       * Returns a physics model associated to this entity.
       * @param un_idx The index of the wanted physics model.
       * @return a physics model associated to this entity.
       * @throws CARGoSException if the index is out of bounds.
       */
      const CPhysicsModel& GetPhysicsModel(size_t un_idx) const;

      /**
       * Returns a physics model associated to this entity.
       * @param un_idx The index of the wanted physics model.
       * @return a physics model associated to this entity.
       * @throws CARGoSException if the index is out of bounds.
       */
      CPhysicsModel& GetPhysicsModel(size_t un_idx);

      /**
       * Returns a physics model associated to this entity.
       * @param str_engine_id The id of the engine to which the physics model belongs.
       * @return a physics model associated to this entity.
       * @throws CARGoSException if the engine id is not found.
       */
      const CPhysicsModel& GetPhysicsModel(const std::string& str_engine_id) const;

      /**
       * Returns a physics model associated to this entity.
       * @param str_engine_id The id of the engine to which the physics model belongs.
       * @return a physics model associated to this entity.
       * @throws CARGoSException if the engine id is not found.
       */
      CPhysicsModel& GetPhysicsModel(const std::string& str_engine_id);

      /**
       * Moves the entity to the wanted position and orientation.
       * The movement is allowed only if the object does not collide
       * with anything once in the new position. In case of multiple
       * models associated to this entity, all the engines are checked.
       * @param c_position The wanted position.
       * @param c_orientation The wanted orientation.
       * @param b_check_only If <tt>false</tt>, the movement is executed; otherwise, the object is not actually moved.
       * @return <tt>true</tt> if no collisions were detected, <tt>false</tt> otherwise.
       * @see CPhysicsModel::MoveTo()
       */
      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      /**
       * Returns <tt>true</tt> if this entity is colliding with another object.
       * @return <tt>true</tt> if this entity is colliding with another object.
       */
      virtual bool IsCollidingWithSomething() const;

      virtual std::string GetTypeDescription() const {
         return "body";
      }

   protected:

      /**
       * Calculates the bounding box of this entity.
       * In case of association to multiple models, the resulting bounding
       * box is the largest that contains all the bounding boxes of each model.
       * @see CPhysicsModel::CalculateBoundingBox()
       */
      void CalculateBoundingBox();
      
   protected:
      
      bool m_bMovable;
      CPhysicsModel::TMap m_tPhysicsModelMap;
      CPhysicsModel::TVector m_tPhysicsModelVector;
      SBoundingBox* m_sBoundingBox;
      SAnchor* m_psOriginAnchor;
      std::map<std::string, SAnchor*> m_mapAnchors;
      std::vector<SAnchor*> m_vecEnabledAnchors;
      CVector3 m_cInitOriginPosition;
      CQuaternion m_cInitOriginOrientation;

   };

   /**
    * Returns <tt>true</tt> if the anchor id matches the given id.
    * This method makes it easier to run functions such as std::find().
    * @param ps_anchor A pointer to the anchor.
    * @param str_id The id to compare.
    * @return <tt>true</tt> if the anchor id matches the given id.
    */
   extern bool operator==(const SAnchor* ps_anchor,
                          const std::string& str_id);

   /****************************************/
   /****************************************/

   typedef std::vector<CEmbodiedEntity*> TEmbodiedEntityVector;
   typedef std::map<std::string, CEmbodiedEntity*> TEmbodiedEntityMap;
   typedef CSet<CEmbodiedEntity*> TEmbodiedEntitySet;

   /****************************************/
   /****************************************/

   /**
    * @cond HIDDEN_SYMBOLS
    */
   class CEmbodiedEntitySpaceHashUpdater : public CSpaceHashUpdater<CEmbodiedEntity> {
      
   public:
      
      virtual void operator()(CAbstractSpaceHash<CEmbodiedEntity>& c_space_hash,
                              CEmbodiedEntity& c_element);

   private:

      SInt32 m_nMinX, m_nMinY, m_nMinZ;
      SInt32 m_nMaxX, m_nMaxY, m_nMaxZ;

   };
   /**
    * @endcond
    */
   
   /****************************************/
   /****************************************/

   class CEmbodiedEntityGridUpdater : public CGrid<CEmbodiedEntity>::COperation {

   public:

      CEmbodiedEntityGridUpdater(CGrid<CEmbodiedEntity>& c_grid);
      virtual bool operator()(CEmbodiedEntity& c_entity);

   private:

      CGrid<CEmbodiedEntity>& m_cGrid;
      SInt32 m_nMinI, m_nMinJ, m_nMinK;
      SInt32 m_nMaxI, m_nMaxJ, m_nMaxK;
   };

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

      inline bool operator<(const SEmbodiedEntityIntersectionItem& s_item) {
         return TOnRay < s_item.TOnRay;
      }
   };

   struct SEmbodiedEntityIntersectionData {
      bool Intersection;
      std::vector<SEmbodiedEntityIntersectionItem*> IntersectedEntities;

      SEmbodiedEntityIntersectionData() :
         Intersection(false) {}

      SEmbodiedEntityIntersectionData(std::vector<SEmbodiedEntityIntersectionItem*>& c_entities) :
         Intersection(c_entities.size() > 0),
         IntersectedEntities(c_entities) {}
   };

   extern bool GetClosestEmbodiedEntityIntersectedByRay(SEmbodiedEntityIntersectionItem& s_item,
                                                        const CRay3& c_ray);

   extern bool GetClosestEmbodiedEntityIntersectedByRay(SEmbodiedEntityIntersectionItem& s_item,
                                                        const CRay3& c_ray,
                                                        CEmbodiedEntity& c_entity);

   /****************************************/
   /****************************************/

}

#endif
