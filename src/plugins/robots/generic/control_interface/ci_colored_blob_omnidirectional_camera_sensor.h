#ifndef CI_COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_SENSOR_H
#define CI_COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_SENSOR_H

namespace argos {
   class CCI_ColoredBlobOmnidirectionalCameraSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/datatypes/color.h>
#include <vector>

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /**
    * This class provides the most general interface to a camera.
    * The camera sensor enables the user to extract information from the images
    * acquired by the simulated or by the physical camera.
    * This interface defines also the basic type of information that at the moment
    * it is possible to extract from image processing on the real robot, that is the position
    * of the colored LED of neighboring robots.
    * The camera can be enabled and disabled, in order to save computation time.
    */
   class CCI_ColoredBlobOmnidirectionalCameraSensor: public CCI_Sensor {

   public:

      /**
       * An SBlob represents a generic colored 2D segment in the image.
       *
       * A blob has a color as defined by the CColor class. The position of the blob is given in
       * polar coordinates. The angle is counted counter-clockwise watching the FootBot from top, as to respect the
       * general frame convention. As for the distances, both the distance in cms from the robot center and in pixels from the
       * optical center in the image are given. The last attribute is the area of the blob in pixels.
       *
       * @see CColor
       */
      struct SBlob {
         /* Color */
         CColor Color;
         /* Angle */
         CRadians Angle;
         /* Distance in cms */
         Real Distance;
         /**
          * Constructor
          */
         SBlob() :
            Color(CColor::BLACK),
            Distance(0.0f) {
         }
         /**
          * Constructor with paramters
          * @param c_color Blob color
          * @param c_angle Blob center
          * @param f_distance Blob distance in cm
          */
         SBlob(const CColor& c_color,
               const CRadians& c_angle,
               Real f_distance) :
            Color(c_color),
            Angle(c_angle),
            Distance(f_distance) {
         }

         friend std::ostream& operator<<(std::ostream& c_os, const SBlob& s_blob) {
            c_os << "(Color = " << s_blob.Color << "," << "Angle = " << ToDegrees(s_blob.Angle) << ","
                 << "Distance = " << s_blob.Distance << ")";
            return c_os;
         }
      };

      /**
       * Vector of pointers to colored blobs.
       */
      typedef std::vector<SBlob*> TBlobList;

      /**
       * It represents the readings collected through the camera at a specific time step.
       * It consists of two members, the vector of colored blobs detected and a counter which stores the time step
       * at which the blobs became available.
       */
      struct SReadings {
         TBlobList BlobList;
         UInt64 Counter;

         SReadings() :
            Counter(0) {
         }

         friend std::ostream& operator<<(std::ostream& c_os, const SReadings& s_reading) {
            c_os << "Counter: " <<  s_reading.Counter << std::endl;
            for (size_t i = 0; i < s_reading.BlobList.size(); i++) {
               c_os << "Blob[" << i << "]: " << s_reading.BlobList[i] << std::endl;
            }
            return c_os;
         }
      };

   public:

      /**
       * Constructor
       */
      CCI_ColoredBlobOmnidirectionalCameraSensor() {}

      /**
       * Destructor
       */
      virtual ~CCI_ColoredBlobOmnidirectionalCameraSensor() {}

      /**
       * Returns a reference to the current camera readings.
       * @return A reference to the current camera readings.
       */
      const SReadings& GetReadings() const;

      /**
       * Enables image acquisition and processing.
       */
      virtual void Enable() = 0;

      /**
       * Disables image acquisition and processing.
       */
      virtual void Disable() = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SReadings m_sReadings;

   };

}

#endif
