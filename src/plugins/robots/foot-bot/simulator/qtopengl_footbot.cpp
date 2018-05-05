/**
 * @file <argos3/plugins/robots/foot-bot/simulator/qtopengl_footbot.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_footbot.h"
#include "footbot_entity.h"
#include "footbot_distance_scanner_equipped_entity.h"
#include "footbot_turret_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   /* All measures are in meters */

   /* General */
   static const Real INTER_MODULE_GAP              = 0.0015f;
   static const Real HEIGHT                        = 0.258f;
   /* Wheel measures */
   static const Real WHEEL_WIDTH                   = 0.022031354f;
   static const Real HALF_WHEEL_WIDTH              = WHEEL_WIDTH * 0.5f;
   static const Real WHEEL_RADIUS                  = 0.029112741f;
   static const Real WHEEL_DIAMETER                = WHEEL_RADIUS * 2.0f;
   static const Real INTERWHEEL_DISTANCE           = 0.127f;
   static const Real HALF_INTERWHEEL_DISTANCE      = INTERWHEEL_DISTANCE * 0.5f;
   /* Track measures */
   static const Real INTERTRACK_DISTANCE           = 0.064600514f;
   static const Real HALF_INTERTRACK_DISTANCE      = INTERTRACK_DISTANCE * 0.5f;
   static const Real TRACK_WHEELS_DISTANCE         = 0.100156677f;
   static const Real HALF_TRACK_WHEELS_DISTANCE    = TRACK_WHEELS_DISTANCE * 0.5f;
   /* Base module measures */
   static const Real BATTERY_SOCKET_ELEVATION      = 0.006f;
   static const Real BATTERY_SOCKET_LENGTH         = 0.150302467f;
   static const Real HALF_BATTERY_SOCKET_LENGTH    = BATTERY_SOCKET_LENGTH * 0.5f;
   static const Real BATTERY_SOCKET_WIDTH          = 0.037600133f;
   static const Real HALF_BATTERY_SOCKET_WIDTH     = BATTERY_SOCKET_WIDTH * 0.5f;
   static const Real BASE_MODULE_HEIGHT            = 0.0055f;
   static const Real BASE_MODULE_RADIUS            = 0.085036758f;
   static const Real PROXIMITY_SENSOR_HEIGHT       = 0.004f;  // unused
   static const Real PROXIMITY_SENSOR_WIDTH        = 0.007f;  // unused
   static const Real PROXIMITY_SENSOR_DEPTH        = 0.0025f; // unused
   /* Gripper module measures */
   static const Real GRIPPER_MODULE_ELEVATION      = BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER + BASE_MODULE_HEIGHT + INTER_MODULE_GAP;
   static const Real GRIPPER_MODULE_HEIGHT         = 0.027f;
   static const Real GRIPPER_MODULE_HALF_HEIGHT    = GRIPPER_MODULE_HEIGHT * 0.5f;
   static const Real GRIPPER_MODULE_INNER_RADIUS   = 0.069f;
   static const Real GRIPPER_MODULE_BARRIER_HEIGHT = 0.0075f;
   static const Real GRIPPER_MECHANICS_LENGTH      = 0.026094485f;
   static const Real GRIPPER_MECHANICS_WIDTH       = 0.02f;
   static const Real GRIPPER_MECHANICS_HALF_WIDTH  = GRIPPER_MECHANICS_WIDTH * 0.5f;
   static const Real GRIPPER_CLAW_LENGTH           = 0.01f;
   static const Real GRIPPER_CLAW_WIDTH            = 0.013f;
   static const Real GRIPPER_CLAW_HALF_WIDTH       = GRIPPER_CLAW_WIDTH * 0.5f;
   static const Real GRIPPER_CLAW_OFFSET           = GRIPPER_MODULE_INNER_RADIUS + GRIPPER_MECHANICS_LENGTH;
   static const Real GRIPPER_CLAW_ELEVATION        = GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HALF_HEIGHT;
   /* RAB measures */
   static const Real RAB_ELEVATION                 = GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT + INTER_MODULE_GAP;
   static const Real RAB_HEIGHT                    = 0.0086f;
   static const Real RAB_MAX_RADIUS                = 0.08f;
   static const Real RAB_MIN_RADIUS                = 0.075f;
   /* Distance scanner measures */
   static const Real DISTANCE_SCANNER_ELEVATION         = RAB_ELEVATION + RAB_HEIGHT;
   static const Real DISTANCE_SCANNER_RADIUS            = 0.05f;
   static const Real DISTANCE_SCANNER_HEIGHT            = 0.0235f;
   static const Real DISTANCE_SCANNER_SENSOR_WIDTH      = 0.035858477f;
   static const Real DISTANCE_SCANNER_SENSOR_HALF_WIDTH = DISTANCE_SCANNER_SENSOR_WIDTH * 0.5f;
   static const Real DISTANCE_SCANNER_SENSOR_HEIGHT     = 0.021124933f;
   static const Real DISTANCE_SCANNER_SENSOR_DEPTH      = 0.018f;
   /* iMX module */
   static const Real IMX_MODULE_ELEVATION               = DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT;
   static const Real IMX_MODULE_RADIUS                  = 0.065f;
   static const Real IMX_MODULE_HEIGHT                  = 0.010199866f;
   /* Beacon */
   static const Real BEACON_ELEVATION            = IMX_MODULE_ELEVATION + IMX_MODULE_HEIGHT;
   static const Real BEACON_RADIUS               = 0.021f;
   static const Real BEACON_HEIGHT               = 0.0201f;
   /* Camera */
   static const Real CAMERA_ELEVATION            = BEACON_ELEVATION + BEACON_HEIGHT;
   static const Real CAMERA_RADIUS               = BEACON_RADIUS;
   static const Real CAMERA_HEIGHT               = 0.104f;

   /****************************************/
   /****************************************/

   CQTOpenGLFootBot::CQTOpenGLFootBot() :
      m_unVertices(40),
      m_fLEDAngleSlice(360.0f / 12.0f) {
      /* Reserve the needed display lists */
      m_unLists = glGenLists(13);

      /* Assign indices for better referencing (later) */
      m_unBasicWheelList            = m_unLists;
      m_unWheelList                 = m_unLists + 1;
      m_unTrackList                 = m_unLists + 2;
      m_unBaseList                  = m_unLists + 3;
      m_unGrippableSliceList        = m_unLists + 4;
      m_unGripperMechanicsList      = m_unLists + 5;
      m_unGripperClawList           = m_unLists + 6;
      m_unRABList                   = m_unLists + 7;
      m_unDistanceScannerSensorList = m_unLists + 8;
      m_unDistanceScannerList       = m_unLists + 9;
      m_unIMXList                   = m_unLists + 10;
      m_unBeaconList                = m_unLists + 11;
      m_unCameraList                = m_unLists + 12;

      /* Create the materialless wheel display list */
      glNewList(m_unBasicWheelList, GL_COMPILE);
      MakeWheel();
      glEndList();

      /* Create the wheel display list */
      glNewList(m_unWheelList, GL_COMPILE);
      RenderWheel();
      glEndList();

      /* Create the track display list */
      glNewList(m_unTrackList, GL_COMPILE);
      RenderTrack();
      glEndList();

      /* Create the base module display list */
      glNewList(m_unBaseList, GL_COMPILE);
      RenderBase();
      glEndList();

      /* Create the grippable slice display list */
      glNewList(m_unGrippableSliceList, GL_COMPILE);
      RenderGrippableSlice();
      glEndList();

      /* Create the gripper mechanics display list */
      glNewList(m_unGripperMechanicsList, GL_COMPILE);
      RenderGripperMechanics();
      glEndList();

      /* Create the gripper claw display list */
      glNewList(m_unGripperClawList, GL_COMPILE);
      RenderGripperClaw();
      glEndList();

      /* Create the gripper claw display list */
      glNewList(m_unRABList, GL_COMPILE);
      RenderRAB();
      glEndList();

      /* Create the single distance scanner sensor display list */
      glNewList(m_unDistanceScannerSensorList, GL_COMPILE);
      RenderDistanceScannerSensor();
      glEndList();

      /* Create the distance scanner display list */
      glNewList(m_unDistanceScannerList, GL_COMPILE);
      RenderDistanceScanner();
      glEndList();

      /* Create the iMX display list */
      glNewList(m_unIMXList, GL_COMPILE);
      RenderIMX();
      glEndList();

      /* Create the beacon display list */
      glNewList(m_unBeaconList, GL_COMPILE);
      RenderBeacon();
      glEndList();

      /* Create the camera display list */
      glNewList(m_unCameraList, GL_COMPILE);
      RenderCamera();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLFootBot::~CQTOpenGLFootBot() {
      glDeleteLists(m_unLists, 13);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::Draw(CFootBotEntity& c_entity) {
      /* Place the wheels */
      glPushMatrix();
      glTranslatef(0.0f, HALF_INTERWHEEL_DISTANCE, 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
      /* Place the tracks */
      glPushMatrix();
      glTranslatef(0.0f, HALF_INTERTRACK_DISTANCE, 0.0f);
      glCallList(m_unTrackList);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0.0f, -HALF_INTERTRACK_DISTANCE, 0.0f);
      glCallList(m_unTrackList);
      glPopMatrix();
      /* Place the tracks */
      glCallList(m_unBaseList);
      /* Place the gripper module */
      glPushMatrix();
      /* Read gripper orientation from footbot entity */
      GLfloat fGripperOrientation = ToDegrees(c_entity.GetTurretEntity().GetRotation()).GetValue();
      glRotatef(fGripperOrientation, 0.0f, 0.0f, 1.0f);
      /* Place the grippable part of the gripper module (LEDs) */
      glPushMatrix();
      CLEDEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      for(UInt32 i = 0; i < 12; i++) {
         const CColor& cColor = cLEDEquippedEntity.GetLED(i).GetColor();
         glRotatef(m_fLEDAngleSlice, 0.0f, 0.0f, 1.0f);
         SetLEDMaterial(cColor.GetRed()   / 255.0f,
                        cColor.GetGreen() / 255.0f,
                        cColor.GetBlue()  / 255.0f);
         glCallList(m_unGrippableSliceList);
      }
      glPopMatrix();
      /* Place the gripper mechanics */
      glCallList(m_unGripperMechanicsList);
      /* Place the gripper claws */
      /* Read the gripper aperture from footbot entity */
      GLfloat fGripperAperture = c_entity.GetGripperEquippedEntity().GetLockState() * 90.0f;
      glTranslatef(GRIPPER_CLAW_OFFSET, 0.0f, GRIPPER_CLAW_ELEVATION);
      glPushMatrix();
      glRotatef(fGripperAperture, 0.0f, 1.0f, 0.0f);
      glCallList(m_unGripperClawList);
      glPopMatrix();
      glPushMatrix();
      glRotatef(-fGripperAperture, 0.0f, 1.0f, 0.0f);
      glCallList(m_unGripperClawList);
      glPopMatrix();
      glPopMatrix();
      /* Place the RAB */
      glCallList(m_unRABList);
      /* Place the distance scanner */
      glCallList(m_unDistanceScannerList);
      glPushMatrix();
      /* Read dist scanner orientation from footbot entity */
      GLfloat fDistanceScannerOrientation = ToDegrees(c_entity.GetDistanceScannerEquippedEntity().GetRotation()).GetValue();
      glRotatef(fDistanceScannerOrientation, 0.0f, 0.0f, 1.0f);
      glCallList(m_unDistanceScannerSensorList);
      glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
      glCallList(m_unDistanceScannerSensorList);
      glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
      glCallList(m_unDistanceScannerSensorList);
      glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
      glCallList(m_unDistanceScannerSensorList);
      glPopMatrix();
      /* Place the iMX module */
      glCallList(m_unIMXList);
      /* Place the beacon */
      const CColor& cBeaconColor = cLEDEquippedEntity.GetLED(12).GetColor();
      SetLEDMaterial(cBeaconColor.GetRed()   / 255.0f,
                     cBeaconColor.GetGreen() / 255.0f,
                     cBeaconColor.GetBlue()  / 255.0f);
      glCallList(m_unBeaconList);
      /* Place the camera */
      glCallList(m_unCameraList);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::MakeWheel() {
      /* Right side */
      CVector2 cVertex(WHEEL_RADIUS, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      CVector3 cNormal(-1.0f, -1.0f, 0.0f);
      cNormal.Normalize();
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
      /* Left side */
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cNormal.Set(-1.0f, 1.0f, 0.0f);
      cNormal.Normalize();
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
      /* Tire */
      cNormal.Set(1.0f, 0.0f, 0.0f);
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         glVertex3f(cVertex.GetX(),  HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::SetWhitePlasticMaterial() {
      const GLfloat pfColor[]     = {   1.0f, 1.0f, 1.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.9f, 0.9f, 0.9f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::SetBlackTireMaterial() {
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f                   };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::SetCircuitBoardMaterial() {
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 1.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.5f, 0.5f, 1.0f, 1.0f };
      const GLfloat pfShininess[] = { 10.0f                  };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::SetLEDMaterial(GLfloat f_red, GLfloat f_green, GLfloat f_blue) {
      const GLfloat pfColor[]     = { f_red, f_green, f_blue, 1.0f };
      const GLfloat pfSpecular[]  = {  0.0f,    0.0f,   0.0f, 1.0f };
      const GLfloat pfShininess[] = {  0.0f                        };
      const GLfloat pfEmission[]  = { f_red, f_green, f_blue, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderWheel() {
      /* Set material */
      SetWhitePlasticMaterial();
      /* Right side */
      CVector2 cVertex(WHEEL_RADIUS, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      CVector3 cNormal(-1.0f, -1.0f, 0.0f);
      cNormal.Normalize();
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
      /* Left side */
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cAngle = -cAngle;
      cNormal.Set(-1.0f, 1.0f, 0.0f);
      cNormal.Normalize();
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
      /* Tire */
      SetBlackTireMaterial();
      cNormal.Set(1.0f, 0.0f, 0.0f);
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         glVertex3f(cVertex.GetX(),  HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderTrack() {
      /* Set material */
      SetWhitePlasticMaterial();
      /* Place two basic wheels */
      glPushMatrix();
      glTranslatef(HALF_TRACK_WHEELS_DISTANCE, 0.0f, 0.0f);
      glCallList(m_unBasicWheelList);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(-HALF_TRACK_WHEELS_DISTANCE, 0.0f, 0.0f);
      glCallList(m_unBasicWheelList);
      glPopMatrix();
      /* Render the track itself */
      /* Set material */
      SetBlackTireMaterial();
      glEnable(GL_POLYGON_OFFSET_FILL); // Correct flickering due to overlapping surfaces
      glPolygonOffset(-0.1, 1.0);      // Correct flickering due to overlapping surfaces
      /* Top part */
      glBegin(GL_POLYGON);
      glVertex3f(-HALF_TRACK_WHEELS_DISTANCE, -HALF_WHEEL_WIDTH, WHEEL_DIAMETER);
      glVertex3f( HALF_TRACK_WHEELS_DISTANCE, -HALF_WHEEL_WIDTH, WHEEL_DIAMETER);
      glVertex3f( HALF_TRACK_WHEELS_DISTANCE,  HALF_WHEEL_WIDTH, WHEEL_DIAMETER);
      glVertex3f(-HALF_TRACK_WHEELS_DISTANCE,  HALF_WHEEL_WIDTH, WHEEL_DIAMETER);
      glEnd();
      /* Bottom part */
      glBegin(GL_POLYGON);
      glVertex3f(-HALF_TRACK_WHEELS_DISTANCE, -HALF_WHEEL_WIDTH, 0.0f);
      glVertex3f( HALF_TRACK_WHEELS_DISTANCE, -HALF_WHEEL_WIDTH, 0.0f);
      glVertex3f( HALF_TRACK_WHEELS_DISTANCE,  HALF_WHEEL_WIDTH, 0.0f);
      glVertex3f(-HALF_TRACK_WHEELS_DISTANCE,  HALF_WHEEL_WIDTH, 0.0f);
      glEnd();
      /* Round parts */
      CVector2 cVertex(0.0f, WHEEL_RADIUS);
      CVector2 cNormal(0.0f, 1.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      /* Front */
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices / 2; i++) {
         glNormal3f(cNormal.GetX(), 0.0f, cNormal.GetY());
         glVertex3f(-HALF_TRACK_WHEELS_DISTANCE + cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         glVertex3f(-HALF_TRACK_WHEELS_DISTANCE + cVertex.GetX(),  HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Back */
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices / 2; i++) {
         glNormal3f(cNormal.GetX(), 0.0f, cNormal.GetY());
         glVertex3f(HALF_TRACK_WHEELS_DISTANCE + cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         glVertex3f(HALF_TRACK_WHEELS_DISTANCE + cVertex.GetX(),  HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      glDisable(GL_POLYGON_OFFSET_FILL); // Not needed anymore
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderBase() {
      /* Battery socket */
      /* Set material */
      SetWhitePlasticMaterial();
      /* This part covers the top and bottom faces (parallel to XY) */
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUAD_STRIP);
      /* Starting side */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      /* South face */
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      /* East face */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f( HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      /* North face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH,  HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      /* West face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
      glVertex3f(-HALF_BATTERY_SOCKET_LENGTH, -HALF_BATTERY_SOCKET_WIDTH, BATTERY_SOCKET_ELEVATION);
      glEnd();
      /* Circuit board */
      CVector2 cVertex(BASE_MODULE_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER + BASE_MODULE_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      /* Set material */
      SetCircuitBoardMaterial();
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BATTERY_SOCKET_ELEVATION + WHEEL_DIAMETER + BASE_MODULE_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderGrippableSlice() {
      glDisable(GL_CULL_FACE); // This way we can see both faces of the grippable slice
      /* Bottom part */
      CVector2 cVertex(BASE_MODULE_RADIUS, 0.0f);
      cVertex.Rotate(-CRadians::TWO_PI / 12);
      CRadians cAngle(CRadians::TWO_PI / (m_unVertices * 12));
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(0.0f, 0.0f, GRIPPER_MODULE_ELEVATION);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      cAngle = -CRadians::TWO_PI / (m_unVertices * 12);
      /* Side barrier surface (bottom) */
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_BARRIER_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Side barrier surface (top) */
      cNormal.Set(1.0f, 0.0f);
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT - GRIPPER_MODULE_BARRIER_HEIGHT);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Internal rod (bottom) */
      cNormal.Set(BASE_MODULE_RADIUS - GRIPPER_MODULE_INNER_RADIUS,
                  GRIPPER_MODULE_HALF_HEIGHT);
      cNormal.Normalize();
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      CVector2 cVertex2(GRIPPER_MODULE_INNER_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex2.GetX(), cVertex2.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HALF_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION);
         cVertex.Rotate(cAngle);
         cVertex2.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Internal rod (top) */
      cNormal.Set( BASE_MODULE_RADIUS - GRIPPER_MODULE_INNER_RADIUS,
                  -GRIPPER_MODULE_HALF_HEIGHT);
      cNormal.Normalize();
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      cVertex2.Set(GRIPPER_MODULE_INNER_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
         glVertex3f(cVertex2.GetX(), cVertex2.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HALF_HEIGHT);
         cVertex.Rotate(cAngle);
         cVertex2.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      cVertex.Set(BASE_MODULE_RADIUS, 0.0f);
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      glEnable(GL_CULL_FACE); // Back to normal setting: cull back faces
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderGripperMechanics() {
      /* Set material */
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f                   };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
      /* This part covers the top and bottom faces (parallel to XY) */
      glEnable(GL_POLYGON_OFFSET_FILL); // Correct flickering due to overlapping surfaces
      glPolygonOffset(-0.1, 1.0);      // Correct flickering due to overlapping surfaces
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glEnd();
      glDisable(GL_POLYGON_OFFSET_FILL); // Not needed anymore
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUAD_STRIP);
      glNormal3f(0.0f, -1.0f, 0.0f);
      /* Starting vertex */
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      /* East face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS, -GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      /* North face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(GRIPPER_MECHANICS_LENGTH + GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      /* West face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION + GRIPPER_MODULE_HEIGHT);
      glVertex3f(                           GRIPPER_MODULE_INNER_RADIUS,  GRIPPER_MECHANICS_HALF_WIDTH, GRIPPER_MODULE_ELEVATION);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderGripperClaw() {
      /* Set material */
      const GLfloat pfColor[]     = { 0.5f, 0.5f, 0.5f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
      const GLfloat pfShininess[] = { 10.0f                  };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
      /* Draw the claw */
      glDisable(GL_CULL_FACE); // In this way, we can see both sides of the claw
      glNormal3f(0.0f, 0.0f, 1.0f);
      glBegin(GL_QUADS);
      glVertex3f(               0.0f,  GRIPPER_CLAW_HALF_WIDTH, 0.0f);
      glVertex3f(               0.0f, -GRIPPER_CLAW_HALF_WIDTH, 0.0f);
      glVertex3f(GRIPPER_CLAW_LENGTH, -GRIPPER_CLAW_HALF_WIDTH, 0.0f);
      glVertex3f(GRIPPER_CLAW_LENGTH,  GRIPPER_CLAW_HALF_WIDTH, 0.0f);
      glEnd();
      glEnable(GL_CULL_FACE); // Restore back face culling
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderRAB() {
      /* Set material */
      SetWhitePlasticMaterial();
      /* Bottom part */
      CVector2 cVertex(RAB_MAX_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RAB_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector3 cNormal(RAB_MAX_RADIUS - RAB_MIN_RADIUS, 0.0f, RAB_HEIGHT);
      cNormal.Normalize();
      cVertex.Set(RAB_MAX_RADIUS, 0.0f);
      CVector2 cVertex2(RAB_MIN_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex2.GetX(), cVertex2.GetY(), RAB_ELEVATION + RAB_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RAB_ELEVATION);
         cVertex.Rotate(cAngle);
         cVertex2.Rotate(cAngle);
         cNormal.RotateZ(cAngle);
      }
      glEnd();
      /* Top part */
      /* Set material */
      SetCircuitBoardMaterial();
      cVertex.Set(RAB_MIN_RADIUS, 0.0f);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RAB_ELEVATION + RAB_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderDistanceScannerSensor() {
      /* Set material */
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f                   };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
      /* This part covers the top and bottom faces (parallel to XY) */
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glEnd();
      glDisable(GL_POLYGON_OFFSET_FILL); // Not needed anymore
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUAD_STRIP);
      glNormal3f(0.0f, -1.0f, 0.0f);
      /* Starting vertex */
      glVertex3f(                                DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      /* South face */
      glVertex3f(                                DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      /* East face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS, -DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      /* North face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(DISTANCE_SCANNER_SENSOR_DEPTH + DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      /* West face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
      glVertex3f(                                DISTANCE_SCANNER_RADIUS,  DISTANCE_SCANNER_SENSOR_HALF_WIDTH, DISTANCE_SCANNER_ELEVATION);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderDistanceScanner() {
      /* Set material */
      SetWhitePlasticMaterial();
      /* Draw only side surface */
      CVector2 cVertex(DISTANCE_SCANNER_RADIUS, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      CVector2 cNormal(1.0f, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), DISTANCE_SCANNER_ELEVATION + DISTANCE_SCANNER_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), DISTANCE_SCANNER_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderIMX() {
      /* Set material */
      SetWhitePlasticMaterial();
      CVector2 cVertex(IMX_MODULE_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), IMX_MODULE_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(IMX_MODULE_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), IMX_MODULE_ELEVATION + IMX_MODULE_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), IMX_MODULE_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      /* Set material */
      SetCircuitBoardMaterial();
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(IMX_MODULE_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), IMX_MODULE_ELEVATION + IMX_MODULE_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderBeacon() {
      CVector2 cVertex(BEACON_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BEACON_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(BEACON_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BEACON_ELEVATION + BEACON_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BEACON_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(BEACON_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), BEACON_ELEVATION + BEACON_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLFootBot::RenderCamera() {
      /* Set material */
      SetWhitePlasticMaterial();
      CVector2 cVertex(CAMERA_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), CAMERA_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(CAMERA_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), CAMERA_ELEVATION + CAMERA_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), CAMERA_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(CAMERA_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), CAMERA_ELEVATION + CAMERA_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawFootBotNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CFootBotEntity& c_entity) {
         static CQTOpenGLFootBot m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawFootBotSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CFootBotEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawFootBotNormal, CFootBotEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawFootBotSelected, CFootBotEntity);

   /****************************************/
   /****************************************/

}
