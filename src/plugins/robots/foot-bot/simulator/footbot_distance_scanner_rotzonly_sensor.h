/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_rotzonly_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_DISTANCE_SCANNER_SENSOR_H
#define FOOTBOT_DISTANCE_SCANNER_SENSOR_H

namespace argos {
   class CFootBotDistanceScannerRotZOnlySensor;
   class CControllableEntity;
   class CSpace;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_sensor.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_equipped_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/sensor.h>

#include <string>
#include <map>

namespace argos {
   
   class CFootBotDistanceScannerRotZOnlySensor : public CSimulatedSensor,
                                                 public CCI_FootBotDistanceScannerSensor {

   public:

      CFootBotDistanceScannerRotZOnlySensor();

      virtual ~CFootBotDistanceScannerRotZOnlySensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      void UpdateNotRotating();
      void UpdateRotating();

      Real CalculateReadingForRay(const CRay3& c_ray,
                                  Real f_min_distance);

      void CalculateRaysNotRotating();
      void CalculateRaysRotating();

      /**
       * Returns true if the rays must be shown in the GUI.
       * @return true if the rays must be shown in the GUI.
       */
      inline bool IsShowRays() {
         return m_bShowRays;
      }

      /**
       * Sets whether or not the rays must be shown in the GUI.
       * @param b_show_rays true if the rays must be shown, false otherwise
       */
      inline void SetShowRays(bool b_show_rays) {
         m_bShowRays = b_show_rays;
      }

   private:

      CSpace& m_cSpace;
      CFootBotDistanceScannerEquippedEntity* m_pcDistScanEntity;
      CControllableEntity* m_pcControllableEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CRadians m_cLastDistScanRotation;

      bool m_bShowRays;

      CRay3 m_cShortRangeRays0[6];
      CRay3 m_cShortRangeRays2[6];
      CRay3 m_cLongRangeRays1[6];
      CRay3 m_cLongRangeRays3[6];

      /* Internally used to speed up ray calculations */
      CVector3 m_cDirection;
      CVector3 m_cOriginRayStart;
      CVector3 m_cOriginRayEnd;
      CVector3 m_cRayStart;
      CVector3 m_cRayEnd;
   };

}

#endif
