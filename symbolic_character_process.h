#ifndef SYMBOLIC_CHARACTER_PROCESS_H
#define SYMBOLIC_CHARACTER_PROCESS_H
#include<QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "compile.h"

class symbolic_character_process
{
public:
    symbolic_character_process();
    void symbolic_space_newline_comment_jumper(QString &str,int &count);
    void symbolic_header_preprocess(QString &str,int &count);
    void symbolic_replace_define(QString &str,int &count);
    std::string symbolic_str_m_replace(std::string strSrc,const std::string &oldStr, const std::string &newStr,int count);
};

#endif // SYMBOLIC_CHARACTER_PROCESS_H