/**
 * @file <argos3/core/utility/math/pose2.h>
 *
 * @author Tyagaraja Ramaswamy - <rtyagaraja@gmail.com>
 */

#ifndef POSE2_H
#define POSE2_H

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
namespace argos
{
   class CPose2 {
   
   public:
      ~CPose2() {
      }

      /**
       * Default constructor.
       */
      CPose2() : 
         m_cPosition(0,0),
         m_cOrientation(0) {
      }

      /**
       * Copy constructor.
       * @param c_pose The 2D pose to be copied. 
       */
      CPose2(const CPose2& c_pose) : 
         m_cPosition(c_pose.m_cPosition),
         m_cOrientation(c_pose.m_cOrientation) {
      }

      /**
       * Constructor using input position and angle.
       * @param c_position Position of the input pose in CVector2 format.
       * @param c_orientation Orientation of the input pose in CRadians format.
       */
      CPose2(const CVector2& c_position, const CRadians& c_orientation) : 
         m_cPosition(c_position),
         m_cOrientation(c_orientation) {
      }

      /**
       * Constructor using individual coordinate values and angle.
       * @param f_x X coordinate of the pose.
       * @param f_y A-coordinate of the pose.
       * @param f_th Angle of 2D the pose.
       */
      CPose2(const Real f_x, const Real f_y, const Real f_th) : 
         m_cPosition(f_x, f_y),
         m_cOrientation(f_th) {
      }

      /**
       * Returns the position of the pose.
       * @return 2D position of the pose.
       */
      inline const CVector2& GetPosition() const
      { return m_cPosition;}

      /**
       * Returns the orientation of the pose.
       * @return Orientation of the current pose.
       */
      inline const CRadians& GetOrientation() const
      { return m_cOrientation;}

      /**
       * Rotates the current pose by the given offset angle.
       * @param c_offset Offset by which the pose needs to be rotated.
       */
      inline void Rotate(const CRadians& c_offset) {
         m_cOrientation += c_offset;
      }

      /**
       * Translates the current pose by the given vector.
       * @param c_offset Offset vector by which the pose needs to be moved. 
       */
      inline void Translate(const CVector2& c_offset) {
         m_cPosition = (*this)*c_offset;
      }

      /**
       * Equality operator overloading.
       * @param c_pose Input pose with which the current pose needs to be compared.
       * @return True if the current pose is equal to the input pose.
       */
      inline bool operator==(const CPose2& c_pose) const {
         if(m_cPosition.m_fX != c_pose.m_cPosition.m_fX)
            return false;
         if(m_cPosition.m_fY != c_pose.m_cPosition.m_fY)
            return false;
         if(m_cOrientation != c_pose.m_cOrientation)
            return false;
         return true;
      }

      /**
       * Inequality operator overloading.
       * @param c_pose Input pose with which the current pose needs to be compared.
       * @return True if the current pose is not equal to the input pose.
       */
      inline bool operator!=(const CPose2& c_pose) const {
         if(m_cPosition.m_fX == c_pose.m_cPosition.m_fX)
            return false;
         if(m_cPosition.m_fY == c_pose.m_cPosition.m_fY)
            return false;
         if(m_cOrientation == c_pose.m_cOrientation)
            return false;
         return true;
      }

      /**
       * Serialize the contents of the 2D Pose onto a stream.
       * @param c_os The stream.
       * @param c_pose The 2D Pose.
       * @return The new state of the stream.
       */
      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CPose2& c_pose) {
         c_os << "Position("
              << c_pose.m_cPosition.GetX() << ","
              << c_pose.m_cPosition.GetY() << ")\n"
              << "Orientation("
              << c_pose.m_cOrientation.GetValue()
              << " radians"
              << " -> "
              << c_pose.m_cOrientation.GetValue() * CRadians::RADIANS_TO_DEGREES
              << " degrees)";
         return c_os;
      }

   public:
      /**
       * Constructor using input position and angle.
       * @param c_position Position of the input pose in CVector3 format.
       * @param c_orienatation Orientation of the input pose in CQuaternion format.
       */
      CPose2(const CVector3& c_position, const CQuaternion& c_orientation);

      /**
       * Computes the returns the inverse of the current pose.
       * @return Inverse of the current pose.
       */
      CPose2 Inverse() const;

      /**
       * Multiplication operator overloading. Computes and returns the product of the two poses. 
       * @param c_pose Input pose to be multiplied with the current pose.
       * @return The product of the two poses.
       */
      CPose2 operator*(const CPose2& c_pose) const;

      /**
       * Multiplication operator overloading. This method can be used to transform any vector with the current pose.
       * @param c_vec Input vector to be transformed with the current pose.
       * @return The trasformed vector.
       */
      CVector2 operator*(const CVector2& c_vec) const;

      /**
       * Multiplication operator overloading. Multiplies the current pose with the input pose and stores in the current pose.
       * @param c_pose input pose to be multiplied with the current pose.
       * @return Current Pose.
       */
      CPose2& operator*=(const CPose2& c_pose);

   private:
      CVector2 m_cPosition;
      CRadians m_cOrientation;
   };

} // namespace argos
#endif