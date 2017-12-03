/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.h>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */

#ifndef BATTERY_EQUIPPED_ENTITY_H
#define BATTERY_EQUIPPED_ENTITY_H

namespace argos {
   class CBatteryEquippedEntity;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <map>

namespace argos {

   class CBatteryEquippedEntity : public CEntity {

   public:

       ENABLE_VTABLE();


   public:

       CBatteryEquippedEntity(CComposableEntity* pc_parent);

       CBatteryEquippedEntity(CComposableEntity *pc_parent,
                                    const std::string& str_id,
                                    UInt16 un_full_capacity = 0,
                                    Real f_ideal_discharge = 0,
                                    Real f_moving_discharge = 0);

       virtual ~CBatteryEquippedEntity();

       virtual void Init(TConfigurationNode& t_tree);

       virtual std::string GetTypeDescription() const {
          return "battery";
       }

       virtual void Enable();

       virtual void Disable();

       virtual void Update();

       inline const Real GetBatLevel() const {
           return m_fLevel;
       }

       inline const UInt16 GetFullCapacity() const {
           return m_unFullCapacity;
       }

       inline const UInt16 GetAvailCapacity() const {
           return m_unAvailCapacity;
       }

       inline const UInt16 GetTimeRemaining() const {
           return m_unFullCapacity;
       }

       inline void SetFullCapacity(UInt16 un_full_capacity) {
           m_unFullCapacity = un_full_capacity;
       }

       inline void SetBatLevel(Real f_bat_level) {
           m_fLevel = f_bat_level;
       }

   protected:

       /** Available battery level is always between 0 and 1 */
       Real m_fLevel;
       /** Battery level in mAh */
       UInt16 m_unFullCapacity;
       /** Available capacity in mAh*/
       UInt16 m_unAvailCapacity;
       /** Time remaining in seconds */
       UInt16 m_unTimeRemaining;
       /** Ideal Discharge */
       Real m_fIdealDischarge;
       /** Moving Dischrage */
       Real m_fMovingDischarge;

       /** Previous Robot Location */
       CVector3 m_fPrevPosition;
//       CQuaternion m_fPrevOrientation;

       CEmbodiedEntity* m_pcEmbodiedEntity;

   };

}

#endif
