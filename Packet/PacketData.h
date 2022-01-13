#ifndef PacketData_H
#define PacketData_H
#include "PacketHelper.h"
#include <string>
#include <string.h>

using namespace std;

class PlayOutChat {
private:
    int PacketID = 1;
    int playerId;     // 6 char
    char message[256];//256 maximum
public:
    PlayOutChat() {
    }
    PlayOutChat(string message,int playerId) {
        this->playerId = playerId; 
        strcpy(this->message, message.c_str());
    }
    PlayOutChat(const PlayOutChat& copy) {
        playerId = copy.playerId;
        strcpy(this->message, copy.message);
    }
    string getMessage() {
        return string(message);
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code += string(message);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayOutChat decode(char* buf) {
        PlayOutChat newData;
        int dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        int c;
        for (c = 0;index < strlen(buf);index++,c++){
            newData.message[c] = buf[index];
        }
        newData.message[c] = '\0';
        newData.playerId = dplayerId;
        return newData;

    }
    int getPlayerId(){
        return playerId;
    }
};

class PlayOutSetNickName {
private:
    int PacketID = 2;
    char nickname[64];//nick name limit
public:
    PlayOutSetNickName() {
    }
    PlayOutSetNickName(string nickname) {
        strcpy(this->nickname, nickname.c_str());
    }
    PlayOutSetNickName(const PlayOutSetNickName& copy) {
        strcpy(this->nickname, copy.nickname);
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += string(nickname);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
};


class PlayOutPlayerMove {
private:
    int PacketID = 3; // 2char
    int x;            // 6char
    int y;            // 6char
    int face;         // 1char
    int playerId;     // 6char
public:
    PlayOutPlayerMove() {
    }
    PlayOutPlayerMove(int x, int y, int face, int playerId) {
        this->x = x;
        this->y = y;
        this->face = face;
        this->playerId = playerId;
    }
    PlayOutPlayerMove(const PlayOutPlayerMove& copy) {
        this->x = copy.x;
        this->y = copy.y;
        this->face = copy.face;
        this->playerId = copy.playerId;
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::SixDigitIntEncode(x);
        code += PacketHelper::SixDigitIntEncode(y);
        code += PacketHelper::OneDigitIntEncode(face);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayOutPlayerMove decode(char* buf) {
        if (strlen(buf) != 6 + 6 + 1 + 6) return PlayOutPlayerMove();
        PlayOutPlayerMove newData;
        int dx = 0, dy = 0, dface = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 6; c++, index++) {
            dx = dx * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dy = dy * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 1; c++, index++) {
            dface = dface * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        newData.x = dx;
        newData.y = dy;
        newData.face = dface;
        newData.playerId = dplayerId;
        return newData;

    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getFace() {
        return face;
    }
    int getPlayerId() {
        return playerId;
    }
};

class PlayOutPlayerStop {
private:
    int PacketID = 4;
    int last_x;
    int last_y;
    int last_face;
    int playerId;
public:
    PlayOutPlayerStop() {
    }
    PlayOutPlayerStop(int last_x, int last_y, int last_face, int playerId) {
        this->last_x = last_x;
        this->last_y = last_y;
        this->last_face = last_face;
        this->playerId = playerId;
    }
    PlayOutPlayerStop(const PlayOutPlayerStop& copy) {
        this->last_x = copy.last_x;
        this->last_y = copy.last_y;
        this->last_face = copy.last_face;
        this->playerId = copy.playerId;
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::SixDigitIntEncode(last_x);
        code += PacketHelper::SixDigitIntEncode(last_y);
        code += PacketHelper::OneDigitIntEncode(last_face);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayOutPlayerStop decode(char* buf) {
        if (strlen(buf) != 6 + 6 + 1 + 6) return PlayOutPlayerStop();
        PlayOutPlayerStop newData;
        int dx = 0, dy = 0, dface = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 6; c++, index++) {
            dx = dx * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dy = dy * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 1; c++, index++) {
            dface = dface * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        newData.last_x = dx;
        newData.last_y = dy;
        newData.last_face = dface;
        newData.playerId = dplayerId;
        return newData;

    }
    int getX() {
        return last_x;
    }
    int getY() {
        return last_y;
    }
    int getFace() {
        return last_face;
    }
    int getPlayerId() {
        return playerId;
    }
};

class PlayInPlayerLogin {
private:
    int PacketID = 5;
    int successFlag; // 1 char
    int playerId;    // 6 chars
public:
    PlayInPlayerLogin() {
    }
    PlayInPlayerLogin(int successFlag, int playerId) {
        this->successFlag = successFlag;
        this->playerId = playerId;
    }
    PlayInPlayerLogin(const PlayInPlayerLogin& copy) {
        this->successFlag = copy.successFlag;
        this->playerId = copy.playerId;
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::OneDigitIntEncode(successFlag);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayInPlayerLogin decode(char* buf) {
        if (strlen(buf) != 1 + 6) return PlayInPlayerLogin();
        PlayInPlayerLogin newData;
        int dsuccessFlag = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 1; c++, index++) {
            dsuccessFlag = dsuccessFlag * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        newData.successFlag = dsuccessFlag;
        newData.playerId = dplayerId;
        return newData;

    }
    int isSuccess() {
        return successFlag;
    }
    int getPlayerId() {
        return playerId;
    }
};

class PlayOutPlayerSpawn{
private:
    int PacketID = 6;
    int x;           //6char
    int y;           //6char
    int face;        //1char
    int playerId;    //6char
    char playerNickname[64]; // 64char at most
public:
    PlayOutPlayerSpawn() {
    }
    PlayOutPlayerSpawn(int x, int y, int face, int playerId,std::string nickname) {
        this->x = x;
        this->y = y;
        this->face = face;
        this->playerId = playerId;
        strcpy(playerNickname,nickname.c_str());
    }
    PlayOutPlayerSpawn(const PlayOutPlayerSpawn& copy) {
        this->x = copy.x;
        this->y = copy.y;
        this->face = copy.face;
        this->playerId = copy.playerId;
        strcpy(playerNickname,copy.playerNickname);
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::SixDigitIntEncode(x);
        code += PacketHelper::SixDigitIntEncode(y);
        code += PacketHelper::OneDigitIntEncode(face);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code += string(playerNickname);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayOutPlayerSpawn decode(char* buf) {
        PlayOutPlayerSpawn newData;
        int dx = 0, dy = 0, dface = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 6; c++, index++) {
            dx = dx * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dy = dy * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 1; c++, index++) {
            dface = dface * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        int c;
        for (c = 0 ; index < strlen(buf);c++,index++){
            newData.playerNickname[c] = buf[index]; 
        }
        newData.playerNickname[c] = '\0'; //debug

        newData.x = dx;
        newData.y = dy;
        newData.face = dface;
        newData.playerId = dplayerId;
        return newData;

    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getFace() {
        return face;
    }
    int getPlayerId() {
        return playerId;
    }
    string getNickname(){
        return string(playerNickname);
    }
};

class PlayOutPlayerDespawn{
private:
    int PacketID = 7;
    int playerId;    //6char
public:
    PlayOutPlayerDespawn() {
    }
    PlayOutPlayerDespawn(int playerId) {
        this->playerId = playerId;
    }
    PlayOutPlayerDespawn(const PlayOutPlayerDespawn& copy) {
        this->playerId = copy.playerId;
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayOutPlayerDespawn decode(char* buf) {
        if (strlen(buf) != 6) return PlayOutPlayerDespawn();
        PlayOutPlayerDespawn newData;
        int dx = 0, dy = 0, dface = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        newData.playerId = dplayerId;
        return newData;

    }
    int getPlayerId() {
        return playerId;
    }
};


// this used to be client -> server and check the coord 
// after this packet 
// server will sent a client play in login success packet to make client to join the server 
// maybe during this if client is play out play out player log in 
// server will "broadcast" play out player spawn packet
// if the received play in login success packet is failed
// then broadcast play out player despawn  

// what if not receive play out player login?

class PlayOutPlayerLogin {
private:
    int PacketID = 8;
    int x;           //6char
    int y;           //6char
    int face;        //1char
    int playerId;    //6char
    char playerNickname[64];
public:
    PlayOutPlayerLogin() {
    }
    PlayOutPlayerLogin(int x, int y, int face, int playerId,std::string nickname) {
        this->x = x;
        this->y = y;
        this->face = face;
        this->playerId = playerId;
        strcpy(playerNickname,nickname.c_str());
    }
    PlayOutPlayerLogin(const PlayOutPlayerLogin& copy) {
        this->x = copy.x;
        this->y = copy.y;
        this->face = copy.face;
        this->playerId = copy.playerId;
        strcpy(playerNickname,copy.playerNickname);
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::SixDigitIntEncode(x);
        code += PacketHelper::SixDigitIntEncode(y);
        code += PacketHelper::OneDigitIntEncode(face);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code += string(playerNickname);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayOutPlayerLogin decode(char* buf) {
        PlayOutPlayerLogin newData;
        int dx = 0, dy = 0, dface = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 6; c++, index++) {
            dx = dx * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dy = dy * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 1; c++, index++) {
            dface = dface * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        int c;
        for (c = 0 ; index < strlen(buf);c++,index++){
            newData.playerNickname[c] = buf[index]; 
        }
        newData.playerNickname[c] = '\0'; //debug

        newData.x = dx;
        newData.y = dy;
        newData.face = dface;
        newData.playerId = dplayerId;
        return newData;

    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getFace() {
        return face;
    }
    int getPlayerId() {
        return playerId;
    }
    string getNickname(){
        return string(playerNickname);
    }
};



class PlayInLoginSuccess {
private:
    int PacketID = 9;
    
    int successFlag; // 1 char
    int playerId;    // 6 chars
    char playerNickname[64];// 64 chars max
public:
    PlayInLoginSuccess() {
    }
    PlayInLoginSuccess(int successFlag, int playerId,std::string nickname) {
        this->successFlag = successFlag;
        this->playerId = playerId;
        strcpy(playerNickname,nickname.c_str());
    }
    PlayInLoginSuccess(const PlayInLoginSuccess& copy) {
        this->successFlag = copy.successFlag;
        this->playerId = copy.playerId;
        strcpy(playerNickname,copy.playerNickname);
    }
    string encode() {
        string code;
        code += PacketHelper::PacketIDEncode(PacketID);
        code += PacketHelper::OneDigitIntEncode(successFlag);
        code += PacketHelper::SixDigitIntEncode(playerId);
        code += string(playerNickname);
        code = PacketHelper::PacketLengthEncode(code) + code;
        return code;
    }
    static PlayInLoginSuccess decode(char* buf) {
        PlayInLoginSuccess newData;
        int dsuccessFlag = 0, dplayerId = 0;
        int index = 0;
        for (int c = 0; c < 1; c++, index++) {
            dsuccessFlag = dsuccessFlag * 10 + buf[index] - '0';
        }
        for (int c = 0; c < 6; c++, index++) {
            dplayerId = dplayerId * 10 + buf[index] - '0';
        }
        int c;
        for (c = 0 ; index < strlen(buf);c++,index++){
            newData.playerNickname[c] = buf[index]; 
        }
        newData.playerNickname[c] = '\0'; //debug

        newData.successFlag = dsuccessFlag;
        newData.playerId = dplayerId;
        return newData;

    }
    int isSuccess() {
        return successFlag;
    }
    int getPlayerId() {
        return playerId;
    }

    string getNickname(){
        return string(playerNickname);
    }
};

#endif
