#ifndef SYMBOLIC_CHARACTER_PROCESS_H
#define SYMBOLIC_CHARACTER_PROCESS_H
#include "mainwindow.h"
#define MAX_DEFINE 100
class symbolic_character_process
{
public:
    std::string replace_A[MAX_DEFINE];
    std::string replace_B[MAX_DEFINE];
    symbolic_character_process();
    void symbolic_space_newline_comment_jumper(std::string &str,int &count);
    void symbolic_header_preprocess(std::string &str,int &count);
    void symbolic_replace_define(std::string &str,int &count);
    std::string symbolic_str_m_replace(std::string strSrc,const std::string &oldStr, const std::string &newStr,int count);
};

#endif // SYMBOLIC_CHARACTER_PROCESS_H
