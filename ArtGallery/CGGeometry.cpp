//
//  CGGeometry.cpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#include "CGGeometry.hpp"
#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#include <OpenGL/gl3.h>

#elif defined _WIN32 || defined _WIN64
#    include <glut.h>
#endif
#include <iostream>
#include "CG.hpp"

CGGeometrySource* CGGeometry::geometrySourceForSemantic(CGGeometrySourceSemantic semantic) {
    
    for(std::vector<CGGeometrySource*>::iterator it = geometrySources.begin(); it != geometrySources.end(); ++it) {
        if ((*it)->semantic == semantic) {
            return  *it;
        }
    }

    return NULL;
}

void CGGeometry::presentationGeometry() {
 
/*
    glBindVertexArray(m_vertexArrayObject);
    
    if(geomertyElements.size() > 0) {
        CGGeometryElement *element = geomertyElements[0];
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[CGBufferLocationIndex]);
        // Draw the triangles !
        glDrawElements(
                       GL_TRIANGLES,      // mode
                       element->primitiveCount,    // count
                       GL_UNSIGNED_INT,   // type
                       (void*)0           // element array buffer offset
                       );

    } else {
        
        glDrawArrays(GL_TRIANGLES, 0, vectorCount);

    }
    
    glBindVertexArray(0);
    
    */
        
    CGGeometrySource *vertexSource = geometrySourceForSemantic(CGGeometrySourceSemanticVertex);
    CGGeometrySource *colorSource = geometrySourceForSemantic(CGGeometrySourceSemanticColor);
    CGGeometrySource *normalSource = geometrySourceForSemantic(CGGeometrySourceSemanticNormal);
    CGGeometrySource *texCoordSource = geometrySourceForSemantic(CGGeometrySourceSemanticTexcoord);
    
    glBegin(GL_TRIANGLES);
    float* dataArray = static_cast<float*>(vertexSource->data);
    
    float *colorArray = NULL;
    if (colorSource != NULL) {
         colorArray = static_cast<float*>(colorSource->data);
    }
    
    float *normalArray = NULL;
    if (normalSource != NULL) {
        normalArray = static_cast<float*>(normalSource->data);
    }
    
    float *texCoordArray = NULL;
    if (texCoordSource != NULL) {
        texCoordArray = static_cast<float*>(texCoordSource->data);
    }
    
    for (int i = 0; i < vertexSource->vectorCount; i++) {
        int vertexIndex = i * 3;
        float x = dataArray[vertexIndex+0];
        float y = dataArray[vertexIndex+1];
        float z = dataArray[vertexIndex+2];
        
        if (colorArray != NULL) {
            float colorX = colorArray[vertexIndex+0];
            float colorY = colorArray[vertexIndex+1];
            float colorZ = colorArray[vertexIndex+2];
            glColor3f(colorX, colorY, colorZ);
        }
        
        if (normalArray != NULL) {
            float normalX = dataArray[vertexIndex+0];
            float normalY = dataArray[vertexIndex+1];
            float normalZ = dataArray[vertexIndex+2];
            glNormal3f(normalX, normalY, normalZ);
        }
        
        if (texCoordArray != NULL) {
            float s = dataArray[vertexIndex+0];
            float t = dataArray[vertexIndex+1];
            glTexCoord2f(s, t);
        }
        
        glVertex3f(x, y, z);
    }
    
    glEnd();
    
    
    
}

CGMaterial* CGGeometry::firstMaterial() {
    
    return materials[0];
    
}

void CGGeometry::setMaterial(CGMaterial* material) {
    
    materials.pop_back();
    materials.push_back(material);
    
}

CGBufferLocation CGGeometry::bufferLocationForGeometrySemantic(CGGeometrySourceSemantic semantic) {
    
    switch (semantic) {
        case CGGeometrySourceSemanticVertex:
            return CGBufferLocationVertex;
            break;
        case CGGeometrySourceSemanticColor:
            return CGBufferLocationColor;
            break;
        case CGGeometrySourceSemanticNormal:
            return CGBufferLocationNormal;
        case CGGeometrySourceSemanticTexcoord:
            return CGBufferLocationUV;
    }
}


void CGGeometry::prepareBufferForGeometrySource(CGGeometrySource *source) {
    
    CGBufferLocation location = bufferLocationForGeometrySemantic(source->semantic);
    
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[location]);
    glBufferData(GL_ARRAY_BUFFER, source->bytesPerComponent * source->vectorCount , source->data, GL_STATIC_DRAW);
    glVertexAttribPointer(attributeLocations[location], 3, GL_FLOAT, GL_FALSE, 0, 0);

    vectorCount = source->vectorCount;

}

void CGGeometry::initBuffers() {
 
    /*
    // Setup Defaults
    glVertexAttrib3f(attributeLocations[CGBufferLocationColor], 1,1,1);
//    glVertexAttrib2f(CGBufferLocationUV, 0, 0);
    
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    
    // Disable All Buffers by default
    glDisableVertexAttribArray(CGBufferLocationVertex);
    glDisableVertexAttribArray(CGBufferLocationColor);
    glDisableVertexAttribArray(CGBufferLocationUV);
    glDisableVertexAttribArray(CGBufferLocationNormal);
    
    std::cout << "POSITION BUFFER: " << CGBufferLocationVertex << std::endl;
    std::cout << "COLOR BUFFER: " << CGBufferLocationColor << std::endl;

    for(std::vector<CGGeometrySource*>::iterator it = geometrySources.begin(); it != geometrySources.end(); ++it) {
        CGGeometrySource *source = *it;
        prepareBufferForGeometrySource(source);
    }
    
    // Get First Element
    for(std::vector<CGGeometryElement*>::iterator it = geomertyElements.begin(); it != geomertyElements.end(); ++it) {
        CGGeometryElement *element = *it;
        glGenBuffers(1, &m_vertexArrayBuffers[CGBufferLocationIndex]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[CGBufferLocationIndex]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, element->primitiveCount * element->bytesPerIndex, element->data, GL_STATIC_DRAW);
        
    }
    
    glBindVertexArray(m_vertexArrayObject);
 */


    
    
}



CGGeometry::CGGeometry(std::string *filename)  {
    



}




CGMaterial* CGGeometry::materialWithName(std::string* name) {
    
    for (int i = 0; i < materials.size(); i++) {
        CGMaterial *material = materials[i];
        
        if (material->name != NULL && *material->name == *name) {
            return material;
        }
    }
    
    return NULL;
}
