#include "symbolic_character_process.h"

extern compile g_compile_o;
symbolic_character_process::symbolic_character_process()
{

}
void symbolic_character_process::symbolic_space_newline_comment_jumper(std::string &str,int &count)
{
    int flag=0,total=0;
    std::string buffer = str;
    total = str.size();
    total = total - 16;//16 ' ' in the end
    for(;count<total;count++)
    {

        //space
        if(' ' == buffer[count])
        {
            flag = 0;
            continue;
        }
        //new line | TAB (horizontal tab)
        if( '\n' == buffer[count] || 9 == buffer[count] )
        {
            flag = 0;
            continue;
        }
        // "//" comment
        if( '/' == buffer[count] || '/' == buffer[count+1])
        {
            count = count+2;
            while((count) < total)
            {
                if('\n' == buffer[count])
                {
                    flag=1;
                    count++;
                    break;
                }
                else
                {
                    flag=0;
                    count++;
                }
            }
            break;
        }
        if( '/' == buffer[count] || '*' == buffer[count+1])
        {
            count = count+2;
            while((count) < total)
            {
                if('*' == buffer[count] && '/' == buffer[count+1])
                {
                    count = count + 2;
                    flag = 1;
                    break;
                }
                else
                {
                    count++;
                    flag=0;
                }
            }
            break;
        }
        flag = 1;
        if(flag)
            break;

    }
}
void symbolic_character_process::symbolic_header_preprocess(std::string &str,int &count)
{
    int flag=0,total=0,tmp_count=0,clear_count=0,merge=0;
    std::string buffer = str;
    std::string tmp = "";
    std::string def = "define";
    std::string inc = "include";
    std::string combinstr = "";
    total = str.size();
    total = total - 16;//16 ' ' in the end
    while(!flag||count<total)
    {
        this->symbolic_space_newline_comment_jumper(str,count);
        //qDebug()<<count<<endl;
        if('#'==buffer[count])
        {
            count++;
            this->symbolic_space_newline_comment_jumper(str,count);
            //qDebug()<<count<<endl;
            if('A'<= buffer[count] && 'Z'>=buffer[count] || 'a'<= buffer[count] && 'z'>=buffer[count] || '_'==buffer[count])
            {
                tmp.clear();
                tmp_count=0;
                tmp[tmp_count] = buffer[count];
                tmp_count++;
                count++;
                while('0'<= buffer[count] && '9'>=buffer[count] ||'A'<= buffer[count] && 'Z'>=buffer[count] || 'a'<= buffer[count] && 'z'>=buffer[count] || '_'==buffer[count])
                {
                    tmp[tmp_count] = buffer[count];
                    tmp_count++;
                    count++;
                }
                tmp[tmp_count]='\0';
                //going to include, and store current code to code block array
                if(strcmp(tmp.c_str(),inc.c_str())==0)//include found
                {
                    //qDebug()<<tmp.c_str()<<'-'<<inc.c_str()<<endl;
                    this->symbolic_space_newline_comment_jumper(str,count);
                    //qDebug()<<count<<endl;
                    if('<'==buffer[count])
                    {
                        count++;
                        this->symbolic_space_newline_comment_jumper(str,count);
                        //qDebug()<<count<<endl;
                        tmp.clear();
                        tmp_count=0;
                        while('.'== buffer[count] ||'0'<= buffer[count] && '9'>=buffer[count] ||'A'<= buffer[count] && 'Z'>=buffer[count] || 'a'<= buffer[count] && 'z'>=buffer[count] || '_'==buffer[count])
                        {
                            tmp[tmp_count] = buffer[count];
                            tmp_count++;
                            count++;
                        }
                        tmp[tmp_count]='\0';
                        this->symbolic_space_newline_comment_jumper(str,count);
                        //qDebug()<<count<<endl;
                        if('>'==buffer[count])
                        {
                               count++;
                               buffer = buffer.substr(count);
                               str = buffer; //save current code
                               g_compile_o.m_codeblock[g_compile_o.m_codeblock_count]=str;
                               g_compile_o.m_codeblock_count++;
                               //open the header file and load it into str and clear the count
                               QString filename ="src/";
                               filename.append(tmp.c_str());
                               QFile file(filename);
                               if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
                               {
                                   qDebug()<<"Can't open the file!"<<endl;
                                   return;
                               }
                               QTextStream out(&file);
                               str = out.readAll().toStdString();
                               count=0;
                               clear_count = 0;

                               file.flush();
                               file.close();

                               // append 16 ' '
                               str.append("                ");
                               buffer = str;
                               merge = 1;
                               continue;
                        }

                    }
                }
                //going to define,
                if( strcmp(tmp.c_str(),def.c_str()) == 0 )//define found
                {
                    //append
                    combinstr.append("#define ");
                    //part 1
                    //qDebug()<<tmp.c_str()<<'-'<<def.c_str()<<endl;
                    this->symbolic_space_newline_comment_jumper(str,count);
                    //qDebug()<<count<<endl;
                    tmp.clear();
                    tmp_count=0;
                    while('0'<= buffer[count] && '9'>=buffer[count] ||'A'<= buffer[count] && 'Z'>=buffer[count] || 'a'<= buffer[count] && 'z'>=buffer[count] || '_'==buffer[count])
                    {
                            tmp[tmp_count] = buffer[count];
                            tmp_count++;
                            count++;
                    }
                    tmp[tmp_count]='\0';
                    //append
                    //qDebug()<<tmp.c_str()<<endl;
                    combinstr.append(tmp.c_str());
                    //part2
                    this->symbolic_space_newline_comment_jumper(str,count);
                    //qDebug()<<count<<endl;
                    tmp.clear();
                    tmp_count=0;
                    while('0'<= buffer[count] && '9'>=buffer[count] ||'A'<= buffer[count] && 'Z'>=buffer[count] || 'a'<= buffer[count] && 'z'>=buffer[count] || '_'==buffer[count])
                    {
                            tmp[tmp_count] = buffer[count];
                            tmp_count++;
                            count++;
                    }
                    tmp[tmp_count]='\0';
                    //qDebug()<<tmp.c_str()<<endl;
                    combinstr.append(" ");
                    combinstr.append(tmp.c_str());
                    combinstr.append("\n");
                    //qDebug()<<combinstr.c_str()<<endl;

                    //clear original #define in header file
                    while(clear_count<count)
                    {
                        buffer[clear_count]=' ';
                        clear_count++;
                    }
                    continue;

                }
                else
                {
                    qDebug()<<"start with # , but not define and include"<<endl;
                    return;
                }
            }
        }
        else
        {
            count++;
        }


        if(merge)
        {
            //flag = 1;

            combinstr.append(buffer);
            if(g_compile_o.m_codeblock_count)
            {
                g_compile_o.m_codeblock_count--;
                buffer = g_compile_o.m_codeblock[g_compile_o.m_codeblock_count];
                flag = 0;
            }
            else
            {
                flag = 1;
                str= combinstr;
                count = 0;
            }
            qDebug()<<combinstr.c_str()<<endl;

        }

    }
    count=0;
}
void symbolic_character_process::symbolic_replace_define(std::string &str,int &count)
{

    std::string buffer = str;
    std::string def = "#define";
    std::string replace_A[MAX_SRC_FILE]={""};
    std::string replace_B[MAX_SRC_FILE]={""};
    std::string tmp = "";

    int pos = 0,replace_count=0,tmp_count1=0,tmp_count2=0,index=0,i=0;

    this->symbolic_space_newline_comment_jumper(str,count);
    while ((pos = buffer.find(def, pos)) != std::string::npos)  // npos是没找到，
    {
        tmp_count1 = 0;
        while('\n' != buffer[pos+tmp_count1])
        {
            tmp_count1++;
        }
        //find define A 1  -> A and 1
        std::string _buffer = buffer.substr(pos,tmp_count1);
        i=0;
        i=i+8;
        while(i<tmp_count1-pos)
        {
                tmp.clear();
                tmp_count2=0;
                while('0'<= _buffer[i] && '9'>=_buffer[i] ||'A'<= _buffer[i] && 'Z'>=_buffer[i] || 'a'<= _buffer[i] && 'z'>=_buffer[i] || '_'==_buffer[i])
                {
                        tmp[tmp_count2] = _buffer[i];
                        tmp_count2++;
                        i++;
                }
                tmp[tmp_count2]='\0';
                qDebug()<<tmp.c_str()<<endl;
                replace_A[index].insert(0,tmp.c_str());
                i++;
                tmp.clear();
                tmp_count2=0;
                while('0'<= _buffer[i] && '9'>=_buffer[i] ||'A'<= _buffer[i] && 'Z'>=_buffer[i] || 'a'<= _buffer[i] && 'z'>=_buffer[i] || '_'==_buffer[i])
                {
                        tmp[tmp_count2] = _buffer[i];
                        tmp_count2++;
                        i++;
                }
                tmp[tmp_count2]='\0';
                qDebug()<<tmp.c_str()<<endl;
                replace_B[index].insert(0,tmp.c_str());
                qDebug()<<replace_A[index].c_str()<<"-"<<replace_B[index].c_str()<<endl;
                index++;

        }
        buffer = buffer.substr(pos+tmp_count1);
        pos=0;
    }
    //replace the define from A to 1
    while(index--)
    {
        buffer = this->symbolic_str_m_replace(buffer,replace_A[index],replace_B[index],-1);
        qDebug()<<buffer.c_str()<<endl;
    }

    //last return
    str = buffer;
    count = 0;
}


std::string symbolic_character_process::symbolic_str_m_replace(std::string strSrc,const std::string &oldStr, const std::string &newStr,int count)
{
    std::string strRet=strSrc;
    size_t pos = 0;
    int l_count=0;
    if(-1 == count) // replace all
        count = strRet.size();
    while ((pos = strRet.find(oldStr, pos)) != std::string::npos)
    {
        strRet.replace(pos, oldStr.size(), newStr);
        if(++l_count >= count) break;
        pos += newStr.size();
    }
    return strRet;
}
