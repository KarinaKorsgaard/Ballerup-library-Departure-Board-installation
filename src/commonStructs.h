//
//  commonStructs.h
//  boardingPassGenerator
//
//  Created by Karina Korsgaard Jensen on 21/03/2018.
//

#ifndef commonStructs_h
#define commonStructs_h

struct Destinations{
    string destination;
    string number;
    vector<string>material;
    vector<string>sources;
    vector<string>materialDescription;
    double time;
    string str;
    ofTexture * emoji;
    int currentMaterial = 0;
};

#endif /* commonStructs_h */
