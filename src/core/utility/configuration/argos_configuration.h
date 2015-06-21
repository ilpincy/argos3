/**
 * @file <argos3/core/utility/configuration/argos_configuration.h>
 *
 * @brief This file provides some functions to deal with XML data.
 *
 * ARGoS is configured though an XML file. All the functions provided here are used to deal with that file.
 * Internally, these functions exploit the
 * <a href="http://www.grinninglizard.com/tinyxml/index.html" target="_blank">tinyxml library</a>.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ARGOS_CONFIGURATION_H
#define ARGOS_CONFIGURATION_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/configuration/tinyxml/ticpp.h>
#include <string>

namespace argos {

   /****************************************/
   /****************************************/

   /** The ARGoS configuration XML node */
   typedef ticpp::Element TConfigurationNode;
   /** The iterator for the ARGoS configuration XML node */
   typedef ticpp::Iterator <ticpp::Element> TConfigurationNodeIterator;
   /** The iterator for the attributes of an XML node */
   typedef ticpp::Iterator <ticpp::Attribute> TConfigurationAttributeIterator;

   /****************************************/
   /****************************************/

   /**
    * Given a tree root node, returns <tt>true</tt> if one of its child nodes has the wanted name.
    * Given an XML tree, this function checks for the existence of a first-level node in this tree (i.e., a direct child of the
    * root node) with the given name.
    * @param t_node the root node of the XML tree
    * @param str_tag the name of the wanted child node
    * @return <tt>true</tt> when the node exists, <tt>false</tt> otherwise
    */
   inline bool NodeExists(TConfigurationNode& t_node,
                          const std::string& str_tag) throw() {
      TConfigurationNodeIterator it(str_tag);
      it = it.begin(&t_node);
      return it != NULL;
   }

   /****************************************/
   /****************************************/

   /**
    * Given a tree root node, returns the first of its child nodes with the wanted name.
    * Given an XML tree, this function checks for the existence of a first-level node in this tree (i.e., a direct child of the
    * root node) with the given name.
    * @param t_node the root node of the XML tree
    * @param str_tag the name of the wanted child node
    * @return the first child node with the given name
    * @throw CARGoSException if no child node with the wanted name exists
    */
   inline TConfigurationNode& GetNode(TConfigurationNode& t_node,
                                      const std::string& str_tag) {
      try {
         TConfigurationNodeIterator it(str_tag);
         it = it.begin(&t_node);
         if(it == NULL) {
            THROW_ARGOSEXCEPTION("Node '" << str_tag << "' not found");
         }
         return *it;
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error searching for '" << str_tag << "' ", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Adds an XML node as child of another XML node.
    * The node is added at the end of the children of the parent node.
    * @param t_parent_node the parent node
    * @param t_child_node the child node to add to the parent
    * @throw CARGoSException if an error occurred
    */
   inline void AddChildNode(TConfigurationNode& t_parent_node,
                            TConfigurationNode& t_child_node) {
      try {
         t_parent_node.InsertEndChild(t_child_node);
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error inserting node '" << t_child_node << "' into node '" << t_parent_node << "'", ex);
      }      
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the text of the passed XML node
    * A node text is as follows:
    * <pre>
    * <mynode1>this is the text</mynode1>
    * <mynode2>25</mynode2>
    * <mynode3>3.14, 5.87</mynode3>
    * </pre>
    * This function is templetized. This means that you can pass any variable type to this function and parsing will occur
    * automatically. For instance:
    * <pre>
    * std::string strText;
    * GetNodeText(tMyNode1, strText); // tMyNode1 points to <mynode1>
    *                                 // strText now is "this is the text"
    * UInt32 unText;
    * GetNodeText(tMyNode2, unText);  // tMyNode2 points to <mynode2>
    *                                 // unText now is 25
    * CVector2 cText;
    * GetNodeText(tMyNode3, cText);   // tMyNode3 points to <mynode3>
    *                                 // cText now is CVector2(3.14, 5.87)
    * </pre>
    * @param t_node the node
    * @param t_buffer a buffer where the text must be stored
    * @throw CARGoSException if an error occurred (i.e., parse error) or the node has no text value
    * @see GetNodeTextOrDefault()
    */
   template <typename T>
   void GetNodeText(TConfigurationNode& t_node,
                    T& t_buffer) {
      try {
         t_node.GetText(&t_buffer);
      }
      catch(std::exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Parse error", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the text of the passed XML node, or the passed default value.
    * This function works like GetNodeText(). The only difference is that if no text value is found, the passed default is
    * stored in the buffer variable instead.
    * @param t_node the node
    * @param t_buffer a buffer where the text must be stored
    * @param t_default a default value to be used if the node has no text
    * @throw CARGoSException if an error occurred (i.e., parse error)
    * @see GetNodeText()
    */
   template <typename T>
   void GetNodeTextOrDefault(TConfigurationNode& t_node,
                             T& t_buffer,
                             const T& t_default) {
      try {
         t_node.GetTextOrDefault(&t_buffer, t_default);
      }
      catch(std::exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Parse error", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns <tt>true</tt> if the specified attribute of a node exists.
    * @param t_node the node whose attribute we want to search for
    * @param str_attribute the name of the attribute to search for
    * @return <tt>true</tt> if the attribute exists, <tt>false</tt> otherwise
    */
   inline bool NodeAttributeExists(TConfigurationNode& t_node,
                                   const std::string& str_attribute) {
      return t_node.HasAttribute(str_attribute);
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute.
    * XML nodes can have attributes:
    * <code>
    * <mynode attribute1="this is a string attribute"
    *         attribute2="-87"
    *         attribute3="-0.5, 12.3, 4" />
    * </code>
    * This function is templetized. This means that you can pass any variable type to this function and parsing will occur
    * automatically. For instance:
    * <code>
    * std::string strValue;
    * GetNodeAttribute(tMyNode, "attribute1", strValue); // tMyNode points to <mynode>
    *                                                    // strValue now is "this is a string attribute"
    * SInt32 nValue;
    * GetNodeAttribute(tMyNode, "attribute2", nValue);   // tMyNode points to <mynode>
    *                                                    // nValue now is -87
    * CVector3 cValue;
    * GetNodeAttribute(tMyNode, "attribute3", cValue);   // tMyNode points to <mynode>
    *                                                    // cValue now is CVector3(-0.5, 12.3, 4)
    * </code>
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param t_buffer a buffer where the value must be stored
    * @throw CARGoSException if an error occurred (i.e., parse error) or the attribute does not exist
    * @see GetNodeAttributeOrDefault()
    */
   template <typename T>
   void GetNodeAttribute(TConfigurationNode& t_node,
                         const std::string& str_attribute,
                         T& t_buffer) {
      try {
         t_node.GetAttribute(str_attribute, &t_buffer, true);
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute.
    * This function is an overloaded version of the templetized GetNodeAttribute() in which the buffer is a boolean.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param b_buffer a buffer where the value must be stored
    * @throw CARGoSException if an error occurred (i.e., parse error) or the attribute does not exist
    * @see GetNodeAttribute()
    */
   inline void GetNodeAttribute(TConfigurationNode& t_node,
                                const std::string& str_attribute,
                                bool& b_buffer) {
      std::string strBuffer;
      try {
         t_node.GetAttribute(str_attribute, &strBuffer, true);
         if(strBuffer == "true") {
            b_buffer = true;
         }
         else if(strBuffer == "false") {
            b_buffer = false;
         }
         else {
            THROW_ARGOSEXCEPTION("Cannot convert '" << strBuffer << "' into a bool. Accepted values: 'true', 'false'.");
         }
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute.
    * This function is an overloaded version of the templetized GetNodeAttribute() in which the buffer is a UInt8.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param un_buffer a buffer where the value must be stored
    * @throw CARGoSException if an error occurred (i.e., parse error) or the attribute does not exist
    * @see GetNodeAttribute()
    */
   inline void GetNodeAttribute(TConfigurationNode& t_node,
                                const std::string& str_attribute,
                                UInt8& un_buffer) {
      try {
         UInt32 unTmpBuffer;
         t_node.GetAttribute(str_attribute, &unTmpBuffer, true);
         un_buffer = unTmpBuffer;
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute.
    * This function is an overloaded version of the templetized GetNodeAttribute() in which the buffer is a SInt8.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param n_buffer a buffer where the value must be stored
    * @throw CARGoSException if an error occurred (i.e., parse error) or the attribute does not exist
    * @see GetNodeAttribute()
    */
   inline void GetNodeAttribute(TConfigurationNode& t_node,
                                const std::string& str_attribute,
                                SInt8& n_buffer) {
      try {
         SInt32 nTmpBuffer;
         t_node.GetAttribute(str_attribute, &nTmpBuffer, true);
         n_buffer = nTmpBuffer;
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute, or the passed default value.
    * This function works like GetNodeAttribute(). The only difference is that if the wanted attribute does not exist, the
    * passed default is stored in the buffer instead.
    * stored in the buffer variable instead.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param t_buffer a buffer where the value must be stored
    * @param t_default a default value to be used if the attribute does not exist
    * @throw CARGoSException if an error occurred (i.e., parse error)
    * @see GetNodeAttribute()
    */
   template <typename T>
   void GetNodeAttributeOrDefault(TConfigurationNode& t_node,
                                  const std::string& str_attribute,
                                  T& t_buffer,
                                  const T& t_default) {
      try {
         t_node.GetAttributeOrDefault(str_attribute, &t_buffer, t_default);
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute, or the passed default value.
    * This function is an overloaded version of the templetized GetNodeAttributeOrDefault() in which the buffer is a boolean.
    * stored in the buffer variable instead.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param b_buffer a buffer where the value must be stored
    * @param b_default a default value to be used if the attribute does not exist
    * @throw CARGoSException if an error occurred (i.e., parse error)
    * @see GetNodeAttributeOrDefault()
    */
   inline void GetNodeAttributeOrDefault(TConfigurationNode& t_node,
                                         const std::string& str_attribute,
                                         bool& b_buffer,
                                         const bool b_default) {
      std::string strBuffer;
      const std::string strDefault = (b_default ? "true" : "false");
      try {
         t_node.GetAttributeOrDefault(str_attribute, &strBuffer, strDefault);
         if(strBuffer == "true") {
            b_buffer = true;
         }
         else if(strBuffer == "false") {
            b_buffer = false;
         }
         else {
            THROW_ARGOSEXCEPTION("Cannot convert '" << strBuffer << "' into a bool. Accepted values: 'true', 'false'.");
         }
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute, or the passed default value.
    * This function is an overloaded version of the templetized GetNodeAttributeOrDefault() in which the buffer is a UInt8.
    * stored in the buffer variable instead.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param un_buffer a buffer where the value must be stored
    * @param un_default a default value to be used if the attribute does not exist
    * @throw CARGoSException if an error occurred (i.e., parse error)
    * @see GetNodeAttributeOrDefault()
    */
   inline void GetNodeAttributeOrDefault(TConfigurationNode& t_node,
                                         const std::string& str_attribute,
                                         UInt8& un_buffer,
                                         const UInt8 un_default) {
      try {
         UInt32 unTmpBuffer;
         t_node.GetAttributeOrDefault(str_attribute, &unTmpBuffer, static_cast<UInt32>(un_default));
         un_buffer = unTmpBuffer;
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Returns the value of a node's attribute, or the passed default value.
    * This function is an overloaded version of the templetized GetNodeAttributeOrDefault() in which the buffer is a SInt8.
    * stored in the buffer variable instead.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param n_buffer a buffer where the value must be stored
    * @param n_default a default value to be used if the attribute does not exist
    * @throw CARGoSException if an error occurred (i.e., parse error)
    * @see GetNodeAttributeOrDefault()
    */
   inline void GetNodeAttributeOrDefault(TConfigurationNode& t_node,
                                         const std::string& str_attribute,
                                         SInt8& n_buffer,
                                         const SInt8 n_default) {
      try {
         SInt32 nTmpBuffer;
         t_node.GetAttributeOrDefault(str_attribute, &nTmpBuffer, static_cast<SInt32>(n_default));
         n_buffer = nTmpBuffer;
      }
      catch(ticpp::Exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing attribute \"" << str_attribute << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Sets the value of the wanted node's attribute.
    * If the attribute does not exist, it is created.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param t_value the value to set
    */
   template <typename T>
   void SetNodeAttribute(TConfigurationNode& t_node,
                         const std::string& str_attribute,
                         const T& t_value) {
      t_node.SetAttribute(str_attribute, t_value);
   }

   /****************************************/
   /****************************************/

   /**
    * Sets the value of the wanted node's attribute.
    * This function is an overloaded version of the templetized SetNodeAttribute() for boolean values.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param b_value the value to set
    * @see SetNodeAttribute()
    */
   inline void SetNodeAttribute(TConfigurationNode& t_node,
                                const std::string& str_attribute,
                                const bool b_value) {
      if(b_value) {
         t_node.SetAttribute(str_attribute, "true");
      }
      else {
         t_node.SetAttribute(str_attribute, "false");
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Sets the value of the wanted node's attribute.
    * This function is an overloaded version of the templetized SetNodeAttribute() for SInt8 values.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param n_value the value to set
    * @see SetNodeAttribute()
    */
   inline void SetNodeAttribute(TConfigurationNode& t_node,
                                const std::string& str_attribute,
                                const SInt8 n_value) {
      t_node.SetAttribute(str_attribute, static_cast<SInt32>(n_value));
   }

   /****************************************/
   /****************************************/

   /**
    * Sets the value of the wanted node's attribute.
    * This function is an overloaded version of the templetized SetNodeAttribute() for UInt8 values.
    * @param t_node the node
    * @param str_attribute the name of the wanted attribute
    * @param un_value the value to set
    * @see SetNodeAttribute()
    */
   inline void SetNodeAttribute(TConfigurationNode& t_node,
                                const std::string& str_attribute,
                                const UInt8 un_value) {
      t_node.SetAttribute(str_attribute, static_cast<UInt32>(un_value));
   }

   /****************************************/
   /****************************************/

}

#endif
