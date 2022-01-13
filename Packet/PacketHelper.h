#ifndef PacketHelper_H
#define PacketHelper_H
#include <string>
using namespace std;


class PacketHelper {
public:
    static string PacketIDEncode(int id) {
        // hex encode
        char result[4];
        sprintf(result, "%02X", id);
        return string(result);
    }
    static string PacketLengthEncode(string& current) {
        char result[11];
        sprintf(result, "%010ld", current.size());
        return string(result);
    }
    static string SixDigitIntEncode(int i) {
        char result[7];
        sprintf(result , "%06d", i);
        return string(result);
    }
    static string TenDigitIntEncode(unsigned long int i) {
        char result[11];
        sprintf(result , "%010lu", i);
        return string(result);
    }
    static string OneDigitIntEncode(int i) {
        char result[2];
        sprintf(result, "%1d", i);
        return string(result);
    }
};

#endif
