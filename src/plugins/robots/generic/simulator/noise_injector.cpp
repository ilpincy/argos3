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
       m_pcRNG(CRandom::CreateRNG("argos")),
       m_bEnabled(false) {}

   /****************************************/
   /****************************************/

   bool CNoiseInjector::BernoulliEvent() {
     return m_pcRNG->Bernoulli(InjectNoise());
   }

   /****************************************/
   /****************************************/

   std::string CNoiseInjector::GetQueryDocumentation(
       const SDocumentationQuerySpec& c_spec) {
     std::string doc =
         "If the '" + c_spec.strXMLTag + "' child tag is added to the " + c_spec.strDocName + " configuration,\n"
         "then the 'model' attribute is required and is used to specify the noise model\n"
         "used to calculate the amount of noise applied each timestep the " +
         c_spec.strDocName + "\nis enabled/active. If the '" + c_spec.strXMLTag +
         "' child tag does not exist, or the " + c_spec.strDocName + "\nis disabled no noise is injected.\n\n"

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
                   "        <!-- Uniform(-0.1,0.1) noise -->\n"
                   "        <" + c_spec.strXMLParent + " implementation=\"default\">\n"
                   "          <" + c_spec.strXMLTag + " model=\"uniform\"\n"
                   "                 level=\"0.1\" />\n"
                   "        </" + c_spec.strXMLParent + ">\n"
                   "        <!-- Guassian(0.0, 0.1) noise -->\n"
                   "        <" + c_spec.strXMLParent + " implementation=\"default\">\n"
                   "          <" + c_spec.strXMLTag + " model=\"gaussian\"\n"
                   "                 stddev=\"0.0\"\n"
                   "                 mean=\"0.1\" />\n"
                   "        </" + c_spec.strXMLParent + ">\n"
                   "        ...\n"
                   "      </" + c_spec.strSAAType +"s>\n"
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
