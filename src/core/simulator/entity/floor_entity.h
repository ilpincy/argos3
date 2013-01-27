/**
 * @file <argos3/core/simulator/entity/floor_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FLOOR_ENTITY_H
#define FLOOR_ENTITY_H

namespace argos {
   class CFloorEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   class CFloorEntity : public CEntity {

   public:

      class CFloorColorSource {

      public:

         virtual ~CFloorColorSource() {}

         virtual void Reset() {}

         virtual CColor GetColorAtPoint(Real f_x,
                                        Real f_y) = 0;

         virtual void SaveAsImage(const std::string& str_path) = 0;

      };

   public:

      ENABLE_VTABLE();

      enum EColorSource {
         UNSET = 0,
         FROM_IMAGE,
         FROM_LOOP_FUNCTIONS
      };

   public:

      /**
       * Class constructor.
       * It is meant to be used in conjuction with the Init() method.
       */
      CFloorEntity();

      /**
       * Class constructor.
       * Creates a floor with the given id and the given image file as color source.
       * The given path can include environment variables, which are expanded
       * internally.
       */
      CFloorEntity(const std::string& str_id,
                   const std::string& str_file_name);

      /**
       * Class constructor.
       * Creates a floor with the given id, using the loop functions as color source.
       */
      CFloorEntity(const std::string& str_id,
                   UInt32 un_pixels_per_meter);

      virtual ~CFloorEntity();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      inline CColor GetColorAtPoint(Real f_x,
                                    Real f_y) {
         ARGOS_ASSERT(m_pcColorSource != NULL,
                      "The floor entity \"" <<
                      GetId() <<
                      "\" has no associated color source.");
         return m_pcColorSource->GetColorAtPoint(f_x, f_y);
      }

      inline bool HasChanged() const {
         return m_bHasChanged;
      }

      inline void SetChanged() {
         m_bHasChanged = true;
      }

      inline void ClearChanged() {
         m_bHasChanged = false;
      }

      inline void SaveAsImage(const std::string& str_path) {
         m_pcColorSource->SaveAsImage(str_path);
      }

      virtual std::string GetTypeDescription() const {
         return "floor_entity";
      }

      virtual void Update() {}

   private:

      EColorSource       m_eColorSource;
      CFloorColorSource* m_pcColorSource;
      bool               m_bHasChanged;
   };
}

#endif
