/**
 * @file <argos3/plugins/simulator/entities/battery_sensor_equipped_entity.h>
 *
 * @author
 */

#ifndef BATTERY_SENSOR_EQUIPPED_ENTITY_H
#define BATTERY_SENSOR_EQUIPPED_ENTITY_H

namespace argos {
   class CBatterySensorEquippedEntity;
}

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <map>

namespace argos {

   class CBatterySensorEquippedEntity : public CEntity {

   public:

       ENABLE_VTABLE();


   public:

       CBatterySensorEquippedEntity(CComposableEntity* pc_parent);

       CBatterySensorEquippedEntity(CComposableEntity *pc_parent,
                                    const std::string& str_id);

       virtual ~CBatterySensorEquippedEntity();

       virtual void Init(TConfigurationNode& t_tree);

       virtual std::string GetTypeDescription() const {
          return "battery_sensors";
       }

       virtual void Enable();

       virtual void Disable();

   protected:

       SInt16 m_nCapacity;

   };

}

#endif
