/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_stretchable_object_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_STRETCHABLE_OBJECT_MODEL_H
#define DYNAMICS2D_STRETCHABLE_OBJECT_MODEL_H

namespace argos {
   class CDynamics2DGrippable;
   class CDynamics2DStretchableObjectModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>

namespace argos {

   /****************************************/
   /****************************************/

   /**
    * A stretchable and grippable object model for the dynamics 2D engine.
    * This class is used by CDynamics2DBoxModel and CDynamics2DCylinderModel
    * to share common code.
    * If you want to add an additional stretchable object, you just need to
    * subclass this class and implement the constructor similarly to
    * what is done in CDynamics2DBoxModel and CDynamics2DCylinderModel.
    * @see CDynamics2DBoxModel
    * @see CDynamics2DCylinderModel
    */
   class CDynamics2DStretchableObjectModel : public CDynamics2DSingleBodyObjectModel {

   public:

      CDynamics2DStretchableObjectModel(CDynamics2DEngine& c_engine,
                                        CComposableEntity& c_entity);

      virtual ~CDynamics2DStretchableObjectModel();

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual void Reset();

      virtual void UpdateFromEntityStatus() {}

      inline Real GetMass() const {
         return m_fMass;
      }

      inline void SetMass(Real f_mass) {
         m_fMass = f_mass;
      }

      inline void SetGrippable(CDynamics2DGrippable* pc_grippable) {
         m_pcGrippable = pc_grippable;
      }
      
      /**
       * Sets the linear friction of this object.
       * You must call this method strictly *after* SetBody().
       */
      void SetLinearFriction(Real f_max_bias,
                             Real f_max_force);

      /**
       * Sets the angular friction of this object.
       * You must call this method strictly *after* SetBody().
       */
      void SetAngularFriction(Real f_max_bias,
                              Real f_max_force);

   private:

      Real m_fMass;
      CDynamics2DGrippable* m_pcGrippable;
      cpConstraint*         m_ptLinearFriction;
      cpConstraint*         m_ptAngularFriction;
   };

   /****************************************/
   /****************************************/

}

#endif
