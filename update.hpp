#ifndef __UPDATE_HPP_
#define __UPDATE_HPP_

#include <iostream>
using namespace std;
#include "raylib.h"
#include "definitions.hpp"
#include "HSMProgram.hpp"

template <typename FUNC_T , typename ARG_T >
class BUTTON{
    Rectangle pos;
    Rectangle pos_default;
    Color cor;
    std::string title;
    bool flag_selected;
    FUNC_T (*function)( ARG_T );

    public:
    BUTTON( Rectangle rec , Color cor , string name , FUNC_T (*func)( ARG_T ) ){
        this->pos_default = rec;
        this->pos = rec;
        this->cor = cor;
        this->title = name;
        this->flag_selected = false;

        this->function = func;
    }

    void call( ARG_T arg_prog ){ this->function( arg_prog ); }

    Rectangle get_pos_default(){ return pos_default; }
    Rectangle getpos(){ return pos; }
    void setpos( Rectangle pos ){ this->pos = pos; }
    void setx( float x ){ this->pos.x = x; }
    void sety( float y ){ this->pos.y = y; }
    void setwidth( float width ){ this->pos.width = width; }
    void setheight( float height ){ this->pos.height = height; }
    
    Color getcor(){ return cor; }
    void setcor( Color cor ){ this->cor = cor; }
   
    bool get_flag_selected(){ return flag_selected; }
    void set_flag_selected( bool status ){ flag_selected = status; }

    std::string gettitle(){ return title; }
};



template <typename FUNC_T , typename ARG_T >
void UpdateButton( BUTTON<FUNC_T,ARG_T> &button , ARG_T prog ){

    if( CheckCollisionPointRec( GetMousePosition() ,  button.getpos() ) ){
        button.set_flag_selected( true );
        button.setx( button.get_pos_default().x - BUTTONS_GROW_EFFECT );
        button.sety( button.get_pos_default().y - BUTTONS_GROW_EFFECT );
        button.setwidth( button.get_pos_default().width + 2 * BUTTONS_GROW_EFFECT );
        button.setheight( button.get_pos_default().height + 2 * BUTTONS_GROW_EFFECT );
        if( IsMouseButtonPressed( MOUSE_LEFT_BUTTON ) )
            button.call( prog );
        
    }else{
        button.set_flag_selected( false );
        button.setpos( button.get_pos_default() );
    }

}

template <typename FUNC_T , typename ARG_T >
void UpdateButton_Circle( BUTTON<FUNC_T,ARG_T> &button , HSMProgram **prog ){

    if( CheckCollisionPointCircle( GetMousePosition() , (Vector2){button.getpos().x , button.getpos().y} , button.getpos().width  ) ){
        button.set_flag_selected( true );
        if( IsMouseButtonPressed( MOUSE_LEFT_BUTTON ) )
            button.call( prog );
        
    }else{
        button.set_flag_selected( false );
        button.setpos( button.get_pos_default() );
    }

}

void UpdateStack( int &sfv , bool &f_u , bool &f_d ){
    static bool delay = false;

    if( !delay ){
        if( sfv < 127 - STACK_QTD_PAG )
            if( IsKeyDown( KEY_F2 ) )
                sfv++, f_u = delay = true;
        
        if( sfv > 0 )
            if( IsKeyDown( KEY_F3 ) )
                sfv-- , f_d = delay = true;
    }else
        delay = false;
}




void UpdateMouseCursor(){
    if( CheckCollisionPointRec( GetMousePosition() , EDITOR_REC ) )
        SetMouseCursor( MOUSE_CURSOR_IBEAM );
    else
        SetMouseCursor( MOUSE_CURSOR_DEFAULT );

}
#endif  // __UPDATE_HPP_