//
//  CharacterFlip.h
//  text_2
//
//  Created by Karina Korsgaard Jensen on 13/03/2018.
//

#ifndef CharacterFlip_h
#define CharacterFlip_h


class CharacterFlip{
public:
    float animationTime;
    ofMesh mesh;
    int charWidth, charHeight;
    double localTime;
    int from, to;
    bool getNewChar;

    vector<ofTexture> * tex;
    
    void setup(int cw, int ch, float at, vector<ofTexture> * _tex){
        charWidth = cw;
        charHeight = ch;
        animationTime = at;
        getNewChar = false;

        tex = _tex;
        
        localTime = 0.0;
        from = '0';

        mesh.addVertex(ofVec2f(0, 0));
        mesh.addVertex(ofVec2f(charWidth, 0));
        
        mesh.addVertex(ofVec2f(0, charHeight/2));
        mesh.addVertex(ofVec2f(charWidth, charHeight/2));
        
        mesh.addVertex(ofVec2f(0, charHeight));
        mesh.addVertex(ofVec2f(charWidth, charHeight));
        
        
        mesh.addTexCoord(ofVec2f(0, 0));
        mesh.addTexCoord(ofVec2f(charWidth, 0));
        
        mesh.addTexCoord(ofVec2f(0, charHeight/2));
        mesh.addTexCoord(ofVec2f(charWidth, charHeight/2));
        
        mesh.addTexCoord(ofVec2f(0, charHeight));
        mesh.addTexCoord(ofVec2f(charWidth, charHeight));
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    }
    
    void update(double time, int _from, int next, int end, float at){
        
        if(next!=end){
           // cout << next <<" "<< end << endl;
            localTime+=time;
            animationTime = at;
            
            from = _from;
            to = next;
            
            
            
            if(localTime < animationTime/2){
                
                float aTime = localTime/(animationTime/2);
                float ypos = aTime*(charHeight/2);
                float xpos = ypos*0.4;
                
                mesh.getVertices()[0].set(ofVec2f(-xpos, ypos));
                mesh.getVertices()[1].set(ofVec2f(charWidth+xpos, ypos));
                
                mesh.getVertices()[4].set(ofVec2f(0, charHeight));
                mesh.getVertices()[5].set(ofVec2f(charWidth, charHeight));
            }
            if(localTime > animationTime/2){
                if(!getNewChar){
                    getNewChar = true;
                }
                
                float aTime = (localTime-animationTime/2)/(animationTime/2);
                float ypos = aTime*charHeight/2;
                float xpos = (charHeight/2 - ypos)*0.4;
                
                mesh.getVertices()[0].set(ofVec2f(0, 0));
                mesh.getVertices()[1].set(ofVec2f(charWidth, 0));
                
                mesh.getVertices()[4].set(ofVec2f(-xpos, ypos+charHeight/2));
                mesh.getVertices()[5].set(ofVec2f(charWidth+xpos, ypos+charHeight/2));
            }
            if(localTime > animationTime){
                localTime = 0.0;
                from = next;
                getNewChar = true;
                correctMesh();
                //cout <<"done "<< next <<" "<< end << endl;
            }
        }
    }
    void correctMesh(){
        mesh.getVertices()[0].set(ofVec2f(0, 0));
        mesh.getVertices()[1].set(ofVec2f(charWidth, 0));
        
        mesh.getVertices()[2].set(ofVec2f(0, charHeight/2));
        mesh.getVertices()[3].set(ofVec2f(charWidth, charHeight/2));
        
        mesh.getVertices()[4].set(ofVec2f(0, charHeight));
        mesh.getVertices()[5].set(ofVec2f(charWidth, charHeight));
    }
    
    
    void draw(){
        if(to<tex->size() && from < tex->size()){
            if(localTime < animationTime/2){
                //draw a on top of b
                tex->at(to).draw(0,0);
                
                tex->at(from).bind();
                mesh.draw();
                tex->at(from).unbind();
            }
            if(localTime > animationTime/2){
                // be on top of a
                tex->at(from).draw(0,0);
                tex->at(to).bind();
                mesh.draw();
                tex->at(to).unbind();
            }
        }
    }
};


#endif /* CharacterFlip_h */
