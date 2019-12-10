/**
 * @file <argos3/plugins/robots/generic/noise_injector.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   /****************************************/
   /****************************************/

   CNoiseInjector::CNoiseInjector() :
    m_pcRNG(nullptr),
    m_bEnabled(false),
    m_model(model::ekNONE),
    m_fGaussianMean(0.0),
    m_fGaussianStdDev(1.0) {}

   /****************************************/
   /****************************************/

   void CNoiseInjector::Init(TConfigurationNode& t_tree) {
      try {
         /* parse noise model */
         std::string strModel;
         GetNodeAttribute(t_tree, "model", strModel);
         if ("uniform" == strModel) {
           InitUniform(t_tree);
           m_bEnabled = true;
           m_model = model::ekUNIFORM;
           m_pcRNG = CRandom::CreateRNG("argos");
         } else if ("gaussian" == strModel) {
           InitGaussian(t_tree);
           m_bEnabled = true;
           m_model = model::ekGAUSSIAN;
           m_pcRNG = CRandom::CreateRNG("argos");
         } else if ("none" != strModel) {
           THROW_ARGOSEXCEPTION("Bad noise model specified");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error while initializing noise injection", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitUniform(TConfigurationNode& t_tree) {
     /* Parse noise level */
     Real fNoiseLevel = 0.0f;
     GetNodeAttribute(t_tree, "level", fNoiseLevel);
     if (fNoiseLevel < 0.0f) {
       THROW_ARGOSEXCEPTION("Uniform noise level must be specified as non-negative value");
     }
     else if (fNoiseLevel > 0.0f) {
       m_cUniformRange.Set(-fNoiseLevel, fNoiseLevel);
     }
   }

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitUniform(const CRange<Real>& c_range) {
     m_model = ekUNIFORM;
     m_cUniformRange = c_range;
     m_bEnabled = true;
   }

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitGaussian(TConfigurationNode& t_tree) {
     /* Parse stddev */
     GetNodeAttributeOrDefault(t_tree,
                               "stddev",
                               m_fGaussianStdDev,
                               m_fGaussianStdDev);
     if (m_fGaussianStdDev < 0.0f) {
       THROW_ARGOSEXCEPTION("Gaussian standard deviation cannot be negative");
     }
     /* parse mean */
     GetNodeAttributeOrDefault(t_tree,
                               "mean",
                               m_fGaussianMean,
                               m_fGaussianMean);
   }

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitGaussian(Real f_mean, Real f_std_dev) {
     m_model = ekGAUSSIAN;
     m_fGaussianMean = f_mean;
     m_fGaussianStdDev = f_std_dev;
     m_bEnabled = true;
   }

   /****************************************/
   /****************************************/

   bool CNoiseInjector::BernoulliEvent() {
     return m_pcRNG->Bernoulli(InjectNoise());
   }

   /****************************************/
   /****************************************/

   Real CNoiseInjector::InjectNoise() {
     switch (m_model) {
       case ekNONE:
         return 0.0;
       case ekGAUSSIAN:
         return m_pcRNG->Gaussian(m_fGaussianStdDev, m_fGaussianMean);
       case ekUNIFORM:
         return m_pcRNG->Uniform(m_cUniformRange);
       default:
         THROW_ARGOSEXCEPTION("Bad noise model specified");
     }
   }

   /****************************************/
   /****************************************/
   std::string CNoiseInjector::GetQueryDocumentation(
       const SDocumentationQuerySpec& c_spec) {
     std::string doc =
         "If the '" + c_spec.strXMLTag + "' child tag is added to the " + c_spec.strDocName + " " +
         c_spec.strSAAType + " configuration,\n"
         "then the 'model' attribute is required and is used to specify the\n"
         "noise model used to calculate the amount of noise applied each timestep the\n" +
         c_spec.strDocName + " is enabled/active. If the '" + c_spec.strXMLTag + "' child tag does not exist,\n"
         "or the " + c_spec.strDocName + " " + c_spec.strSAAType + " is disabled no noise is injected.\n\n"

         "The following noise models can be specified:\n\n" +

         "- 'none' - Do not inject any noise; this model exists to allow for the inclusion of\n"
         "           the '" + c_spec.strXMLTag + "' tag without necessarily enabling noise injection.\n\n"

         "- 'uniform' - Injects uniformly distributed noise Uniform(-'level','level). The 'level'\n"
         "              attribute is required for this noise model.\n\n"

         "- 'gaussian' - Injects Gaussian('mean','stddev') noise. Both the 'mean' and 'stddev'\n"
         "               attributes are optional, and default to 0.0 and 1.0, respectively,\n"
         "               if omitted.\n\n";

     std::string strExamples =
         "Example XML configurations:\n\n"

         "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <" + c_spec.strSAAType +"s>\n"
                   "        ...\n"
                   "        <!-- Uniformly distributed noise -->\n"
                   "        <" + c_spec.strXMLParent + " implementation=\"default\">\n"
                   "          <" + c_spec.strXMLTag + " model=\"uniform\"\n"
                   "                 level=\"0.1\" />\n"
                   "        </" + c_spec.strXMLParent + ">\n"
                   "        <!-- Gaussian noise -->\n"
                   "        <" + c_spec.strXMLParent + " implementation=\"default\">\n"
                   "          <" + c_spec.strXMLTag + " model=\"gaussian\"\n"
                   "                 stddev=\"0.1\"\n"
                   "                 mean=\"0.1\" />\n"
                   "        </" + c_spec.strXMLParent + ">\n"
                   "        ...\n"
                   "      </" + c_spec.strSAAType +">\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
         "  </controllers>\n\n";
     if (!c_spec.bShowExamples) {
       return doc;
     } else {
       return doc + strExamples;
     }
   }

} // namespace argos
