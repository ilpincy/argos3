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
#include <map>
#include <vector>
#include <string>

namespace argos {

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

   class CPhysicsModel {

   public:

      typedef std::map<std::string, CPhysicsModel*> TMap;
      typedef std::vector<CPhysicsModel*> TVector;

   public:

      CPhysicsModel(CPhysicsEngine& c_engine,
                    CEmbodiedEntity& c_entity) :
         m_cEngine(c_engine),
         m_cEmbodiedEntity(c_entity),
         m_sBoundingBox() {}

      virtual ~CPhysicsModel() {}

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
       * Moves the entity to the wanted position and orientation.
       * The movement is allowed only if the object does not collide
       * with anything once in the new position.
       * @param c_position The wanted position.
       * @param c_orientation The wanted orientation.
       * @param b_check_only If <tt>false</tt>, the movement is executed; otherwise, the object is not actually moved.
       * @return <tt>true</tt> if no collisions were detected, <tt>false</tt> otherwise.
       */
      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) = 0;

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

      friend class CEmbodiedEntity;

   protected:

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
       * A map of anchor method holders.
       * @see CAnchorMethodHolder
       */
      std::map<SAnchor*, CAnchorMethodHolder*> m_mapAnchorMethodHolders;

      /**
       * A map of thunks.
       * @see Thunk
       * @see TThunk
       */
      std::map<SAnchor*, TThunk> m_mapThunks;

   public:

      /**
       * Registers an anchor method.
       * @param MODEL A user-defined subclass of CPhysicsModel.
       * @param s_anchor The anchor to pass as a parameter to the anchor method.
       * @param pt_method The actual user-defined pointer-to-method.
       */
      template <typename MODEL>
      void RegisterAnchorMethod(SAnchor& s_anchor, void(MODEL::*pt_method)(SAnchor&));

      /**
       * Calls an anchor method for the given anchor.
       * @param s_anchor The anchor.
       */
      void Call(SAnchor& s_anchor);

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
      CAnchorMethodHolderImpl<MODEL>& cMethodHolder = static_cast<CAnchorMethodHolderImpl<MODEL>&>(*m_mapAnchorMethodHolders[&s_anchor]);
      /* Call the user-defined method */
      (cImpl.*(cMethodHolder.Method))(s_anchor);
   }

   template <typename MODEL>
   void CPhysicsModel::RegisterAnchorMethod(SAnchor& s_anchor,
                                              void(MODEL::*pt_method)(SAnchor&)) {
      /* Add the thunk to the VTable */
      m_mapThunks[&s_anchor] = &CPhysicsModel::Thunk<MODEL>;
      /* Add the method holder to the map */
      m_mapAnchorMethodHolders[&s_anchor] = new CAnchorMethodHolderImpl<MODEL>(pt_method);
   }

   /****************************************/
   /****************************************/

}

#endif
