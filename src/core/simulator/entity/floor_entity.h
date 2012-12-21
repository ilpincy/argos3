/**
 * @file core/simulator/entity/floor_entity.h
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
         virtual CColor GetColorAtPoint(Real f_x,
                                        Real f_y) = 0;

         virtual void SaveAsImage(const std::string& str_path) = 0;

      };

   public:

      CFloorEntity();
      virtual ~CFloorEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy() {}

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

      inline virtual std::string GetTypeDescription() const {
         return "floor_entity";
      }

   private:

      CFloorColorSource* m_pcColorSource;
      CVector3           m_cFloorSize;
      std::string        m_strColorSource;
      unsigned int       m_unPixelsPerMeter;
      bool               m_bHasChanged;
   };
}

#endif
