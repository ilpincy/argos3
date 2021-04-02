/**
 * @file <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SPIRI_ENTITY_H
#define SPIRI_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CEmbodiedEntity;
   class CSpiriEntity;
   class CQuadRotorEntity;
   class CRABEquippedEntity;
   class CPerspectiveCameraEquippedEntity;
   class CBatteryEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CSpiriEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:
      static const CDegrees PERSPECTIVE_CAMERA_DEFAULT_APERTURE;
      static const Real PERSPECTIVE_CAMERA_DEFAULT_FOCAL_LENGTH;
      static const Real PERSPECTIVE_CAMERA_DEFAULT_RANGE;
      static const UInt32 PERSPECTIVE_CAMERA_DEFAULT_IMAGE_HEIGHT;
      static const UInt32 PERSPECTIVE_CAMERA_DEFAULT_IMAGE_WIDTH;

      static const Real RAB_DEFAULT_RANGE;
      static const size_t RAB_DEFAULT_MSG_SIZE;
      static const Real RAB_DEFAULT_ELEVATION;
      static const CQuaternion RAB_DEFAULT_ROT_OFFSET;

      CSpiriEntity();

      CSpiriEntity(const std::string& str_id,
                   const std::string& str_controller_id,
                   const CVector3& c_position = CVector3(),
                   const CQuaternion& c_orientation = CQuaternion(),
                   Real f_rab_range = RAB_DEFAULT_RANGE,
                   size_t un_rab_data_size = RAB_DEFAULT_MSG_SIZE,
                   const std::string& str_bat_model = "",
                   const CRadians& c_cam_aperture = ToRadians(PERSPECTIVE_CAMERA_DEFAULT_APERTURE),
                   Real f_cam_range = PERSPECTIVE_CAMERA_DEFAULT_RANGE);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CQuadRotorEntity& GetQuadRotorEntity() {
         return *m_pcQuadRotorEntity;
      }

      inline CRABEquippedEntity& GetRABEquippedEntity() {
         return *m_pcRABEquippedEntity;
      }

      inline CPerspectiveCameraEquippedEntity& GetPerspectiveCameraEquippedEntity() {
         return *m_pcPerspectiveCameraEquippedEntity;
      }

      inline CBatteryEquippedEntity& GetBatterySensorEquippedEntity() {
          return *m_pcBatteryEquippedEntity;
      }

      virtual std::string GetTypeDescription() const {
         return "spiri";
      }

   private:

      CControllableEntity*              m_pcControllableEntity;
      CEmbodiedEntity*                  m_pcEmbodiedEntity;
      CQuadRotorEntity*                 m_pcQuadRotorEntity;
      CRABEquippedEntity*               m_pcRABEquippedEntity;
      CPerspectiveCameraEquippedEntity* m_pcPerspectiveCameraEquippedEntity;
      CBatteryEquippedEntity*           m_pcBatteryEquippedEntity;
   };

}

#endif
