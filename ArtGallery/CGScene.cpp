//
//  CGScene.cpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#include "CGScene.hpp"

GLenum CGScene::currentLightID() {
    switch (lightCount) {
        case 0:
            return GL_LIGHT0;
        case 1:
            return GL_LIGHT1;
        case 2:
            return GL_LIGHT2;
        case 3:
            return GL_LIGHT3;
        case 4:
            return GL_LIGHT4;
        case 5:
            return GL_LIGHT5;
        case 6:
            return GL_LIGHT6;
        case 7:
            return GL_LIGHT7;
        default:
            return GL_LIGHT7;
    }
}

void CGScene::renderNode(CGNode *node) {
    
    if(node->hidden) {
        return;
    }
    
    glPushMatrix();
    translate(node->position);
    
    // Check if euler angles is unmodified
    if (node->eulerAngles == CGVector3()) {
        
        glRotatef(node->rotation.w, node->rotation.x, node->rotation.y, node->rotation.z);
        
    } else {
        // Use Euler Angles
        CGVector3 eulerAngles = node->eulerAngles;
        glRotatef(eulerAngles.x, 1, 0, 0);
        glRotatef(eulerAngles.y, 0, 1, 0);
        glRotatef(eulerAngles.z, 0, 0, 1);
        
    }
    glScalef(node->scale.x, node->scale.y, node->scale.z);
    // Add Geometry
    if (node->geometry != NULL) {
    
        // Load Material
        CGMaterial *material = node->geometry->firstMaterial();
        if (material != NULL) {
            
         material->loadMaterial();
            
        }
        
        node->geometry->presentationGeometry();
     
    }
    
    // Add Light
    if (node->light != NULL) {
        
        CGLight *light = node->light;
        
        GLenum lightID = currentLightID();
        float lightW = 1.0;
        if (light->type == CGLightTypeDirectional) {
            lightW = 0.0;
        }
        GLfloat position[] = {node->position.x, node->position.y, node->position.z, lightW};
        
        // Set position
        glLightfv(lightID, GL_POSITION, position);
        
        float colorValues[] = {light->color.r, light->color.g, light->color.b, light->color.a};
        
        // Set Color
        glLightfv(lightID, GL_AMBIENT, colorValues);
        glLightfv(lightID, GL_DIFFUSE, colorValues);
        
        glLightf(lightID, GL_QUADRATIC_ATTENUATION, light->attenuationFalloffExponent);
        glLightf(lightID, GL_LINEAR_ATTENUATION, light->linearAttenuation);
        glLightf(lightID, GL_CONSTANT_ATTENUATION, light->constantAttenuation);
    
        if (light->type == CGLightTypeSpot) {
            
            // Set Spot light related properties
            glLightf(lightID, GL_SPOT_CUTOFF, light->spotOuterAngle);
            GLfloat directionValues[] = {node->rotation.x, node->rotation.y, node->rotation.y};
            glLightfv(lightID, GL_SPOT_DIRECTION, directionValues);
        } else {
            // Set back to default
            glLightf(lightID, GL_SPOT_CUTOFF, 180);
            GLfloat directionValues[] = {0,0, -1};
            glLightfv(lightID, GL_SPOT_DIRECTION, directionValues);
        }
        
        
        // Enable Light
        glEnable(lightID);
    
        lightCount++;
        
    }
    
    // Render Child nodes
    for (int i = 0; i < node->childNodes.size(); i++) {
        CGNode *childNode = node->childNodes[i];
        renderNode(childNode);
    }
    
    glPopMatrix();
}

CGScene::CGScene() {
    
    rootNode = new CGNode();
    
    backgroundColor = CGColorWhite();
    
    globalAmbientColor = CGColor(0.2, 0.2, 0.2, 1.0);
    
    lightCount = 0;
    
    autoenablesDefaultLighting = false;
    
    color = CGColorRed();
    
}

void CGScene::render() {
    
    // Setup Lighting
    lightCount = 0;
    
    // Check if root node exists
    if (rootNode == NULL) {
        return;
    }
    
    setContextColor(color);
    
    // Render root node recursively render all child nodes
    renderNode(rootNode);

}