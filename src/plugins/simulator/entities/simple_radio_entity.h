/**
 * @file <argos3/plugins/simulator/entities/simple_radio_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef SIMPLE_RADIO_ENTITY_H
#define SIMPLE_RADIO_ENTITY_H

namespace argos {
   class CSimpleRadioEntity;
   class CSimpleRadioMedium;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <mutex>

namespace argos {

   class CSimpleRadioEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

   public:

      typedef std::vector<CSimpleRadioEntity*> TList;

   public:

      CSimpleRadioEntity(CComposableEntity* pc_parent);

      CSimpleRadioEntity(CComposableEntity* pc_parent,
                         const std::string& str_id,
                         CSimpleRadioMedium& c_medium,
                         Real f_transmit_range);
                                            
      virtual ~CSimpleRadioEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Destroy();

      virtual void SetEnabled(bool b_enabled);

      /**
       * Returns a constant reference to the received messages.
       * @return A constant reference to the received messages.
       * @see ReceiveMessage()
       */
      inline const std::vector<std::pair<CVector3, CByteArray> >& GetMessages() const {
         return m_vecMessages;
      }

      /**
       * Returns a reference to the received messages.
       * @return A reference to the received messages.
       * @see ReceiveMessage()
       */
      inline std::vector<std::pair<CVector3, CByteArray> >& GetMessages() {
         return m_vecMessages;
      }

      /**
       * Adds data received by the radio
       * @param c_origin the origin of the message in the global coordinate system.
       * @param c_message a byte array containing the actual message.
       * @see GetMessages()
       */
      inline void ReceiveMessage(const CVector3& c_origin, const CByteArray& c_message) {
         std::unique_lock<std::mutex> cLock(m_mtxMessages);
         m_vecMessages.emplace_back(c_origin, c_message);
      }

      /**
       * Checks if there has been data received by the radio
       * @return A boolean value representing whether data has been received
       * @see GetMessages()
       * @see ClearMessages()
       */
      inline bool HasMessages() const {
         return !m_vecMessages.empty();
      }

      /**
       * Returns the transmission range of the radio.
       * @return the transmission range of the radio.
       * @see SetRange()
       */
      inline Real GetRange() const {
         return m_fRange;
      }

      /**
       * Sets the tranmission range of the radio
       * @param f_range the new transmission range of the radio in metres.
       * @see GetRange()
       */
      inline void SetRange(Real f_range) {
         m_fRange = f_range;
      }

      virtual std::string GetTypeDescription() const {
         return "radio";
      }

      /**
       * Returns <tt>true</tt> if this radio is associated to a medium.
       * @return <tt>true</tt> if this radio is associated to a medium.
       * @see CRadioMedium
       */
      inline bool HasMedium() const {
         return m_pcMedium != nullptr;
      }

      /**
       * Returns the medium associated to this radio.
       * @return The medium associated to this radio.
       * @see CRadioMedium
       */
      CSimpleRadioMedium& GetMedium() const;

      /**
       * Sets the medium associated to this entity.
       * @param c_medium The medium to associate to this entity.
       * @see CRadioMedium
       */
      void SetMedium(CSimpleRadioMedium& c_medium);

   protected:

      CSimpleRadioMedium* m_pcMedium;
      Real m_fRange;
      std::vector<std::pair<CVector3, CByteArray> > m_vecMessages;
      std::mutex m_mtxMessages;

   };

   /****************************************/
   /****************************************/

   class CSimpleRadioEntitySpaceHashUpdater : public CSpaceHashUpdater<CSimpleRadioEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CSimpleRadioEntity>& c_space_hash,
                              CSimpleRadioEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

   class CSimpleRadioEntityGridUpdater : public CGrid<CSimpleRadioEntity>::COperation {

   public:

      CSimpleRadioEntityGridUpdater(CGrid<CSimpleRadioEntity>& c_grid);
      virtual bool operator()(CSimpleRadioEntity& c_entity);

   private:

      CGrid<CSimpleRadioEntity>& m_cGrid;
      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
