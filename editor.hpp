#ifndef __EDITOR_HPP_
#define __EDITOR_HPP_

#include <iostream>
#include <string>
#include <algorithm>

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

Vector2 pixelToEditor_col_lin(  HSMProgram *prog , Vector2 pos_pixel = VECm1 ){
    // if( pos_pixel == VECm1 )
    if( pos_pixel.x == VECm1.x && pos_pixel.y == VECm1.y )
        pos_pixel = GetMousePosition();
    
    Vector2 col_lin;
    col_lin.y = static_cast<int>(pos_pixel.y - EDITOR_REC_Y ) / EDITOR_TEXT_LINE_HEIGHT;
    col_lin.x = 0;

    string txt = prog->get_program_line( col_lin.y );
    while( MeasureText( txt.substr(0 , col_lin.x ).c_str() , EDITOR_TEXT_FZ ) + EDITOR_SPACING_TEXT_LEFT + EDITOR_MARGEM < pos_pixel.x 
    && col_lin.x < txt.length() )
        col_lin.x++;

    return col_lin;
}

void EDITOR::UpdateEditor( HSMProgram *prog ){
    string str_aux;

    // Control
    if( IsKeyPressed( KEY_DOWN ) ){
        current_lin++;
        
        if( current_lin >= prog->get_qtd_lines() - 1 )
            prog->add_line();

        current_col = prog->get_program_line( current_lin ).length();

    }
    if( current_lin > 0 )
        if( IsKeyPressed( KEY_UP ) ){
            current_lin--;
            current_col = prog->get_program_line( current_lin ).length();
        }
    if( IsKeyPressed( KEY_RIGHT ) )
        if( current_col < prog->get_program_line( current_lin ).length() )
            current_col++;
    if( current_col > 0 )
        if( IsKeyPressed( KEY_LEFT ) )
            current_col--;

    if( IsKeyPressed( KEY_ENTER ) ){
        // add line
        str_aux = prog->get_program_line( current_lin );
        prog->add_line( current_lin );
        current_lin++;
        
        
        // Restore the content of line above
        for( int j =  current_col - 1 ; j >= 0 ; j-- ){
            prog->insert_char_prog_line( str_aux[ j ] , 0 , current_lin - 1 );
        }
        
        // Remove wrong content form actual line
        for( int j = 0; j < current_col ; j++ ){
            prog->remove_char_prog_line( 0 , current_lin );
        }
        // Transfer content for next line
        // for( int j = prog->get_program_line( current_lin - 1 ).length() - 1 ; j >= current_col ; j-- ){
        //     prog->insert_char_prog_line( prog->get_program_line( current_lin - 1 )[ j ] , 0 , current_lin );
        // }

        // Column indicator to zero
        current_col = 0;
    }
    if( CheckCollisionPointRec( GetMousePosition() , EDITOR_REC ) ){
        if( IsMouseButtonPressed( MOUSE_LEFT_BUTTON ) ){
            current_col = pixelToEditor_col_lin( prog ).x;
            current_lin = pixelToEditor_col_lin( prog ).y;
        }

    }
    // END and HOME
    if( IsKeyPressed( KEY_END ) )
        current_col = prog->get_program_line( current_lin ).length();
    
    if( IsKeyPressed( KEY_HOME ) )
        current_col = 0;


    // Input text
    char ch;
    if( ( ch = GetCharPressed() ) != 0 ){
        prog->insert_char_prog_line( (char)ch , current_col , current_lin );
        current_col++;
    }

    // Remove Text
    if( IsKeyPressed( KEY_BACKSPACE ) ){
        if( current_col > 0 ){
            prog->remove_char_prog_line(current_col - 1 , current_lin );
            current_col--;
        }else
            if( current_lin > 0 ){
                str_aux = prog->get_program_line( current_lin );
                prog->remove_line( current_lin ); 
                current_lin--;
                current_col = prog->get_program_line( current_lin ).length();

                for( int j = 0 ; j < str_aux.length() ; j++ )
                    prog->append_char_prog_line( str_aux[ j ] , current_lin );
            }
    }
    if( IsKeyPressed( KEY_DELETE ) ){
        if( current_col <  prog->get_program_line( current_lin ).length() ){
            prog->remove_char_prog_line(current_col , current_lin );
        }else
            if( current_lin < prog->get_qtd_lines() - 1 ){
                str_aux = prog->get_program_line( current_lin + 1 );
                prog->remove_line( current_lin + 1 ); 
                for( int j = 0 ; j < str_aux.length() ; j++ )
                    prog->append_char_prog_line( str_aux[ j ] , current_lin );
            }
    }
}

void EDITOR::print_editor_content( HSMProgram &prog ){
    string inst , inst_ws;
    Color color;

    for( int l = 0 ; l < prog.get_qtd_lines() ; l++ ){
        inst = prog.get_program_line( l );
        inst_ws = inst;
        inst_ws.erase( remove( inst_ws.begin() , inst_ws.end() , ' ') , inst_ws.end() );
        
        color = EDITOR_DEFAULT_COLOR_TEXT;
        

        if( inst_ws == "add"
        || inst_ws == "sub"
        || inst_ws == "mul"
        || inst_ws == "div"
        || inst_ws == "mod" )
            color = EDITOR_HIGHLIGHT_ARITHIMETIC_INSTRUCTION;
        else if( inst_ws == "NOT"
        || inst_ws == "OR"
        || inst_ws == "AND"
        || inst_ws == "MIR" )
            color = EDITOR_HIGHLIGHT_LOGIC_INSTRUCTION;
        else if( inst_ws == "pop" )
            color = EDITOR_HIGHLIGHT_CONTROL_STACK_INSTRUCTION;
        else if( contain( inst , "jmp" )
        || contain( inst , "jz" )
        || contain( inst , "jnz" )
        || contain( inst , "jzr" )
        || contain( inst , "jnzr" ) )
            color = EDITOR_HIGHLIGHT_CONTROL_EXECUTION_INSTRUCTION;
        else if( inst_ws == "out" )
            color = EDITOR_HIGHLIGHT_IO_INSTRUCTION;
        
        if( contain( inst , "##" ) ||
        !( contain( inst , "push" ) || contain( inst , "jmp" ) || contain( inst , "jz" ) || contain( inst , "jnz" ) || contain( inst , "jzr" ) || contain( inst , "jnzr" ) ) )
            DrawText(
                inst.c_str(),
                EDITOR_REC.x + EDITOR_SPACING_TEXT_LEFT,
                EDITOR_REC_Y + l * EDITOR_TEXT_LINE_HEIGHT,
                EDITOR_TEXT_FZ,
                contain(inst , "##" ) ? EDITOR_HIGHLIGHT_COMMENT : color
            );
        else{
            int space; 
            if( contain( inst , "push" ) )
                space = inst.find("push") + 4;
            if( contain( inst , "jmp" ) )
                space = inst.find("jmp") + 3;
            if( contain( inst , "jz" ) )
                space = inst.find("jz") + 2;
            if( contain( inst , "jnz" ) )
                space = inst.find("jnz") + 3;
            if( contain( inst , "jzr" ) )
                space = inst.find("jzr") + 3;
            if( contain( inst , "jnzr" ) )
                space = inst.find("jnzr") + 4;

            string inst_e = inst.substr( 0 , space );
            string arg = inst.substr( space);

            DrawText(
                inst_e.c_str(),
                EDITOR_REC.x + EDITOR_SPACING_TEXT_LEFT,
                EDITOR_REC_Y + l * EDITOR_TEXT_LINE_HEIGHT,
                EDITOR_TEXT_FZ,
                contain( inst , "push" ) ? EDITOR_HIGHLIGHT_CONTROL_STACK_INSTRUCTION : EDITOR_HIGHLIGHT_CONTROL_EXECUTION_INSTRUCTION
            );
            DrawText(
                arg.c_str(),
                EDITOR_REC.x + EDITOR_SPACING_TEXT_LEFT + MeasureText( (inst_e ).c_str() , EDITOR_TEXT_FZ ),
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