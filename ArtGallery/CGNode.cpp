//
//  CGNode.cpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#include "CGNode.hpp"

void CGNode::addChildNode(CGNode *node) {
    // Set parent node
    node->parentNode = this;
    // Add to array of child nodes
    childNodes.push_back(node);
}

CGNode::CGNode() {

    this->hidden = false;
    
    this->position = CGVector3();
    
    this->scale = CGVector3(1,1,1);
    
    camera = NULL;
    
    light = NULL;
    
    geometry = NULL;
    
}

CGNode::CGNode(CGGeometry *geometry) {
    
    this->geometry = geometry;
    
    this->hidden = false;
    
    this->scale = CGVector3(1,1,1);
    
    this->eulerAngles = CGVector3(0,0,0);
    
    camera = NULL;
    
    light = NULL;
    
}

void CGNode::removeFromParentNode() {
    
    if (parentNode != NULL) {
        //parentNode->childNodes.po
    }
}

CGNode::CGNode( const CGNode &obj) {
    
    hidden = obj.hidden;

    // Copy references to objects
    
    geometry = obj.geometry;
    
    light = obj.light;
    
    camera = obj.camera;
    
    // Copy position information
    
    position = obj.position;
    
    scale = obj.scale;
    
    rotation = obj.rotation;
    
    
}