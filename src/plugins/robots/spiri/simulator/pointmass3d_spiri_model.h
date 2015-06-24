/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_spiri_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_SPIRI_H
#define POINTMASS3D_SPIRI_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DQuadRotorModel;
   class CPointMass3DSpiriModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_quadrotor_model.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>

namespace argos {

   class CPointMass3DSpiriModel : public CPointMass3DQuadRotorModel {

   public:
      
      CPointMass3DSpiriModel(CPointMass3DEngine& c_engine,
                             CSpiriEntity& c_spiri);
      virtual ~CPointMass3DSpiriModel() {}

   private:

      void UpdateRABAnchor(SAnchor& s_anchor);
      
      void UpdateCameraAnchor(SAnchor& s_anchor);
      
   };

}

#endif
