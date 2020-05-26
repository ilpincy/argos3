/**
 * @file <argos3/core/utility/math/pose2.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef POSE2_H
#define POSE2_H

#include <argos3/core/utility/math/vector2.h>
namespace argos
{
   class CPose2 {
   
   public:
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
    * @param[in] position of the input pose
    * @param[in] orientation of the input pose
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

   private:
      CVector2 m_cPosition;
      CRadians m_cOrientation;


   };

} // namespace argos
#endif