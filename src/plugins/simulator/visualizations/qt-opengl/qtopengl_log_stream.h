/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <simulator/visualizations/qtopengl_log_buffer.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef QTOPENGL_LOG_STREAM_H
#define QTOPENGL_LOG_STREAM_H

namespace argos {
   class CQTOpenGLLogStream;
}

#include <QTextEdit>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/string_utilities.h>

namespace argos {

   class CQTOpenGLLogStream : public std::basic_streambuf<char> {

   public:

      CQTOpenGLLogStream(std::ostream& c_stream,
                         QTextEdit* c_textedit) :
         m_cStream(c_stream),
         m_pcTextEdit(c_textedit),
         m_cSpace(CSimulator::GetInstance().GetSpace()) {
         /* Copy the original stream buffer */
         m_pcOldStream = m_cStream.rdbuf();
         /* Replace the streambuffer */
         m_cStream.rdbuf(this);
      }

      virtual ~CQTOpenGLLogStream() {
         /* Replace old buffer */
         m_cStream.rdbuf(m_pcOldStream);
      }

      virtual int_type overflow(int_type t_value) {
         if (t_value == '\n') {
            std::string strTmp(m_strBuffer);
            Replace(strTmp, "<", "&lt;");
            Replace(strTmp, ">", "&gt;");
            strTmp = "<b>[t=" + ToString(m_cSpace.GetSimulationClock()) + "]</b> " + strTmp;
            m_pcTextEdit->append(strTmp.c_str());
            m_strBuffer.erase(m_strBuffer.begin(), m_strBuffer.end());
         }
         else {
            m_strBuffer += t_value;
         }
         return t_value;
      }

      virtual std::streamsize xsputn(const char* pc_message,
                                     std::streamsize un_size) {
         /* Add the message to text stream */
         m_strBuffer.append(pc_message, pc_message + un_size);
         size_t nPos = 0;
         do {
            /* Look for a newline */
            nPos = m_strBuffer.find('\n');
            if (nPos != std::string::npos) {
               /* Newline found: display text from the beginning of the stream till the newline */
               /* Get the portion to display */
               std::string strTmp(m_strBuffer.begin(), m_strBuffer.begin() + nPos);
               /* Format the text to display */
               Replace(strTmp, "<", "&lt;");
               Replace(strTmp, ">", "&gt;");
               strTmp = "<b>[t=" + ToString(m_cSpace.GetSimulationClock()) + "]</b> " + strTmp;
               /* Append it to the text windoe */
               m_pcTextEdit->append(strTmp.c_str());
               /* Erase the displayed portion from the text stream */
               m_strBuffer.erase(m_strBuffer.begin(), m_strBuffer.begin() + nPos + 1);
            }
         } while (nPos != std::string::npos);
         /* No more newlines found, return */
         return un_size;
      }

   private:

      std::ostream& m_cStream;
      std::streambuf* m_pcOldStream;
      std::string m_strBuffer;
      QTextEdit* m_pcTextEdit;
      CSpace& m_cSpace;
   };

}

#endif
