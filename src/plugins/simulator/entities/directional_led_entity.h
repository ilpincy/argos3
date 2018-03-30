/**
 * @file <argos3/plugins/simulator/entities/directional_led_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DIRECTIONAL_LED_ENTITY_H
#define DIRECTIONAL_LED_ENTITY_H

namespace argos {
   class CDirectionalLEDEntity;
   class CDirectionalLEDMedium;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>

namespace argos {

   class CDirectionalLEDEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CDirectionalLEDEntity*> TList;
      typedef CSet<CDirectionalLEDEntity*> TSet;

   public:

      CDirectionalLEDEntity(CComposableEntity* pc_parent);

      CDirectionalLEDEntity(CComposableEntity* pc_parent,
                            const std::string& str_id,
                            const CVector3& c_position,
                            const CQuaternion& c_orientation,
                            const CRadians& c_observable_angle,
                            const CColor& c_color);

      virtual ~CDirectionalLEDEntity() {}

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
       * Returns the observable angle of the LED.
       * @return the observable angle of the LED.
       */
      inline const CRadians& GetObservableAngle() const {
         return m_cObservableAngle;
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
         return "directional_led";
      }

      /**
       * Returns <tt>true</tt> if this directional LED is associated to a medium.
       * @return <tt>true</tt> if this directional LED is associated to a medium.
       * @see CDirectionalLEDMedium
       */
      inline bool HasMedium() const {
         return m_pcMedium != nullptr;
      }

      /**
       * Returns the medium associated to this directional LED.
       * @return The medium associated to this directional LED.
       * @see CDirectionalLEDMedium
       */
      CDirectionalLEDMedium& GetMedium() const;

      /**
       * Sets the medium associated to this entity.
       * @param c_medium The medium to associate to this entity.
       * @see CDirectionalLEDMedium
       */
      void SetMedium(CDirectionalLEDMedium& c_medium);

   protected:

      CRadians m_cObservableAngle;
      CColor m_cColor;
      CColor m_cInitColor;
      CDirectionalLEDMedium* m_pcMedium;

   };

   /****************************************/
   /****************************************/

   class CDirectionalLEDEntitySpaceHashUpdater : public CSpaceHashUpdater<CDirectionalLEDEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CDirectionalLEDEntity>& c_space_hash,
                              CDirectionalLEDEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

   class CDirectionalLEDEntityGridUpdater : public CGrid<CDirectionalLEDEntity>::COperation {

   public:

      CDirectionalLEDEntityGridUpdater(CGrid<CDirectionalLEDEntity>& c_grid);
      virtual bool operator()(CDirectionalLEDEntity& c_entity);

   private:

      CGrid<CDirectionalLEDEntity>& m_cGrid;
      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
