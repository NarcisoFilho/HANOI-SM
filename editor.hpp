#ifndef __EDITOR_HPP_
#define __EDITOR_HPP_

#include <iostream>
#include <string>

using namespace std;

#include "raylib.h"
#include "definitions.hpp"
#include "HSMProgram.hpp"
#include "utils.hpp"

class EDITOR{
    int current_lin;
    int current_col;

    public:
    EDITOR(){
        current_lin = 0;
        current_col = 0;
    }

    void UpdateEditor( HSMProgram * );
    void DrawEditor( HSMProgram * );
    void print_editor_content( HSMProgram & );


};

void EDITOR::UpdateEditor( HSMProgram *prog ){
    // Control
    if( IsKeyPressed( KEY_DOWN ) )
        current_lin++;
    if( current_lin > 0 )
        if( IsKeyPressed( KEY_UP ) )
            current_lin--;
    if( IsKeyPressed( KEY_RIGHT ) )
        current_col++;
    if( current_col > 0 )
        if( IsKeyPressed( KEY_LEFT ) )
            current_col--;

}

void EDITOR::print_editor_content( HSMProgram &prog ){
    string inst;
    Color color;

    for( int l = 0 ; l < prog.get_qtd_lines() ; l++ ){
        inst = prog.get_program_line( l );

        if( inst == "add"
        || inst == "sub"
        || inst == "mul"
        || inst == "div"
        || inst == "mod" )
            color = EDITOR_HIGHLIGHT_ARITHIMETIC_INSTRUCTION;
        else if( inst == "NOT"
        || inst == "OR"
        || inst == "AND"
        || inst == "MIR" )
            color = EDITOR_HIGHLIGHT_LOGIC_INSTRUCTION;
        else if( inst == "pop"
        || inst == "ifz"
        || inst == "ifnz" )
            color = EDITOR_HIGHLIGHT_CONTROL_INSTRUCTION;
        else if( inst == "OUT" )
            color = EDITOR_HIGHLIGHT_IO_INSTRUCTION;
        
        if( !contain( inst , "push" ) )
            DrawText(
                inst.c_str(),
                EDITOR_REC.x + EDITOR_SPACING_TEXT_LEFT,
                EDITOR_REC_Y + l * EDITOR_TEXT_LINE_HEIGHT,
                EDITOR_TEXT_FZ,
                color
            );
        else{
            int space = inst.find("push") + 5;
            string inst_e = inst.substr( 0 , space );
            string arg = inst.substr( inst.find("push") + 5);

            DrawText(
                inst_e.c_str(),
                EDITOR_REC.x + EDITOR_SPACING_TEXT_LEFT,
                EDITOR_REC_Y + l * EDITOR_TEXT_LINE_HEIGHT,
                EDITOR_TEXT_FZ,
                EDITOR_HIGHLIGHT_CONTROL_INSTRUCTION
            );
            DrawText(
                arg.c_str(),
                EDITOR_REC.x + EDITOR_SPACING_TEXT_LEFT + MeasureText( (inst_e+" ").c_str() , EDITOR_TEXT_FZ ),
                EDITOR_REC_Y + l * EDITOR_TEXT_LINE_HEIGHT,
                EDITOR_TEXT_FZ,
                contain( arg , "$R" ) ? EDITOR_HIGHLIGHT_REGISTER : EDITOR_HIGHLIGHT_CONSTANTE
            );
        }
    }

    // Cursor
    inst = prog.get_program_line( current_lin );
    static int flag_blind = 0;
    float cursor_x = EDITOR_MARGEM + EDITOR_SPACING_TEXT_LEFT + MeasureText( inst.substr(0 , current_col).c_str() , EDITOR_TEXT_FZ );
    float cursor_y = EDITOR_REC_Y + EDITOR_TEXT_LINE_HEIGHT * current_lin;

    
    if( flag_blind > 12 )
        DrawLineEx(
            (Vector2){ cursor_x, cursor_y },
            (Vector2){ cursor_x, cursor_y + EDITOR_TEXT_LINE_HEIGHT, },
            3,
            GREEN
        );

    if( flag_blind ) 
        flag_blind--;
    else
        flag_blind = 32;
}

void EDITOR::DrawEditor( HSMProgram *prog ){

    DrawRectangleRec( EDITOR_REC , RAYWHITE );

    if( prog != NULL )
        this->print_editor_content( *prog );
}


#endif  // __EDITOR_HPP_