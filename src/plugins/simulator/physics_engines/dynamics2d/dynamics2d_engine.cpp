/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_engine.h"
#include "dynamics2d_model.h"
#include "dynamics2d_gripping.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

#include <cmath>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DEngine::CDynamics2DEngine() :
      m_fBoxLinearFriction(1.49),
      m_fBoxAngularFriction(1.49),
      m_fCylinderLinearFriction(1.49),
      m_fCylinderAngularFriction(1.49),
      m_ptSpace(NULL),
      m_ptGroundBody(NULL),
      m_fElevation(0.0f) {
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CPhysicsEngine::Init(t_tree);
         /* Parse XML */
         GetNodeAttributeOrDefault(t_tree, "elevation", m_fElevation, m_fElevation);
         if(NodeExists(t_tree, "friction")) {
            TConfigurationNode& tNode = GetNode(t_tree, "friction");
            GetNodeAttributeOrDefault(tNode, "box_linear_friction",       m_fBoxLinearFriction,       m_fBoxLinearFriction);
            GetNodeAttributeOrDefault(tNode, "box_angular_friction",      m_fBoxAngularFriction,      m_fBoxAngularFriction);
            GetNodeAttributeOrDefault(tNode, "cylinder_linear_friction",  m_fCylinderLinearFriction,  m_fCylinderLinearFriction);
            GetNodeAttributeOrDefault(tNode, "cylinder_angular_friction", m_fCylinderAngularFriction, m_fCylinderAngularFriction);
         }
         /* Override volume top and bottom with the value of m_fElevation */
         if(!GetVolume().TopFace)    GetVolume().TopFace    = new SHorizontalFace;
         if(!GetVolume().BottomFace) GetVolume().BottomFace = new SHorizontalFace;
         GetVolume().TopFace->Height    = m_fElevation;
         GetVolume().BottomFace->Height = m_fElevation;
         /* Initialize physics */
         cpInitChipmunk();
         cpResetShapeIdCounter();
         /* Used to attach static geometries so that they won't move and to simulate friction */
         m_ptGroundBody = cpBodyNew(INFINITY, INFINITY);
         /* Create the space to contain the movable objects */
         m_ptSpace = cpSpaceNew();
         /* Subiterations to solve constraints.
            The more, the better for precision but the worse for speed
         */
         m_ptSpace->iterations = GetIterations();
         /* Spatial hash */
         if(NodeExists(t_tree, "spatial_hash")) {
            TConfigurationNode& tNode = GetNode(t_tree, "spatial_hash");
            cpFloat fSize;
            UInt32 unNum;
            GetNodeAttribute(tNode, "cell_size", fSize);
            GetNodeAttribute(tNode, "cell_num",  unNum);
            cpSpaceUseSpatialHash(m_ptSpace, fSize, unNum);
         }
         /* Gripper-Gripped callback functions */
         cpSpaceAddCollisionHandler(
            m_ptSpace,
            SHAPE_GRIPPER,
            SHAPE_GRIPPABLE,
            BeginCollisionBetweenGripperAndGrippable,
            ManageCollisionBetweenGripperAndGrippable,
            NULL,
            NULL,
            NULL);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the dynamics 2D engine \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Reset() {
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
      cpSpaceReindexStatic(m_ptSpace);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Update() {
      /* Update the physics state from the entities */
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      /* Perform the step */
      for(size_t i = 0; i < GetIterations(); ++i) {
         for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
             it != m_tPhysicsModels.end(); ++it) {
            it->second->UpdatePhysics();
         }
         cpSpaceStep(m_ptSpace, GetPhysicsClockTick());
      }
      /* Update the simulated space */
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Destroy() {
      /* Empty the physics model map */
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
      /* Get rid of the physics space */
      cpSpaceFree(m_ptSpace);
      cpBodyFree(m_ptGroundBody);
   }

   /****************************************/
   /****************************************/

   size_t CDynamics2DEngine::GetNumPhysicsModels() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEngine::AddEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CDynamics2DOperationAddEntity, CDynamics2DEngine, SOperationOutcome>
         (*this, c_entity);
      cpResetShapeIdCounter();
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEngine::RemoveEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CDynamics2DOperationRemoveEntity, CDynamics2DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   struct SDynamics2DSegmentHitData {
      TEmbodiedEntityIntersectionData& Intersections;
      const CRay3& Ray;

      SDynamics2DSegmentHitData(TEmbodiedEntityIntersectionData& t_data,
                                const CRay3& c_ray) :
         Intersections(t_data),
         Ray(c_ray) {}
   };

   static void Dynamics2DSegmentQueryFunc(cpShape* pt_shape, cpFloat f_t, cpVect, void* pt_data) {
      /* Get the data associated to this query */
      SDynamics2DSegmentHitData& sData = *reinterpret_cast<SDynamics2DSegmentHitData*>(pt_data);
      /* Hit found, is it within the limits? */
      CDynamics2DModel& cModel = *reinterpret_cast<CDynamics2DModel*>(pt_shape->body->data);
      CVector3 cIntersectionPoint;
      sData.Ray.GetPoint(cIntersectionPoint, f_t);
      if((cIntersectionPoint.GetZ() >= cModel.GetBoundingBox().MinCorner.GetZ()) &&
         (cIntersectionPoint.GetZ() <= cModel.GetBoundingBox().MaxCorner.GetZ()) ) {
         /* Yes, a real hit */
         sData.Intersections.push_back(
            SEmbodiedEntityIntersectionItem(
               &cModel.GetEmbodiedEntity(),
               f_t));
      }
   }

   void CDynamics2DEngine::CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                                    const CRay3& c_ray) const {
      /* Query all hits along the ray */
      SDynamics2DSegmentHitData sHitData(t_data, c_ray);
      cpSpaceSegmentQuery(
         m_ptSpace,
         cpv(c_ray.GetStart().GetX(), c_ray.GetStart().GetY()),
         cpv(c_ray.GetEnd().GetX()  , c_ray.GetEnd().GetY()  ),
         CP_ALL_LAYERS,
         CP_NO_GROUP,
         Dynamics2DSegmentQueryFunc,
         &sHitData);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::PositionPhysicsToSpace(CVector3& c_new_pos,
                                                  const CVector3& c_original_pos,
                                                  const cpBody* pt_body) {
      c_new_pos.SetX(pt_body->p.x);
      c_new_pos.SetY(pt_body->p.y);
      c_new_pos.SetZ(c_original_pos.GetZ());
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::OrientationPhysicsToSpace(CQuaternion& c_new_orient,
                                                     cpBody* pt_body) {
      c_new_orient.FromAngleAxis(CRadians(pt_body->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::AddPhysicsModel(const std::string& str_id,
                                            CDynamics2DModel& c_model) {
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::RemovePhysicsModel(const std::string& str_id) {
      CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.find(str_id);
      if(it != m_tPhysicsModels.end()) {
         delete it->second;
         m_tPhysicsModels.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("Dynamics2D model id \"" << str_id << "\" not found in dynamics 2D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CDynamics2DEngine,
                           "dynamics2d",
                           "Carlo Pinciroli [ilpincy@gmail.com]",
                           "1.0",
                           "A 2D dynamics physics engine.",
                           "This physics engine is a 2D dynamics engine based on the Chipmunk library\n"
                           "(http://code.google.com/p/chipmunk-physics) version 6.0.1.\n\n"

                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "The 'id' attribute is necessary and must be unique among the physics engines.\n"
                           "If two engines share the same id, initialization aborts.\n\n"

                           "OPTIONAL XML CONFIGURATION\n\n"

                           "It is possible to set how many iterations this physics engine performs between\n"
                           "each simulation step. By default, this physics engine performs 10 steps every\n"
                           "two simulation steps. This means that, if the simulation step is 100ms, the\n"
                           "physics engine step is, by default, 10ms. Sometimes, collisions and joints are\n"
                           "not simulated with sufficient precision using these parameters. By increasing\n"
                           "the number of iterations, the temporal granularity of the solver increases and\n"
                           "with it its accuracy, at the cost of higher computational cost. To change the\n"
                           "number of iterations per simulation step use this syntax:\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\"\n"
                           "                iterations=\"20\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "The plane of the physics engine can be translated on the Z axis, to simulate\n"
                           "for example hovering objects, such as flying robots. To translate the plane\n"
                           "2m up the Z axis, use the 'elevation' attribute as follows:\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\"\n"
                           "                elevation=\"2.0\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "When not specified, the elevation is zero, which means that the plane\n"
                           "corresponds to the XY plane.\n\n"

                           "The friction parameters between the ground and movable boxes and cylinders can\n"
                           "be overridden. You can set both the linear and angular friction parameters.\n"
                           "The default value is 1.49 for each of them. To override the values, use this\n"
                           "syntax (all attributes are optional):\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\"\n"
                           "      <friction box_linear_friction=\"1.0\"\n"
                           "                box_angular_friction=\"2.0\"\n"
                           "                cylinder_linear_friction=\"3.0\"\n"
                           "                cylinder_angular_friction=\"4.0\" />\n"
                           "    </dynamics2d>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "For the the robots that use velocity-based control, such as ground robots with\n"
                           "the differential_steering actuator (e.g. the foot-bot and the e-puck), it is\n"
                           "possible to customize robot-specific attributes that set the maximum force and\n"
                           "torque the robot has. The syntax is as follows, taking a foot-bot as example:\n\n"

                           "  <arena ...>\n"
                           "    ...\n"
                           "    <foot-bot id=\"fb0\">\n"
                           "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                           "      <controller config=\"mycntrl\" />\n"
                           "      <!-- Specify new value for max_force and max_torque -->\n"
                           "      <dynamics2d>\n"
                           "        <differential_steering max_force=\"0.1\" max_torque=\"0.1\"/>\n"
                           "      </dynamics2d>\n"
                           "    </foot-bot>\n"
                           "    ...\n"
                           "  </arena>\n\n"

                           "The attributes 'max_force' and 'max_torque' are both optional, and they take the\n"
                           "robot-specific default if not set. Check the code of the dynamics2d model of the\n"
                           "robot you're using to know the default values.\n\n"

                           "Multiple physics engines can also be used. If multiple physics engines are used,\n"
                           "the disjoint union of the area within the arena assigned to each engine must cover\n"
                           "the entire arena without overlapping. If the entire arena is not covered,robots can\n"
                           "\"escape\" the configured physics engines and cause a fatal exception (this is not an\n"
                           "issue when a single physics engine is used, because the engine covers the entire arena\n"
                           "by default). To use multiple physics engines, use the following syntax (all attributes\n"
                           "are mandatory):\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d0\"\n"
                           "      <boundaries>\n"
                           "        <top height=\"1.0\"/>\n"
                           "        <botton height=\"0.0\"/>\n"
                           "        <sides>\n"
                           "          <vertex point=\"0.0, 0.0\"/>\n"
                           "          <vertex point=\"4.0, 0.0\"/>\n"
                           "          <vertex point=\"4.0, 4.0\"/>\n"
                           "          <vertex point=\"0.0, 4.0\"/>\n"
                           "        </sides>\n"
                           "      </boundaries>\n"
                           "    </dynamics2d>\n"
                           "    <dynamics2d id=\"dyn2d1\"\n"
                           "     ..."
                           "    </dynamics2d>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "'Top' and 'Bottom' attributes are more relevant for 3D physics engines, and should\n"
                           "be generally set to 1.0 and 0.0, respectively, for 2D engines. A physics engine can\n"
                           "be defined having any number of sides >= 3, as long as the sides from a closed\n"
                           "polygon in the 2D plane (vertices must be declared in the XML file in\n"
                           "counter-clockwise order).  In the above example, the physics engine \"dyn2d0\" is\n"
                           "assigned to the area within the arena with lower left coordinates (0,0) and upper\n"
                           "right coordinates (4,4) and verticle are specified in counter clockwise order: LL,\n"
                           "LR, UR, UL.\n\n"

                           "OPTIMIZATION HINTS\n\n"

                           "1. A single physics engine is generally sufficient for small swarms (say <= 50\n"
                           "   robots) within a reasonably small arena to obtain faster than real-time\n"
                           "   performance with optimized code. For larger swarms and/or large arenas multiple\n"
                           "   engines should be used for maximum performance.\n\n"


                           "2. In general, using the same number of ARGoS threads as physics engines gives\n"
                           "   maximum performance (1-thread per engine per CPU core).\n\n"

                           "3. Using multiple engines in simulations with any of the following characteristics\n"
                           "   generally incurs more overhead due to thread context switching than the\n"
                           "   performance benefits from multiple engines:\n\n"

                           "   - Small swarms\n"
                           "   - Small arenas\n"
                           "   - Less available ARGoS threads than assigned physics engines\n"
                           "   - Less available CPU cores than assigned ARGoS threads\n\n"


                           "4. A good starting strategy for physics engine boundary assignment is to assign\n"
                           "   each physics engine the same amount of area within the arena. This will be\n"
                           "   sufficient for most cases. Depending on the nature of the simulation, using\n"
                           "   non-homogeneous physics engine sizes may yield increased performance. An example\n"
                           "   would be a narrow hallway between larger open areas in the arena--the hallway\n"
                           "   will likely experience increased robot density and assigning more physics\n"
                           "   engines to that area than the relatively unpopulated open areas may increase\n"
                           "   performance.\n\n"

                           "5. By default, this engine uses the bounding-box tree method for collision shape\n"
                           "   indexing. This method is the default in Chipmunk and it works well most of the\n"
                           "   times. However, if you are running simulations with hundreds or thousands of\n"
                           "   identical robots, a different shape collision indexing is available: the\n"
                           "   spatial hash. The spatial hash is a grid stored in a hashmap. To get the max\n"
                           "   out of this indexing method, you must set two parameters: the cell size and the\n"
                           "   suggested minimum number of cells in the space. According to the documentation\n"
                           "   of Chipmunk, the cell size should correspond to the size of the bounding box of\n"
                           "   the most common object in the simulation; the minimum number of cells should be\n"
                           "   at least 10x the number of objects managed by the physics engine. To use\n"
                           "   this indexing method, use this syntax (all attributes are mandatory):\n\n"

                           "   <physics_engines>\n"
                           "     ...\n"
                           "     <dynamics2d id=\"dyn2d\">\n"
                           "       <spatial_hash>\n"
                           "         <cell_size=\"1.0\"/>\n"
                           "         <cell_num=\"2.0\"/>\n"
                           "       </spatial_hash>\n"
                           "     </dynamics2d>\n"
                           "     ...\n"
                           "   </physics_engines>\n"
                           ,

                           "Usable"
                           );

}
