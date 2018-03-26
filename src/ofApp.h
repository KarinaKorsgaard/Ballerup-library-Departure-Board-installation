#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxJSON.h"

#include "wordHandeler.h"
#include "commonStructs.h"
#include "boadingPassGenerator.h"
#include <codecvt>
#define NUM_DESTINATIONS 10
#define MOTION_INPUT 10

/*struct Destinations{
    string destination;
    string number;
    vector<string>material;
    vector<string>materialDescription;
    double time;
	string str;
};*/

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
  
    double timeSinceLastFlip;
    void doRandomFlip(int amount);
    std::map<string,ofTexture>emojis;
    
    int charWidth;
    int charHeight;
    ofTrueTypeFont font;

    vector<int>destination_indxes;
    vector<WordHandeler> wh_number;
    vector<WordHandeler> wh_destination;
    vector<WordHandeler> wh_material;
    vector<WordHandeler> wh_numberOfMaterials;
    
    vector<Destinations>desitnations;
    
    ofxPanel gui;
    ofParameter<float>animationTime;
    ofParameter<float>randomFlipInterval;
    ofParameter<float>materialSwapTime;
    
    std::map<char,char> map;
    
    vector<ofTexture>textures;
    vector<ofTexture>textures_yellow;

    ofTexture arrow, empty;
    
    void printBoardingPass(int d);
    BoardingPassGenerator bpg;
    
    bool        bSendSerialMessage;            // a flag for sending serial
    char        bytesRead[3];                // data from serial, we will be trying to read 3
    char        bytesReadString[4];            // a string needs a null terminator, so we need 3 + 1 bytes
    int            nBytesRead;                    // how much did we read?
    int            nTimesRead;                    // how many times did we read?
    float        readTime;                    // when did we last read?

    ofSerial    serial;
    void initialiseArdiono();
    void readArduino();
    int input = -1;
    int p_input = -1;
    bool motioninput = false;
    bool isInitialized = false;
    
    void roundedRect(float x, float y, float w, float h, float r) {
        ofBeginShape();
        ofVertex(x+r, y);
        ofVertex(x+w-r, y);
        quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
        ofVertex(x+w, y+h-r);
        quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
        ofVertex(x+r, y+h);
        quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
        ofVertex(x, y+r);
        quadraticBezierVertex(x, y, x+r, y, x, y+r);
        ofEndShape();
    }
    
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
        float cp1x = prevX + 2.0/3.0*(cpx - prevX);
        float cp1y = prevY + 2.0/3.0*(cpy - prevY);
        float cp2x = cp1x + (x - prevX)/3.0;
        float cp2y = cp1y + (y - prevY)/3.0;
        
        // finally call cubic Bezier curve function
        ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
    };
    ofTexture tUp, tLo;


    ofBufferObject drawTexture(ofColor c, int w, int h, string l, ofImage emoji){
        ofFbo fbo;
        fbo.allocate(w, h, GL_RGBA);
        
        ofRectangle r;
        if(l != "")
            r = font.getStringBoundingBox(l, 0, 0);
        else
            r = ofRectangle(0,0,emoji.getWidth(),emoji.getHeight());
        
        int x = w/2 - r.width/2;
        int y = h/2 - r.height/2;
        if(l != "")y+=font.getLineHeight()/2 + 3;
        
        fbo.begin();
        ofClear(0);
        ofSetColor(20);
        roundedRect(0,0,charWidth,charHeight,5);
        
        ofSetColor(c);
        if(l!="")font.drawString(l, x, y);
        else emoji.draw(x,y);
        ofNoFill();
        ofSetLineWidth(0.5);
        ofSetColor(50);
        //roundedRect(0.5,0.5,charWidth-1,charHeight-1,6);
        //ofDrawRectangle(0.5, 0.5, charWidth-1, charHeight-1);
        //ofDrawLine(0.5, charHeight/2, charWidth-0.5, charHeight/2);
        ofFill();
        ofSetColor(255);
        //ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));
        fbo.end();
        
        ofBufferObject buffer;
        buffer.allocate(charWidth * charHeight * 4, GL_STATIC_DRAW);
        fbo.getTexture().copyTo(buffer);
        
        return buffer;
    }
    
    bool debug = true;


    
};
