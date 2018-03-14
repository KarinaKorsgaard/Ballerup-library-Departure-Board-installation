//
//  wordHandeler.h
//  text_2
//
//  Created by Karina Korsgaard Jensen on 13/03/2018.
//

#ifndef wordHandeler_h
#define wordHandeler_h


#include "CharacterFlip.h"

class WordHandeler{
public:
    int charWidth, charHeight;
    string next_string;
    vector<CharacterFlip>characters;
    std::map<string,int> map;
    bool weird;
    vector<int>chars;
    int max_length = 30;
    
    void setup(int cw, int ch, float at, vector<ofTexture> * _tex, int _max_length, string letters){
        charWidth = cw;
        charHeight = ch;
        next_string = getString("set");
        max_length = _max_length;
        
        vector<string>_alphabet = ofSplitString(letters, " ");
        _alphabet.push_back(" ");
 
        for(int i = 0; i<_alphabet.size();i++) {
            map[_alphabet[i]] = (i+1)%_alphabet.size();
        }
        
        characters.resize(max_length);
        
        for(int i = 0; i<characters.size();i++){
            characters[i].setup(cw, ch, at, _tex);
            characters[i].from = 0;
        }
    }
    
    
    void update(float animationTime){
        int charInString=0;
        for(int i = 0; i<characters.size();i++){
            
            if(characters[MAX(i-1,0)].getNewChar||i==0){
                
                int end = getSubString(next_string, charInString);
                int next = characters[i].from+1;
                next = next%map.size();
                
                characters[i].update(ofGetLastFrameTime(), characters[i].from, next, end, animationTime);
                
                charInString++;
                if(weird)charInString++;
                charInString = MIN(charInString,max_length);
            }
        }
    }

    void draw(int x, int y){
        ofPushMatrix();
        ofTranslate(x,y);
        for(auto &c: characters){
            ofTranslate(charWidth,0);
            c.draw();
        }
        ofPopMatrix();
    }
    
    void changeString(string str){
        next_string = getString(str);
    }
    

    
    int getSubString(string s, int i){
        char c = (s[i]);
        weird = false;
        string result = s.substr(i,1);
        if(c == '\303'){
            result = s.substr(i,2);
            weird = true;
        }
        if(map.find(result)!=map.end())
            return map.find(result)->second;
        else return 0;
    }
    
   
private:
    string getString(string str){
        for(int i = str.size();i<max_length;i++)
            str.append("  ");
        str = str.substr(0,max_length);
        return ofToUpper(str);
    }
};
#endif /* wordHandeler_h */
