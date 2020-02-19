/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_magnetism_plugin.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Haitham Elfaham - <haithamelfaham@gmail.com>
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 */

#include "dynamics3d_magnetism_plugin.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   void CDynamics3DMagnetismPlugin::Init(TConfigurationNode& t_tree) {
      GetNodeAttributeOrDefault(t_tree, "force_constant", m_fForceConstant, m_fForceConstant);
      GetNodeAttributeOrDefault(t_tree, "max_distance", m_fMaxDistance, m_fMaxDistance);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMagnetismPlugin::RegisterModel(CDynamics3DModel& c_model) {
      for(std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body : c_model.GetBodies()) {
         for(const CDynamics3DModel::CAbstractBody::SData::SDipole& s_dipole :
             ptr_body->GetData().Dipoles) {
            m_vecDipoles.emplace_back(ptr_body,
                                      s_dipole.GetField,
                                      s_dipole.Offset);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMagnetismPlugin::UnregisterModel(CDynamics3DModel& c_model) {
      std::vector<SMagneticDipole>::iterator itRemove =
         std::remove_if(std::begin(m_vecDipoles),
                        std::end(m_vecDipoles),
                        [&c_model] (const SMagneticDipole& c_magnetic_dipole) {
                           return(&(c_magnetic_dipole.Body->GetModel()) == &c_model);
                        });
      m_vecDipoles.erase(itRemove, std::end(m_vecDipoles));
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMagnetismPlugin::Update() {
      if(m_vecDipoles.size() < 2) {
         /* Nothing to do */
         return;
      }
      for(std::vector<SMagneticDipole>::iterator itDipole0 = std::begin(m_vecDipoles);
          itDipole0 != (std::end(m_vecDipoles) - 1);
          ++itDipole0) {
         for(std::vector<SMagneticDipole>::iterator itDipole1 = std::next(itDipole0, 1);
             itDipole1 != std::end(m_vecDipoles);
             ++itDipole1) {
            const btTransform& cTransformDipole0 = itDipole0->Offset * itDipole0->Body->GetTransform();
            const btTransform& cTransformDipole1 = itDipole1->Offset * itDipole1->Body->GetTransform();
            const btVector3& cPositionDipole0 = cTransformDipole0.getOrigin();
            const btVector3& cPositionDipole1 = cTransformDipole1.getOrigin();
            /* calculate the distance between the two magnetic bodies */
            btScalar fDistance = cPositionDipole0.distance(cPositionDipole1);
            /* optimization - don't calculate magnetism for dipoles more than m_fMaxDistance apart */
            if(fDistance > m_fMaxDistance) {
               continue;
            }
            /* perform Barnes-hut algorithm */
            /* calculate the normalized seperation between the two dipoles, pointing from Dipole1 to Dipole0*/
            const btVector3& cNormalizedSeparation =
               btVector3(cPositionDipole0 - cPositionDipole1) / fDistance;
            /* calculate the rotated field of dipole 0 */
            const btVector3& cRotatedFieldDipole0 =
               itDipole0->Body->GetTransform().getBasis() * itDipole0->GetField();
            /* calculate the rotated field of dipole 1 */
            const btVector3& cRotatedFieldDipole1 =
               itDipole1->Body->GetTransform().getBasis() * itDipole1->GetField();
            /* We now have cRotatedFieldDipole0 and cRotatedFieldDipole1 as the magnetic moments
               (i.e., m0, m1), cNormalizedSeparation as the direction unit vector from Dipole 1
               to Dipole 0 (i.e., n), fDistance is the scalar distance between the dipoles (i.e.,
               d), and B0 is the magnetic flux density at Dipole 0.
                  B0 = u0/4pi * [3n(n.m1) - m1] / d^3
                  T0 = m0 * B0
                     = u0.4pi/d^3 * [3 (m1.n)(m0 * n) - m0 * m1]

                  F0 = grad(m0.B0)
                     = u0.4pi/d^4 * [-15n(m0.n)(m1.n) + 3n(m0.m1) + 3(m0(m1.n)+m1(m0.n))]
            */
            /* calculate the intermediate cross and dot products */
            const btVector3& cCrossProduct01 = cRotatedFieldDipole0.cross(cRotatedFieldDipole1);
            const btVector3& cCrossProduct0 = cRotatedFieldDipole0.cross(cNormalizedSeparation);
            const btVector3& cCrossProduct1 = cRotatedFieldDipole1.cross(cNormalizedSeparation);
            btScalar fDotProduct01 = cRotatedFieldDipole0.dot(cRotatedFieldDipole1);
            btScalar fDotProduct0 = cRotatedFieldDipole0.dot(cNormalizedSeparation);
            btScalar fDotProduct1 = cRotatedFieldDipole1.dot(cNormalizedSeparation);
            /* calculate the magnetic force and torque */
            const btVector3& cTorque0 =
               ((3 * fDotProduct1 * cCrossProduct0) - cCrossProduct01) *
               m_fForceConstant / btPow(fDistance, 3);
            const btVector3& cTorque1 =
               ((3 * fDotProduct0 * cCrossProduct1) + cCrossProduct01) *
               m_fForceConstant / btPow(fDistance, 3);
            const btVector3& cForce = (m_fForceConstant / btPow(fDistance, 4)) *
               ((-15 * cNormalizedSeparation * fDotProduct1 * fDotProduct0) +
                (3 * cNormalizedSeparation * fDotProduct01) +
                (3 * (fDotProduct1 * cRotatedFieldDipole0 + fDotProduct0 * cRotatedFieldDipole1)));
            /* apply torques and forces to the bodies */
            itDipole0->Body->ApplyForce(cForce, (itDipole0->Offset).getOrigin());
            itDipole0->Body->ApplyTorque(cTorque0);
            itDipole1->Body->ApplyForce(-cForce, (itDipole1->Offset).getOrigin());
            itDipole1->Body->ApplyTorque(cTorque1);
         }
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_DYNAMICS3D_PLUGIN(CDynamics3DMagnetismPlugin,
                              "magnetism",
                              "Michael Allwright [allsey87@gmail.com]",
                              "1.0",
                              "Applies forces and torques between magnetic dipoles in the simulation",
                              "For a description on how to use this plugin, please consult the documentation\n"
                              "for the dynamics3d physics engine plugin",
                              "Usable");

   /****************************************/
   /****************************************/

}
