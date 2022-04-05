#include <iostream>
#include <fstream>

#include "raylib.h"

#include "definitions.hpp"
#include "Hanoi_SM.hpp"
#include "HSMProgram.hpp"
#include "update.hpp"
#include "draw.hpp"
#include "editor.hpp"


#include "dirent.h"


template<typename T>
int run( HSM<T>&hsm , HSMProgram prog );


string get_name(){
    string name = "";
    bool flag_cancelar = false;
    bool flag_coletado = false;
    const float font_size = 4 * NAME_COLECTOR_BOX_HEIGHT / 5.0;
        
    do{
        BeginDrawing();
            DrawRectangleRec( NAME_COLECTOR_BOX , NAME_COLECTOR_BOX_INTERN_COLOR );
            DrawRectangleLinesEx( NAME_COLECTOR_BOX , NAME_COLECTOR_BOX_THICK , NAME_COLECTOR_BOX_EXTERN_COLOR );
            //DrawText()
        EndDrawing();

        if( IsKeyPressed( KEY_ENTER) )
            flag_coletado = true;
    }while( !flag_coletado && !flag_cancelar );

    return name;
}

string select_hsm_file(){
    string title = "==== LOAD FILE ====";
    string item_name;
    string file_name_selected;
    Rectangle rec_item;
    Color cor;
    int i;
    float title_fz = 45;
    float item_fz = 25;
    bool flag_cancelar = false;
    bool flag_coletado = false;
    bool flag_item_selected;
    const float font_size_T = 4 * NAME_COLECTOR_BOX_HEIGHT / 5.0;

    DIR *folder;
    struct dirent *item;

    folder = opendir("./saves/");

    if( folder != NULL ){
        do{
            BeginDrawing();
                // Moldura
                DrawRectangleRec( FILE_SELECTION_BOX , FILE_SELECTION_BOX_INTERN_COLOR );
                DrawRectangleLinesEx( FILE_SELECTION_BOX , FILE_SELECTION_BOX_THICK , FILE_SELECTION_BOX_EXTERN_COLOR );
                
                // Título
                DrawText( 
                    title.c_str() , 
                    FILE_SELECTION_BOX.x + ( FILE_SELECTION_BOX.width - MeasureText( title.c_str() , title_fz ) ) / 2 , 
                    FILE_SELECTION_BOX.y + FILE_SELECTION_BOX_THICK + 2 , 
                    title_fz ,
                    BLACK 
                );

                // Conteúdo
                for( int k = 0 ; k < 2 + 0 ; k++ )
                    readdir(folder);    // descarta alguns itens ( "." e ".." sempre)
                
                i = 1;
                flag_item_selected = false;
                while( item = readdir(folder)  ){
                    item_name = item->d_name;

                    rec_item = FILE_SELECTION_BOX;
                    rec_item.height = item_fz + 2;
                    rec_item.y = FILE_SELECTION_BOX.y + FILE_SELECTION_BOX_THICK + 35 + rec_item.height * i;

                    if( CheckCollisionPointRec( GetMousePosition() , rec_item) ){
                        cor = GOLD;
                        flag_item_selected = true;
                        file_name_selected = item_name;
                    }else
                        cor = DARKBROWN;
                    DrawText( 
                        item_name.c_str() , 
                        rec_item.x + ( rec_item.width - MeasureText( item_name.c_str() , item_fz ) ) / 2 , 
                        rec_item.y, 
                        item_fz ,
                        cor 
                    );

                    i++;
                }
                rewinddir(folder);
            EndDrawing();
           
            if( flag_item_selected )
                if( IsMouseButtonPressed( MOUSE_LEFT_BUTTON ) )
                    flag_coletado = true; 

            if( IsKeyPressed( KEY_ENTER) )
                flag_coletado = true;

            if( IsKeyPressed( KEY_ESCAPE ) 
            || IsMouseButtonPressed( MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(GetMousePosition(),FILE_SELECTION_BOX) )
                flag_cancelar = true;
                
        }while( !flag_coletado && !flag_cancelar );

        closedir(folder);

        if( flag_cancelar )
            file_name_selected = "CANCELED";

        return file_name_selected;
    }else
        return "CANCELED";
}
void load_function( HSMProgram **prog ){
    string nome( select_hsm_file() );
    string path = "saves\\" + nome;
    cout << "File Name: " << nome << endl; 
    
    if( nome != "CANCELED" ){
        if( *prog != NULL )
            delete *prog;

        try{
            *prog = new HSMProgram ( path );
        }catch( bad_alloc xa ){
            cout << "Hanoi SM porgram allocation Failure" << endl;
            exit( EXIT_FAILURE );
        }

        cout << "===== Program loaded sucefully =====" << endl;
        cout << "File Name: " << nome << endl; 
        (*prog)->print();
        
        // hsm.run( prog );
    }
}


void save_function( HSMProgram *prog , string name ){
}

int main(){
    InitWindow( WINDOW_WIDTH , WINDOW_HEIGHT , "HANOI StackMachine" );
    SetExitKey(0);
    SetTargetFPS( 30 );

    // Interface
    EDITOR editor;
    BUTTON button_load( BUTTON_LOAD , BUTTON_LOAD_COLOR , "LOAD" , load_function );
    BUTTON button_save( BUTTON_SAVE , BUTTON_SAVE_COLOR , "SAVE" , load_function );
    BUTTON button_play( BUTTON_PLAY , BUTTON_PLAY_COLOR , "PLAY" , load_function );
    int stack_first_visible = 0;
    bool stack_f_up = false;
    bool stack_f_down = false;
    
    // Stack Machine
    HSM<int> hsm;
    HSMProgram *prog = NULL;
 
    while( !WindowShouldClose() ){
        UpdateMouseCursor();
        UpdateButton( button_load , &prog );
        UpdateButton( button_save , &prog );
        UpdateButton_Circle( button_play , &prog );
        
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
            DrawArrowPlay();
        EndDrawing();

    }
    
    CloseWindow();
    return 0;
}



