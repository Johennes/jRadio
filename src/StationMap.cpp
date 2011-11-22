//
//
//      StationMap.cpp
//      
//      Copyright 2011 Johannes Marbach <JohannesMarbach@googlemail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      


#include "StationMap.h"

#include "FileUtil.h"

#include <iostream>
#include <stdexcept>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    StationMap::StationMap()
    {
    }
    
    StationMap::~StationMap()
    {
    }
    
    
    // XML parsing & writing methods
    
    void StationMap::read_from_file(std::string file)
    {
        clear(); // Reset map
        
        if (! FileUtil::file_exists(file))
        {
            return;
        }
        
        // Parse file
        xmlDocPtr document = xmlReadFile(file.c_str(), 0, 0);
        if (! document)
        {
            throw std::runtime_error("Parsing of station list failed.");
            return;
        }
        
        // Get root node
        xmlNode* rootNode = xmlDocGetRootElement(document);
        if (! xmlStrEqual(rootNode->name, BAD_CAST "stations"))
        {
            throw std::runtime_error("Could not find root element in station list.");
            return;
        }
        
        // Loop over child nodes
        for (xmlNode* stationNode = rootNode->children; stationNode; stationNode = stationNode->next)
        {
            if (stationNode->type == XML_ELEMENT_NODE)
            {
                if (! xmlStrEqual(stationNode->name, BAD_CAST "station"))
                {
                    throw std::runtime_error("Unknown XML element "
                        + std::string((char*)stationNode->name) + " found in station list.");
                    return;
                }
                
                Station station;
                
                // Loop over station child nodes
                for (xmlNode* elementNode = stationNode->children; elementNode; elementNode = elementNode->next)
                {
                    if (elementNode->type == XML_ELEMENT_NODE)
                    {
                        if (xmlStrEqual(elementNode->name, BAD_CAST "id"))
                        {
                            station.ID = atoi((char*)xmlNodeGetContent(elementNode));
                        }
                        else if (xmlStrEqual(elementNode->name, BAD_CAST "title"))
                        {
                            station.title = (char*)xmlNodeGetContent(elementNode);
                        }
                        else if (xmlStrEqual(elementNode->name, BAD_CAST "genre"))
                        {
                            station.genre = (char*)xmlNodeGetContent(elementNode);
                        }
                        else if (xmlStrEqual(elementNode->name, BAD_CAST "mrl"))
                        {
                            station.MRL = (char*)xmlNodeGetContent(elementNode);
                        }
                        else if (xmlStrEqual(elementNode->name, BAD_CAST "iconpath"))
                        {
                            station.iconPath = (char*)xmlNodeGetContent(elementNode);
                        }
                    }
                }
                
                if (station.ID >= 0)
                {
                    (*this)[station.ID] = station;
                }
            }
        }
        
        // Memory clean-up
        xmlFreeDoc(document);
        xmlCleanupParser();
    }
    
    void StationMap::write_to_file(std::string file) const
    {
        xmlChar* tmp;
        
        // Initialize writer
        xmlTextWriterPtr writer = xmlNewTextWriterFilename(file.c_str(), 0);
        if (! writer) {
            throw std::runtime_error("XML writer initialization failed.");
            return;
        }
        
        // Start document
        int rc = xmlTextWriterStartDocument(writer, 0, "UTF-8", 0);
        if (rc < 0) {
            throw std::runtime_error("XML writer failed to start document.");
            return;
        }
        
        // Start root element
        rc = xmlTextWriterStartElement(writer, BAD_CAST "stations");
        if (rc < 0) {
            throw std::runtime_error("XML writer failed to start root element.");
            return;
        }
        
        for (const_iterator iter = this->begin(); iter != end(); ++iter)
        {
            Station station = iter->second;
            
            // Start station element
            rc = xmlTextWriterStartElement(writer, BAD_CAST "station");
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to start station element.");
                return;
            }
            
            // Write ID element
            rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "id", "%d", station.ID);
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to write id element.");
                return;
            }
            
            // Write title element
            tmp = convert_input(station.title);
            rc = xmlTextWriterWriteElement(writer, BAD_CAST "title", tmp);
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to write title element.");
                return;
            }
            if (tmp) xmlFree(tmp);
            
            // Write genre element
            tmp = convert_input(station.genre);
            rc = xmlTextWriterWriteElement(writer, BAD_CAST "genre", tmp);
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to write genre element.");
                return;
            }
            if (tmp) xmlFree(tmp);
            
            // Write mrl element
            tmp = convert_input(station.MRL);
            rc = xmlTextWriterWriteElement(writer, BAD_CAST "mrl", tmp);
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to write mrl element.");
                return;
            }
            if (tmp) xmlFree(tmp);
            
            // Write iconpath element
            tmp = convert_input(station.iconPath);
            rc = xmlTextWriterWriteElement(writer, BAD_CAST "iconpath", tmp);
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to write iconpath element.");
                return;
            }
            if (tmp) xmlFree(tmp);
            
            // Close station element
            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                throw std::runtime_error("XML writer failed to close station element.");
                return;
            }
        }
        
        // Close root element
        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            throw std::runtime_error("XML writer failed to close root element.");
            return;
        }
        
        // End document
        rc = xmlTextWriterEndDocument(writer);
        if (rc < 0) {
            throw std::runtime_error("XML writer failed to end document.");
            return;
        }
        
        // Memory clean-up
        xmlFreeTextWriter(writer);
        xmlCleanupParser();
    }
    
    
    // Miscellaneous methods
        
    xmlChar* StationMap::convert_input(std::string input) const
    {
        // Initialize encoding handler
        xmlCharEncodingHandlerPtr pHandler = xmlFindCharEncodingHandler("UTF-8");
        if (! pHandler)
        {
            throw std::runtime_error("Failed to initialize encoding handler.");
            return 0;
        }
        
        // Allocate output string
        int size = input.length() + 1;
        int out_size = size * 2 - 1;
        xmlChar* output = (unsigned char*)xmlMalloc((size_t)out_size);
        if (! output)
        {
            throw std::runtime_error("Failed to allocate XML output string.");
            return 0;
        }
        
        // COnvert input string
        int temp = size - 1;
        int rc = pHandler->input(output, &out_size, (const xmlChar*)(input.c_str()), &temp);
        if ((rc < 0) || (temp - size + 1))
        {
            throw std::runtime_error("Conversion to xmlChar failed.");
            xmlFree(output);
            return 0;
        }
        else
        {
            output = (unsigned char*)xmlRealloc(output, out_size + 1);
            output[out_size] = 0;  // Add null termination
        }
        
        return output;
    }
    
    
} // namespace jRadio
