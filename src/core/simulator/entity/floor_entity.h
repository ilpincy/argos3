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

#ifdef ARGOS_WITH_FREEIMAGE
         virtual void SaveAsImage(const std::string& str_path) = 0;
#endif

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
#ifdef ARGOS_WITH_FREEIMAGE
      CFloorEntity(const std::string& str_id,
                   const std::string& str_file_name);
#endif
      
      /**
       * Class constructor.
       * Creates a floor with the given id, using the loop functions as color source.
       */
      CFloorEntity(const std::string& str_id,
                   UInt32 un_pixels_per_meter);

      /**
       * Class destructor.
       */
      virtual ~CFloorEntity();

      /**
       * Initializes the entity from an XML tree.
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * Restores the initial state of the floor.
       */
      virtual void Reset();

      /**
       * Returns the color at the given point.
       * @param f_x The x coordinate on the floor
       * @param f_y The y coordinate on the floor
       * @returns the color at the given point
       */
      inline CColor GetColorAtPoint(Real f_x,
                                    Real f_y) {
         ARGOS_ASSERT(m_pcColorSource != NULL,
                      "The floor entity \"" <<
                      GetId() <<
                      "\" has no associated color source.");
         return m_pcColorSource->GetColorAtPoint(f_x, f_y);
      }

      /**
       * Returns <tt>true</tt> if the floor color has changed.
       * It is mainly used by the OpenGL visualization to know when to create a new texture.
       * @return <tt>true</tt> if the floor color has changed.
       */
      inline bool HasChanged() const {
         return m_bHasChanged;
      }

      /**
       * Marks the floor color as changed.
       * @see HasChanged
       */
      inline void SetChanged() {
         m_bHasChanged = true;
      }

      /**
       * Marks the floor color as not changed.
       * @see HasChanged
       */
      inline void ClearChanged() {
         m_bHasChanged = false;
      }

      /**
       * Saves the current floor color to the given file.
       * Used by the OpenGL visualization to create a texture.
       * The type of the file is inferred from the extension of the passed path.
       * @param str_path The path of the output file.
       */
#ifdef ARGOS_WITH_FREEIMAGE
      void SaveAsImage(const std::string& str_path);
#endif

      virtual std::string GetTypeDescription() const {
         return "floor";
      }

   private:

      /**
       * Contains the type of color source for the floor.
       */
      EColorSource       m_eColorSource;

      /**
       * Pointer to the actual color source for the floor.
       */
      CFloorColorSource* m_pcColorSource;

      /**
       * Set to <tt>true</tt> when the floor color has changed.
       */
      bool               m_bHasChanged;
   };
}

#endif
