#ifndef SCENENODE_H
#define SCENENODE_H

#include "View.h"
#include "FastTrackball.h"
#include <vector>
#include <map>

#define clamp(x, lower, upper) (min(upper, max(x,lower)))

class SceneNode {
    public:
        Model *object;
        vector<SceneNode*> children;
        SceneNode *parent;
        unsigned int *tex;
        int texnumber;
        // bool enableTexture = true;

        glm::mat4 *relTransform;
        glm::mat4 *comTransform;
        glm::mat4 *drawTransform;
        glm::mat4 *initialTransform;

        bool enable = true;

        SceneNode(string path, int type, int method, SceneNode* parent);
        
        virtual void render(Shader* shader, View *v, unsigned int *textures);
        virtual void update();
        void updateSecondContext(SceneNode* root);
 
        ~SceneNode();
};


#endif
