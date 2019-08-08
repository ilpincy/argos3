/**
 * @file <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DIRECTIONAL_LED_EQUIPPED_ENTITY_H
#define DIRECTIONAL_LED_EQUIPPED_ENTITY_H

namespace argos {
   class CDirectionalLEDEquippedEntity;
   class CDirectionalLEDEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_entity.h>
#include <vector>

namespace argos {

   /**
    * A container of CDirectionalLEDEntity.
    * <p>
    * This is a convenience class that acts a container of CDirectionalLEDEntity objects.
    * It is mostly useful when a robot is equipped with a number of LEDs, and you
    * want to manage them comfortably.
    * </p>
    * @see CDirectionalLEDEntity
    */
   class CDirectionalLEDEquippedEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      struct SInstance {
         CDirectionalLEDEntity& LED;
         SAnchor& Anchor;
         CVector3 PositionOffset;
         CQuaternion OrientationOffset;
         SInstance(CDirectionalLEDEntity& c_led,
                   SAnchor& s_anchor,
                   const CVector3& c_position_offset,
                   const CQuaternion& c_orientation_offset);
         using TVector = std::vector<SInstance>;
      };

   public:

      /**
       * Class constructor.
       * @param pc_parent The parent entity.
       */
      CDirectionalLEDEquippedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * @param pc_parent The parent entity.
       * @param str_id The id of this entity.
       */
      CDirectionalLEDEquippedEntity(CComposableEntity* pc_parent,
                                    const std::string& str_id);


      virtual ~CDirectionalLEDEquippedEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Enable();

      virtual void Disable();

      /**
       * Programmatically creates a new directional LED.
       * @param str_id The identifier of the LED.
       * @param c_position The position of the LED.
       * @param c_orientation The orientation of the LED.
       * @param s_anchor The anchor of the LED.
       * @param c_observable_angle The observable angle of the LED.
       * @param c_color The color of the LED.
       * @see GetInstances()
       * @see GetLED()
       */
      void AddLED(const std::string& str_id,
                  const CVector3& c_position,
                  const CQuaternion& c_orientation,
                  SAnchor& s_anchor,
                  const CRadians& c_observable_angle,
                  const CColor& c_color);

      /**
       * Returns an LED by numeric index.
       * @param un_index The index of the wanted LED.
       * @return An LED by numeric index.
       * @see GetInstances()
       */
      CDirectionalLEDEntity& GetLED(UInt32 un_index);

      /**
       * Returns an LED by numeric index as a const reference.
       * @param un_index The index of the wanted LED.
       * @return An LED as a const reference.
       * @see GetInstances()
       */
      const CDirectionalLEDEntity& GetLED(UInt32 un_index) const;

      /**
       * Returns all the LEDs.
       * @return All the LEDs.
       * @see GetInstances()
       */
      inline SInstance::TVector& GetInstances() {
         return m_vecInstances;
      }

      /**
       * Returns the LEDs as a const reference.
       * @return The LEDs as a const reference.
       * @see GetInstances()
       */
      inline const SInstance::TVector& GetInstances() const {
         return m_vecInstances;
      }

      /**
       * Sets the color of an LED.
       * @param un_index The index of the wanted LED.
       * @param c_color The color of the LED.
       */
      void SetLEDColor(UInt32 un_index, const CColor& c_color);

      /**
       * Sets the color of all the LEDs to the same value.
       * @param c_color The color of the LEDs.
       * @see SetLEDColors()
       */
      void SetLEDColors(const CColor& c_color);

      /**
       * Sets the color of all the LEDs to the given setting.
       * @param vec_colors A vector containing the colors of the LEDs.
       * @see SetLEDColors()
       * @throws CARGoSException if the size of the passed vector is different from
       * the number of LED.
       */
      void SetLEDColors(const std::vector<CColor>& vec_colors);

      /**
       * Sets the medium associated to this entity.
       * @param c_medium The medium to associate to this entity.
       * @see CDirectionalLEDMedium
       */
      void SetMedium(CDirectionalLEDMedium& c_medium);

      virtual std::string GetTypeDescription() const {
         return "directional_leds";
      }

   protected:

      virtual void UpdateComponents();

   protected:

      /** List of the LEDs managed by this entity */
      SInstance::TVector m_vecInstances;
   };

}

#endif
