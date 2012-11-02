/**
 * @file <argos3/core/simulator/entity/floor_entity.cpp>
 *
 * @author Carlo Pinciroli - <cpinciroli@ulb.ac.be>
 */

#include "floor_entity.h"
#include <FreeImagePlus.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/loop_functions.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CFloorColorFromImageFile : public CFloorEntity::CFloorColorSource {

   public:

      CFloorColorFromImageFile(const std::string& str_file_name,
                               Real f_arena_width,
                               Real f_arena_height) :
         m_cHalfArenaSize(f_arena_width * 0.5f,
                          f_arena_height * 0.5f) {
         if(!m_cImage.load(str_file_name.c_str())) {
            THROW_ARGOSEXCEPTION("Could not load image \"" <<
                                 str_file_name <<
                                 "\"");
         }
         m_fArenaToImageCoordinateXFactor = m_cImage.getWidth() / f_arena_width;
         m_fArenaToImageCoordinateYFactor = m_cImage.getHeight() / f_arena_height;
      }

      virtual ~CFloorColorFromImageFile() {}

      virtual CColor GetColorAtPoint(Real f_x,
                                     Real f_y) {
         /* Compute coordinates on the image */
         UInt32 x = (f_x + m_cHalfArenaSize.GetX()) * m_fArenaToImageCoordinateXFactor;
         UInt32 y = (f_y + m_cHalfArenaSize.GetY()) * m_fArenaToImageCoordinateYFactor;
         /* Check the bit depth */
         if(m_cImage.getBitsPerPixel() <= 8) {
            RGBQUAD* ptColorPalette;
            BYTE tPixelIndex;
            /* 1, 4 or 8 bits per pixel */
            if(! m_cImage.getPixelIndex(x, y, &tPixelIndex)) {
               THROW_ARGOSEXCEPTION("Unable to access image pixel at (" << x << "," << y <<
                                    "). Image size (" << m_cImage.getWidth() << "," <<
                                    m_cImage.getHeight() << ")");
            }
            ptColorPalette = m_cImage.getPalette();
            return CColor(ptColorPalette[tPixelIndex].rgbRed,
                          ptColorPalette[tPixelIndex].rgbGreen,
                          ptColorPalette[tPixelIndex].rgbBlue);
         }
         else {
            /* 16, 24 or 32 bits per pixel */
            RGBQUAD tColorPixel;
            if(! m_cImage.getPixelColor(x, y, &tColorPixel)) {
               THROW_ARGOSEXCEPTION("Unable to access image pixel at (" << x << "," << y <<
                                    "). Image size (" << m_cImage.getWidth() << "," <<
                                    m_cImage.getHeight() << ")");
            }
            return CColor(tColorPixel.rgbRed,
                          tColorPixel.rgbGreen,
                          tColorPixel.rgbBlue);
         }
      }

      virtual void SaveAsImage(const std::string& str_path) {
         m_strImageFileName = str_path;
         m_cImage.save(str_path.c_str());
      }

      virtual std::string GetImageFileName() const {
         return m_strImageFileName;
      }

   private:

      fipImage m_cImage;
      Real m_fArenaToImageCoordinateXFactor;
      Real m_fArenaToImageCoordinateYFactor;
      CVector2 m_cHalfArenaSize;
      std::string m_strImageFileName;

   };

   /****************************************/
   /****************************************/

   class CFloorColorFromLoopFunctions : public CFloorEntity::CFloorColorSource {

   public:

      CFloorColorFromLoopFunctions(UInt32 un_pixels_per_meter,
                                   Real f_arena_width,
                                   Real f_arena_height) :
         m_cLoopFunctions(CSimulator::GetInstance().GetLoopFunctions()),
         m_unPixelsPerMeter(un_pixels_per_meter),
         m_cHalfArenaSize(f_arena_width * 0.5f,
                          f_arena_height * 0.5f) {
      }

      virtual ~CFloorColorFromLoopFunctions() {
      }

      virtual CColor GetColorAtPoint(Real f_x,
                                     Real f_y) {
    	 return m_cLoopFunctions.GetFloorColor(CVector2(f_x, f_y));
      }

      virtual void SaveAsImage(const std::string& str_path) {
         fipImage cImage(FIT_BITMAP, m_unPixelsPerMeter * m_cHalfArenaSize.GetX()*2, m_unPixelsPerMeter * m_cHalfArenaSize.GetY()*2, 24);
         Real fFactor = 1.0f / static_cast<Real>(m_unPixelsPerMeter);
         CVector2 cFloorPos;
         CColor cARGoSPixel;
         RGBQUAD tFIPPixel;
         for(UInt32 y = 0; y < cImage.getHeight(); ++y) {
            for(UInt32 x = 0; x < cImage.getWidth(); ++x) {
               cFloorPos.Set(x * fFactor, y * fFactor);
               cFloorPos -= m_cHalfArenaSize;
               cARGoSPixel = m_cLoopFunctions.GetFloorColor(cFloorPos);
               tFIPPixel.rgbRed = cARGoSPixel.GetRed();
               tFIPPixel.rgbGreen = cARGoSPixel.GetGreen();
               tFIPPixel.rgbBlue = cARGoSPixel.GetBlue();
               cImage.setPixelColor(x, y, &tFIPPixel);
            }
         }
         if(!cImage.save(str_path.c_str())) {
            THROW_ARGOSEXCEPTION("Cannot save image \"" << str_path << "\" for floor entity.");
         }
      }

   private:

      CLoopFunctions& m_cLoopFunctions;
      UInt32 m_unPixelsPerMeter;
      CVector2 m_cHalfArenaSize;
   };

/****************************************/
/****************************************/

   CFloorEntity::CFloorEntity() :
      CEntity(NULL),
      m_pcColorSource(NULL),
      m_bHasChanged(true) {}

/****************************************/
/****************************************/

   CFloorEntity::~CFloorEntity() {
      if(m_pcColorSource != NULL) {
         delete m_pcColorSource;
      }
   }

/****************************************/
/****************************************/

   void CFloorEntity::Init(TConfigurationNode& t_tree) {
      /* Init parent */
      CEntity::Init(t_tree);
      /* Get arena size */
      m_cFloorSize = CSimulator::GetInstance().GetSpace().GetArenaSize();
      /* Parse XML */
      GetNodeAttribute(t_tree, "source", m_strColorSource);
      if(m_strColorSource == "image") {
         std::string strPath;
         GetNodeAttribute(t_tree, "path", strPath);
         strPath = ExpandARGoSInstallDir(strPath);
         m_pcColorSource = new CFloorColorFromImageFile(strPath,
                                                        m_cFloorSize.GetX(),
                                                        m_cFloorSize.GetY());
      }
      else if(m_strColorSource == "loop_functions") {
         GetNodeAttribute(t_tree, "pixels_per_meter", m_unPixelsPerMeter);
         m_pcColorSource = new CFloorColorFromLoopFunctions(m_unPixelsPerMeter,
                                                            m_cFloorSize.GetX(),
                                                            m_cFloorSize.GetY());
      }
      else {
         THROW_ARGOSEXCEPTION("Unknown image source \"" <<
                              m_strColorSource <<
                              "\" for the floor entity \"" <<
                              GetId() <<
                              "\"");
      }
   }

/****************************************/
/****************************************/

   void CFloorEntity::Reset() {
      /* Reset parent */
      CEntity::Reset();

      /* Recompute the color source from the loop function */
      if(m_strColorSource == "loop_functions") {
         delete m_pcColorSource;
         m_pcColorSource = new CFloorColorFromLoopFunctions(m_unPixelsPerMeter,
                                                            m_cFloorSize.GetX(),
                                                            m_cFloorSize.GetY());
      }
   }

/****************************************/
/****************************************/

   REGISTER_ENTITY(CFloorEntity,
                   "floor",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "1.0",
                   "It contains the properties of the arena floor.",
                   "The floor entity contains the properties of the arena floor. In the current\n"
                   "implementation, it contains only the color of the floor. The floor color is\n"
                   "detected by the robots' ground sensors.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <floor id=\"floor\"\n"
                   "           source=\"SOURCE\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'source' attribute specifies where to get the color of the floor from. Its\n"
                   "value, here denoted as SOURCE, can assume the following values:\n\n"
                   "  image            The color is calculated from the passed image file\n"
                   "  loop_functions   The color is calculated calling the loop functions\n\n"
                   "When 'source' is set to 'image', as showed in the following example, you have\n"
                   "to specify the image path in the additional attribute 'path':\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <floor id=\"floor\"\n"
                   "           source=\"image\"\n"
                   "           path=\"/path/to/imagefile.ext\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "Many image formats are available, such as PNG, JPG, BMP, GIF and many more.\n"
                   "Refer to the FreeImage webpage for a complete list of supported image formats\n"
                   "(http://freeimage.sourceforge.net/features.html).\n\n"
                   "When 'source' is set to 'loop_functions', as showed in the following example,\n"
                   "an image is implicitly created to be used as texture for graphical\n"
                   "visualizations. The algorithm that creates the texture needs to convert from\n"
                   "meters (in the arena) to pixels (of the texture). You control how many pixels\n"
                   "per meter are used with the attribute 'pixels_per_meter'. Clearly, the higher\n"
                   "value, the higher the quality, but also the slower the algorithm and the bigger\n"
                   "the texture. The algorithm is called only once at init time, so the fact that\n"
                   "it is slow is not so important. However, the image size is limited by OpenGL.\n"
                   "Every implementation has its own limit, and you should check yours if any\n"
                   "texture-related problem arises. Now for the example:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <floor id=\"floor\"\n"
                   "           source=\"loop_functions\"\n"
                   "           pixels_per_meter=\"100\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being.\n",
                   "Ready for use"
      );

}
