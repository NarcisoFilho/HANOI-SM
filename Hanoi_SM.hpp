#ifndef __HANOI_SM_HPP_
#define __HANOI_SM_HPP_

#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <ctime>

#include "HSMProgram.hpp"
#include "utils.hpp"
using namespace std;

#define STACK_SIZE_DEFAULT 128
#define QTD_REGISTERS 6


/** class HSM : Stack Machine
 * @brief Uma máquina de pilha com diversas operações
 * 
 * 
 */
template <typename T = int>
class HSM{
    T R[QTD_REGISTERS];            // Machine's Registers
    T *stack;       // Stack HSM memory region
    int tos;        // Top of the stack
    int height;     // Stack's height ( Number of Elements )
    long long int size;       // Stack's size in Bytes
    
    
    public:
    bool flag_execution;
    int line_execution;
    bool delay_execution;
    bool flag_error;

    protected:
    int tipo_erro;

    public:
    // Constructor + Destructor
    HSM( int height = STACK_SIZE_DEFAULT  );
    ~HSM();

    // Arithimetics
    void add();
    void sub();
    void mul();
    void div();
    void mod();

    // Logics
    void NOT();
    void OR();
    void AND();
    void MIR();

    // Control
    void push( T n );
    void pushR( int r = 0 );
    void pop();
    void pop( int r );

    // I/O
    void out();

    // JUMP's
    void jmp( int );
    void jz( int );
    void jnz( int );
    void jzr( int , int );
    void jnzr( int , int );

    // Others
    void nop(){}
    void reset();
    int run( HSMProgram* );

    // Just for draw and debuging
    T get( int n ){ return stack[ n ]; }
    int get_tos(){ return tos; }
    int get_height(){ return height; }
    T getR( int r = 0 ){ return R[ r ]; }
    int get_line_execution(){ return line_execution; }
    int get_error_code(){ return tipo_erro; }


    // Execution
    int step( HSMProgram* );
    void stop_error( int );

};

// Constructor + Destructor ======================================================================================================
template <typename T>
HSM<T>::HSM( int height ){
    // Allocate memory space to the stack region
    stack = new T[height];
    this->height = height;          // Information about how much elements stack support
    size = height * sizeof( T );    // Information about how much space the stack ocupe

    // Set stack's top indicator to the base( -1 )
    tos = -1;

    // Reinicialize Stack Memory
    this->reset();
}

template <typename T>
HSM<T>::~HSM(){
    delete[] stack;
}

// Arithimetics ================================================================================================================== 
template <typename T>
void HSM<T>::add(){
    if( tos > 0 )
        R[0] = stack[tos--] + stack[tos--];
}

template <typename T>
void HSM<T>::sub(){
    if( tos > 0 )
        R[0] = -stack[tos--] + stack[tos--];
}

template <typename T>
void HSM<T>::mul(){
    if( tos > 0 )
        R[0] = stack[tos--] * stack[tos--];
}

template <typename T>
void HSM<T>::div(){
    if( tos > 0 ){
        T n1 = stack[ tos ];
        T n2 = stack[tos -1];
        R[0] = n1 / n2;
        tos -= 2;
    }
}

template <typename T>
void HSM<T>::mod(){
    if( typeid(T) != typeid(int) && typeid(T) != typeid(char) )
        this->nop();
    else
        if( tos > 0 )
            R[0] = stack[tos--] % stack[tos--];
}

// Logics ========================================================================================================================
template <typename T>
void HSM<T>::NOT(){
    stack[tos] = -stack[tos];
}

template <typename T>
void HSM<T>::OR(){
    stack[tos] = -stack[tos];
}

template <typename T>
void HSM<T>::AND(){
    stack[tos] = -stack[tos];
}

template <typename T>
void HSM<T>::MIR(){
    stack[tos] = -stack[tos];
}

// Control =======================================================================================================================
template <typename T>
void HSM<T>::push( T n ){
    stack[ ++tos ] = n;
}

template <typename T>
void HSM<T>::pushR( int r ){
    stack[ ++tos ] = R[ r ];
}

template <typename T>
void HSM<T>::pop(){    
    if( tos >= 0 ){
        stack[ tos-- ] = 0;
    }
}

template <typename T>
void HSM<T>::pop( int r ){
    if( tos >= 0 ){
        R[ r ] = stack[ tos ];
        stack[ tos-- ] = 0;
    }
}

// I/O =======================================================================================================================
template <typename T>
void HSM<T>::out(){
    if( tos >= 0 )
        cout << stack[ tos ] << endl;
    else
        cout << "== EMPTY STACK ==" << endl;
}

// JUMP's =======================================================================================================================
template <typename T>
void HSM<T>::jmp( int n ){
    line_execution += n - 1 ;
    if( line_execution < 0 )
        line_execution = -1;
}

template <typename T>
void HSM<T>::jz( int n ){
    if( !stack[ tos ] ){
        line_execution += n - 1;
        if( line_execution < 0 )
            line_execution = 0;
    }
}


template <typename T>
void HSM<T>::jnz( int n ){
    if( stack[ tos ] ){
        line_execution += n - 1;
        if( line_execution < 0 )
            line_execution = 0;
    }
}


template <typename T>
void HSM<T>::jzr( int n , int r ){
    if( !R[ r ] ){
        line_execution += n - 1;
        if( line_execution < 0 )
            line_execution = 0;
    }
}

template <typename T>
void HSM<T>::jnzr( int n , int r ){
    if( R[ r ] ){
        line_execution += n - 1;
        if( line_execution < 0 )
            line_execution = 0;
    }
}


// Others =======================================================================================================================
template <typename T>
void HSM<T>::reset(){
    tos = -1;
    
    // Clear Stack Memory
    for( int i = 0 ; i < height ; i++ )
        stack[ i ] = 0;

    // Clear Registers
    for( int r = 0 ; r < QTD_REGISTERS ; r++ )
        R[ r ] = 0;

    flag_execution = false;
    line_execution = -1;
    delay_execution = false;
    flag_error = false;
}

template<typename T>
int HSM<T>::step( HSMProgram *prog ){
    string str;
    string aux;
    int n;
    int r;
    T value;
    int pos;
    int flag_push_who_v;
    int flag_jump_who_v;
    int flag_blank_line;

    if( line_execution < prog->get_qtd_lines() - 1 ){
        line_execution++;
        str = prog->get_program_line( line_execution );

        if( contain(str , "##") )
            return 1;

        flag_blank_line = true;
        for( int k = 0 ; k < str.size() ; k++ )
            if( str[ k ] != ' ' && str[ k ] != '\n' && str[ k ] != '\r' && str[ k ] != '\b'&& str[ k ] != '\t' ){
                flag_blank_line = false;
                break;
            }
            
        if(!flag_blank_line ){
            if( contain( str , "out" ) ){
                this->out();
            }else if( contain( str , "add" ) )     
                this->add();
            else if( contain( str , "sub" ) )     
                this->sub();
            else if( contain( str , "mul" ) )     
                this->mul();
            else if( contain( str , "div" ) )     
                this->div();
            else if( contain( str , "mod" ) )     
                this->mod();
            else if( contain( str , "NOT" ) )     
                this->NOT();
            else if( contain( str , "OR" ) )     
                this->OR();
            else if( contain( str , "AND" ) )     
                this->AND();
            else if( contain( str , "MIR" ) )     
                this->MIR();
            else if( contain( str , "pop" ) ){
                if( tos < 0 )
                    return ERROR_3_POP;

                int space = str.find("pop") + 3;
                string inst_e = str.substr( 0 , space );
                string arg = str.substr( space);
                
                if( contain( arg , "$R" )
                || flag_push_who_v ){
                    if( contain( str , "$R6" ) )
                        this->pop( 5 );
                    else if( contain( str , "$R5" ) )
                        this->pop( 4 );
                    else if( contain( str , "$R4" ) )
                        this->pop( 3 );
                    else if( contain( str , "$R3" ) )
                        this->pop( 2 );
                    else if( contain( str , "$R2" ) )
                        this->pop( 1 );
                    else 
                        this->pop();
                        
                }else 
                    this->pop();
            }     
            else if( contain( str , "push" ) ){
                if( tos >= STACK_SIZE_DEFAULT - 1 )
                    return ERROR_4_PUSH;

                pos = str.find( "push" );
                flag_push_who_v = true;

                for( int k = pos + 4 ; k < str.size() ; k++ )
                    if( str[ k ] != ' ' && str[ k ] != '\n'  )
                        flag_push_who_v = false;

                if( contain( str , "$R" )
                || flag_push_who_v ){
                    if( contain( str , "$R6" ) )
                        this->pushR( 5 );
                    else if( contain( str , "$R5" ) )
                        this->pushR( 4 );
                    else if( contain( str , "$R4" ) )
                        this->pushR( 3 );
                    else if( contain( str , "$R3" ) )
                        this->pushR( 2 );
                    else if( contain( str , "$R2" ) )
                        this->pushR( 1 );
                    else 
                        this->pushR();
                        
                }
                else{
                    value = stoi( str.substr( pos + 4 ) );
                    this->push( value );
                }
            }else if( contain( str , "jmp" ) || contain( str , "jz" ) || contain( str , "jnz" ) || contain( str , "jzr" ) || contain( str , "jnzr" ) ){
                int space; 
                if( contain( str , "jmp" ) )
                    space = str.find("jmp") + 3;
                if( contain( str , "jz" ) )
                    space = str.find("jz") + 2;
                if( contain( str , "jnz" ) )
                    space = str.find("jnz") + 3;
                if( contain( str , "jzr" ) )
                    space = str.find("jzr") + 3;
                if( contain( str , "jnzr" ) )
                    space = str.find("jnzr") + 4;

                string inst_e = str.substr( 0 , space );
                string arg = str.substr( space);
                
                
                if( !contain( str , "jzr" ) && !contain( str , "jnzr" ) ){
                    // aux = str.substr( str.find("jmp") );
                    // aux.erase( remove( aux.begin() , aux.end() , ' ' ) , aux.end() );
                    inst_e.erase( remove( inst_e.begin() , inst_e.end() , ' ' ) , inst_e.end() );
                    arg.erase( remove( arg.begin() , arg.end() , ' ' ) , arg.end() );
                    n = atoi( arg.c_str() );
                    // line_execution += n + ( n > 0 ? 1 : -1 );
                    if( contain( str , "jmp" ) )
                        this->jmp( n );
                    if( contain( str , "jz" ) )
                        this->jz( n );
                    if( contain( str , "jnz" ) )
                        this->jnz( n );
                }else{
                    inst_e.erase( remove( inst_e.begin() , inst_e.end() , ' ' ) , inst_e.end() );
                    string arg1;
                    string arg2;
                    int sep = 0;
                    while( arg[ sep ] == ' ' )
                            sep++;

                    arg.erase( remove( arg.begin() , arg.begin() + sep , ' ' ) , arg.begin() + sep );

                    sep = 0;
                    while( arg[ sep ] != ' ' )
                        sep++;
                    arg1 = arg.substr( 0 , sep );
                    
                    arg2 = arg.substr( sep );
                    arg2.erase( remove( arg2.begin() , arg2.end() , ' ') , arg2.end() );

                    int n1 = atoi( arg1.c_str() );

                    int n2;
                    if( arg2 == "$R2")
                        n2 = 1;
                    if( arg2 == "$R3")
                        n2 = 2;
                    if( arg2 == "$R4")
                        n2 = 3;
                    if( arg2 == "$R")
                        n2 = 0;

                    if( contain( inst_e , "jzr" ) )
                        this->jzr( n1 , n2 );
                    if( contain( inst_e , "jnzr" ) )
                        this->jnzr( n1 , n2 );
                }
            }else{
                return ERROR_1_INVALID_INSTRUCTION;
            } 
        }
    }else{
        flag_execution = 0;
        line_execution = -1;
    }

    return 1;
}

template<typename T>
void HSM<T>::stop_error( int error_cod ){
    flag_execution = false;
    flag_error = true;

    tipo_erro = error_cod - 10;
}

/** run: Interpret the program
 * @brief 
 * 
 * @return int : OK_RUN if all is ok and error_code if there is some error
 */

template<typename T>
int HSM<T>::run( HSMProgram *prog ){
    int retorno = -1;
    static clock_t time_last_line = -DELAY_EXECUTION;  // Time in miliseconds 

    if( flag_error ){
        flag_error = 0;
        line_execution = -1;
    }
    if( (1000.0 * clock()) / CLOCKS_PER_SEC - time_last_line >= DELAY_EXECUTION ){
        retorno = step( prog );

        if( retorno >= 10 )
            stop_error( retorno );

        time_last_line = (1000.0 * clock()) / CLOCKS_PER_SEC;
    }

    return retorno;
}




#endif  //__HANOI_SM_HPP_



