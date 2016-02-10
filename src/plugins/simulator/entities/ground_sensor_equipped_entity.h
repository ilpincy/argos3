/**
 * @file <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef GROUND_SENSOR_EQUIPPED_ENTITY_H
#define GROUND_SENSOR_EQUIPPED_ENTITY_H

namespace argos {
   class CGroundSensorEquippedEntity;
}

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <map>

namespace argos {

   class CGroundSensorEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      enum ESensorType {
         TYPE_BLACK_WHITE = 0,
         TYPE_GRAYSCALE
      };

      struct SSensor {
         typedef std::vector<SSensor*> TList;

         CVector2 Offset;
         ESensorType Type;
         SAnchor& Anchor;
 
         SSensor(const CVector2& c_position,
                 ESensorType e_type,
                 SAnchor& s_anchor) :
            Offset(c_position),
            Type(e_type),
            Anchor(s_anchor) {}
      };

   public:

      CGroundSensorEquippedEntity(CComposableEntity* pc_parent);

      CGroundSensorEquippedEntity(CComposableEntity* pc_parent,
                                  const std::string& str_id);

      virtual ~CGroundSensorEquippedEntity();

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "ground_sensors";
      }

      virtual void Enable();

      virtual void Disable();

      inline size_t GetNumSensors() const {
         return m_tSensors.size();
      }

      inline SSensor& GetSensor(size_t un_idx) {
         return *m_tSensors[un_idx];
      }

      inline SSensor::TList& GetSensors() {
         return m_tSensors;
      }

      void AddSensor(const CVector2& c_offset,
                     ESensorType e_type,
                     SAnchor& s_anchor);

      void AddSensorRing(const CVector2& c_center,
                         Real f_radius,
                         const CRadians& c_start_angle,
                         ESensorType e_type,
                         UInt32 un_num_sensors,
                         SAnchor& s_anchor);

   protected:
      
      ESensorType ParseType(const std::string& str_type) const;

   protected:

      /** The list of sensors */
      SSensor::TList m_tSensors;

   };

}

#endif
