#include "compile.h"
extern cpu_instructions g_instruction_o;
std::string g_keywords[]={"if","else","do","while","for","sizeof","break","continue","return","goto","rom_run","_ACC","_R1","_R2","_R3","_C","_Z","_A","_BIT0","_BIT1","_BIT2","_BIT3","_BIT4","_BIT5","_RAM","NOP"};

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
    this->m_RAM_allocate_address = 0;
    this->m_RAM_allocate_address_interrupted=0;
    this->m_RAM_allocate_address_interrupt_status=0;
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

    //init key words
    this->m_keyword = g_keywords;
    this->m_keyword_total = sizeof(g_keywords)/sizeof(g_keywords[0]);


}
unsigned int compile::items_repeat(std::string str[],unsigned int numbers[], unsigned int count,unsigned int int_or_string)
{
    std::map <std::string,int> sm; //for string
    std::map <int,int> im;//for int
    unsigned int ret = 0;// 1 means error, 2 means has item repeated. 3 means has no item repeated.

    // show content:
    /*  for (std::map <std::string,int>::iterator it=m.begin(); it!=m.end(); ++it)
    {
        qDebug() << it->first.c_str() << " => " << it->second << '\n';
    }*/

    if(0==int_or_string && str)
    {
        for (int i = 0; i < count; i++)
        {
            if (sm.end() != sm.find(str[i]))
            {
                sm[str[i]]++;
            }
            else
            {
                sm.insert(std::pair<std::string, int>(str[i], 1));
            }
        }
        for (auto it : sm)
        {
            if(it.second>1)
            {
                   ret = 2;
                   break;//has repeat
            }
            else
            {
                ret = 3;
            }
        }

    }
    else if(1==int_or_string && numbers)//for int
    {
        for (int i = 0; i < count; i++)
        {
            if (im.end() != im.find(numbers[i]))
            {
                im[numbers[i]]++;
            }
            else
            {
                im.insert(std::pair<int, int>(numbers[i], 1));
            }
        }
        for (auto it : im)
        {
            if(it.second>1)
            {
                ret = 2;
                break;//has repeat
            }
            else
            {
                ret = 3;
            }
        }
    }
    else
    {
        ret = 1;
    }

    return ret;

}
unsigned int compile::items_conflict(std::string ssrc[],std::string sdes[],unsigned int isrc[],unsigned int ides[],unsigned int src_count,unsigned int des_count,unsigned int int_or_string)
{
    std::map <std::string,int> sm; //for string
    std::map <std::string,int>::iterator it_s;
    std::map <int,int> im;//for int
    std::map <int,int>::iterator it_i;
    unsigned int k=0;
    unsigned int ret = 0;// 1 means error, 2 means has item repeated. 3 means has no item repeated.

    // show content:
    /*  for (std::map <std::string,int>::iterator it=m.begin(); it!=m.end(); ++it)
    {
        qDebug() << it->first.c_str() << " => " << it->second << '\n';
    }*/

    if(0==int_or_string && sdes)
    {

        for (int i = 0; i < des_count; i++)
        {
            if (sm.end() != sm.find(sdes[i]))
            {
                sm[sdes[i]]++;
            }
            else
            {
                sm.insert(std::pair<std::string, int>(sdes[i], 1));
            }
        }

        for(int i;i<src_count;i++)
        {
            it_s = sm.find(ssrc[i]);
            if(it_s == sm.end())
            {
                ret = 3;
            }
            else
            {
                ret = 2;//has repeat
                return ret;
            }
        }

    }
    else if(1==int_or_string && ides)//for int
    {
        for (int i = 0; i < des_count; i++)
        {
            if (im.end() != im.find(ides[i]))
            {
                im[ides[i]]++;
            }
            else
            {
                im.insert(std::pair<int, int>(ides[i], 1));
            }
        }

        for(int i;i<src_count;i++)
        {
            it_i = im.find(isrc[i]);
            if(it_i == im.end())
            {
                ret = 3;
            }
            else
            {
                ret = 2;
                return ret;//has repeat
            }
        }
    }
    else
    {
        ret = 1;
    }

    return ret;


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
std::string compile::get_valuable(std::string str)
{
    //get_valuable_lost,or;
    //get_valuable_-error
    //get_valuable_=error
    //get_valuable_go2funcitonscan
    //get_valuable_(error
    //get_valuable_wrong_valuable_name
    //it should return ';'
    std::string str_tmp="";
    std::string status="";
    std::size_t length = this->m_code.length();
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    int value=0;
    unsigned int count=0;
    do
    {
        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
        this->m_local_valuable_type[this->m_total_local_valuables] = str;
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
        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);

        this->m_local_valuable_name[this->m_total_local_valuables] = "";
        if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
        {
                pos_s=this->m_code_count;
                pos_e=1;
                this->m_code_count++;
                while('0'<= this->m_code[this->m_code_count] && '9'>=this->m_code[this->m_code_count]||'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                this->m_local_valuable_name[this->m_total_local_valuables] = this->m_code.substr(pos_s,pos_e);//get valuable name
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                //if there is no inital value, and if ',' or ';', just give ram like a=10;
                if(','== this->m_code[this->m_code_count]||';'== this->m_code[this->m_code_count])
                {
                    this->m_local_valuable_address[this->m_total_local_valuables] = this->m_RAM_allocate_address;
                    if("char"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned char"==this->m_local_valuable_type[this->m_total_local_valuables])
                    {
                        this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                    }
                    else
                    {
                        if("long int"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned long int"==this->m_local_valuable_type[this->m_total_local_valuables])
                        {
                            this->m_RAM_allocate_address = this->m_RAM_allocate_address+4;
                        }
                        else
                        {
                            this->m_RAM_allocate_address = this->m_RAM_allocate_address+2;//for int and unsigned int
                        }
                    }
                    this->m_total_local_valuables++;
                    status = this->m_code[this->m_code_count];
                    this->m_code_count++;
                }
                else
                {
                    if('='== this->m_code[this->m_code_count])
                    {
                        this->m_code_count++;
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        if('-'== this->m_code[this->m_code_count])
                        {
                            this->m_code_count++;
                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                            //get value
                            if('0' <= this->m_code[this->m_code_count] && '9' >=this->m_code[this->m_code_count] || 39 == this->m_code[this->m_code_count])//39 is '
                            {
                                    //get value
                                    value = ~this->get_number_value()+1;
                                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                    if(',' == this->m_code[this->m_code_count] || ';' == this->m_code[this->m_code_count])
                                    {

                                        this->m_local_valuable_address[this->m_total_local_valuables] = this->m_RAM_allocate_address;
                                        if("char"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned char"==this->m_local_valuable_type[this->m_total_local_valuables])
                                        {
                                            this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                            this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                                        }
                                        else
                                        {
                                            if("long int"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned long int"==this->m_local_valuable_type[this->m_total_local_valuables])
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
                                        }
                                        this->m_total_local_valuables++;
                                        status = this->m_code[this->m_code_count];
                                        this->m_code_count++;
                                    }
                                    else
                                    {
                                        status="get_valuable_lost,or;";
                                    }
                                }
                                else
                                {
                                    status="get_valuable_-error";
                                }

                            }
                            else
                            {

                                if('0' <= this->m_code[this->m_code_count] && '9' >=this->m_code[this->m_code_count] || 39 == this->m_code[this->m_code_count])//39 is '
                                {
                                    value = this->get_number_value();
                                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                    if(',' == this->m_code[this->m_code_count] || ';' == this->m_code[this->m_code_count])
                                    {
                                        this->m_local_valuable_address[this->m_total_local_valuables] = this->m_RAM_allocate_address;
                                        if("char"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned char"==this->m_local_valuable_type[this->m_total_local_valuables])
                                        {
                                            this->init_RAM_with_value(this->m_RAM_allocate_address,value);
                                            this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                                        }
                                        else
                                        {
                                            if("long int"==this->m_local_valuable_type[this->m_total_local_valuables]||"unsigned long int"==this->m_local_valuable_type[this->m_total_local_valuables])
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
                                        }
                                        this->m_total_local_valuables++;
                                        status = this->m_code[this->m_code_count];
                                        this->m_code_count++;
                                    }
                                    else
                                    {
                                          status="get_valuable_lost,or;";
                                    }
                                }
                                else
                                {
                                    status="get_valuable_=error";
                                }
                            }

                        }
                        else
                        {
                            //if it is array, more more complicated ?????
                            if('['== this->m_code[this->m_code_count])
                            {

                            }
                            else
                            {
                                if('('== this->m_code[this->m_code_count]) /* && count == 0)*/
                                {
                                    status="get_valuable_go2funcitonscan";
                                }
                                else
                                {
                                    status="get_valuable_(error";
                                }
                            }
                        }

                }


        }//if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
        else
        {
            status="get_valuable_wrong_valuable_name";
        }
        //count++
        //check if ram is enough
        //if(?????this->m_RAM_allocate_address)
        //{
        //status=RAM_OVERFLOW
        //}
    }while(","==status);

    return status;
}
std::string compile::scan_valuable()
{
    //scan_valuable_codeover
    //scan_valuable_over
    //scan_valuable_unsignederror
    //scan_valuable_signederror
    //scan_valuable_charintlongerror
    //scan_valuable_local_valuable_repeat
    //scan_valuable_local_valuable_keywords_conflict
    std::string status = "";
    std::string buffer_tmp="";
    unsigned int pos_save=0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    unsigned int scan_flag=0;
    this->m_total_local_valuables = 0;
    do
    {
        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
        pos_save = this->m_code_count;//save it
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
            buffer_tmp = this->m_code.substr(pos_s,pos_e);
            //unsigned char,unsigned int,unsigned long int
            if(buffer_tmp == "unsigned")//it will have signed char, signed int, signed long int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                pos_s=this->m_code_count;
                pos_e=1;
                while('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                buffer_tmp = this->m_code.substr(pos_s,pos_e);//get char/int/int long
                if(buffer_tmp == "char")
                {
                    qDebug()<<"unsigned char"<<endl;
                    status = this->get_valuable("unsigned char");
                }
                else
                {
                    if(buffer_tmp == "int")
                    {
                        qDebug()<<"unsigned int"<<endl;
                        status = this->get_valuable("unsigned int");
                    }
                    else
                    {
                        if(buffer_tmp == "long")
                        {
                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
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
                                buffer_tmp = this->m_code.substr(pos_s,pos_e);//get signed or unsigned
                                if(buffer_tmp == "int")
                                {

                                    qDebug()<<"unsigned long int"<<endl;
                                    status = this->get_valuable("unsigned long int");
                                }
                                else
                                {
                                    status="scan_valuable_unsignederror";//return status
                                }
                            }
                            else
                            {
                                status="scan_valuable_unsignederror";
                            }
                        }
                        else
                        {
                            status="scan_valuable_unsignederror";
                        }
                    }
                }
            }
            else
            {
                status="scan_valuable_unsignederror";
            }

            //signed char,signed int,signed long int
            if (buffer_tmp == "signed")//it will have signed char, signed int, signed long int
            {
                qDebug()<<buffer_tmp.c_str()<<endl;
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                pos_s=this->m_code_count;
                pos_e=1;
                while('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    pos_e++;
                    this->m_code_count++;
                }
                buffer_tmp = this->m_code.substr(pos_s,pos_e);//get char/int/int long
                if(buffer_tmp == "char")
                {
                    qDebug()<<"signed char"<<endl;
                    status = this->get_valuable("signed char");
                }
                else
                {
                    if(buffer_tmp == "int")
                    {
                        qDebug()<<"signed int"<<endl;
                        status = this->get_valuable("signed int");
                    }
                    else
                    {
                        if(buffer_tmp == "long")
                        {
                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
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
                                buffer_tmp = this->m_code.substr(pos_s,pos_e);//get signed or unsigned
                                if(buffer_tmp == "int")
                                {

                                    qDebug()<<"signed long int"<<endl;
                                    status = this->get_valuable("signed long int");
                                }
                                else
                                {
                                    status="scan_valuable_signederror";//return status
                                }
                            }
                            else
                            {
                                status="scan_valuable_signederror";
                            }
                        }
                        else
                        {
                            status="scan_valuable_signederror";
                        }
                    }
                }
            }
            else
            {
                status="scan_valuable_signederror";
            }

            //char, int, long int
            if (buffer_tmp == "char")
            {
                qDebug()<<"char"<<endl;
                status = this->get_valuable("char");
            }
            else
            {
                if (buffer_tmp == "int")//int
                {
                    qDebug()<<"int"<<endl;
                    status = this->get_valuable("int");
                }
                else
                {
                    if (buffer_tmp == "long") //long int
                    {
                        qDebug()<<"long int"<<endl;
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        if('i'== this->m_code[this->m_code_count]&&'n'== this->m_code[this->m_code_count+1]&&'t'== this->m_code[this->m_code_count+2]&&' '== this->m_code[this->m_code_count+3])
                        {
                            this->m_code_count+3;
                            status = this->get_valuable("long int");
                        }
                        else
                        {
                            status="scan_valuable_charintlongerror";
                        }
                    }
                    else
                    {
                        this->m_code_count = pos_save;
                        status="scan_valuable_over";
                    }
                }

            }
        }
        else
        {
            if(' ' == this->m_code[this->m_code_count])
            {
                status="scan_valuable_codeover";
            }
            else
            {
                if('@' == this->m_code[this->m_code_count])
                {
                    this->m_code_count++;
                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                    if('0'<= this->m_code[this->m_code_count] && '9'>=this->m_code[this->m_code_count])
                    {
                        if(this->m_RAM_allocate_address_interrupt_status==0)
                        {
                            this->m_RAM_allocate_address_interrupted = this->m_RAM_allocate_address;

                        }
                        this->m_RAM_allocate_address = this->get_number_value();
                        this->m_RAM_allocate_address_interrupt_status = 1;
                        status=";";
                    }
                    else
                    {
                        if(this->m_RAM_allocate_address_interrupt_status==1)
                        {
                            this->m_RAM_allocate_address = this->m_RAM_allocate_address_interrupted;
                            this->m_RAM_allocate_address_interrupt_status=0;
                        }

                    }

                }
                else
                {
                    this->m_code_count = pos_save;
                    status="scan_valuable_over";
                }
            }

        }

    }while(";"==status);
    scan_flag = 1;
    //7 constants conflict with constants
    //8 constants conflict with keywords

    //9 local valuables conflict with local valuables
    if(scan_flag && 2 == this->items_repeat(this->m_local_valuable_name,NULL,this->m_total_local_valuables,0))
    {
        scan_flag = 0;
        status = "scan_valuable_local_valuable_repeat";
    }
    //10 local valuables conflict with constants

    //11 local valuables conflict with keywords
    if(scan_flag && 2 == this->items_conflict(this->m_local_valuable_name,this->m_keyword,NULL,NULL,this->m_total_local_valuables,this->m_keyword_total,0) )
    {
        scan_flag = 0;
        status = "scan_valuable_local_valuable_keywords_conflict";
    }
    //12 global valuables conflict with constants

    if ( "get_valuable_go2funcitonscan" == status)
    {
        this->m_code_count = pos_save;
    }

    return status;



}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//scan funcitons
std::string compile::compiling_all(std::string str)
{
    //compiling_all_m_func_name_conflict_m_global_valuable_name
    //compiling_all_m_func_name_conflict_m_keyword
    //compiling_all_m_func_name_repeated
    //compiling_all_(_error
    //compiling_all_function_define_{_error
    std::string status = "";
    status = str;
    if("scan_valuable_over" == status||"get_valuable_go2funcitonscan"==status)
    {
        this->m_MAIN_entry_in_ROM = this->m_ROM_PAGE_use * SIZE_PER_PAGE + this->m_ROM_Cahce_c;
        g_instruction_o.I_PAGE_JUMP(0);
        this->m_ROM_PAGE[this->m_ROM_PAGE_use] = this->m_ROM_Cache;
        this->m_ROM_PAGE_byte_use_c[this->m_ROM_PAGE_use]=this->m_ROM_Cahce_c;
        //m_global stuff
        this->m_total_global_valuables = this->m_total_local_valuables;
        memcpy(this->m_global_valuable_name, this->m_local_valuable_name, sizeof(this->m_local_valuable_name));
        memcpy(this->m_global_valuable_type, this->m_local_valuable_type, sizeof(this->m_local_valuable_type));
        memcpy(this->m_global_valuable_address, this->m_local_valuable_address, sizeof(this->m_local_valuable_address));
        this->m_global_end_address = this->m_RAM_allocate_address;
        //
        this->m_total_functions = 0;
        this->m_func_paramter_base_address[this->m_total_functions] = 0;
        this->m_ROM_PAGE_init_f = 0;

        do
        {
            this->m_RAM_allocate_address = this->m_global_end_address;
            status = this->getRAMaddress4function();//getRAMaddress4function_{notfouond    getRAMaddress4function_over    getRAMaddress4function_{}mismatch    getRAMaddress4function_functionnotdefine
            if("getRAMaddress4function_over"==status)//success
            {
                status = this->scan_function_definition();//?????
                if(")"==status)
                {
                   qDebug()<<"NEED-we are compiling "<<this->m_func_name[this->m_total_functions].c_str()<<" function now - looking for {"<<endl;
                   this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                   if('{'== this->m_code[this->m_code_count])//function start with {
                   {
                        this->m_code_count++;
                        //first scan the internal function valuables
                        status = this->scan_valuable();//scan_valuable_codeover,scan_valuable_over,scan_valuable_unsignederror,scan_valuable_signederror,scan_valuable_charintlongerror ?????
                        //function name checking repeat conflict
                        if(2 == this->items_repeat(this->m_func_name,NULL,this->m_total_functions,0))
                        {
                            status = "compiling_all_m_func_name_repeated";
                            break;
                        }
                        //function name and global valuable conflict
                        if(2 == this->items_conflict(this->m_func_name,this->m_global_valuable_name,NULL,NULL,this->m_total_functions,this->m_total_global_valuables,0))
                        {
                            status = "compiling_all_m_func_name_conflict_m_global_valuable_name";
                            break;
                        }
                        //function name and constants conflict ?????

                        //function name and key words conflict
                        if(2 == this->items_conflict(this->m_func_name,this->m_keyword,NULL,NULL,this->m_total_functions,this->m_keyword_total,0))
                        {
                            status = "compiling_all_m_func_name_conflict_m_keyword";
                            break;
                        }
                        //fucntion paramter name and fucntion paramter name repeat conflict

                        //fucntion paramter name and local valuable conflict

                        //fucntion paramter name and constants conflict

                        //fucntion paramter name and key words conflict

                        if("scan_valuable_over"==status)
                        {
                            this->m_func_RAM_end_address[this->m_total_functions] = this->m_RAM_allocate_address;//all local valuables ends
                            if('}'== this->m_code[this->m_code_count])
                            {
                                this->function_ends_processing();
                                this->m_code_count++;
                                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);

                                if(' '== this->m_code[this->m_code_count])
                                {
                                        status = compile_ends_processing();//?????
                                }
                                else
                                {
                                        status = "}";
                                }


                            }
                            else//statements compiling
                            {
                                status = statement_processing();// real compile code ?????
                                if("statement_processing_ok"==status)
                                {
                                    this->function_ends_processing();//dealwith ??????
                                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                    if(' '== this->m_code[this->m_code_count])
                                    {
                                            status = this->compile_ends_processing();//?????
                                    }
                                    else
                                    {
                                            status = "}";
                                    }
                                }
                            }

                        }
                        else
                        {
                            if("scan_valuable_go2funcitonscan"==status)
                            {
                                //something wrong
                                status = "compiling_all_(_error";
                            }
                        }

                   }
                   else
                   {
                       status="compiling_all_function_define_error";
                   }
                }
            }//if("getRAMaddress4function_over"==status)//success


        } while("}"==status);
    }

    return status;
}

std::string compile::getRAMaddress4function()
{
    unsigned int brackets_c = 0;
    std::string func_name="";
    std::string status="";//getRAMaddress4function_{notfouond    getRAMaddress4function_over    getRAMaddress4function_{}mismatch    getRAMaddress4function_functionnotdefine
    unsigned int pos_save=0;
    unsigned int pos_s=0;
    unsigned int pos_e=0;
    unsigned int func_query_c=0;
    pos_save = this->m_code_count;
    while('{'!=this->m_code[this->m_code_count] && this->m_code_count < (this->m_code.size()-16))
    {
        this->m_code_count++;
    }

    if('{'==this->m_code[this->m_code_count])
    {
        brackets_c=1;
        status = "keep_going";
        this->m_code_count++;
        do{
            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
            if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
            {
                func_name="";
                pos_s=this->m_code_count;
                pos_e=1;
                this->m_code_count++;
                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                            status = "getRAMaddress4function_functionnotdefine";
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
                        status = "getRAMaddress4function_over";//get ram address over for functions
                    }
                    else
                    {
                        this->m_code_count++;
                    }
                }
                if(' '==this->m_code[this->m_code_count])
                {
                       status = "getRAMaddress4function_{}mismatch";
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
                else
                {
                    if('}'==this->m_code[this->m_code_count])
                    {
                        brackets_c--;
                        this->m_code_count++;
                        if(0==brackets_c)
                        {
                            this->m_code_count = pos_save;
                            status = "getRAMaddress4function_over";//get ram address over for functions
                        }
                    }
                    else
                    {
                        if(' '==this->m_code[this->m_code_count])
                        {
                               status = "getRAMaddress4function_{}mismatch";
                        }
                        else
                        {
                            this->m_code_count++;
                        }
                    }
                }
            }
        }while("keep_going"==status);

    }
    else
    {
        status = "getRAMaddress4function_{notfouond";//function scan error
    }

    return status;

}



std::string compile::scan_function_definition()
{
    //scan_function_definition_return_type_error
    //scan_function_definition_error_after_unsigned_long
    //scan_function_definition_error_after_unsigned
    //
    //scan_function_definition_error_after_signed
    //scan_function_definition_error_after_signed_long
    //
    //scan_function_definition_error_after_long

    std::string status="";
    unsigned int count = 0;
    unsigned int tmp = 0;
    std::string tmp_name="";
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
        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                        status = "scan_function_definition_error_after_unsigned";
                    }
                    if("int"==tmp_name)
                    {
                        status = this->scan_function_name_paramters("unsigned int");
                    }
                    else
                    {
                        status = "scan_function_definition_error_after_unsigned";
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
                            while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                                status = "scan_function_definition_error_after_unsigned_long";
                            }
                        }
                        else
                        {
                            status = "scan_function_definition_error_after_unsigned_long";
                        }
                    }
                    else
                    {
                        status = "scan_function_definition_error_after_unsigned";
                    }
                }
                else
                {
                    status = "scan_function_definition_error_after_unsigned";
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
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                        status = "scan_function_definition_error_after_signed";
                    }
                    if("int"==tmp_name)
                    {
                        status = this->scan_function_name_paramters("signed int");
                    }
                    else
                    {
                        status = "scan_function_definition_error_after_signed";
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
                            while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                                status = "scan_function_definition_error_after_signed_long";
                            }
                        }
                        else
                        {
                            status = "scan_function_definition_error_after_signed_long";
                        }
                    }
                    else
                    {
                        status = "scan_function_definition_error_after_signed_long";
                    }
                }
                else
                {
                    status = "scan_function_definition_return_type_error";
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
                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                            status = "scan_function_definition_error_after_long";
                        }
                    }
                    else
                    {
                        status = "scan_function_definition_error_after_long";
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
        status = "scan_function_definition_return_type_error";
    }
    return status;
}

std::string compile::scan_function_name_paramters(std::string str)
{
    //scan_function_name_paramters_name_error
    //scan_function_name_paramters_error_before_(
    //scan_function_name_paramters_error_before_)
    //scan_function_name_paramters_void_lose_
    //scan_function_name_paramters_paramter_error
    //scan_function_name_paramters_paramter_error_after_unsigned_long
    //scan_function_name_paramters_paramter_error_after_unsigned
    //scan_function_name_paramters_paramter_error_after_signed_long
    //scan_function_name_paramters_paramter_error_after_signed
    //scan_function_name_paramters_paramter_error_on_long
    //scan_function_name_paramters_paramter_error_on_long_int

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
            pos_e=0;
            do{
                pos_e++;
                this->m_code_count++;
            }while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count]);
            tmp_name = this->m_code.substr(pos_s,pos_e);//get function name
            this->m_func_name[this->m_total_functions] = tmp_name;
            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
            if('('<= this->m_code[this->m_code_count])
            {
                this->m_func_paramter_offset_address[this->m_total_functions] = this->m_func_paramter_base_address[this->m_total_functions];
                this->m_code_count++;
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    count = this->m_code_count;
                    tmp_name="";
                    pos_s=this->m_code_count;
                    pos_e=1;
                    this->m_code_count++;
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                    {
                        pos_e++;
                        this->m_code_count++;
                    }
                    tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                    if("void"==tmp_name)
                    {
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        if(')'==this->m_code[this->m_code_count])
                        {
                            this->m_code_count++;
                            status = ")";
                        }
                        else
                        {
                            status = "scan_function_name_paramters_void_lose_)";
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
                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
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
                                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                        {
                                            pos_e++;
                                            this->m_code_count++;
                                        }
                                        tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                        if("char"==tmp_name)
                                        {
                                                status = add_function_paramters("unsigned char");
                                        }
                                        if("int"==tmp_name)
                                        {
                                                status = add_function_paramters("unsigned int");
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
                                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                                {
                                                    pos_e++;
                                                    this->m_code_count++;
                                                }
                                                tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                                if("int"==tmp_name)
                                                {
                                                        status = add_function_paramters("unsigned long int");
                                                }
                                                else
                                                {
                                                        status = "scan_function_name_paramters_paramter_error_after_unsigned_long";
                                                }
                                            }
                                            else
                                            {
                                                status = "scan_function_name_paramters_paramter_error_after_unsigned_long";
                                            }
                                        }
                                        else
                                        {
                                                status = "scan_function_name_paramters_paramter_error_after_unsigned";
                                        }
                                    }
                                    else
                                    {
                                        status = "scan_function_name_paramters_paramter_error_after_unsigned";
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
                                        while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                        {
                                            pos_e++;
                                            this->m_code_count++;
                                        }
                                        tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                        if("char"==tmp_name)
                                        {
                                                status = add_function_paramters("char");
                                        }
                                        if("int"==tmp_name)
                                        {
                                                status = add_function_paramters("int");
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
                                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                                {
                                                    pos_e++;
                                                    this->m_code_count++;
                                                }
                                                tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                                if("int"==tmp_name)
                                                {
                                                        status = add_function_paramters("long int");
                                                }
                                                else
                                                {
                                                        status = "scan_function_name_paramters_paramter_error_after_signed_long";
                                                }
                                            }
                                            else
                                            {
                                                status = "scan_function_name_paramters_paramter_error_after_signed_long";
                                            }
                                        }
                                        else
                                        {
                                                status = "scan_function_name_paramters_paramter_error_after_signed";
                                        }
                                    }
                                    else
                                    {
                                        status = "scan_function_name_paramters_paramter_error_after_signed";
                                    }
                                }
                                if("char"==tmp_name)
                                {
                                    status = add_function_paramters("char");
                                }
                                else
                                {
                                    if("int"==tmp_name)
                                    {
                                        status = add_function_paramters("int");
                                    }
                                    else
                                    {
                                        if("long"==tmp_name)
                                        {
                                            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                                            if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                            {
                                                tmp_name="";
                                                pos_s=this->m_code_count;
                                                pos_e=1;
                                                this->m_code_count++;
                                                while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                                                {
                                                    pos_e++;
                                                    this->m_code_count++;
                                                }
                                                tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter type
                                                if("int"==tmp_name)
                                                {
                                                        status = add_function_paramters("long int");
                                                }
                                                else
                                                {
                                                        status = "scan_function_name_paramters_paramter_error_on_long_int";
                                                }
                                            }
                                            else
                                            {
                                                status = "scan_function_name_paramters_paramter_error_on_long";
                                            }

                                        }
                                        else
                                        {
                                            this->m_code_count = count;
                                            status = add_function_paramters("int");
                                        }
                                    }
                                }
                            }
                            else
                            {
                                status = "scan_function_name_paramters_paramter_error";
                            }


                        }while(","==status);
                    }

                }
                else
                {
                    if(')'== this->m_code[this->m_code_count])
                    {
                        this->m_code_count++;
                        status = ")";
                    }
                    else
                    {
                        status = "scan_function_name_paramters_error_before_)";
                    }
                }

            }
            else
            {
                status = "scan_function_name_paramters_error_before_(";
            }


            if(")"==status)//finished paramters scan as good one
            {
                this->m_func_return_address[this->m_total_functions] = this->m_RAM_allocate_address;
                this->m_func_paramter_base_address[this->m_total_functions+1] = this->m_func_paramter_offset_address[this->m_total_functions];//next paramter base address is this time offset address
                this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                {
                    tmp_name="";
                    pos_s=this->m_code_count;
                    pos_e=1;
                    this->m_code_count++;
                    while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
                    {
                        pos_e++;
                        this->m_code_count++;
                    }
                    tmp_name = this->m_code.substr(pos_s,pos_e);
                    if("rom"==tmp_name)
                    {
                        //this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        //?????

                    }
                    else
                    {
                        status="";//?????
                    }
                }
                else
                {
                    //?????
                }
            }
    }
    else
    {
       status = "scan_function_name_paramters_name_error";
    }

    return status;

}

std::string compile::add_function_paramters(std::string str)
{
    //unsigned int m_func_paramter_base_address[MAX_FUNCTION];per function, m_func_paramter_base_address[MAX_FUNCTION] = m_func_paramter_base_address[MAX_FUNCTION] + m_func_paramter_offset_address[MAX_FUNCTION];
    //unsigned int m_func_paramter_offset_address[MAX_FUNCTION]; per function,but will keep change if you have many paramters, mean which paramter.(int a , int b , int c....), it will be 0 1 2...

    //std::string  m_func_paramter_name[MAX_FUNCTION]; per paramter(m_func_paramter_offset_address) not per funcion!, all functions use it,m_func_paramter_name[m_func_paramter_offset_address];
    //std::string  m_func_paramter_type[MAX_FUNCTION]; per paramter(m_func_paramter_offset_address) not per funcion!, all functions use it,m_func_paramter_type[m_func_paramter_offset_address];
    //unsigned int m_func_paramter_address[MAX_FUNCTION];per paramter(m_func_paramter_offset_address) not per funcion!, all functions use it,m_func_paramter_type[m_func_paramter_offset_address];


    //add_function_paramters_paramter_name_error
    //
    //
    //
    std::string status="";
    std::string tmp_name="";
    unsigned int pos_s=0;
    unsigned int pos_e=0;

    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);

    this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]] = str;

    if('*'== this->m_code[this->m_code_count])
    {
        this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]] = this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]]+"*";
        this->m_code_count++;
    }
    if('*'== this->m_code[this->m_code_count])
    {
        this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]] = this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]]+"*";
        this->m_code_count++;
    }

    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);

    this->m_func_paramter_name[this->m_func_paramter_offset_address[this->m_total_functions]]="";

    if('A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
    {
            tmp_name="";
            pos_s=this->m_code_count;
            pos_e=1;
            this->m_code_count++;
            while('0'<= this->m_code[this->m_code_count] && '9'>= this->m_code[this->m_code_count] || 'A'<= this->m_code[this->m_code_count] && 'Z'>=this->m_code[this->m_code_count] || 'a'<= this->m_code[this->m_code_count] && 'z'>=this->m_code[this->m_code_count] || '_'==this->m_code[this->m_code_count])
            {
                pos_e++;
                this->m_code_count++;
            }
            tmp_name = this->m_code.substr(pos_s,pos_e);//get paramter name
            this->m_func_paramter_name[this->m_func_paramter_offset_address[this->m_total_functions]] = tmp_name;

            this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);

            if(','== this->m_code[this->m_code_count] || ')'== this->m_code[this->m_code_count])
            {
                this->m_func_paramter_address[this->m_func_paramter_offset_address[this->m_total_functions]] = this->m_RAM_allocate_address;
                if("char"==this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]]||"unsigned char"==this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]])
                {
                    this->m_RAM_allocate_address = this->m_RAM_allocate_address+1;
                }
                else
                {
                    if("long int"==this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]]||"unsigned long int"==this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]])
                    {
                        this->m_RAM_allocate_address = this->m_RAM_allocate_address+4;
                    }
                    else
                    {
                        this->m_RAM_allocate_address = this->m_RAM_allocate_address+2;
                    }
                }
                //next paramter in (.......)
                this->m_func_paramter_offset_address[this->m_total_functions] = this->m_func_paramter_offset_address[this->m_total_functions] + 1;
                status = this->m_code[this->m_code_count];
                this->m_code_count++;
            }
            else
            {
               /* if('['== this->m_code[this->m_code_count])
                {
                    this->m_code_count++;
                    this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                    if(']'== this->m_code[this->m_code_count])
                    {
                        this->m_code_count++;
                        this->symbolic_space_newline_comment_jumper((std::string&)this->m_code,(int&)this->m_code_count);
                        this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]]=this->m_func_paramter_type[this->m_func_paramter_offset_address[this->m_total_functions]] + "[]";
                        if(','== this->m_code[this->m_code_count] || ')'== this->m_code[this->m_code_count])
                        {
                            this->m_func_paramter_address[this->m_func_paramter_offset_address[this->m_total_functions]] = this->m_RAM_allocate_address;
                            this->m_RAM_allocate_address = this->m_RAM_allocate_address+2;
                            this->m_func_paramter_offset_address[this->m_total_functions] = this->m_func_paramter_offset_address[this->m_total_functions] + 1;
                            status = this->m_code[this->m_code_count];
                            this->m_code_count++;
                        }
                        else
                        {
                            status = "add_function_paramters_lost ] for paramters";
                        }
                    }
                    else
                    {
                        status = "add_function_paramters_lost ] for paramters";
                    }

                }
                else
                {
                    status = "add_function_paramters_lost , ) [ for paramters";
                }*/
            }

    }
    else
    {
        status="add_function_paramters_paramter_name_error";
    }
    return status;
}

//////////////////////////////////////////////////////////////////
void compile::function_ends_processing()
{
    if("main" != this->m_func_name[this->m_total_functions] && "void" == this->m_func_type[this->m_total_functions])
    {
        if("share_in_page"==this->m_func_share[this->m_total_functions])
        {
            g_instruction_o.I_FUN_RETURN_IN_PAGE(this->m_func_return_address[this->m_total_functions]);
        }
        else
        {
            g_instruction_o.I_FUN_RETURN_BETWEEN_PAGES(this->m_func_return_address[this->m_total_functions]);
        }

    }
    this->m_ROM_PAGE[((this->m_func_entry_address[this->m_total_functions])/SIZE_PER_PAGE)] = this->m_ROM_Cache;

    this->m_ROM_PAGE_byte_use_c[((this->m_func_entry_address[this->m_total_functions])/SIZE_PER_PAGE)] = this->m_ROM_Cahce_c;

    this->m_total_functions++;
    qDebug()<<"MINMIN "<<"FUNCTION "<<this->m_func_name[this->m_total_functions].c_str()<<" compile ok in page "<<((this->m_func_entry_address[this->m_total_functions])/SIZE_PER_PAGE)<<"size "<<this->m_ROM_PAGE_byte_use_c[(this->m_func_entry_address[this->m_total_functions])/SIZE_PER_PAGE]<<endl;
}


std::string compile::compile_ends_processing()
{
    //compile_ends_processing_main???sucks
    //compile_ends_processing_ok_completed
    //compile_ends_processing_ROM_overfloat
    std::string status = "";
    unsigned int count = 0;

    std::vector<unsigned char>  output;

    if("main"==this->m_func_name[this->m_total_functions-1])
    {
        status = "compile_ends_processing_ok_completed";
        qDebug()<<"MINMIN "<< "modifying main function entry address " <<this->m_func_entry_address[this->m_total_functions]<<endl;
        this->m_ROM_Cache = this->m_ROM_PAGE[this->m_MAIN_entry_in_ROM/SIZE_PER_PAGE];


        this->m_ROM_Cache[(this->m_MAIN_entry_in_ROM & (SIZE_PER_PAGE-1))+2] = (unsigned char)(this->m_func_entry_address[this->m_total_functions] >>16);
        this->m_ROM_Cache[(this->m_MAIN_entry_in_ROM & (SIZE_PER_PAGE-1))+9] = (unsigned char)(this->m_func_entry_address[this->m_total_functions] >>8);
        this->m_ROM_Cache[(this->m_MAIN_entry_in_ROM & (SIZE_PER_PAGE-1))+16] = (unsigned char)(this->m_func_entry_address[this->m_total_functions]);

        this->m_ROM_PAGE[this->m_MAIN_entry_in_ROM/SIZE_PER_PAGE] = this->m_ROM_Cache;

        output.clear();
        count=0;
        do{
            this->m_ROM_Cache = this->m_ROM_PAGE[count];
            this->m_ROM_Cahce_c = this->m_ROM_PAGE_byte_use_c[count];
            qDebug()<<"MINMIN "<< "ROM page "<<count<<" used bytes "<<this->m_ROM_Cahce_c<<" remains bytes "<<(SIZE_PER_PAGE-SIZE_PAGE_JUMP-this->m_ROM_Cahce_c)<<"(page jumper instructions take 48 bytes)"<<endl;

            if(this->m_ROM_Cahce_c < (SIZE_PER_PAGE-SIZE_PAGE_JUMP))
            {
                //give 256 ' '
                while((this->m_ROM_Cahce_c+256) < (SIZE_PER_PAGE-SIZE_PAGE_JUMP))
                {
                    for(int i=0;i<256;i++)
                    {
                        this->m_ROM_Cache.push_back(' ');
                    }
                    this->m_ROM_Cahce_c = m_ROM_Cahce_c + 256;
                }
                //if still not enough , then give nop();
                g_instruction_o.set_buff_count(&this->m_ROM_Cache,&this->m_ROM_Cahce_c);
                while(this->m_ROM_Cahce_c<(SIZE_PER_PAGE-SIZE_PAGE_JUMP))
                {
                    g_instruction_o.I_NOP();
                }
                //?????


                //?????
                output.insert(output.end(),this->m_ROM_Cache.begin(),this->m_ROM_Cache.end());
                count++;

            }
            else
            {
                status = "compile_ends_processing_ROM_overfloat";
                count = MAX_PAGE;
            }
        }
        while(count<this->m_ROM_PAGE_use);

        if("compile_ends_processing_ok_completed" == status)
        {
            if(this->m_func_RAM_end_address[this->m_total_functions] < SIZE_PER_PAGE && this->m_func_RAM_end_address[this->m_total_functions] < (SIZE_PER_PAGE-SIZE_PAGE_JUMP))
            {
                qDebug()<<"MINMIN "<< "RAM used bytes "<<this->m_func_RAM_end_address[this->m_total_functions]<<" remains bytes "<<(512*1024-this->m_func_RAM_end_address[this->m_total_functions])<<endl;
            }
            else
            {
                status = "compile_ends_processing_ROM_overfloat";
            }
        }

        if("compile_ends_processing_ok_completed" == status)
        {
            //output the code to CPU.bin
            QString filename = "CPU.bin";
            QFile fileout(filename);
            if (fileout.open(QFile::ReadWrite | QFile::Text))
            {
             QTextStream out(&fileout);
             for(auto it = output.begin(); it != output.end(); ++it)
             {
                out<<*it;
             }

             fileout.close();
            }
        }

    }
    else
    {
        status = "compile_ends_processing_main???sucks";
    }
    return status;
}

std::string compile::statement_processing()
{
    //statement_processing_ok
    std::string status="";
    return status;
}



