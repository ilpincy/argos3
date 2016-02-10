/**
 * @file <argos3/core/simulator/physics_engine/physics_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_MODEL_H
#define PHYSICS_MODEL_H

namespace argos {
   class CPhysicsModel;
   class CPhysicsEngine;
   class CRay3;
   class CQuaternion;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <map>
#include <vector>
#include <string>

namespace argos {

   /****************************************/
   /****************************************/

   /**
    * An anchor related to the body of an entity.
    * Anchors are used by entities as reference points. For instance, an LED ring could
    * use an anchor as the center of the ring. As the anchor moves in the space, the LED
    * positions would be calculated with respect to it.
    * An anchor is always initially disabled. To use it, you must first enable it.
    */
   struct SAnchor {
      /** The embodied entity that owns this anchor */
      CEmbodiedEntity& Body;
      /** The id of the anchor */
      std::string Id;
      /** The index of the anchor assigned by the embodied entity.
          Indices are used for fast lookup of anchor methods in physics engines. */
      UInt32 Index;
      /** The initial position of the anchor wrt the body coordinate system */
      CVector3 OffsetPosition;
      /** The initial orientation of the anchor wrt the body coordinate system */
      CQuaternion OffsetOrientation;
      /** The position of the anchor wrt the global coordinate system */
      CVector3 Position;
      /** The orientation of the anchor wrt the global coordinate system */
      CQuaternion Orientation;
      /** A counter for the devices using this anchor */
      UInt32 InUseCount;
      /**
       * Struct constructor.
       * Initializes the anchor using the provided information.
       * InUseCount is initialized to 0, i.e., the anchor is initially disabled.
       * @param str_id The id of the anchor.
       * @param c_offset_position The position of the anchor wrt the body coordinate system.
       * @param c_offset_orientation The orientation of the anchor wrt the body coordinate system.
       * @see CEmbodiedEntity::AddAnchor
       * @see CEmbodiedEntity::EnableAnchor
       * @see CEmbodiedEntity::DisableAnchor
       */
      SAnchor(CEmbodiedEntity& c_body,
              const std::string& str_id,
              UInt32 un_index,
              const CVector3& c_offset_position,
              const CQuaternion& c_offset_orientation,
              const CVector3& c_position,
              const CQuaternion& c_orientation);
      /**
       * Enables this anchor.
       */
      void Enable();
      /**
       * Disables this anchor.
       */
      void Disable();
   };

   /****************************************/
   /****************************************/

   struct SBoundingBox {
      CVector3 MinCorner;
      CVector3 MaxCorner;

      inline bool Intersects(const SBoundingBox& s_bb) const {
         return
            (MinCorner.GetX() < s_bb.MaxCorner.GetX()) && (MaxCorner.GetX() > s_bb.MinCorner.GetX()) &&
            (MinCorner.GetY() < s_bb.MaxCorner.GetY()) && (MaxCorner.GetY() > s_bb.MinCorner.GetY()) &&
            (MinCorner.GetZ() < s_bb.MaxCorner.GetZ()) && (MaxCorner.GetZ() > s_bb.MinCorner.GetZ());
      }
   };

   /****************************************/
   /****************************************/

   class CPhysicsModel {

   public:

      typedef std::map<std::string, CPhysicsModel*> TMap;
      typedef std::vector<CPhysicsModel*> TVector;

   public:

      CPhysicsModel(CPhysicsEngine& c_engine,
                    CEmbodiedEntity& c_entity);

      virtual ~CPhysicsModel() {
         while(!m_vecAnchorMethodHolders.empty()) {
            delete m_vecAnchorMethodHolders.back();
            m_vecAnchorMethodHolders.pop_back();
         }
      }

      /**
       * Returns the physics engine associated to this physics model.
       * @return The physics engine associated to this physics model.
       */
      inline CPhysicsEngine& GetEngine() {
         return m_cEngine;
      }

      /**
       * Returns the embodied entity associated to this physics model.
       * @return The embodied entity associated to this physics model.
       */
      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return m_cEmbodiedEntity;
      }

      /**
       * Returns the embodied entity associated to this physics model.
       * @return The embodied entity associated to this physics model.
       */
      inline const CEmbodiedEntity& GetEmbodiedEntity() const {
         return m_cEmbodiedEntity;
      }

      /**
       * Updates the status of the associated entity.
       * This method performs all the calculations to update the status of the
       * entity associated to this model. In particular, it is in this method
       * that anchors get updated and transfer to other engines is scheduled.
       * This method internally calls:
       * <ul>
       * <li>CalculateBoundingBox()
       * <li>CalculateAnchors()
       * <li>CComposableEntity::UpdateComponents()
       * </ul>
       * @see CalculateBoundingBox()
       * @see CalculateAnchors()
       * @see CComposableEntity::UpdateComponents()
       */
      virtual void UpdateEntityStatus();

      /**
       * Updates the state of this model from the status of the associated entity.
       * This method takes the current state of the associated entity (e.g., desired
       * wheel speed, turret rotationss, etc.) and updates the state of this model.
       * Typically, in this method you apply forces and set speeds.
       */
      virtual void UpdateFromEntityStatus() = 0;

      /**
       * <p>
       * Moves the entity to the wanted position and orientation within this engine.
       * When you create a new model, you must implement this method. Don't forget
       * to call UpdateEntityStatus() after you've changed the position.
       * </p>
       * <p>
       * This method should never be called directly from user code. It is called
       * internally by CEmbodiedEntity::MoveTo(), which is meant to be called
       * from user code.
       * </p>
       * <p>
       * CEmbodiedEntity::MoveTo() never calls this method if the embodied entity
       * is static. This is because static entities cannot be moved around. Thus,
       * there's no need in your code to check whether the associated entity is
       * static.
       * </p>
       * @param c_position The wanted position.
       * @param c_orientation The wanted orientation.
       */
      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation) = 0;

      /**
       * Returns an axis-aligned box that contains the physics model.
       * The bounding box is often called AABB.
       * @returns The bounding box as a const struct.
       */
      inline const SBoundingBox& GetBoundingBox() const {
         return m_sBoundingBox;
      }

      /**
       * Calculates the axis-aligned box that contains the entire physics model.
       * The bounding box is often called AABB.
       */
      virtual void CalculateBoundingBox() = 0;

      /**
       * Calculates the anchors associated to this model.
       * The default implementation of this method should fit all possible
       * use cases. There's usually no need to override it.
       */
      virtual void CalculateAnchors();

      /**
       * Returns <tt>true</tt> if this model is colliding with another model.
       * @return <tt>true</tt> if this model is colliding with another model.
       */
      virtual bool IsCollidingWithSomething() const = 0;

      /**
       * Returns an axis-aligned box that contains the physics model.
       * The bounding box is often called AABB.
       * @returns The bounding box as a modifiable struct.
       */
      inline SBoundingBox& GetBoundingBox() {
         return m_sBoundingBox;
      }

   private:

      CPhysicsEngine& m_cEngine;
      CEmbodiedEntity& m_cEmbodiedEntity;
      SBoundingBox m_sBoundingBox;

   private:

      /**
       * Pointer-to-thunk type definition.
       * @see Thunk
       */
      typedef void (CPhysicsModel::*TThunk)(SAnchor&);

      /**
       * The base anchor method holder.
       * @see CAnchorMethodHolderImpl
       */
      class CAnchorMethodHolder {};

      /**
       * The actual anchor method holder.
       * This template class holds a pointer to a user-defined method.
       * @param MODEL A user-defined subclass of CPhysicsModel.
       * @see CAnchorMethodHolder
       */
      template <typename MODEL> class CAnchorMethodHolderImpl : public CAnchorMethodHolder {
      public:
         typedef void (MODEL::*TMethod)(SAnchor&);
         TMethod Method;
         CAnchorMethodHolderImpl(TMethod t_method) : Method(t_method) {}
      };

   private:

      /**
       * A templetized thunk.
       * This is a trampoline method that internally performs the
       * dispatch to a user-defined method.
       * @param MODEL A user-defined subclass of CPhysicsModel.
       * @param s_anchor The anchor to pass as parameter.
       * @see TThunk
       */
      template <typename USER_IMPL>
      void Thunk(SAnchor& s_anchor);

   private:

      /**
       * A list of anchor method holders.
       * @see CAnchorMethodHolder
       */
      std::vector<CAnchorMethodHolder*> m_vecAnchorMethodHolders;

      /**
       * A list of thunks.
       * @see Thunk
       * @see TThunk
       */
      std::vector<TThunk> m_vecThunks;

   public:

      /**
       * Registers an anchor method.
       * @param MODEL A user-defined subclass of CPhysicsModel.
       * @param s_anchor The anchor to pass as a parameter to the anchor method.
       * @param pt_method The actual user-defined pointer-to-method.
       */
      template <typename MODEL>
      void RegisterAnchorMethod(const SAnchor& s_anchor,
                                void(MODEL::*pt_method)(SAnchor&));

   };

   /****************************************/
   /****************************************/

   template <typename MODEL>
   void CPhysicsModel::Thunk(SAnchor& s_anchor) {
      /*
       * When this method is called, the static type of 'this'
       * is CPhysicsModel. Since we want to call
       * method in MODEL (subclass of CPhysicsModel),
       * we need a cast. The cast is static because we trust
       * the user on not doing anything stupid.
       */
      MODEL& cImpl = static_cast<MODEL&>(*this);
      /* Cast the method holder to its effective type */
      CAnchorMethodHolderImpl<MODEL>& cMethodHolder = static_cast<CAnchorMethodHolderImpl<MODEL>&>(*m_vecAnchorMethodHolders[s_anchor.Index]);
      /* Call the user-defined method */
      (cImpl.*(cMethodHolder.Method))(s_anchor);
   }

   template <typename MODEL>
   void CPhysicsModel::RegisterAnchorMethod(const SAnchor& s_anchor,
                                            void(MODEL::*pt_method)(SAnchor&)) {
      /* Add the thunk to the VTable */
      m_vecThunks[s_anchor.Index] = &CPhysicsModel::Thunk<MODEL>;
      /* Add the method holder to the map */
      m_vecAnchorMethodHolders[s_anchor.Index] = new CAnchorMethodHolderImpl<MODEL>(pt_method);
   }

   /****************************************/
   /****************************************/

}

#endif
