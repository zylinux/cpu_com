#ifndef COMPILE_H
#define COMPILE_H
#include "mainwindow.h"
#define SIZE_PER_PAGE 524288
#define MAX_SRC_FILE 65535
#define MAX_LOCAL_VALUABLES 65535
#define MAX_PAGE 256
#define MAX_FUNCTION 65535
class compile
{
public:
    //main code section
    std::string m_code;
    int m_code_count;
    //when need to glue code together in preprocess
    std::string m_codeblock[MAX_SRC_FILE];
    int m_codeblock_count;
    //local valuables
    unsigned int m_total_local_valuables;
    std::string m_local_valuable_type[MAX_LOCAL_VALUABLES];
    std::string m_local_valuable_name[MAX_LOCAL_VALUABLES];
    unsigned int m_local_valuable_address[MAX_LOCAL_VALUABLES];
    //global valuables
    unsigned int m_total_global_valuables;
    std::string m_global_valuable_type[MAX_LOCAL_VALUABLES];
    std::string m_global_valuable_name[MAX_LOCAL_VALUABLES];
    unsigned int m_global_valuable_address[MAX_LOCAL_VALUABLES];
    unsigned int m_global_end_address;
    //allocate address in RAM
    unsigned int m_RAM_allocate_address;

    //ROM tmp cache
    std::vector<unsigned char>  m_ROM_Cache;
    unsigned int m_ROM_Cahce_c;
    //ROM page
    unsigned int m_ROM_PAGE_use;
    std::vector<unsigned char>  m_ROM_PAGE[MAX_PAGE];
    unsigned int m_ROM_PAGE_byte_use_c[MAX_PAGE];
    //RAM page switch control
    unsigned int m_ROM_PAGE_init_f;

    //main entry address
    unsigned int m_MAIN_entry_in_ROM;

    //functions
    unsigned int    m_total_functions;
    std::string     m_func_name[MAX_FUNCTION];
    std::string     m_func_type[MAX_FUNCTION];
    unsigned int    m_func_entry_address[MAX_FUNCTION];
    unsigned int    m_func_return_address[MAX_FUNCTION];
    std::string     m_func_share[MAX_FUNCTION];
    unsigned int m_func_RAM_end_address[MAX_FUNCTION];
    unsigned int m_func_paramter_base_address[MAX_FUNCTION];
    unsigned int m_func_paramter_offset_address[MAX_FUNCTION];
    std::string  m_func_paramter_name[MAX_FUNCTION];
    std::string  m_func_paramter_type[MAX_FUNCTION];
    unsigned int m_func_paramter_address[MAX_FUNCTION];



    compile();
    int symbolic_space_newline_comment_jumper(std::string &str, int &count);
    unsigned char get_valuable(std::string str);
    unsigned char scan_valuable();
    int get_number_value();
    void init_RAM_with_value(unsigned int RAM_allocate_address,unsigned int value);
    ////////////////////////////////////
    unsigned char scan_funciton();
    unsigned int getRAMaddress4function();
    std::string scan_function_definition();
    std::string scan_function_name_paramters(std::string str);
    std::string add_function_paramters(std::string str);
};

#endif // COMPILE_H
