//
//  CGGeometry.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGGeometry_hpp
#define CGGeometry_hpp

#include <stdio.h>
#include <vector>
#include "CGMaterial.hpp"
#include "CGGeometrySource.hpp"
#include <string>
#include "CG.hpp"
#include "CGGeometryElement.hpp"

enum CGBufferLocation
{
    CGBufferLocationVertex,
    CGBufferLocationColor,
    CGBufferLocationUV,
    CGBufferLocationNormal,
    CGBufferLocationIndex
};

class CGGeometry {
    
    static const unsigned int NUM_BUFFERS = 5;
    
    GLuint m_vertexArrayObject;
    
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    
    unsigned int m_numIndices;
    
    CGBufferLocation bufferLocationForGeometrySemantic(CGGeometrySourceSemantic semantic);
    
    void prepareBufferForGeometrySource(CGGeometrySource *source);
public:
    
    GLuint *attributeLocations;
    
    void initBuffers();
    
    GLuint vertexBuffer;
    
    GLuint indexBufferID;
    
    std::vector<CGMaterial*> materials;
    
    std::vector<CGGeometrySource*> geometrySources;
    
    std::vector<CGGeometryElement*> geomertyElements;
    
    virtual void presentationGeometry();
    
    CGMaterial* firstMaterial();
    
    CGMaterial* materialWithName(std::string* name);
    
    void setMaterial(CGMaterial* material);
    
    CGGeometrySource* geometrySourceForSemantic(CGGeometrySourceSemantic semantic);
    
    std::string *name;
    
    int vectorCount;
    
    CGGeometry() {
        
        materials.push_back(new CGMaterial());
        
        m_vertexArrayBuffers[CGBufferLocationIndex] = -1;
 
    }
    

    CGGeometry(std::string *filename);
};

#endif /* CGGeometry_hpp */
