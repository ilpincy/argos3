/**
 * @file <argos3/core/utility/math/pose2.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
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
    * @brief default constructor
    */
   CPose2() : 
      m_cPosition(0,0),
      m_cOrientation(0) {
   }

   /**
    * @brief copy constructor
    * @param[in] the 2D pose to be copied 
    */
   CPose2(const CPose2& c_pose) : 
      m_cPosition(c_pose.m_cPosition),
      m_cOrientation(c_pose.m_cOrientation) {
   }

   /**
    * @brief constructor using input position and angle
    * @param[in] position of the input pose in CVector2 format
    * @param[in] orientation of the input pose in CRadians format
    */
   CPose2(const CVector2& c_position, const CRadians& c_orientation) : 
      m_cPosition(c_position),
      m_cOrientation(c_orientation) {
   }

   /**
    * @brief constructor using individual coordinate values and angle
    * @param[in] x-coordinate of the pose
    * @param[in] y-coordinate of the pose
    * @param[in] angle of the pose
    */
   CPose2(const Real f_x, const Real f_y, const Real f_th) : 
      m_cPosition(f_x, f_y),
      m_cOrientation(f_th) {
   }

   /**
    * @brief function returns the position of the pose
    * @return 2D position of the pose
    */
   const CVector2& GetPosition() const
   { return m_cPosition;}

   /**
    * @brief function returns the orientation of the pose
    * @return orientation of the current pose
    */
   const CRadians& GetOrientation() const
   { return m_cOrientation;}

   /**
    * @brief equal to operator overloading
    * @param[in] input pose with which the current pose needs to be compared
    * @return returns true if the current pose is equal to the input pose 
    */
   bool operator==(const CPose2& c_pose) const {
      if(m_cPosition.m_fX != c_pose.m_cPosition.m_fX)
         return false;
      if(m_cPosition.m_fY != c_pose.m_cPosition.m_fY)
         return false;
      if(m_cOrientation != c_pose.m_cOrientation)
         return false;
      return true;
   }

   /**
    * @brief inequality operator overloading
    * @param[in] input pose with which the current pose needs to be compared
    * @return returns true if the current pose is not equal to the input pose 
    */
   bool operator!=(const CPose2& c_pose) const {
      if(m_cPosition.m_fX == c_pose.m_cPosition.m_fX)
         return false;
      if(m_cPosition.m_fY == c_pose.m_cPosition.m_fY)
         return false;
      if(m_cOrientation == c_pose.m_cOrientation)
         return false;
      return true;
   }


   public:
   /**
    * @brief constructor using input position and angle
    * @param[in] position of the input pose in CVector3 format
    * @param[in] orientation of the input pose in CQuaternion format
    */
   CPose2(const CVector3& c_position, const CQuaternion& c_orientation);

   /**
    * @brief function rotates the current pose by the given offset angle
    * @param[in] offset by which the pose needs to be rotated
    */
   void Rotate(const CRadians& c_offset);

   /**
    * @brief function translates the current pose by the given vector
    * @param[in] offset vector by which the pose needs to be moved 
    */
   void Translate(const CVector2& c_offset);

   /**
    * @brief function computes the returns the inverse of the current pose
    * @return returns the inverse of the current pose
    */
   CPose2 Inverse() const;

   /**
    * @brief multiplication operator overloading. Computes and returns the product of the two poses 
    * @param[in] input pose to be multiplied with the current pose
    * @return returns the product of the two poses
    */
   CPose2 operator*(const CPose2& c_pose) const;

   /* edit this comment */
   /**
    * @brief multiplication operator overloading. Computes the product of the a pose and a vector ab=nd returns the resulting vector
    * @param[in] input vector to be multiplied with the current pose
    * @return returns the product of the two poses
    */
   CVector2 operator*(const CVector2& c_vec) const;

   /**
    * @brief multiplication operator overloading. Multiplies the current pose with the input pose and stores in the current pose
    * @param[in] input pose to be multiplied with the current pose
    */
   CPose2& operator*=(const CPose2& c_pose);

   private:
      CVector2 m_cPosition;
      CRadians m_cOrientation;
   };

} // namespace argos
#endif