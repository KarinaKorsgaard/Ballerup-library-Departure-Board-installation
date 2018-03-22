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


    ofShader * grade;
    
    int max_length = 30;
    
    void setup(int cw, int ch, float at, vector<ofTexture> * _tex, int _max_length, string letters, ofTexture * tUp, ofTexture * tLo){
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
            characters[i].setup(cw, ch, at, _tex, tUp, tLo);
            characters[i].from = 0;
            //characters[i].setEmoji
        }
    }
    
    
    void update(float animationTime){
        int charInString=0;
        for(int i = 0; i<characters.size();i++){
            
            if(characters[MAX(i-1,0)].getNewChar||i==0){
                
                int end = getSubString(next_string,i,charInString);
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
            c.draw();
            ofTranslate(charWidth*1.05,0);
        }
        ofPopMatrix();
    }
    
    void changeString(string str, ofTexture * e = NULL){
        next_string = getString(str);
        if(e!=NULL)setEmoji(e);
    }
    
    void setEmoji(ofTexture * t){
        for(int i = 0; i<characters.size();i++){
            characters[i].setEmoji(t);
        }
    }

    
    int getSubString(string s,int itr, int i){
        if(itr>max_length-2 && wordIsToLong) return map["..."];
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
        wordIsToLong = false;
        if(str.size()>max_length)wordIsToLong=true;
        
        for(int i = str.size();i<max_length;i++)
            str.append("  ");
        //if(str.length()>max_length)
        str = str.substr(0,max_length);
        return ofToUpperUnicode(str);
    }
    
    //--------------------------------------------------
    string ofToUpperUnicode(const string & src){
        string dst = ofToUpper(src);
        dst = ReplaceAll(dst,"æ", "Æ");
        dst = ReplaceAll(dst,"ø", "Ø");
        dst = ReplaceAll(dst,"å", "Å");
        return dst;
    }
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
    
    bool wordIsToLong;
};
#endif /* wordHandeler_h */
