/**
 * @file <argos3/core/simulator/entity/led_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LED_ENTITY_H
#define LED_ENTITY_H

namespace argos {
   class CLEDEntity;
   class CLEDMedium;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>

namespace argos {

   class CLEDEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CLEDEntity*> TList;
      typedef CSet<CLEDEntity*> TSet;

   public:

      CLEDEntity(CComposableEntity* pc_parent);

      CLEDEntity(CComposableEntity* pc_parent,
                 const std::string& str_id,
                 const CVector3& c_position,
                 const CColor& c_color);

      virtual ~CLEDEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Destroy();

      virtual void SetEnabled(bool b_enabled);

      /**
       * Returns the current color of the LED.
       * @return the current color of the LED.
       * @see GetInitColor()
       * @see SetColor()
       */
      inline const CColor& GetColor() const {
         return m_cColor;
      }

      /**
       * Returns the color with which the LED was initialized.
       * When the simulation is reset, the LED color is set to this value.
       * @return the color with which the LED was initialized.
       * @see GetColor()
       * @see SetInitColor()
       */
      inline const CColor& GetInitColor() const {
         return m_cInitColor;
      }

      /**
       * Sets the current color of the LED.
       * @param c_color the wanted color.
       * @see GetColor()
       */
      void SetColor(const CColor& c_color);

      /**
       * Sets the initialization color for this LED.
       * When the simulation is reset, the LED color is set to this value.
       * @param c_color the initialization color for this LED.
       * @see GetInitColor()
       * @see SetColor()
       */
      inline void SetInitColor(const CColor& c_color) {
         m_cInitColor = c_color;
      }

      virtual std::string GetTypeDescription() const {
         return "led";
      }

      /**
       * Adds the LEDs to the wanted LED medium.
       * If this LED has already been added to a medium, the LED is
       * removed from that medium and then added to the passed one.
       * This behavior is to enforce that, at any time, an LED is
       * under the control of (at most) a single medium.
       * @param c_medium The LED medium.
       * @see CLEDMedium
       */
      void AddToMedium(CLEDMedium& c_medium);

      /**
       * Removes the LEDs from the wanted LED medium.
       * @param c_medium The LED medium.
       * @see CLEDMedium
       */
      void RemoveFromMedium();

      /**
       * Returns <tt>true</tt> if this LED is associated to a medium.
       * @return <tt>true</tt> if this LED is associated to a medium.
       * @see CLEDMedium
       */
      inline bool HasMedium() const {
         return m_pcMedium != NULL;
      }

      /**
       * Returns the medium associated to this LED.
       * @return The medium associated to this LED.
       * @see CLEDMedium
       */
      CLEDMedium& GetMedium() const;

   protected:

      CColor m_cColor;
      CColor m_cInitColor;
      CLEDMedium* m_pcMedium;

   };

   /****************************************/
   /****************************************/

   class CLEDEntitySpaceHashUpdater : public CSpaceHashUpdater<CLEDEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CLEDEntity>& c_space_hash,
                              CLEDEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

   class CLEDEntityGridUpdater : public CGrid<CLEDEntity>::COperation {

   public:

      CLEDEntityGridUpdater(CGrid<CLEDEntity>& c_grid);
      virtual bool operator()(CLEDEntity& c_entity);

   private:

      CGrid<CLEDEntity>& m_cGrid;
      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
