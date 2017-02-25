#ifndef ENIGMA_H_INCLUDED
#define ENIGMA_H_INCLUDED

class Enigma{
    const static char alphabet[];
    const static char rotor1[];
    const static char rotor2[];
    const static char rotor3[];
    const static char reflectorB[][2];

    char rotor1inv[26], rotor2inv[26], rotor3inv[26];

    int rotor1ofset, rotor2ofset, rotor3ofset;
    char plugboard[13][2];

public:
    Enigma();
    void setRotor1(char c);
    void setRotor2(char c);
    void setRotor3(char c);
    char getRotor1();
    char getRotor2();
    char getRotor3();
    bool putInPlugboard(char c1, char c2);
    bool removeFromPlugboard(char c1, char c2);
    void clearPlugboard();
    char** getPlugboard();
    char encodeChar(char c);
    const char* encodeString(const char* str);
};

#endif 
