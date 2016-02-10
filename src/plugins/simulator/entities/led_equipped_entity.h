/**
 * @file <argos3/plugins/simulator/entities/led_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LED_EQUIPPED_ENTITY_H
#define LED_EQUIPPED_ENTITY_H

namespace argos {
   class CLEDEquippedEntity;
   class CLEDEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/led_entity.h>
#include <map>

namespace argos {

   /**
    * A container of CLEDEntity.
    * <p>
    * This is a convenience class that acts a container of CLEDEntity objects. It
    * is mostly useful when a robot is equipped with a number of LEDs, and you
    * want to manage them comfortably.
    * </p>
    * <p>
    * You can define a positional entity as the <em>reference</em> of this entity.
    * In this way, if the reference entity moves, this entity will follow automatically.
    * The contained LEDs will also move accordingly. If you don't define a reference
    * entity, the LEDs won't move.
    * </p>
    * @see CLEDEntity
    */
   class CLEDEquippedEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

      typedef std::map<std::string, CLEDEquippedEntity*> TMap;

   public:

      struct SActuator {
         typedef std::vector<SActuator*> TList;

         CLEDEntity& LED;
         CVector3 Offset;
         SAnchor& Anchor;

         SActuator(CLEDEntity& c_led,
                   const CVector3& c_offset,
                   SAnchor& s_anchor);
         void Enable();
         void Disable();
      };

   public:

      /**
       * Class constructor.
       * @param pc_parent The parent entity.
       */
      CLEDEquippedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * @param pc_parent The parent entity.
       * @param str_id The id of this entity.
       */
      CLEDEquippedEntity(CComposableEntity* pc_parent,
                         const std::string& str_id);

      /**
       * Class destructor.
       */
      ~CLEDEquippedEntity();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Enable();

      virtual void Disable();

      /**
       * Adds an LED to this entity.
       * @param c_offset The position of the LED wrt the anchor.
       * @param s_anchor The anchor of the LED.
       * @param c_color The color of the LED.
       */
      void AddLED(const CVector3& c_offset,
                  SAnchor& s_anchor,
                  const CColor& c_color = CColor::BLACK);

      /**
       * Adds a ring of LEDs to this entity.
       * @param c_center The position of LED ring center wrt the reference entity.
       * @param f_radius The radius of the LED ring.
       * @param c_start_angle The angle at which the first LED must be placed. Expressed wrt the local entity <em>x</em>-axis.
       * @param un_num_leds The number of LEDs to place along the ring.
       * @param s_anchor The anchor of the LED.
       * @param c_color The color of the LEDs.
       */
      void AddLEDRing(const CVector3& c_center,
                      Real f_radius,
                      const CRadians& c_start_angle,
                      UInt32 un_num_leds,
                      SAnchor& s_anchor,
                      const CColor& c_color = CColor::BLACK);

      /**
       * Returns an LED by numeric index.
       * @param un_index The index of the wanted LED.
       * @return An LED by numeric index.
       * @see GetAllLEDs()
       */
      CLEDEntity& GetLED(UInt32 un_index);

      /**
       * Returns all the LEDs.
       * @return All the LEDs.
       * @see GetLED()
       */
      inline SActuator::TList& GetLEDs() {
         return m_tLEDs;
      }

      /**
       * Returns the offset position of the given LED.
       * The actual position of an LED is calculated as the vector sum of 
       * the offset position and the anchor.
       * @return The offset position of the given LED.
       */
      inline const CVector3& GetLEDOffset(size_t un_idx) const {
         ARGOS_ASSERT(un_idx < m_tLEDs.size(),
                      "CLEDEquippedEntity::GetLEDOffset() : index " <<
                      un_idx <<
                      " out of bounds [0:" <<
                      m_tLEDs.size()-1 <<
                      "]" );
         return m_tLEDs[un_idx]->Offset;
      }

      /**
       * Sets the position of an LED.
       * @param un_index The index of the wanted LED.
       * @param c_offset The position of the LED wrt the anchor.
       */
      void SetLEDOffset(UInt32 un_index,
                        const CVector3& c_offset);

      /**
       * Sets the color of an LED.
       * @param un_index The index of the wanted LED.
       * @param c_color The color of the LED.
       */
      void SetLEDColor(UInt32 un_index,
                       const CColor& c_color);

      /**
       * Sets the color of all the LEDs to the same value.
       * @param c_color The color of the LEDs.
       * @see SetAllLEDsColors()
       */
      void SetAllLEDsColors(const CColor& c_color);

      /**
       * Sets the color of all the LEDs to the given setting.
       * @param vec_colors A vector containing the colors of the LEDs.
       * @see SetAllLEDsColors()
       * @throws CARGoSException if the size of the passed vector is different from
       * the number of LED.
       */
      void SetAllLEDsColors(const std::vector<CColor>& vec_colors);

      /**
       * Adds the LEDs to the wanted LED medium.
       * @param c_medium The LED medium.
       * @see CLEDMedium
       */
      void AddToMedium(CLEDMedium& c_medium);

      /**
       * Removes the LEDs from the associated LED medium.
       * @see CLEDMedium
       */
      void RemoveFromMedium();

      virtual std::string GetTypeDescription() const {
         return "leds";
      }

   protected:

      virtual void UpdateComponents();

   protected:

      /** List of the LEDs managed by this entity */
      SActuator::TList m_tLEDs;
   };

}

#endif
