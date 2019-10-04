#include "compile.h"
extern cpu_instructions g_instruction_o;
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

    this->m_total_local_valuables = 0;
    for(i=0;i<MAX_LOCAL_VALUABLES;i++)
    {
        this->m_local_valuable_type[i]="";
        this->m_local_valuable_name[i]="";
        this->m_local_valuable_address[i]=0;
    }
    this->m_RAM_allocate_address = 25;
    //global valuables
    this->m_total_global_valuables=0;
    this->m_global_end_address=0;
    for(i=0;i<MAX_LOCAL_VALUABLES;i++)
    {
        this->m_global_valuable_type[i]="";
        this->m_global_valuable_name[i]="";
        this->m_global_valuable_address[i]=0;
    }

    //ROM tmp cache
    this->m_ROM_Cache.clear();
    this->m_ROM_Cahce_c = 0;
    //ROM page
    this->m_ROM_PAGE_use = 0;
    for(i=0;i<MAX_PAGE;i++)
    {
        this->m_ROM_PAGE[i].clear();
        this->m_ROM_PAGE_byte_use_c[i] = 0;
    }
    //RAM page switch control
    this->m_ROM_PAGE_init_f=0;

    //Main address
    this->m_MAIN_entry_in_ROM=0;

    //functions
    this->m_total_functions = 0;
    for(i=0;i<MAX_PAGE;i++)
    {
        this->m_func_name[i]="";
        this->m_func_type[i]="";
        this->m_func_entry_address[i]=0;
        this->m_func_return_address[i]=0;
        this->m_func_share[i]="";
        this->m_func_RAM_end_address[i]=0;
        this->m_func_paramter_base_address[i]=0;
        this->m_func_paramter_offset_address[i]=0;
        this->m_func_paramter_name[i]="";
        this->m_func_paramter_type[i]="";
        this->m_func_paramter_address[i]=0;
    }
}


int compile::symbolic_space_newline_comment_jumper(std::string &str, int &count)
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
    return buffer[count];
}

void compile::init_RAM_with_value(unsigned int RAM_allocate_address,unsigned int value)
{
    unsigned char byte_data=0;
    if( ((this->m_ROM_Cahce_c+7)>(SIZE_PER_PAGE-74) ) && this->m_ROM_PAGE_init_f )//program too much over 512bytes need to switch to second one
    {
        //page_jump(SIZE_PER_PAGE*(this->m_ROM_PAGE_use+1));
        this->m_ROM_PAGE[this->m_ROM_PAGE_use] = this->m_ROM_Cache;
        this->m_ROM_PAGE_byte_use_c[this->m_ROM_PAGE_use] = this->m_ROM_Cahce_c;
        this->m_ROM_Cache.clear();
        this->m_ROM_Cahce_c = 0;
        this->m_ROM_PAGE_use++;
    }
    byte_data = (unsigned char)value;
    g_instruction_o.I_A_set(byte_data);
    byte_data = (unsigned char)(RAM_allocate_address&0x000000ff);
    g_instruction_o.I_A0_set(byte_data);
    byte_data = (unsigned char)((RAM_allocate_address>>8)&0x00ffff00);
    g_instruction_o.I_A1_set(byte_data);
    byte_data = (unsigned char)((RAM_allocate_address>>16)&0x00ff0000);
    g_instruction_o.I_A2_set(byte_data);
    //need a nop
    g_instruction_o.I_NOP();
    g_instruction_o.I_A_TO_RAM();


}

int compile::get_number_value()
{
    std::string str_tmp="";
    int ret = 0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;

    if('0'== this->m_code[this->m_code_count] && 'x' == this->m_code[this->m_code_count+1] || 'X'== this->m_code[this->m_code_count+1])
    {
            pos_s=this->m_code_count;
            pos_e=1;
            this->m_code_count = this->m_code_count+2;
            while('0'<= this->m_code[this->m_code_count] && '9'>=this->m_code[this->m_code_count]
                  || 'a'<= this->m_code[this->m_code_count] && 'f'>=this->m_code[this->m_code_count]
                  || 'A'<= this->m_code[this->m_code_count] && 'F'>=this->m_code[this->m_code_count])
            {
                pos_e++;
                this->m_code_count++;
            }
            str_tmp = this->m_code.substr(pos_s,pos_e);//get valuable name
            ret = std::stoul(str_tmp, nullptr, 16);
    }
    if('0' <= this->m_code[this->m_code_count] && '9' >= this->m_code[this->m_code_count])
    {
            pos_s=this->m_code_count;
            pos_e=1;
            this->m_code_count++;
            while('0'<= this->m_code[this->m_code_count] && '9'>=this->m_code[this->m_code_count])
            {
                pos_e++;
                this->m_code_count++;
            }
            str_tmp = this->m_code.substr(pos_s,pos_e);//get valuable name
            ret = std::stoul(str_tmp, nullptr, 10);
    }
    return ret;
}
unsigned char compile::get_valuable(std::string str)
{
    //unsigned char
    //unsigned int
    //unsigned long int
    //char
    //int
    //long int
    //it should return ';'
    std::string str_tmp="";
    unsigned char ret = 0;
    std::size_t length = this->m_code.length();
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    int value=0;
    int flag=0; //if ; , ends with 1, if , keep doing
    //steps
    //get this->m_local_valuable_type[i]="";

    //this->m_local_valuable_name[this->m_total_local_valuables]="";
    while(!flag)
    {
        this->m_local_valuable_type[this->m_total_local_valuables] = str;

        if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
        {
            qDebug()<<"file ends"<<endl;
            return ' ';
        }
        //if it is * or **, add it into type;
        //if *
        if('*' == this->m_code[this->m_code_count])
        {
            this->m_local_valuable_type[this->m_total_local_valuables]=this->m_local_valuable_type[this->m_total_local_valuables]+"*";
            this->m_code_count++;
        }
        //if **
        if('*' == this->m_code[this->m_code_count])
        {
            this->m_local_valuable_type[this->m_total_local_valuables]=this->m_local_valuable_type[this->m_total_local_valuables]+"*";
            this->m_code_count++;
        }
        if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
        {
            qDebug()<<"file ends"<<endl;
            return ' ';
        }

        if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
        {
                pos_s=this->m_code_count;
                pos_e=1;
                this->m_code_count++;
                while('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                this->m_local_valuable_name[this->m_total_local_valuables] = this->m_code.substr(pos_s,pos_e);//get valuable name
        }
        if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
        {
            qDebug()<<"file ends"<<endl;
            return ' ';
        }
        if('('== this->m_code[this->m_code_count])//we found a fuction , need to return 1, tell up level
        {
            ret = 1;
            return ret;
        }
        //if there is no inital value, and if ',' or ';', just give ram like a=10;
        if(','== this->m_code[this->m_code_count]||';'== this->m_code[this->m_code_count])
        {
            this->m_local_valuable_address[this->m_total_local_valuables] = this->m_RAM_allocate_address;
            if("char"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned char"==this->m_local_valuable_type[this->m_total_local_valuables])
            {
                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
            }
            else if("long int"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned long int"==this->m_local_valuable_type[this->m_total_local_valuables])
            {
                this->m_RAM_allocate_address = this->m_RAM_allocate_address+4;
            }
            else
            {
                this->m_RAM_allocate_address = this->m_RAM_allocate_address+2;//for int and unsigned int
            }
            this->m_total_local_valuables++;
            if(';'== this->m_code[this->m_code_count])
            {
                flag = 1;
                ret = this->m_code[this->m_code_count];
            }
            else//means , need keep doing scan in this funciton
            {
                flag = 0;
                this->m_code_count++;
                //this->m_local_valuable_type[this->m_total_local_valuables] = str;
            }
        }
        else if('='== this->m_code[this->m_code_count])//assign value
        {
            this->m_code_count++;
            if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
            {
                qDebug()<<"file ends"<<endl;
                return ' ';
            }
            if('-'== this->m_code[this->m_code_count])
            {
                //get value
                if('0' <= this->m_code[this->m_code_count] && '9' >=this->m_code[this->m_code_count] || 39 == this->m_code[this->m_code_count])//39 is '
                {
                        //get value
                        value = ~this->get_number_value()+1;
                        if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
                        {
                            qDebug()<<"file ends"<<endl;
                            return ' ';
                        }

                        if(',' == this->m_code[this->m_code_count] || ';' == this->m_code[this->m_code_count])
                        {

                            this->m_local_valuable_address[this->m_total_local_valuables] = this->m_RAM_allocate_address;
                            if("char"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned char"==this->m_local_valuable_type[this->m_total_local_valuables])
                            {
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                            }
                            else if("long int"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned long int"==this->m_local_valuable_type[this->m_total_local_valuables])
                            {
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;//long int 4 bytes
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                            }
                            else
                            {
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;//int 2 bytes
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                            }
                            this->m_total_local_valuables++;
                            if(';'== this->m_code[this->m_code_count])
                            {
                                flag = 1;
                                ret = this->m_code[this->m_code_count];
                            }
                            else//means , need keep doing scan in this funciton
                            {
                                flag = 0;
                                //this->m_local_valuable_type[this->m_total_local_valuables] = str;
                            }
                            this->m_code_count++;
                        }

                 }
            }
            else if ('0' <= this->m_code[this->m_code_count] && '9' >=this->m_code[this->m_code_count] || 39 == this->m_code[this->m_code_count])//39 is '
            {
                        //get value
                        value = this->get_number_value();
                        if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
                        {
                            qDebug()<<"file ends"<<endl;
                            return ' ';
                        }

                        if(',' == this->m_code[this->m_code_count] || ';' == this->m_code[this->m_code_count])
                        {
                            this->m_local_valuable_address[this->m_total_local_valuables] = this->m_RAM_allocate_address;
                            if("char"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned char"==this->m_local_valuable_type[this->m_total_local_valuables])
                            {
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                            }
                            else if("long int"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned long int"==this->m_local_valuable_type[this->m_total_local_valuables])
                            {
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;//long int 4 bytes
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                            }
                            else
                            {
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;//int 2 bytes
                                this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                            }
                            this->m_total_local_valuables++;
                            if(';'== this->m_code[this->m_code_count])
                            {
                                flag = 1;
                                ret = this->m_code[this->m_code_count];
                            }
                            else//means , need keep doing scan in this funciton
                            {
                                flag = 0;
                                //this->m_local_valuable_type[this->m_total_local_valuables] = str;
                            }
                            this->m_code_count++;
                        }

            }

        }
    }

    //if it is array, more more complicated

    //check if ram is enough

    //
    return ret;
}
unsigned char compile::scan_valuable()
{
    std::string _buffer = this->m_code;
    std::size_t length = this->m_code.length();
    std::string buffer_tmp="";
    unsigned int pos_save=0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    unsigned char flag=0;//0 means keep doing, flag 1 means finish scaning, flag 2 means arrive code end
    //1 get string int char .....

    while(!flag&&this->m_code_count<length)
    {
        //space newline comment jumper
        if(this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count)==' ')
        {
            flag=2;//this should not be a white space, if it is means that no code need to deal with anymore
            qDebug()<<"file ends"<<flag<<endl;
            continue;
        }
        pos_save = this->m_code_count;//save it
        if('A'<= _buffer[this->m_code_count] && 'Z'>=_buffer[this->m_code_count] || 'a'<= _buffer[this->m_code_count] && 'z'>=_buffer[this->m_code_count] || '_'==_buffer[this->m_code_count])
        {
            pos_s=this->m_code_count;
            pos_e=1;
            this->m_code_count++;
            while('A'<= _buffer[this->m_code_count] && 'Z'>=_buffer[this->m_code_count] || 'a'<= _buffer[this->m_code_count] && 'z'>=_buffer[this->m_code_count] || '_'==_buffer[this->m_code_count])
            {
                pos_e++;
                this->m_code_count++;
            }
            buffer_tmp = _buffer.substr(pos_s,pos_e);//get signed or unsigned
            if(buffer_tmp == "unsigned")//it will have signed char, signed int, signed long int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                //space newline comment jumper
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                pos_s=this->m_code_count;
                pos_e=1;
                while('A'<= _buffer[this->m_code_count] && 'Z'>=_buffer[this->m_code_count] || 'a'<= _buffer[this->m_code_count] && 'z'>=_buffer[this->m_code_count] || '_'==_buffer[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                buffer_tmp = _buffer.substr(pos_s,pos_e);//get char/int/int long
                if(buffer_tmp == "char")
                {
                    buffer_tmp="unsigned char";
                    qDebug()<<buffer_tmp.c_str()<<endl;
                    if(';'== this->get_valuable(buffer_tmp))
                    {
                        continue;
                    }
                    else
                    {
                        //function
                        this->m_code_count = pos_save;
                        return 1;
                    }
                }
                else if(buffer_tmp == "int")
                {
                    buffer_tmp="unsigned int";
                    qDebug()<<buffer_tmp.c_str()<<endl;
                    if(';'== this->get_valuable(buffer_tmp))
                    {
                        continue;
                    }
                    else
                    {
                        //function
                        this->m_code_count = pos_save;
                        return 1;
                    }
                }
                else if(buffer_tmp == "long int")
                {
                    buffer_tmp="unsigned long int";
                    qDebug()<<buffer_tmp.c_str()<<endl;
                    if(';'== this->get_valuable(buffer_tmp))
                    {
                        continue;
                    }
                    else
                    {
                        //function
                        this->m_code_count = pos_save;
                        return 1;
                    }
                }
                else
                {
                    qDebug()<<"No type"<<buffer_tmp.c_str()<<endl;
                }
                qDebug()<<buffer_tmp.c_str()<<endl;
            }
            else if (buffer_tmp == "signed")//it will have signed char, signed int, signed long int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                //space newline comment jumper
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                pos_s=this->m_code_count;
                pos_e=1;
                while('A'<= _buffer[this->m_code_count] && 'Z'>=_buffer[this->m_code_count] || 'a'<= _buffer[this->m_code_count] && 'z'>=_buffer[this->m_code_count] || '_'==_buffer[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                buffer_tmp = _buffer.substr(pos_s,pos_e);//get char/int/int long
                if(buffer_tmp == "char")
                {
                    qDebug()<<buffer_tmp.c_str()<<endl;
                    if(';'== this->get_valuable(buffer_tmp))
                    {
                        continue;
                    }
                    else
                    {
                        //function
                        this->m_code_count = pos_save;
                        return 1;
                    }
                }
                else if(buffer_tmp == "int")
                {
                    qDebug()<<buffer_tmp.c_str()<<endl;
                    if(';'== this->get_valuable(buffer_tmp))
                    {
                        continue;
                    }
                    else
                    {
                        //function
                        this->m_code_count = pos_save;
                        return 1;
                    }
                }
                else if(buffer_tmp == "long int")
                {
                    qDebug()<<buffer_tmp.c_str()<<endl;
                    if(';'== this->get_valuable(buffer_tmp))
                    {
                        continue;
                    }
                    else
                    {
                        //function
                        this->m_code_count = pos_save;
                        return 1;
                    }

                }
                else
                {
                    qDebug()<<"No type"<<buffer_tmp.c_str()<<endl;
                }
            }
            else if (buffer_tmp == "char")//int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                if(';'== this->get_valuable(buffer_tmp))
                {
                    continue;
                }
                else
                {
                    //function
                    this->m_code_count = pos_save;
                    return 1;
                }
            }
            else if (buffer_tmp == "int")//int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                if(';'== this->get_valuable(buffer_tmp))
                {
                    continue;
                }
                else
                {
                    //function
                    this->m_code_count = pos_save;
                    return 1;
                }
            }
            else if (buffer_tmp == "long") //long int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                if(';'== this->get_valuable(buffer_tmp))
                {
                    continue;
                }
                else
                {
                    //function
                    this->m_code_count = pos_save;
                    return 1;
                }
            }
            else
            {
                flag = 1 ;//scan finish
                qDebug()<<"scan ends"<<flag<<endl;
                continue;
            }
        }
        else if('@'==_buffer[this->m_code_count])//special registers
        {
            qDebug()<<"@"<<endl;
        }
        else
        {
            flag = 1 ;//scan finish
            qDebug()<<"scan ends"<<flag<<endl;
            continue;
        }

    }

    //2 is unsigned

    //3 is signed

    //4 char

    //5 int

    //6 long

    //7 constants conflict with constants

    //8 constants conflict with keywords

    //9 local valuables conflict with valuables

    //10 local valuables conflict with constants

    //11 local valuables conflict with keywords

    //12 global valuables conflict with constants

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//scan funcitons
unsigned char compile::scan_funciton()
{
    unsigned int ret=0;
    std::string status = "";
    this->m_MAIN_entry_in_ROM = this->m_ROM_PAGE_use * SIZE_PER_PAGE + this->m_ROM_Cahce_c;
    //page_jump(0);
    this->m_ROM_PAGE[this->m_ROM_PAGE_use] = this->m_ROM_Cache;
    this->m_ROM_PAGE_byte_use_c[this->m_ROM_PAGE_use]=this->m_ROM_Cahce_c;

    this->m_total_global_valuables = this->m_total_local_valuables;
    memcpy(this->m_global_valuable_type, this->m_local_valuable_type, sizeof(this->m_local_valuable_type));
    memcpy(this->m_global_valuable_name, this->m_local_valuable_name, sizeof(this->m_local_valuable_name));
    memcpy(this->m_global_valuable_address, this->m_local_valuable_address, sizeof(this->m_local_valuable_address));
    this->m_global_end_address = this->m_RAM_allocate_address;
    this->m_ROM_PAGE_init_f = 0;
    //functionbaseaddress[0]=0;
    //functiontotalnumber=0;
    do
    {
        this->m_RAM_allocate_address = this->m_global_end_address;
        ret = this->getRAMaddress4function();////1 function not define,2 means ram get address, ok 3 {} mismatch 4 error , we need 2
        if(2==ret)//success
        {
            status = this->scan_function_definition();
            if(")"==status)
            {
               qDebug()<<"we are compiling function - "<<endl;
            }
        }


    } while('}'==this->m_code[this->m_code_count]);
}

unsigned int compile::getRAMaddress4function()
{
    unsigned int brackets_c = 0;
    std::string func_name="";
    unsigned int pos_save=0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    unsigned int func_query_c=0;
    unsigned int ret=0;//1 function not define,2 means ram get address, ok 3 {} mismatch 4 error
    unsigned int flag = 0;
    pos_save = this->m_code_count;
    while('{'!=this->m_code[this->m_code_count] && this->m_code_count< (this->m_code.size()-16))
    {
        this->m_code_count++;
    }

    if('{'==this->m_code[this->m_code_count])
    {
        brackets_c=1;
        this->m_code_count++;
        do{
            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
            if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
            {
                func_name="";
                pos_s=this->m_code_count;
                pos_e=1;
                this->m_code_count++;
                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                func_name = this->m_code.substr(pos_s,pos_e);//get function name
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                if('('==this->m_code[this->m_code_count])
                {
                    this->m_code_count++;
                    if("if" != func_name && "while" != func_name && "for" != func_name && "sizeof" != func_name && "return" != func_name)
                    {
                        func_query_c = 0;
                        while(func_query_c < this->m_total_functions && func_name != this->m_func_name[func_query_c])
                        {
                            func_query_c++;
                        }
                        if(func_query_c < this->m_total_functions)
                        {
                            if(this->m_RAM_allocate_address < this->m_func_RAM_end_address[func_query_c])
                            {
                                this->m_RAM_allocate_address = this->m_func_RAM_end_address[func_query_c];
                            }
                        }
                        else
                        {
                            flag = 1;
                            qDebug()<<"function not define -"<<func_name.c_str()<<endl;
                            continue;
                        }

                    }
                }
                if('{'==this->m_code[this->m_code_count])
                {
                    brackets_c++;
                    this->m_code_count++;
                }
                if('}'==this->m_code[this->m_code_count])
                {
                    brackets_c--;
                    if(0==brackets_c)
                    {
                        this->m_code_count = pos_save;
                        flag = 2;//get ram address over for functions
                        continue;
                    }
                    else
                    {
                        this->m_code_count++;
                    }
                }
                if(' '==this->m_code[this->m_code_count])
                {
                       flag = 3;//{} mismatch
                       continue;
                }
                else
                {
                    this->m_code_count++;
                }

            }
            else
            {
                if('{'==this->m_code[this->m_code_count])
                {
                    brackets_c++;
                    this->m_code_count++;
                }
                if('}'==this->m_code[this->m_code_count])
                {
                    brackets_c--;
                    if(0==brackets_c)
                    {
                        this->m_code_count = pos_save;
                        flag = 2;//get ram address over for functions
                        continue;
                    }
                    else
                    {
                        this->m_code_count++;
                    }
                }
                if(' '==this->m_code[this->m_code_count])
                {
                       flag = 3;//{} mismatch
                       continue;
                }
                else
                {
                    this->m_code_count++;
                }
            }
        }while(!flag);

    }
    else
    {
        flag = 4;//function scan error
    }
    ret = flag;
    return flag;

}



std::string compile::scan_function_definition()
{
    unsigned int count = 0;
    unsigned int tmp = 0;
    std::string tmp_name="";
    std::string status="";
    unsigned int pos_save=0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
    {
        count = this->m_code_count;
        //get the function type
        tmp_name="";
        pos_s=this->m_code_count;
        pos_e=1;
        this->m_code_count++;
        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
        {
            pos_e++;
            this->m_code_count++;
        }
        tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
        //go to
        if("unsigned"==tmp_name)
        {
            tmp=1;
        }
        else if ("signed"==tmp_name)
        {
            tmp=2;
        }
        else if("char"==tmp_name)
        {
            tmp=3;
        }
        else if("int"==tmp_name)
        {
            tmp=4;
        }
        else if("long"==tmp_name)
        {
            tmp=5;
        }
        else if("void"==tmp_name)
        {
            tmp=6;
        }
        else
        {
            tmp=7;
        }

        switch (tmp) {
        case 1:
            {
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                //
                if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    //get the long int stuff
                    tmp_name="";
                    pos_s=this->m_code_count;
                    pos_e=1;
                    this->m_code_count++;
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                    {
                        pos_e++;
                        this->m_code_count++;
                    }
                    tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
                    if("char"==tmp_name)
                    {
                        status = this->scan_function_name_paramters("unsigned char");
                    }
                    else
                    {
                        status = "define error after signed";
                    }
                    if("int"==tmp_name)
                    {
                        status = this->scan_function_name_paramters("unsigned int");
                    }
                    else
                    {
                        status = "define error after signed";
                    }
                    if("long"==tmp_name)
                    {
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        //
                        if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                        {
                            //get the long int stuff
                            tmp_name="";
                            pos_s=this->m_code_count;
                            pos_e=1;
                            this->m_code_count++;
                            while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                            {
                                pos_e++;
                                this->m_code_count++;
                            }
                            tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
                            if("int"==tmp_name)
                            {
                                status = this->scan_function_name_paramters("unsigned long int");
                            }
                            else
                            {
                                status = "define error after unsigned long";
                            }
                        }
                        else
                        {
                            status = "define error after long";
                        }
                    }
                    else
                    {
                        status = "define error after signed";
                    }
                }
                else
                {
                    status = "define error after signed";
                }
            }
            break;
        case 2:
            {
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                //
                if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    //get the long int stuff
                    tmp_name="";
                    pos_s=this->m_code_count;
                    pos_e=1;
                    this->m_code_count++;
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                    {
                        pos_e++;
                        this->m_code_count++;
                    }
                    tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
                    if("char"==tmp_name)
                    {
                        status = this->scan_function_name_paramters("signed char");
                    }
                    else
                    {
                        status = "define error after signed";
                    }
                    if("int"==tmp_name)
                    {
                        status = this->scan_function_name_paramters("signed int");
                    }
                    else
                    {
                        status = "define error after signed";
                    }
                    if("long"==tmp_name)
                    {
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        //
                        if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                        {
                            //get the long int stuff
                            tmp_name="";
                            pos_s=this->m_code_count;
                            pos_e=1;
                            this->m_code_count++;
                            while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                            {
                                pos_e++;
                                this->m_code_count++;
                            }
                            tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
                            if("int"==tmp_name)
                            {
                                status = this->scan_function_name_paramters("signed long int");
                            }
                            else
                            {
                                status = "define error after signed long";
                            }
                        }
                        else
                        {
                            status = "define error after signed long";
                        }
                    }
                    else
                    {
                        status = "define error after signed";
                    }
                }
                else
                {
                    status = "define error after signed";
                }
            }
            break;
        case 3:
                {
                    status = this->scan_function_name_paramters(tmp_name);
                }
            break;
        case 4:
                {
                    status = this->scan_function_name_paramters(tmp_name);
                }
            break;
        case 5:
                {
                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                    //
                    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                    {
                        //get the long int stuff
                        tmp_name="";
                        pos_s=this->m_code_count;
                        pos_e=1;
                        this->m_code_count++;
                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                        {
                            pos_e++;
                            this->m_code_count++;
                        }
                        tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
                        if("int"==tmp_name)
                        {
                            status = this->scan_function_name_paramters("long int");
                        }
                        else
                        {
                            status = "define error after long";
                        }
                    }
                    else
                    {
                        status = "define error after long";
                    }
                }

            break;
        case 6:
                {
                    status = this->scan_function_name_paramters(tmp_name);
                }
            break;
        default:
                {
                    this->m_code_count = count;
                    status = this->scan_function_name_paramters("int");
                }
            break;
        }

    }
    else
    {
        status = "function return type error";
    }
    return status;
}

std::string compile::scan_function_name_paramters(std::string str)
{
    std::string status="";
    std::string tmp_name="";
    unsigned int page=0;
    unsigned int count = 0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
    this->m_func_type[this->m_total_functions] = str;
    if('*'== this->m_code[this->m_code_count])
    {
        this->m_func_type[this->m_total_functions] = this->m_func_type[this->m_total_functions] + "*";
        this->m_code_count++;
    }
    if('*'== this->m_code[this->m_code_count])
    {
        this->m_func_type[this->m_total_functions] = this->m_func_type[this->m_total_functions] + "*";
        this->m_code_count++;
    }
    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
    this->m_func_name[this->m_total_functions] = "";
    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
    {
            //get function name
            tmp_name="";
            pos_s=this->m_code_count;
            pos_e=1;
            this->m_code_count++;
            while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
            {
                pos_e++;
                this->m_code_count++;
            }
            tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
            this->m_func_name[this->m_total_functions] = tmp_name;
            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
            if('('<= this->m_code[this->m_code_count])
            {
                this->m_func_paramter_base_address[this->m_total_functions]=this->m_func_paramter_offset_address[this->m_total_functions];
                this->m_code_count++;
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    count = this->m_code_count;
                    tmp_name="";
                    pos_s=this->m_code_count;
                    pos_e=1;
                    this->m_code_count++;
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                    {
                        pos_e++;
                        this->m_code_count++;
                    }
                    tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                    if("void"==tmp_name)
                    {
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        this->m_code_count++;
                        if(')'==this->m_code[this->m_code_count])
                        {
                            this->m_code_count++;
                            status = ")";
                        }
                        else
                        {
                            status = "function void paramter error before )";
                        }
                    }
                    else
                    {
                        this->m_code_count = count;
                        do{
                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                            if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                            {
                                count = this->m_code_count;
                                tmp_name="";
                                pos_s=this->m_code_count;
                                pos_e=1;
                                this->m_code_count++;
                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                {
                                    pos_e++;
                                    this->m_code_count++;
                                }
                                tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                if("unsigned"==tmp_name)
                                {
                                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                    {
                                        tmp_name="";
                                        pos_s=this->m_code_count;
                                        pos_e=1;
                                        this->m_code_count++;
                                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                        {
                                            pos_e++;
                                            this->m_code_count++;
                                        }
                                        tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                        if("char"==tmp_name)
                                        {
                                                status = add_function_paramters("unsigned char");
                                                continue;
                                        }
                                        if("int"==tmp_name)
                                        {
                                                status = add_function_paramters("unsigned int");
                                                continue;
                                        }
                                        if("long"==tmp_name)
                                        {
                                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                            if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                            {
                                                tmp_name="";
                                                pos_s=this->m_code_count;
                                                pos_e=1;
                                                this->m_code_count++;
                                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                                {
                                                    pos_e++;
                                                    this->m_code_count++;
                                                }
                                                tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                                if("int"==tmp_name)
                                                {
                                                        status = add_function_paramters("unsigned long int");
                                                        continue;
                                                }
                                                else
                                                {
                                                        status = "function paramter error on unsigned long int";
                                                }
                                            }
                                            else
                                            {
                                                status = "function paramter error on unsigned long int";
                                            }
                                        }
                                        else
                                        {
                                                status = "function paramter error on unsigned ";
                                        }
                                    }
                                    else
                                    {
                                        status = "function paramter error on unsigned ";
                                    }

                                }
                                if("signed"==tmp_name)
                                {
                                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                    {
                                        tmp_name="";
                                        pos_s=this->m_code_count;
                                        pos_e=1;
                                        this->m_code_count++;
                                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                        {
                                            pos_e++;
                                            this->m_code_count++;
                                        }
                                        tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                        if("char"==tmp_name)
                                        {
                                                status = add_function_paramters("char");
                                                continue;
                                        }
                                        if("int"==tmp_name)
                                        {
                                                status = add_function_paramters("int");
                                                continue;
                                        }
                                        if("long"==tmp_name)
                                        {
                                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                            if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                            {
                                                tmp_name="";
                                                pos_s=this->m_code_count;
                                                pos_e=1;
                                                this->m_code_count++;
                                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                                {
                                                    pos_e++;
                                                    this->m_code_count++;
                                                }
                                                tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                                if("int"==tmp_name)
                                                {
                                                        status = add_function_paramters("long int");
                                                        continue;
                                                }
                                                else
                                                {
                                                        status = "function paramter error on signed long int";
                                                }
                                            }
                                            else
                                            {
                                                status = "function paramter error on signed long int";
                                            }
                                        }
                                        else
                                        {
                                                status = "function paramter error on signed ";
                                        }
                                    }
                                    else
                                    {
                                        status = "function paramter error on signed ";
                                    }
                                }
                                if("char"==tmp_name)
                                {
                                    status = add_function_paramters("char");
                                    continue;
                                }
                                if("int"==tmp_name)
                                {
                                    status = add_function_paramters("int");
                                    continue;
                                }
                                if("long"==tmp_name)
                                {
                                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                    {
                                        tmp_name="";
                                        pos_s=this->m_code_count;
                                        pos_e=1;
                                        this->m_code_count++;
                                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] && 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                        {
                                            pos_e++;
                                            this->m_code_count++;
                                        }
                                        tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                        if("int"==tmp_name)
                                        {
                                                status = add_function_paramters("long int");
                                                continue;
                                        }
                                        else
                                        {
                                                status = "function paramter error on long int";
                                        }
                                    }
                                    else
                                    {
                                        status = "function paramter error on long int";
                                    }

                                }
                                else
                                {
                                    this->m_code_count = count;
                                    status = add_function_paramters("int");
                                    continue;
                                }


                            }
                            else
                            {
                                status = "function paramter error before (";
                            }


                        }while(","==status);
                    }

                }
                if(')'== this->m_code[this->m_code_count])
                {
                    this->m_code_count++;
                    status = ")";
                }
                else
                {
                    status = "function paramter error before )";
                }

            }
            else
            {
                status = "function name error before (";
            }
    }
    else
    {
       status = "function name error";
    }

    return status;

}

std::string compile::add_function_paramters(std::string str)
{
    std::string status="";
    status = ")";
    return status;
}
