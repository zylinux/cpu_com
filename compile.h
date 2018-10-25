#ifndef COMPILE_H
#define COMPILE_H
#include<QString>
#include "preprocess.h"
#define MAX_SRC_FILE 256
class compile
{
public:
    //main code section
    QString m_code;
    int m_code_count;
    //when need to glue code together in preprocess
    QString m_codeblock[MAX_SRC_FILE];
    int m_codeblock_count;
    //
    preprocess m_prep;
    compile();

};

#endif // COMPILE_H
