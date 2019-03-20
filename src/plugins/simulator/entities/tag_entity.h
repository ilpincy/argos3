/**
 * @file <argos3/plugins/simulator/entities/tag_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef TAG_ENTITY_H
#define TAG_ENTITY_H

namespace argos {
   class CTagEntity;
   class CTagMedium;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>

namespace argos {

   class CTagEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CTagEntity*> TList;
      typedef CSet<CTagEntity*> TSet;

   public:

      CTagEntity(CComposableEntity* pc_parent);

      CTagEntity(CComposableEntity* pc_parent,
                 const std::string& str_id,
                 const CVector3& c_position,
                 const CQuaternion& c_orientation,
                 const CRadians& c_observable_angle,
                 Real f_side_length,
                 const std::string& str_payload);

      virtual ~CTagEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Destroy();

      virtual void SetEnabled(bool b_enabled);

      /**
       * Returns the current payload of the tag.
       * @return the current payload of the tag.
       * @see GetInitPayload()
       * @see SetPayload()
       */
      inline const std::string& GetPayload() const {
         return m_strPayload;
      }

      /**
       * Returns the observable angle of the tag.
       * @return the observable angle of the tag.
       */
      inline const CRadians& GetObservableAngle() const {
         return m_cObservableAngle;
      }

      /**
       * Returns the side length of the tag.
       * @return the side length of the tag.
       */
      inline Real GetSideLength() const {
         return m_fSideLength;
      }

      /**
       * Returns the payload with which the tag was initialized.
       * When the simulation is reset, the tag payload is set to this value.
       * @return the payload with which the tag was initialized.
       * @see GetPayload()
       * @see SetInitPayload()
       */
      inline const std::string& GetInitPayload() const {
         return m_strInitPayload;
      }

      /**
       * Sets the current payload of the tag.
       * @param c_payload the wanted payload.
       * @see GetPayload()
       */
      void SetPayload(const std::string& str_payload);

      /**
       * Sets the initialization payload for this tag.
       * When the simulation is reset, the tag payload is set to this value.
       * @param c_payload the initialization payload for this tag.
       * @see GetInitPayload()
       * @see SetPayload()
       */
      inline void SetInitPayload(const std::string& str_payload) {
         m_strInitPayload = str_payload;
      }

      virtual std::string GetTypeDescription() const {
         return "tag";
      }

      /**
       * Returns <tt>true</tt> if this tag is associated to a medium.
       * @return <tt>true</tt> if this tag is associated to a medium.
       * @see CTagMedium
       */
      inline bool HasMedium() const {
         return m_pcMedium != nullptr;
      }

      /**
       * Returns the medium associated to this tag.
       * @return The medium associated to this tag.
       * @see CTagMedium
       */
      CTagMedium& GetMedium() const;

      /**
       * Sets the medium associated to this entity.
       * @param c_medium The medium to associate to this entity.
       * @see CTagMedium
       */
      void SetMedium(CTagMedium& c_medium);


   protected:

      CRadians m_cObservableAngle;
      Real m_fSideLength;
      std::string m_strPayload;
      std::string m_strInitPayload;
      CTagMedium* m_pcMedium;

   };

   /****************************************/
   /****************************************/

   class CTagEntitySpaceHashUpdater : public CSpaceHashUpdater<CTagEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CTagEntity>& c_space_hash,
                              CTagEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

   class CTagEntityGridUpdater : public CGrid<CTagEntity>::COperation {

   public:

      CTagEntityGridUpdater(CGrid<CTagEntity>& c_grid);
      virtual bool operator()(CTagEntity& c_entity);

   private:

      CGrid<CTagEntity>& m_cGrid;
      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
