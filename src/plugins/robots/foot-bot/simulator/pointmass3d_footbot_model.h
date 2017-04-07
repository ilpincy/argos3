/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_footbot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_FOOTBOT_H
#define POINTMASS3D_FOOTBOT_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DQuadRotorModel;
   class CPointMass3DFootBotModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

namespace argos {

   class CPointMass3DFootBotModel : public CPointMass3DModel {

   public:
      
      CPointMass3DFootBotModel(CPointMass3DEngine& c_engine,
                               CFootBotEntity& c_footbot);
      virtual ~CPointMass3DFootBotModel() {}

      virtual void Step();
      
      virtual void UpdateFromEntityStatus();
      
      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual void UpdateOriginAnchor(SAnchor& s_anchor);

   };

}

#endif
