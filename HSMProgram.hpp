#ifndef __HSMPROGRAM_HPP_
#define __HSMPROGRAM_HPP_

#include <iostream>
#include <fstream>

using namespace std;


int count_file_lines( fstream & );

/** HSMProgram: HANOI Stack Machine Program
 * @brief One program for the machine
 * 
 */
class HSMProgram{
    string file_name;
    string *program;
    int qtd_lines;

    public:
    HSMProgram();
    HSMProgram( string );
    ~HSMProgram();

    string get_program_line( int );
    int get_qtd_lines(){ return qtd_lines; }

    void print();
    friend int count_file_lines( fstream& );
    void insert_char_prog_line( char ch , int col , int lin );
    void append_char_prog_line( char ch , int lin );
    void remove_char_prog_line( int col , int lin );
    void add_line();
    void add_line( int );
    void remove_line( int );
};


HSMProgram::HSMProgram(){
    file_name = "Unsaved";
    qtd_lines = static_cast<int>( static_cast<float>( EDITOR_REC.height ) / EDITOR_TEXT_LINE_HEIGHT );

    // Allocation enough space to program
    program = new string[ qtd_lines ];
    for( int k = 0 ; k < qtd_lines ; k++ )
        program[ k ] = "";

    program[ 0 ] = "##Welcome to HANOI StackMachine!";
    program[ 1 ] = "";
    program[ 2 ] = "    ##Write your code here ";
    program[ 3 ] = "    ##or load one file ";
    program[ 4 ] = "    ##Execute it in the green arrow button";
}

HSMProgram::HSMProgram( string file_name ){
    fstream file( file_name.c_str() , ios::in );

    qtd_lines = count_file_lines( file );
    this->file_name = file_name;


    // Allocation enough space to program
    program = new string[ qtd_lines ];

    // Assign program
    file.clear();
    file.seekg(0);    // Restoring to beginning the file pointer

    for( int i = 0 ; i < qtd_lines; i++ )
        getline( file , program[ i ]  ) ; 

    file.close();
}


HSMProgram::~HSMProgram(){
    if( program != NULL )
        delete[] program;
}

void HSMProgram::print(){
    for( int i = 0 ; i < qtd_lines ; i++ )
        cout << program[ i ] << endl;

    

}

int count_file_lines( fstream &file ){
    string buf;
    int qtd_lines = 0;

    // Counting number of lines of the program
    while( !file.eof() ){
        getline( file , buf );
        qtd_lines++;
    }
    file.seekg(0);    // Restoring to beginning the file pointer

    return qtd_lines;
}

string HSMProgram::get_program_line( int line ){
    string s_nop = "nop";

    if( line < qtd_lines )
        return program[line];
    else
        return s_nop;
}

void HSMProgram::insert_char_prog_line( char ch , int col , int lin ){
    int last = program[ lin ].length() - 1;
    
    //if( col >= 0 && col < program[ lin ].length() )
    {
        program[lin].append(" ");
        for( int i = last + 1 ; i > col ; i-- )
            program[lin][ i ] = program[lin][ i - 1 ];
        //program[ lin ].insert( program[ lin ].begin() + col , ch );
        program[ lin ][ col ] = ch;
    }   
}

void HSMProgram::append_char_prog_line( char ch , int lin ){
    program[lin] += ch; 
}

void HSMProgram::remove_char_prog_line( int col , int lin ){
        program[ lin ].erase( col , 1 );   
}

void HSMProgram::add_line(){
    string *new_program = new string[ ++qtd_lines ];;

    for( int i = 0 ; i < qtd_lines - 1 ; i++ )
        new_program[ i ] = program[ i ];
    
    delete[] program;

    program = new_program;

    program[ qtd_lines - 1 ] = "";
}

void HSMProgram::add_line( int n ){
    string *new_program = new string[ ++qtd_lines ];
    int i = 0 , j = 0;

    while(  j < qtd_lines - 1 ){
        new_program[ i ] = program[ j ];
        i++;
        j++;
        if( j == n ) i++;
    }
    
    delete[] program;

    program = new_program;

    program[ n ] = "";
}


void HSMProgram::remove_line( int n ){
    string *new_program = new string[ --qtd_lines ];
    int i = 0 , j = 0;

    while(  i < qtd_lines ){
        new_program[ i ] = program[ j ];
        i++;
        j++;
        if( i == n ) j++;
    }
    
    delete[] program;

    program = new_program;
}



#endif  // __HSMPROGRAM_HPP_
