/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PM3D_MODEL_H
#define PM3D_MODEL_H

namespace argos {
   class CPM3DEngine;
   class CPM3DModel;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_engine.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CPM3DModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CPM3DModel*> TMap;

   public:

      CPM3DModel(CPM3DEngine& c_engine,
                        CEmbodiedEntity& c_entity);

      virtual ~CPM3DModel() {}

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual void Reset();

      virtual void Step() = 0;
      virtual void UpdateFromEntityStatus() = 0;

      virtual bool IsCollidingWithSomething() const;

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const = 0;

      /**
       * Updates the origin anchor associated to the embodied entity.
       */
      virtual void UpdateOriginAnchor(SAnchor& s_anchor);

      /**
       * Returns a reference to the physics engine controlling this model.
       * @return A reference to the physics engine controlling this model.
       */
      inline CPM3DEngine& GetPM3DEngine() {
         return m_cPM3DEngine;
      }

      /**
       * Returns a const reference to the physics engine controlling this model.
       * @return A const reference to the physics engine controlling this model.
       */
      inline const CPM3DEngine& GetPM3DEngine() const {
         return m_cPM3DEngine;
      }

   protected:

      /** Reference to the physics engine */
      CPM3DEngine& m_cPM3DEngine;

      /** The position of the model in this engine. */
      CVector3 m_cPosition;

      /** The orientation of the model in this engine. */
      CQuaternion m_cOrientation;
   };

}

#endif
