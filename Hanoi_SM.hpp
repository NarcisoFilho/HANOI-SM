#ifndef __HANOI_SM_HPP_
#define __HANOI_SM_HPP_

#include <iostream>
#include <typeinfo>
#include <ctime>
#include "HSMProgram.hpp"
#include "utils.hpp"
using namespace std;

#define STACK_SIZE_DEFAULT 128


/** class HSM : Stack Machine
 * @brief Uma máquina de pilha com diversas operações
 * 
 * 
 */
template <typename T = int>
class HSM{
    T R;            // Unic Machine's Register
    T *stack;       // Stack HSM memory region
    int tos;        // Top of the stack
    int height;     // Stack's height ( Number of Elements )
    long long int size;       // Stack's size in Bytes
    
    public:
    bool flag_execution;
    int line_execution;
    bool delay_execution;

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
    void push();
    void pop();

    // I/O
    void out();

    // Others
    void nop(){}
    void reset();
    int run( HSMProgram* );

    // Just for draw and debuging
    T get( int n ){ return stack[ n ]; }
    int get_tos(){ return tos; }
    int get_height(){ return height; }


    // Execution
    int step( HSMProgram* );
    void stop_error();

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
        R = stack[tos--] + stack[tos--];
}

template <typename T>
void HSM<T>::sub(){
    if( tos > 0 )
        R = -stack[tos--] + stack[tos--];
}

template <typename T>
void HSM<T>::mul(){
    if( tos > 0 )
        R = stack[tos--] * stack[tos--];
}

template <typename T>
void HSM<T>::div(){
    if( tos > 0 )
        R = stack[tos--] / stack[tos--];
}

template <typename T>
void HSM<T>::mod(){
    if( typeid(T) != typeid(int) && typeid(T) != typeid(char) )
        this->nop();
    else
        if( tos > 0 )
            R = stack[tos--] % stack[tos--];
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
void HSM<T>::push(){
    stack[ ++tos ] = R;
}

template <typename T>
void HSM<T>::pop(){
    stack[ tos-- ] = 0;
}

// I/O =======================================================================================================================
template <typename T>
void HSM<T>::out(){
    if( tos >= 0 )
        cout << stack[ tos ] << endl;
    else
        cout << "== EMPTY STACK ==" << endl;
}

// Others =======================================================================================================================
template <typename T>
void HSM<T>::reset(){
    tos = -1;
    
    // Clear Stack Memory
    for( int i = 0 ; i < height ; i++ )
        stack[ i ] = 0;

    flag_execution = false;
    line_execution = -1;
    delay_execution = false;

}

template<typename T>
int HSM<T>::step( HSMProgram *prog ){
    string str;
    T value;
    int pos;
    int flag_push_who_v;
    int flag_blank_line;

    if( line_execution < prog->get_qtd_lines() - 1 ){
        line_execution++;
        str = prog->get_program_line( line_execution );
        flag_blank_line = true;
        for( int k = 0 ; k < str.size() ; k++ )
            if( str[ k ] != ' ' && str[ k ] != '\n' && str[ k ] != '\r' && str[ k ] != '\b'&& str[ k ] != '\t' ){
                flag_blank_line = false;
                break;
            }
            
        if(!flag_blank_line ){
            if( contain( str , "out" ) )          
                this->out();
            else if( contain( str , "add" ) )     
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
            else if( contain( str , "pop" ) )     
                this->pop();        
            else if( contain( str , "push" ) ){
                pos = str.find( "push" );
                flag_push_who_v = true;

                for( int k = pos + 4 ; k < str.size() ; k++ )
                    if( str[ k ] != ' ' && str[ k ] != '\n'  )
                        flag_push_who_v = false;

                if( contain( str , "$R" )
                || flag_push_who_v )
                    this->push();
                else{
                    value = stoi( str.substr( pos + 4 ) );
                    this->push( value );
                }
            }else 
                return 0;
        }
    }else{
        flag_execution = 0;
        line_execution = -1;
    }


    return 1;
}

template<typename T>
void HSM<T>::stop_error(){
    flag_execution = false;
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

    if( (1000.0 * clock()) / CLOCKS_PER_SEC - time_last_line >= DELAY_EXECUTION ){
        retorno = step( prog );

        if( !retorno )
            stop_error();

        time_last_line = (1000.0 * clock()) / CLOCKS_PER_SEC;
    }

    return retorno;
}



#endif  //__HANOI_SM_HPP_



