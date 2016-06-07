//
//  CGNode.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGNode_hpp
#define CGNode_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "CGGeometry.hpp"
#include "CGCamera.hpp"
#include "CGLight.hpp"
#include "CG.hpp"

class CGNode {
    
    
public:
    std::string *name;
    
    CGNode *parentNode;
    
    CGGeometry *geometry;
    
    CGCamera *camera;
    
    CGLight *light;
    
    std::vector<CGNode*> childNodes;
    
    void addChildNode(CGNode*);
    
    void removeFromParentNode();
    
    CGNode* childNodeWithName(std::string name, bool recursively);
    
    CGVector3 position;
    
    CGVector4 rotation;
    
    CGVector3 eulerAngles;
    
    CGVector3 scale;
    
    bool hidden;
    
    CGNode();
    
    CGNode(CGGeometry *geometry);
    
    CGNode( const CGNode &obj);  // copy constructor

    
};

#endif /* CGNode_hpp */
