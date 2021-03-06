#include <iostream>
#include <fstream>

#include "raylib.h"

#include "definitions.hpp"
#include "Hanoi_SM.hpp"
#include "HSMProgram.hpp"
#include "update.hpp"
#include "draw.hpp"
#include "editor.hpp"
#include "buttons_functions.hpp"



int main(){
    InitWindow( WINDOW_WIDTH , WINDOW_HEIGHT , "HANOI StackMachine" );
    SetExitKey(0);
    SetTargetFPS( 30 );

    // Interface
    EDITOR editor;
    BUTTON<void, HSMProgram**, int> button_load( BUTTON_LOAD , BUTTON_LOAD_COLOR , "LOAD" , load_function );
    BUTTON<void, HSMProgram*, int> button_save( BUTTON_SAVE , BUTTON_SAVE_COLOR , "SAVE" , save_function );
    BUTTON<void, HSMProgram*, int> button_play( BUTTON_PLAY , BUTTON_PLAY_COLOR , "PLAY" , play_function );
    BUTTON<void, HSMProgram*, int> button_step( BUTTON_STEP , BUTTON_STEP_COLOR , "STEP" , step_function );
    BUTTON<void, HSMProgram*, int> button_restart( BUTTON_RESTART , BUTTON_RESTART_COLOR , "RESTART" , restart_function );
    Texture2D foot = LoadTexture("foot.png");
    Texture2D loop = LoadTexture("loop.png");
    int stack_first_visible = 0;
    bool stack_f_up = false;
    bool stack_f_down = false;
    
    // Stack Machine
    HSM<int> hsm;
    HSMProgram *prog = new HSMProgram;
 
    while( !WindowShouldClose() ){
        UpdateMouseCursor( prog );
        UpdateButton( button_load , &prog );
        UpdateButton( button_save , prog );  
        UpdateButton_Circle( button_play , &prog , &hsm);
        UpdateButton_Circle( button_step , &prog , &hsm);
        UpdateButton_Circle( button_restart , &prog , &hsm);
        if( hsm.flag_execution )
            hsm.run(prog);

        stack_f_down = stack_f_up = false;
        UpdateStack( stack_first_visible , stack_f_up , stack_f_down );
        editor.UpdateEditor( prog );

        BeginDrawing();
        ClearBackground( WINDOW_BG_COLOR );

            editor.DrawEditor( prog );

            DrawStack( hsm , stack_first_visible , stack_f_up , stack_f_down );
            DrawButton( button_load );
            DrawButton( button_save );
            DrawButton_Circle( button_play );
            DrawButton_Circle( button_step );
            DrawButton_Circle( button_restart );
            DrawArrowPlay();
            DrawFootPrint( foot );
            DrawLoop( loop );
            DrawRegisters( &hsm );
            DrawLineIndicator( &hsm , prog );
            DrawScrollBar( prog );
            DrawCorrectionEditor();

        EndDrawing();

    }
    
    CloseWindow();
    return 0;
}



