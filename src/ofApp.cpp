#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // serial
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    int baud = 9600;
#ifdef __APPLE__
    serial.setup(0, baud); //open the first device
    cout << "on mac"<< endl;
#else
    serial.setup("COM4", baud); // windows example
    cout << "on windows"<< endl;
#endif
    //serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
    //serial.setup("/dev/ttyUSB0", baud); //linux example
    
    nTimesRead = 0;
    nBytesRead = 0;
    readTime = 0;
    memset(bytesReadString, 0, 4);
    //    //serial
    
    //    ofSerial serial;
    //    serial.listDevices();
    //    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    //    arduino.connect(deviceList[0].getDeviceName(),9600);
    //
    
    
    gui.setup();
    gui.add(animationTime.set("AnimationTime",1.,5.,0.01));
    gui.add(randomFlipInterval.set("max FlipInterval",1.,0.,10.));
    gui.loadFromFile("settings.xml");
    

    string letters = "0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V X Y Z Æ Ø Å .";
    vector<string>_alphabet = ofSplitString(letters, " ");
    _alphabet.push_back(" ");
    
    
    cout << "num letters " << _alphabet.size() << endl;
    
    font.load("circular.ttf", 20);
    ofRectangle r = font.getStringBoundingBox("Åg", 0, 0);
    charWidth = r.width*1.1; // * 1.1, no idea why.
    charHeight = r.height*2.;
    
    cout << charWidth << " "<< charHeight <<endl;
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    
    ofFbo fbo;
    fbo.allocate(charWidth, charHeight, GL_RGBA);
    for(int i = 0; i<_alphabet.size();i++){
        r = font.getStringBoundingBox(_alphabet[i], 0, 0);
        int x = charWidth/2 - r.width/2;
        int y = charHeight/2 - r.height/2 + font.getLineHeight()/2 + 3;
        
        fbo.begin();
        ofClear(0);
        ofSetColor(0);
        roundedRect(0,0,charWidth,charHeight,5);

        ofSetColor(255);
        font.drawString(_alphabet[i], x, y);
        
        ofNoFill();
        ofSetLineWidth(0.5);
        roundedRect(0.5,0.5,charWidth-1,charHeight-1,6);
        //ofDrawRectangle(0.5, 0.5, charWidth-1, charHeight-1);
        ofDrawLine(0.5, charHeight/2, charWidth-0.5, charHeight/2);
        ofFill();
        //ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));
        fbo.end();
        
        ofBufferObject buffer;
        buffer.allocate(charWidth * charHeight * 4, GL_STATIC_DRAW);
        fbo.getTexture().copyTo(buffer);
        
        ofTexture tex;
        
        textures.push_back(tex);
        textures.back().allocate(charWidth, charHeight, GL_RGBA);
        textures.back().loadData(buffer, GL_RGBA, GL_UNSIGNED_BYTE);
        
    }
    
    wh_number.resize(NUM_DESTINATIONS);
    for(int i = 0; i<wh_number.size();i++)
        wh_number[i].setup(charWidth, charHeight, animationTime, &textures, 5, letters);
    wh_destination.resize(NUM_DESTINATIONS);
    for(int i = 0; i<wh_destination.size();i++)
        wh_destination[i].setup(charWidth, charHeight, animationTime, &textures, 10, letters);
    wh_material.resize(NUM_DESTINATIONS);
    for(int i = 0; i<wh_material.size();i++)
        wh_material[i].setup(charWidth, charHeight, animationTime, &textures, 20, letters);
    
    
    
    // json crap
    ofxJSONElement result;
    std::string file = "example.json";
    
    // Now parse the JSON
    bool parsingSuccessful = result.open(file);
    
    if (parsingSuccessful) {
        if (!result.save("example_output_pretty.json", true))
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written unsuccessfully.";
        else
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written successfully.";
        
        if (!result.save("example_output_fast.json", false))
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written unsuccessfully.";
        else ofLogNotice("ofApp::setup") << "example_output_pretty.json written successfully.";
    }
    else ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    
    desitnations.resize(result.size());
    cout << "detinations "<< desitnations.size()<<endl;
    for(int i = 0; i<desitnations.size();i++){
        const Json::Value& beskrivelse = result[i];
        desitnations[i].destination = beskrivelse["destination"].asString();
        desitnations[i].number = beskrivelse["DK5"].asString();
		desitnations[i].str = beskrivelse["destination"].asString()+".png";
        for(int u = 0; u<5; u++){
            desitnations[i].material.push_back(beskrivelse["materiale"+ofToString(u+1)].asString());
            desitnations[i].materialDescription.push_back(beskrivelse["materialeBeskriv"+ofToString(u+1)].asString());
            if(desitnations[i].material.back()=="") desitnations[i].material.pop_back();
            if(desitnations[i].materialDescription.back()=="") desitnations[i].materialDescription.pop_back();
            
        }
    }
    
    for(int i = 0; i<NUM_DESTINATIONS;i++) {
        destination_indxes.push_back(i);
    }
    
    for(int i = 0; i<NUM_DESTINATIONS;i++){
        int d = destination_indxes[i];
        wh_destination[i].changeString(desitnations[d].destination);
        wh_number[i].changeString(desitnations[d].number);
        wh_material[i].changeString(desitnations[d].material[int(ofRandom(desitnations[d].material.size()))]);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    timeSinceLastFlip += ofGetLastFrameTime();
    
    for(int i = 0; i<NUM_DESTINATIONS;i++){
        wh_destination[i].update(animationTime);
        wh_number[i].update(animationTime);
        wh_material[i].update(animationTime);
        desitnations[i].time += ofGetLastFrameTime();
        
        if(desitnations[i].time > 15){
            desitnations[i].time = 0.0;
            string newMaterial = desitnations[i].material[int(ofRandom(desitnations[i].material.size()))];
            //  if(newMaterial!=wh_material[i].next_string)
            //      wh_material[i].changeString(newMaterial);
        }
        
    }
    
    
    // update serial
    //if (bSendSerialMessage){
    // (1) write the letter "a" to serial:
    //serial.writeByte('a');
    
    // (2) read
    // now we try to read 3 bytes
    // since we might not get them all the time 3 - but sometimes 0, 6, or something else,
    // we will try to read three bytes, as much as we can
    // otherwise, we may have a "lag" if we don't read fast enough
    // or just read three every time. now, we will be sure to
    // read as much as we can in groups of three...
    //

    //}
    //string read = arduino.getString();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    ofBackground(0);
    
    gui.draw();
    
    
    for(int i = 0; i<NUM_DESTINATIONS;i++){
        ofPushMatrix();
        wh_number[i].draw(0, 10 + 1.05*i*charHeight);
        ofTranslate(wh_number[i].max_length*charWidth, 0);
        
        wh_destination[i].draw(0, 10 + 1.05*i*charHeight);
        ofTranslate(wh_destination[i].max_length*charWidth, 0);
        
        wh_material[i].draw(0, 10 + 1.05*i*charHeight);
        ofPopMatrix();
    }
    
    
}
//--------------------------------------------------------------
void ofApp::printBoardingPass(int d){
    // print..
	string cwd = ofFilePath::getCurrentWorkingDirectory();

#ifdef __APPLE__
	string command = "lp "+ pathToDataFolder + "/data/boardingPasses/"+ desitnations[d].str;
	system(command.c_str());
#else
	string command = "SumatraPDF.exe -print-to-default \\data\\boardingPasses\\"+ desitnations[d].str;
	//system(command.c_str());

	//system(command.c_str());
	string path = ofToDataPath("Newfilename.bat");
	ofFile file(path, ofFile::WriteOnly);
	cout <<path << endl;
	file << "@echo off";
	file << "\r\n";
	file << "cd \""+cwd+"\"";
	file << "\r\n";
	file << command;
	file.close();

	system(command.c_str());

	//string t = ofToDataPath("test.bat");
	//cout << path << endl;
	
#endif

}
void ofApp::generateBoardingPasses() {

}
void ofApp::initArduino() {
	nTimesRead = 0;
	nBytesRead = 0;
	int nRead = 0;  // a temp variable to keep count per read

	unsigned char bytesReturned[3];

	memset(bytesReadString, 0, 4);
	memset(bytesReturned, 0, 3);

	while ((nRead = serial.readBytes(bytesReturned, 3)) > 0) {
		nTimesRead++;
		nBytesRead = nRead;
	};

	if (nBytesRead>0) {
		memcpy(bytesReadString, bytesReturned, 3);

		bSendSerialMessage = false;
		readTime = ofGetElapsedTimef();

		string fromArduino = string(bytesReadString);
		char fa = fromArduino[0];
		if (int(fa>15)) {
			int input = ofToInt(fromArduino);
			if (input < 5)printBoardingPass(input);
			else if (input == 5) {
				for (int i = 0; i<NUM_DESTINATIONS; i++) {
					destination_indxes[i] += NUM_DESTINATIONS;
					destination_indxes[i] = destination_indxes[i] % desitnations.size();

					int d = destination_indxes[i];
					wh_destination[i].changeString(desitnations[d].destination);
					wh_number[i].changeString(desitnations[d].number);
					wh_material[i].changeString(desitnations[d].material[int(ofRandom(desitnations[d].material.size()))]);

				}
			}
			else if (input > 6) {
				doRandomFlip(input - 5);
			}

			//cout << fa<<" "<<ofToInt(fromArduino)<<" "<<int(fa) << " bytes "<< nBytesRead << " time " << nTimesRead << endl;
		}
	}
}
void ofApp::doRandomFlip(int amount){
    if(timeSinceLastFlip>randomFlipInterval){
        
        for(int i = 0; i<amount; i++){
            
            int a = floorf(ofRandom(NUM_DESTINATIONS));
            int b = floorf(ofRandom(NUM_DESTINATIONS));
            int c = floorf(ofRandom(NUM_DESTINATIONS));
            
            int aa =(int)ofRandom(wh_number[a].characters.size());
            int bb =(int)ofRandom(wh_destination[b].characters.size());
            int cc =(int)ofRandom(wh_material[c].characters.size());
            
            int ls = textures.size();
            wh_number[a].characters[aa].from=(wh_number[a].characters[aa].from+int(ofRandom(ls-10,ls))) % ls;
            wh_destination[b].characters[bb].from=(wh_destination[b].characters[bb].from+int(ofRandom(ls-10,ls))) % ls;
            wh_material[c].characters[cc].from=(wh_material[c].characters[cc].from+int(ofRandom(ls-10,ls))) % ls;
        }
    }
    // if it is constant noise, nothing happens as it is reset all the time...
    timeSinceLastFlip = 0.0;
    
        
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    doRandomFlip(5);
	int k = key - '0';
	if(k<desitnations.size())
		printBoardingPass(k);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

