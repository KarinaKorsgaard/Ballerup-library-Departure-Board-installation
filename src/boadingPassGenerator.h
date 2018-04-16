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


class BoardingPassGenerator {
public:
    ofTrueTypeFont tidSted_f,mat_f;
    ofTrueTypeFont matb_f, number_f;
    float scale;
    int h,w;
    ofImage bg;
    ofFile printNumberFile;
    int printNumber;
    Destinations MyDest;
    int myCur = 0;
    
    ofFbo fbo;
    ofPixels pix;
    
    void setup(){
        
        ofDisableArbTex();
        fbo.allocate(w,h, GL_RGBA);
        ofEnableArbTex();
        
        pix.allocate(w,h,GL_RGBA);
        
        ofxSVG svg;
        svg.load("bp_generator/boardingtemplate.svg");
        tidSted_f.load("fonts/BergenMono/BergenMono-SemiBold.otf",35);
        mat_f.load("fonts/BergenMono/BergenMono-SemiBold.otf",17);
        matb_f.load("fonts/HaasGrotesk/NHaasGroteskDSPro-55Rg.otf",17);
        matb_f.setLineHeight(matb_f.getLineHeight()*1.1);
        //matb_f.setLetterSpacing(1.1);
        //      matb_fbold.load("fonts/HaasGrotesk/NHaasGroteskDSPro-65Md.otf",16);
        number_f.load("fonts/BergenMono/BergenMono-Bold.otf",28);
        //matb_f.setLetterSpacing(1.2);
        
        rects = getPolyline(svg);
        cout << rects.size() << endl;
        shader.load("bp_generator/sharpen");
        
        layout["dest"] = rects[1];
        layout["time"] = rects[2];
        
        layout["mat1"] = rects[3];
        layout["mat2"] = rects[4];
        layout["mat3"] = rects[5];
        
        
        layout["matb1"] = rects[6];
        layout["matb2"] = rects[7];
        layout["matb3"] = rects[8];
        
        layout["general"] = rects[0];
        
        
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
        
        
        bg.load("bp_generator/boarding_back-01.png");
        w = bg.getWidth();
        h = bg.getHeight();
        
        scale = h/rects[9].height;
        cout <<"bp scale " <<scale<<" w: "<<w<<" h: "<<h<< endl;
        

    }
    
    
    void generate(Destinations d, int current){
        
        
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
        
        ofRectangle g = layout["general"];
        vector<string> general = transformToCollumn("Med biblioteket har du online adgang til en verden af bøger, film, musik, blade og aviser, som er lette at tage med på rejsen.", g.width*scale, matb_f);
        general.push_back("");
        vector<string>concat =transformToCollumn("Find dem på www.bib.ballerup.dk/e-materialer", g.width*scale, matb_f);
        general.insert( general.end(), concat.begin(), concat.end() );
        
        
        fbo.begin();
        ofClear(255);
        ofBackground(255);
        bg.draw(0,0,w,h);
        
        ofSetColor(0);
        
        ofPushMatrix();
        ofTranslate(0,tidSted_f.stringHeight("Å"));
        
        tidSted_f.drawString(toUpper(d.destination), layout["dest"].x*scale, layout["dest"].y*scale);
        tidSted_f.drawString(ofGetTimestampString("%H%M"), layout["time"].x*scale, layout["time"].y*scale);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(0,mat_f.stringHeight("Å"));
        for(int u = 0; u<MIN(3,d.material.size());u++){
            int mi = materialIndx[u];
            ofRectangle a = rects[u+3];
            ofRectangle b = rects[u+6];
            mat_f.drawString(toUpper(d.material[mi]), a.x*scale, a.y*scale);
            vector<string>description = transformToCollumn(d.materialDescription[mi], layout["mat1"].width*scale, matb_f);
            drawCollumn(description ,b.x*scale, b.y*scale, matb_f);
        }
        drawCollumn(general, g.x*scale, g.y*scale, matb_f, 30);
        ofPopMatrix();
        
        ofSetColor(255);
        string number = "#"+ofToString(writeToFile(),6,'0');
        number_f.drawString(number,fbo.getWidth()-number_f.stringWidth(number)-50,70);
        fbo.end();
        
        fbo.readToPixels(pix);
        ofSaveImage(pix, "latest.png", OF_IMAGE_QUALITY_BEST);
        
        
        //}
        // }
        string boardingPass = "latest.png";
        string cwd = ofFilePath::getCurrentWorkingDirectory();
#ifdef __APPLE__
        string command = "lp "+ ofSplitString(cwd,"/bin")[0] + "/bin/data/boardingPasses/"+ boardingPass;
        system(command.c_str());
        // cout << "lp "+ ofSplitString(cwd,"/bin")[0] + "/bin/data/boardingPasses/"+ desitnations[d].str << endl;
#else
        string command =cwd+"\\bin\\SumatraPDF.exe -print-to-default -print-settings \"fit\" "+ cwd +"\\bin\\data\\"+ boardingPass;
        //system(command.c_str());
        cout << command << endl;
        cout << cwd << endl;
        cout << "what" << endl;
        if(!debug)system(command.c_str());
        
        
        //string t = ofToDataPath("test.bat");
        //cout << path << endl;
        
#endif
        return "latest.png";
        
    }
    
    
    
    
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
    string toUpper(string s){
        string res = ofToUpper(s);
        res=ReplaceAll(res,"æ","Æ");
        res=ReplaceAll(res,"ø","Ø");
        res=ReplaceAll(res,"å","Å");
        return res;
    }
    
    // helper functions
    vector<ofRectangle> getPolyline(ofxSVG svg){
        vector<ofRectangle>polys;
        for(int j = svg.getNumPath()-1; j>-1;j--){
            ofPath p = svg.getPathAt(j);
            p.setPolyWindingMode(OF_POLY_WINDING_ODD);
            vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
            polys.push_back(lines[0].getBoundingBox());
            cout << polys.back().width << endl;
        }
        return polys;
    }
    
    vector<string> transformToCollumn(string str, int w, ofTrueTypeFont f){
        
        
        vector<string> result;
        string appending="";
        for(int i = 0; i<str.length();i++){
            string c = str.substr(i, 1);
            if (c == "\303") {
                c = str.substr(i, 2);
                i++;
            }
            appending.append(c);
            
            if(f.stringWidth(appending) > w){
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
    
    void drawCollumn(vector<string> s, int x, int y, ofTrueTypeFont f,int maxLine = 10 ){
        
        if(maxLine<s.size()){
            s[maxLine-1].pop_back();
            s[maxLine-1].append("...");
        }
        for(int i = 0; i < MIN(s.size(),maxLine); i++){
            f.drawString(s[i], x, y+i*f.getLineHeight() );
        }
    }
    bool isSpace(unsigned int c){
        //http://www.fileformat.info/info/unicode/category/Zs/list.htm
        return     c == 0x20 || c == 0xA0 || c == 0x1680 || c == 0x2000 || c == 0x2001
        || c == 0x2002
        || c == 0x2003 || c == 0x2004 || c == 0x2005 || c == 0x2006 || c == 0x2007 || c == 0x2028
        || c == 0x2029
        || c == 0x2008 || c == 0x2009 || c == 0x200A || c == 0x202F || c == 0x205F || c == 0x3000 || c == '/'
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

