#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxJSON.h"

#include "wordHandeler.h"
#define NUM_DESTINATIONS 5

struct Destinations{
    string destination;
    string number;
    vector<string>material;
    vector<string>materialDescription;
    double time;
};

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
    
//    ofArduino arduino;
//    ofMesh mesh1, mesh2;
//
//    string string_a;
//    ofFbo characterFbo;
//
//    double time = 0.0;
    
    int charWidth;
    int charHeight;
    ofTrueTypeFont font;
  
    vector<int>destination_indxes;
    vector<WordHandeler> wh_number;
    vector<WordHandeler> wh_destination;
    vector<WordHandeler> wh_material;
    
    vector<Destinations>desitnations;
    
    ofxPanel gui;
    ofParameter<float>animationTime;
    std::map<char,char> map;
    
    vector<ofTexture>textures;
    
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
    
    void printBoardingPass(int d);

    
    bool        bSendSerialMessage;            // a flag for sending serial
    char        bytesRead[3];                // data from serial, we will be trying to read 3
    char        bytesReadString[4];            // a string needs a null terminator, so we need 3 + 1 bytes
    int            nBytesRead;                    // how much did we read?
    int            nTimesRead;                    // how many times did we read?
    float        readTime;                    // when did we last read?

    ofSerial    serial;
    
   // ofArduino arduino;
    
};
