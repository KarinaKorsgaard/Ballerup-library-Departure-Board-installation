//
//  boadingPassGenerator.h
//  boardingPassGenerator
//
//  Created by Karina Korsgaard Jensen on 21/03/2018.
//

#ifndef boadingPassGenerator_h
#define boadingPassGenerator_h

#include "ofxSvg.h"
#include "ofApp.h"
#include "commonStructs.h"

class BoardingPassGenerator{
public:
    ofTrueTypeFont tidSted_f,mat_f;
    ofTrueTypeFont matb_f, number_f;
    
    ofFile printNumberFile;
    int printNumber;
    
    void setup(){
        ofxSVG svg;
        svg.load("bp_generator/boardingtemplate.svg");
        tidSted_f.load("fonts/BergenMono/BergenMono-SemiBold.otf",25);
        mat_f.load("fonts/BergenMono/BergenMono-SemiBold.otf",15);
        matb_f.load("fonts/HaasGrotesk/NHaasGroteskDSPro-55Rg.otf",14);
        number_f.load("fonts/BergenMono/BergenMono-Bold.otf",20);
        
        rects = getPolyline(svg);
        shader.load("bp_generator/sharpen");
        
        layout["dest"] = rects[1];
        layout["time"] = rects[2];
        
        layout["mat1"] = rects[3];
        layout["mat2"] = rects[4];
        layout["mat3"] = rects[5];
        
        
        layout["matb1"] = rects[6];
        layout["matb2"] = rects[7];
        layout["matb3"] = rects[8];
        
        string newLine = "\r\n";
#ifdef __APPLE__
        newLine = "\n";
#endif
        
        if(ofFile::doesFileExist(ofToDataPath("printNumberFile.txt"))) {
            cout<< "print number exists exists"<< endl;
            auto lines = ofSplitString(ofBufferFromFile("printNumberFile.txt").getText(), newLine);
            printNumber = ofToInt(ofSplitString(lines.back()," ")[0]);
            cout<<"lines: "<< printNumber<<" num lines "<< lines.size()<<endl;
        }else {
            ofFile newFile(ofToDataPath("printNumberFile.txt"),ofFile::WriteOnly); //file doesn't exist yet
            newFile.create(); // now file exists
            newFile.open(ofToDataPath("printNumberFile.txt"),ofFile::WriteOnly);
            newFile << "0 beginning";
            newFile.close();
        }
    }
    
    string generate(Destinations d, int current){
        ofEnableAntiAliasing();
        
        ofImage bg;
        bg.load("bp_generator/boarding_back-01.png");
        int w = bg.getWidth();
        int h = bg.getHeight();
        
        float scale = h/rects[0].height;
        
        ofFbo fbo; // for composing
        ofDisableArbTex();
        fbo.allocate(w,h, GL_RGBA);
        ofEnableArbTex();
        
        ofPixels pix;
        pix.allocate(w,h,GL_RGBA);
        
        ofFbo fbores; // for final output
        fbores.allocate(w,h,GL_RGBA);
        
        shader.load("bp_generator/sharpen");
        
 
 
        ofPushMatrix();
        fbo.begin();
        ofClear(255);
        ofBackground(255);
        
        bg.draw(0,0,w,h);
        
        ofSetColor(0);
        ofTranslate(0, 18);
        int numL = 1;
        string strupper = ofToUpper(d.destination);
        tidSted_f.drawString(strupper, layout["dest"].x*scale, layout["dest"].y*scale);
        tidSted_f.drawString(ofGetTimestampString("%H%M"), layout["time"].x*scale-3, layout["time"].y*scale);

        vector<int>materialIndx;
        materialIndx.resize(3);
        if(d.material.size()>3){
            
            materialIndx[0] = current;
            int temporary = current;
            while(temporary==materialIndx[0])
                temporary = floorf(ofRandom(d.material.size()));
            materialIndx[1] = temporary;
            while(temporary == materialIndx[0] || temporary == materialIndx[1])
                temporary = floorf(ofRandom(d.material.size()));
            materialIndx[2] = temporary;
            
           // cout <<"numbers: "<< materialIndx[0]<<materialIndx[1]<<materialIndx[2]<<endl;
        }
        else{
            for(int u = 0; u<MIN(3,d.material.size());u++)
                materialIndx[u] = u;
        }
        for(int u = 0; u<MIN(3,d.material.size());u++){
            int mi = materialIndx[u];
            string mat = "mat"+ofToString(mi+1);
            string matb = "matb"+ofToString(mi+1);
            
            ofRectangle a = rects[u+3];
            ofRectangle b = rects[u+6];
            mat_f.drawString(d.material[mi], a.x*scale, a.y*scale);
            drawCollumn(d.materialDescription[mi],b.x*scale, b.y*scale, 2);
        }
        
        ofSetColor(255);
        string number = ofToString(writeToFile(),6,'0');
        
        number_f.drawString(number,fbo.getWidth()-115,27);
        
        ofPopMatrix();
        fbo.end();
        
        fbores.begin();
        ofClear(255);
        shader.begin();
        
        fbo.draw(0,0);
        shader.end();
        fbores.end();
        
        fbores.readToPixels(pix);
        ofSaveImage(pix, "latest.png", OF_IMAGE_QUALITY_BEST);
        return "latest.png";
    }
        
    
    // helper functions
    vector<ofRectangle> getPolyline(ofxSVG svg){
        vector<ofRectangle>polys;
        for(int j = svg.getNumPath()-1; j>-1;j--){
            ofPath p = svg.getPathAt(j);
            p.setPolyWindingMode(OF_POLY_WINDING_ODD);
            vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
            polys.push_back(lines[0].getBoundingBox());
        }
        return polys;
    }
    
    vector<string> transformToCollumn(string str){
        int w = layout["matb1"].width;
        
        vector<string> result;
        string appending="";
        for(int i = 0; i<str.length();i++){
            string c = str.substr(i, 1);
            if (c == "\303") {
                c = str.substr(i, 2);
                i++;
            }
            appending.append(c);
            
            if(matb_f.stringWidth(appending) > w){
                // find last space
                string thisLine;
                string toNextLine;
                for(int s = 0; s<appending.length(); s++){
                    if(isSpace(appending[s])){
                        thisLine = appending.substr(0,s+1);
                        toNextLine = appending.substr(s+1,appending.length()-s);
                    }
                }
                result.push_back(thisLine);
                appending = toNextLine;
            }
        }
        result.push_back(appending);
        return result;
    }
    
    void drawCollumn(vector<string> s, int x, int y, int maxLine = 10 ){
        
        if(maxLine<s.size()){
            s[maxLine-1].pop_back();
            s[maxLine-1].append("...");
        }
        for(int i = 0; i < MIN(s.size(),maxLine); i++){
            matb_f.drawString(s[i], x, y+i*matb_f.getLineHeight() );
        }
    }
    bool isSpace(unsigned int c){
        //http://www.fileformat.info/info/unicode/category/Zs/list.htm
        return     c == 0x20 || c == 0xA0 || c == 0x1680 || c == 0x2000 || c == 0x2001
        || c == 0x2002
        || c == 0x2003 || c == 0x2004 || c == 0x2005 || c == 0x2006 || c == 0x2007 || c == 0x2028
        || c == 0x2029
        || c == 0x2008 || c == 0x2009 || c == 0x200A || c == 0x202F || c == 0x205F || c == 0x3000
        //    //https://en.wikipedia.org/wiki/Whitespace_character
        //    || c == 0x0009 //tab
        //    || c == 0x000A //line feed
        //    || c == 0x000B //line tab
        //    || c == 0x000C //form feed
        //    || c == 0x000F //carriage return
        //    || c == 0x0085 //next line
        ;
    }
    
    void drawLatest(){
        if(ofFile::doesFileExist(ofToDataPath(latest))){
            ofImage img;
            img.load(latest);
            img.draw(0,0);
        }
    }
    
    int writeToFile(){
        
        string newLine = "\r\n";
#ifdef __APPLE__
        newLine = "\n";
#endif
        
        printNumber++;
        ofFile newFile;
        newFile.open(ofToDataPath("printNumberFile.txt"),ofFile::Append);
        newFile <<newLine + ofToString(printNumber)+" "+ofGetTimestampString("%Y-%m-%d-%H");
        newFile.close();
        return printNumber;
    }

    
private:
    map<string,ofRectangle> layout;
    ofShader shader;
    vector<ofRectangle>rects;
    string latest;

};

#endif /* boadingPassGenerator_h */
