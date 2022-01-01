#ifndef PacketData_H
#define PacketData_H
#include "PacketHelper.h"
#include <string>
using namespace std;

class PlayOutChat{
    private:
        int PacketID = 1;
        char message[323];//256 + 64[client name limit] + 3 [word '[',']',' ' ]
    public:
        PlayOutChat(){
        }
        PlayOutChat(string message){
            strcpy(this->message,message.c_str());
        }
        PlayOutChat(const PlayOutChat & copy){
            strcpy(this->message,copy.message);
        }
        string getMessage(){
            return string(message);
        }
        string encode(){
            string code;
            code+= PacketHelper::PacketIDEncode(PacketID);
            code+= string(message);
            code = PacketHelper::PacketLengthEncode(code)+code;
            return code;
        }
};

class PlayOutSetNickName{
    private:
        int PacketID = 2;
        char nickname[64];//256 + 64[client name limit]
    public:
        PlayOutSetNickName(){
        }
        PlayOutSetNickName(string nickname){
            strcpy(this->nickname,nickname.c_str());
        }
        PlayOutSetNickName(const PlayOutSetNickName & copy){
            strcpy(this->nickname,copy.nickname);
        }
        string encode(){
            string code;
            code+= PacketHelper::PacketIDEncode(PacketID);
            code+= string(nickname);
            code = PacketHelper::PacketLengthEncode(code)+code;
            return code;
        }
};

#endif