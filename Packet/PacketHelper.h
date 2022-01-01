#ifndef PacketHelper_H
#define PacketHelper_H
#include <string>
using namespace std;


class PacketHelper{
    public:
        static string PacketIDEncode(int id){
            // hex encode
            char result[4];
            sprintf(result,"%02X",id);
            return string(result);
        }
        static string PacketLengthEncode(string& current){
            char result[11];
	        sprintf(result,"%010ld",current.size());
	        return string(result);
        }
};

#endif