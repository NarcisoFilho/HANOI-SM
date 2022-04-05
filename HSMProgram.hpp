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
};


HSMProgram::HSMProgram(){
    file_name = "Unsaved";
    qtd_lines = static_cast<int>( static_cast<float>( EDITOR_REC.height ) / EDITOR_TEXT_LINE_HEIGHT );

    // Allocation enough space to program
    program = new string[ qtd_lines ];
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





#endif  // __HSMPROGRAM_HPP_
