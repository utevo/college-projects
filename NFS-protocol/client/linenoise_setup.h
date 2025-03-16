#ifndef LINENOISE_SETUP_H
#define LINENOISE_SETUP_H


#include "include/linenoise/linenoise/linenoise.h"
#include "string.h"
#include <string>
#include <map>

namespace linenoise_setup{

    const std::map<std::string, std::string> hint_map{
        {"get", " <remote file name> <local file name>"},
        {"get ", "<remote file name> <local file name>"},
        {"put", " <local file name> <remote file name>"},
        {"put ", "<local file name> <remote file name>"},
        {"ls", " <folder path>"},
        {"ls ", "<folder path>"},
        {"record", " <remote file> <record size> <record number>"},
        {"record ", "<remote file> <record size> <record number>"}
    };

    char *hints(const char *buf, int *color, int *bold) {
        std::string s(buf);
        auto it = hint_map.find(s);
        if(it!=hint_map.end()){
            *color = 35;
            *bold = 0;
            return (char*) it->second.c_str();
        }

        return NULL;
    }
    void completion(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'r') {
        linenoiseAddCompletion(lc,"record");
    }
}


};

void custom_linenoise_setup(unsigned int hist_size = 10){
    linenoiseSetHintsCallback(linenoise_setup::hints);
    linenoiseSetCompletionCallback(linenoise_setup::completion);
    linenoiseHistorySetMaxLen(10);
    int success = linenoiseHistoryLoad(".uNSFcommandhistory.txt");
}



#endif