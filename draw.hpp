#ifndef __DRAW_HPP_
#define __DRAW_HPP_

#include "raylib.h"
#include "definitions.hpp"
#include "Hanoi_SM.hpp"
#include "HSMProgram.hpp"
#include "update.hpp"
#include "utils.hpp"

template <typename FUNC_T , typename ARG_T >
void DrawButton( BUTTON<FUNC_T,ARG_T> button ){
    float font_size = 4.5 * button.getpos().height / 5;

    DrawRectangleRec( button.getpos() , BUTTONS_COLOR_BG );
    DrawRectangleLinesEx( button.getpos() , BUTTONS_LINE_THICK , button.getcor() );
    DrawText( 
        button.gettitle().c_str() , 
        button.getpos().x + ( button.getpos().width - MeasureText( button.gettitle().c_str() , font_size ) ) / 2, 
        button.getpos().y + ( button.getpos().height - font_size + DEFAULT_FONT_HEIGHT_ERROR_CORRECTION_FACTOR ) / 2  , 
        font_size , 
        BLACK 
    );


}

template <typename FUNC_T , typename ARG_T >
void DrawButton_Circle( BUTTON<FUNC_T,ARG_T> button ){
    float font_size = 4.5 * button.getpos().height / 5;
    float radius = button.get_pos_default().width; 
    float borda = button.get_pos_default().height;

    DrawCircle( 
        button.getpos().x, 
        button.getpos().y, 
        radius, 
        button.getcor() 
    );
    DrawCircle( 
        button.getpos().x, 
        button.getpos().y, 
        radius - borda, 
        button.get_flag_selected() ? GOLD : BUTTONS_COLOR_BG 
    );
}

void DrawArrowPlay(){
    const float semi_lado = 2 * ( BUTTON_PLAY.width - BUTTON_PLAY.height ) / 3.0f;
    Vector2 center = (Vector2){ BUTTON_PLAY.x , BUTTON_PLAY.y };
    Vector2 a;
    Vector2 b;
    Vector2 c;
    a = b = c = center;

    a.x -= semi_lado; 
    b.x -= semi_lado; 
    c.x += 1.2 * semi_lado; 
    
    a.y -= semi_lado;
    b.y += semi_lado;

    DrawTriangle( a , b , c , BUTTON_PLAY_COLOR );
}


void DrawStackStructure( int begin , bool f_up , bool f_down ){
    int last = begin + STACK_QTD_PAG - 1;

    for( int n = begin ; n <= last ; n++ ){
        DrawRectangle( 
            STACK_REC_X,
            WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ),
            STACK_REC_WIDTH,
            STACK_REC_HEIGHT,
            (n == last && n < 127 || n == begin && begin > 0 ) ? RAYWHITE : STACK_FIELD_BG_COLOR
        );
        DrawRectangleLines( 
            STACK_REC_X,
            WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ),
            STACK_REC_WIDTH,
            STACK_REC_HEIGHT,
            STACK_BORDA_COLOR
        );

        // Index
        if( n != last )
            DrawText( 
                TextFormat("%3d" , n) , 
                STACK_REC_X - 5 - MeasureText( TextFormat("%3d" , n) , STACK_INDEX_FZ ) , 
                WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ) + ( STACK_REC_HEIGHT - STACK_INDEX_FZ ) / 2,
                STACK_INDEX_FZ,
                WHITE
            );

        // Arrows
        if( n == last )
            if( n < 127 ){
                Vector2 vec_base = (Vector2){ 
                    STACK_REC_X + STACK_REC_WIDTH / 2 , 
                    WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ) + 4 * STACK_REC_HEIGHT / 5.0f
                };

                Vector2 va , vb , vc;
                va = vb = vc = vec_base;
                va.x -= 3 * STACK_REC_HEIGHT / 5.0;
                vb.x += 3 * STACK_REC_HEIGHT / 5.0;
                vc.y -= 3 * STACK_REC_HEIGHT / 5.0; 

                DrawTriangle( 
                    va,
                    vb,
                    vc,
                    f_up ? GOLD : STACK_BORDA_COLOR
                );

                DrawText( 
                    "[F2]" , 
                    STACK_REC_X + STACK_REC_WIDTH  - 30 ,  
                    WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ) + (STACK_REC_HEIGHT - STACK_INDEX_FZ) / 2,
                    STACK_INDEX_FZ,
                    f_up ? GOLD : STACK_BORDA_COLOR
                );
            }
        if( n == begin )
            if( begin > 0 ){
                Vector2 vec_base = (Vector2){ 
                    STACK_REC_X + STACK_REC_WIDTH / 2 , 
                    WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ) + 1 * STACK_REC_HEIGHT / 5.0f
                };

                Vector2 va , vb , vc;
                va = vb = vc = vec_base;
                va.x -= 3 * STACK_REC_HEIGHT / 5.0;
                vb.x += 3 * STACK_REC_HEIGHT / 5.0;
                vc.y += 3 * STACK_REC_HEIGHT / 5.0; 

                DrawTriangle( 
                    va,
                    vc,
                    vb,
                    f_down ? GOLD : STACK_BORDA_COLOR
                );

                DrawText( 
                    "[F3]" , 
                    STACK_REC_X + STACK_REC_WIDTH  - 30 ,  
                    WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * ( n - begin + 1 ) + (STACK_REC_HEIGHT - STACK_INDEX_FZ) / 2,
                    STACK_INDEX_FZ,
                    f_down ? GOLD : STACK_BORDA_COLOR
                );
            }
        
    }
}

template<typename T>
void DrawStack( HSM<T> &hsm , int first , bool f_u , bool f_d ){
    
    /// STRUCTURE
    DrawStackStructure( first , f_u , f_d );

    /// VALUES
    for( int n = !first ? first : first + 1 ; n < first + STACK_QTD_PAG - 1 ; n++ ){
    // for( int n = !first ? first : first + 1 ; n < first + STACK_QTD_PAG - (first!= hsm.get_height() - STACK_QTD_PAG ) ; n++ ){
        string nmr_txt =  to_string( hsm.get( n ) );

        DrawText( 
            nmr_txt.c_str() , 
            STACK_REC_X + ( STACK_REC_WIDTH - MeasureText( nmr_txt.c_str(), STACK_FZ ) ) / 2 , 
            WINDOW_HEIGHT - STACK_MARGEM - (float)STACK_REC_HEIGHT * ( n - first + 1 ) ,
            STACK_FZ ,
            STACK_VALUE_COLOR   
        );
    }
    
    /// TOS
    {
        Vector2 vec_s = (Vector2){
            STACK_REC_X - 30,
            WINDOW_HEIGHT - STACK_MARGEM - (float)STACK_REC_HEIGHT * ( hsm.get_tos() - first + 1 ),
        };
        Vector2 vec_e = (Vector2){
            STACK_REC_X + STACK_REC_WIDTH + STACK_MARGEM / 2,
            WINDOW_HEIGHT - STACK_MARGEM - static_cast<float>( STACK_REC_HEIGHT * ( hsm.get_tos() - first + 1 ) ),
        };
        DrawLineEx(
            vec_s,
            vec_e,
            STACK_TOS_THICK,    
            GOLD
        );

        DrawText( 
            "TOP" , 
            STACK_REC_X - 30 - 50 , 
            WINDOW_HEIGHT - STACK_MARGEM - static_cast<float>( STACK_REC_HEIGHT * ( hsm.get_tos() - first + 1 ) ) - STACK_TOS_FZ / 2.0f , 
            STACK_TOS_FZ , 
            GOLD 
        );
    }


}

#endif  // __DRAW_HPP_