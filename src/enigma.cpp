#include "enigma.hpp"

const char Enigma::alphabet[]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
const char Enigma::rotor1[]  ={'E','K','M','F','L','G','D','Q','V','Z','N','T','O','W','Y','H','X','U','S','P','A','I','B','R','C','J'};
const char Enigma::rotor2[]  ={'A','J','D','K','S','I','R','U','X','B','L','H','W','T','M','C','Q','G','Z','N','P','Y','F','V','O','E'};
const char Enigma::rotor3[]  ={'B','D','F','H','J','L','C','P','R','T','X','V','Z','N','Y','E','I','W','G','A','K','M','U','S','Q','O'};
const char Enigma::reflectorB[][2]={{'A', 'Y'}, {'B', 'R'}, {'C', 'U'}, {'D', 'H'}, {'E', 'Q'}, {'F', 'S'}, {'G', 'L'},
                              {'I', 'P'}, {'J', 'X'}, {'K', 'N'}, {'M', 'O'}, {'T', 'Z'}, {'V', 'W'}};

Enigma::Enigma(){ 
    rotor1ofset=rotor2ofset=rotor3ofset=0;

    for(int i=0;i<13;++i){
        plugboard[i][0]=plugboard[i][1]=0;
    }

    for(int i=0;i<26;++i){
        rotor1inv[rotor1[i]-65]=alphabet[i];
    }
    for(int i=0;i<26;++i){
        rotor2inv[rotor2[i]-65]=alphabet[i];
    }
    for(int i=0;i<26;++i){
        rotor3inv[rotor3[i]-65]=alphabet[i];
    }
}

void Enigma::setRotor1(char c){
    if(c>='a' && c<='z') rotor1ofset=c-97;
    else if(c>='A' && c<='Z') rotor1ofset=c-65;
}

void Enigma::setRotor2(char c){
    if(c>='a' && c<='z') rotor2ofset=c-97;
    else if(c>='A' && c<='Z') rotor2ofset=c-65;
}

void Enigma::setRotor3(char c){
    if(c>='a' && c<='z') rotor3ofset=c-97;
    else if(c>='A' && c<='Z') rotor3ofset=c-65;
}

char Enigma::getRotor1(){
    return (char)(65+rotor1ofset);
}

char Enigma::getRotor2(){
    return (char)(65+rotor2ofset);
}

char Enigma::getRotor3(){
    return (char)(65+rotor3ofset);
}

bool Enigma::putInPlugboard(char c1, char c2){
    for(int i=0;i<13;++i){
        if(plugboard[i][0]==c1 || plugboard[i][1]==c1 || plugboard[i][0]==c2 || plugboard[i][1]==c2) return false;
    }

    if(c1>='A' && c1<='Z') c1+=32;
    if(c2>='A' && c2<='Z') c2+=32;

    for(int i=0;i<13;++i){
        if(plugboard[i][0]==0){
            plugboard[i][0]=c1;
            plugboard[i][1]=c2;
            break;
        }
    }

    return true;
}

bool Enigma::removeFromPlugboard(char c1, char c2){
    if(c1>='A' && c1<='Z') c1-=32;
    if(c2>='A' && c2<='Z') c2-=32;

    for(int i=0;i<13;++i){
        if((plugboard[i][0]==c1 && plugboard[i][1]==c2) || (plugboard[i][0]==c2 && plugboard[i][1]==c1)){
            plugboard[i][0]=plugboard[i][1]=0;
            return true;
        }
    }

    return false;
}

void Enigma::clearPlugboard(){
	for(int i=0;i<13;++i){
		plugboard[i][0]=0;
		plugboard[i][1]=0;
	}
}

char** Enigma::getPlugboard(){
    char** output=new char*[13];

    for(int i=0;i<13;++i){
        output[i]=new char[2];
        output[i][0]=plugboard[i][0];
        output[i][1]=plugboard[i][1];
    }

    return output;
}

const char* Enigma::encodeString(const char* input){
    int len=0;
    while(input[len]!='\0') ++len;

    char* output=new char[len];

    for(int i=0;i<len;++i){
        output[i]=encodeChar(input[i]);
    }

    const char* output_string=output;

    return output_string;
}


char Enigma::encodeChar(char c){
    if(c>='A' && c<='Z') c+=32;

    //move rotor
    bool r2Moved=false;
    rotor3ofset=(rotor3ofset+1)%26;
    if(rotor3ofset==('W'-65)){
        rotor2ofset=(rotor2ofset+1)%26;
        r2Moved=true;
    }
    if(rotor2ofset==('F'-65) && r2Moved) rotor1ofset=(rotor1ofset+1)%26;

    //plugboard
    for(int i=0;i<13;++i){
        if(plugboard[i][0]==c){
            c=plugboard[i][1];
        }else if(plugboard[i][1]==c){
            c=plugboard[i][0];
        }
    }


    //to int
    int x=c-97;

    //rotors
    x=rotor3[(x+rotor3ofset)%26]-65-rotor3ofset;
    if(x<0) x+=26;
    x=rotor2[(x+rotor2ofset)%26]-65-rotor2ofset;
    if(x<0) x+=26;
    x=rotor1[(x+rotor1ofset)%26]-65-rotor1ofset;
    if(x<0) x+=26;

    //reflector
    for(int i=0;i<13;++i){
        if(reflectorB[i][0]==(x+65)){
            x=reflectorB[i][1]-65;
        }else if(reflectorB[i][1]==(x+65)){
            x=reflectorB[i][0]-65;
        }
    }

    //rotors
    x=rotor1inv[(x+rotor1ofset)%26]-65-rotor1ofset;
    if(x<0) x+=26;
    x=rotor2inv[(x+rotor2ofset)%26]-65-rotor2ofset;
    if(x<0) x+=26;
    x=rotor3inv[(x+rotor3ofset)%26]-65-rotor3ofset;
    if(x<0) x+=26;

    //to alpha
    c=(char)(x+97);


    //plugboard
    for(int i=0;i<13;++i){
        if(plugboard[i][0]==c){
            c=plugboard[i][1];
        }else if(plugboard[i][1]==c){
            c=plugboard[i][0];
        }
    }

    return c;
}
