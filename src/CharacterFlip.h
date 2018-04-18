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
    
    ofSoundPlayer p;

    ofTexture * emoji;
    
    vector<ofTexture> * tex;
    
    void setEmoji(ofTexture * t){
        emoji = t;
    }
    
    void setup(int cw, int ch, float at, vector<ofTexture> * _tex, ofTexture * tUp, ofTexture * tLow){
        charWidth = cw;
        charHeight = ch;
        animationTime = at;
        getNewChar = false;

       // grade.load("grade");
        p.load("flip3.wav");
        p.setLoop(false);
        
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
        
        shadeUpper = tUp;
        shadeLower = tLow;

    }
    bool stopAnimation = false;
    void update(double time, int next, int end, float at){
        
        if(next == end) {
            stopAnimation = true;
            // cout<< "from "<<from << " next "<< next << " end "<<end<<endl;
        }
        else stopAnimation = false;
        
        if(!stopAnimation){
            
            if(!stopAtEmoji)localTime+=time;
            animationTime = at;
            
        
            to = next;
            
            
            
            if(localTime < animationTime/2){
                
                float aTime = localTime/(animationTime/2);
                float ypos = aTime*(charHeight/2);
                float xpos = ypos*0.4;
                
                mesh.getVertices()[0].set(ofVec2f(0-xpos, ypos));
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
                
                mesh.getVertices()[4].set(ofVec2f(0-xpos, ypos+charHeight/2));
                mesh.getVertices()[5].set(ofVec2f(charWidth+xpos, ypos+charHeight/2));
            }
            if(localTime > animationTime){
                if(to == swapToEmoji && doEmoji) {
                    stopAtEmoji = true;
                    correctMesh();
                }
                else {
                    
                    
                    
                    getNewChar = true;
                    correctMesh();
//
//                    if(next == end) {
//                        stopAnimation = true;
//                        // cout<< "from "<<from << " next "<< next << " end "<<end<<endl;
//                    }
                    //else {
                         from = next;
                         localTime = 0.0;
                    //}
                    
                }
            }
            
            if(stopAtEmoji){
                stopAtEmojiC+=ofGetLastFrameTime();
            }
            if(stopAtEmojiC>3.0){
                stopAtEmojiC = 0.0;
                stopAtEmoji = false; // start the time again
                
                
                localTime = 0.0;
                from = next;
                getNewChar = true;
                correctMesh();
            }
            if(doEmoji && pastEmoji(to, swapToEmoji)){
                doEmoji = false;
            }
        }
        
        else{
            doEmoji = false;
            correctMesh();
        }
    }
    bool pastEmoji(int a, int b){
        bool result = false;

        if(a > (b + 2)%tex->size())result = true;

        return result;
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
                ofSetColor(255);
                
                
                
                if(to == swapToEmoji && doEmoji)emoji->draw(0,0);
                else tex->at(to).draw(0,0); // behind
                
                
                if(from == swapToEmoji && doEmoji)emoji->bind();
                else tex->at(from).bind();
                mesh.draw();
                if(from == swapToEmoji && doEmoji)emoji->unbind();
                else tex->at(from).unbind();
                
                
                ofSetColor(0,localTime/(animationTime/2)*255);
              
                shadeUpper->bind();
                mesh.draw();
                shadeUpper->unbind();
                
               // ofDrawRectangle(0,mesh.getVertices()[0].y,tex->at(from).getWidth(),mesh.getVertices()[0].y);   
            }
            if(localTime > animationTime/2){
                // be on top of a
                ofSetColor(255);
                if(from == swapToEmoji && doEmoji)
                    emoji->draw(0,0);
                else tex->at(from).draw(0,0); // behind
                
               // grade.begin();
               // grade.setUniform1f("amount", localTime/animationTime * 255);
                
                if(to == swapToEmoji && doEmoji)emoji->bind();
                else tex->at(to).bind();
                mesh.draw();
                if(to == swapToEmoji && doEmoji)emoji->unbind();
                else tex->at(to).unbind();
                
                ofSetColor(100,255-(localTime-animationTime/2)/(animationTime/2)*255);
                shadeLower->bind();
                mesh.draw();
                shadeLower->unbind();
                
              //  p.play();
                
             //   grade.end();
            }
            
        }
    }
    
    void flipToEmoji(){
        int ls = tex->size();
        int r = int(ofRandom(ls-20,ls));
        
        from = (from + r) % ls;
        doEmoji = true;
        swapToEmoji = (from + r/2)%ls;
        
    }
    void flip(){
        int ls = tex->size();
        int r = int(ofRandom(ls-20,ls));
        from = (from + r) % ls;
        doEmoji = false;
    }


    ofTexture * shadeUpper;
    ofTexture * shadeLower;
    
    int swapToEmoji;
    bool doEmoji = false;
    int roundsAfterEmoiji = 0;
    
    bool stopAtEmoji = false;
    double stopAtEmojiC = 0.0;
};


#endif /* CharacterFlip_h */
