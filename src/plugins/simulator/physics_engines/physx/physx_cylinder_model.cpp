/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_cylinder_model.h"

namespace argos {

   /****************************************/
   /****************************************/

   /*
    * By default, the cylinder mesh is NULL
    */
   physx::PxConvexMesh* CPhysXCylinderModel::m_pcCylinderMesh = NULL;

   /*
    * Number of slices along the cylinder height
    */
   static const size_t NUM_SLICES = 20;

   /****************************************/
   /****************************************/

   CPhysXCylinderModel::CPhysXCylinderModel(CPhysXEngine& c_engine,
                                            CCylinderEntity& c_entity) :
      CPhysXStretchableObjectModel(c_engine, c_entity) {
      /* Create mesh, if necessary */
      if(m_pcCylinderMesh == NULL) {
         CreateMesh();
      }
      /* Set center of cylinder base */
      m_cBaseCenterLocal.x = 0.0f;
      m_cBaseCenterLocal.y = -c_entity.GetHeight() * 0.5f;
      m_cBaseCenterLocal.z = 0.0f;
      /* Get position and orientation in this engine's representation */
      physx::PxVec3 cPos;
      CVector3ToPxVec3(GetEmbodiedEntity().GetPosition(), cPos);
      physx::PxQuat cOrient;
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOrientation(), cOrient);
      /* Create the transform
       * 1. a translation up by half size on y
       * 2. a rotation around the box base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(physx::PxVec3(0.0f, c_entity.GetHeight() * 0.5f, 0.0f));
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Create cylinder geometry */
      physx::PxConvexMeshGeometry cGeometry(m_pcCylinderMesh,
                                            physx::PxMeshScale(
                                               physx::PxVec3(c_entity.GetRadius(),
                                                             c_entity.GetHeight(),
                                                             c_entity.GetRadius()),
                                               physx::PxQuat::createIdentity()));
      /* Create the cylinder body */
      if(GetEmbodiedEntity().IsMovable()) {
         /*
          * The cylinder is movable
          */
         /* Create the body in its initial position and orientation */
         m_pcDynamicBody = GetPhysXEngine().GetPhysics().createRigidDynamic(cFinalTrans);
         /* Create the shape */
         m_pcShape = m_pcDynamicBody->createShape(cGeometry,
                                                  GetPhysXEngine().GetDefaultMaterial());
         /* Switch continuous collision detection (CCD) on */
         m_pcShape->setFlag(physx::PxShapeFlag::eUSE_SWEPT_BOUNDS, true);
         /* Set body mass */
         physx::PxRigidBodyExt::setMassAndUpdateInertia(*m_pcDynamicBody, m_fMass);
         /* Add body to the scene */
         GetPhysXEngine().GetScene().addActor(*m_pcDynamicBody);
      }
      else {
         /*
          * The cylinder is not movable
          */
         /* Create the body in its initial position and orientation */
         m_pcStaticBody = GetPhysXEngine().GetPhysics().createRigidStatic(cFinalTrans);
         /* Create the shape */
         m_pcShape = m_pcStaticBody->createShape(cGeometry,
                                                 GetPhysXEngine().GetDefaultMaterial());
         /* Add body to the scene */
         GetPhysXEngine().GetScene().addActor(*m_pcStaticBody);
      }
      /* Assign the user data pointer to this model */
      m_pcShape->userData = this;
      /* Calculate bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CPhysXCylinderModel::CreateMesh() {
      /* Array of vertices */
      physx::PxVec3* pcVertices = new physx::PxVec3[NUM_SLICES * 2];
      /* Description of the convex mesh */
      physx::PxConvexMeshDesc cDescription;
      /* Calculate the angle span of a slice */
      CRadians cSlice(CRadians::TWO_PI / NUM_SLICES);
      /* Go through the slices */
      for(size_t i = 0; i < NUM_SLICES; ++i) {
         /* Top face */
         pcVertices[i].x = Cos(cSlice * i);
         pcVertices[i].y = 0.5f;
         pcVertices[i].z = Sin(cSlice * i);
         /* Bottom face */
         pcVertices[i+NUM_SLICES].x = pcVertices[i].x;
         pcVertices[i+NUM_SLICES].y = -0.5f;
         pcVertices[i+NUM_SLICES].z = pcVertices[i].z;
      }
      /* Set up the description */
      cDescription.points.count  = NUM_SLICES * 2;
      cDescription.points.stride = sizeof(physx::PxVec3);
      cDescription.points.data   = pcVertices;
      cDescription.flags         = physx::PxConvexFlag::eCOMPUTE_CONVEX;
      /* Buffer to store the cooked mesh */
      physx::PxDefaultMemoryOutputStream cCookedMeshBuf;
      /* Cook mesh */
      if(! GetPhysXEngine().GetCooking().cookConvexMesh(cDescription,
                                                        cCookedMeshBuf)) {
         THROW_ARGOSEXCEPTION("PhysX engine \"" <<
                              GetPhysXEngine().GetId() <<
                              "\": error creating the cylinder convex mesh");
      }
      /* Buffer to store the mesh data */
      physx::PxDefaultMemoryInputData cMeshData(cCookedMeshBuf.getData(),
                                                cCookedMeshBuf.getSize());
      /* Create mesh */
      m_pcCylinderMesh = GetPhysXEngine().GetPhysics().createConvexMesh(cMeshData);
      /* Dispose of vertex array */
      delete[] pcVertices;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CCylinderEntity, CPhysXCylinderModel);

   /****************************************/
   /****************************************/

}
