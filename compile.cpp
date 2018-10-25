#include "compile.h"

compile::compile()
{
    int i=0;
    for(i=0;i<MAX_SRC_FILE;i++)
    {
        m_codeblock[i] = "";

    }
    this->m_codeblock_count = 0;
    this->m_code = "";
    this->m_code_count = 0;

}
