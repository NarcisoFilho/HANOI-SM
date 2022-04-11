#ifndef __BUTTONS_FUNCTIONS_HPP_
#define __BUTTONS_FUNCTIONS_HPP_

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
    string title = "=== SAVE FILE ===";
    string sub_title = "Input with the file name:";
    string name = "";
    bool flag_cancelar = false;
    bool flag_coletado = false;
    const float font_size = 3 * NAME_COLECTOR_BOX_HEIGHT / 5.0;
    float title_fz = 50;
        
    do{
        if( GetKeyPressed() != 0 )
            if( !IsKeyPressed( KEY_BACKSPACE ) &&  !IsKeyPressed( KEY_ENTER ) )
                name.append( 1 , static_cast<char>( GetCharPressed() ) );
            
        if( IsKeyPressed( KEY_BACKSPACE ) )
            if( name.length() > 0 )
                name.pop_back();
                // name.resize( name.size() - 1 );

        BeginDrawing();
            DrawRectangleRec( NAME_COLECTOR_BOX_BG , NAME_COLECTOR_BOX_BG_COLOR );
            DrawRectangleLinesEx( NAME_COLECTOR_BOX_BG , 4 ,  DARKBROWN );
            DrawRectangleRec( NAME_COLECTOR_BOX , NAME_COLECTOR_BOX_INTERN_COLOR );
            DrawRectangleLinesEx( NAME_COLECTOR_BOX , NAME_COLECTOR_BOX_THICK , NAME_COLECTOR_BOX_EXTERN_COLOR );
            
            // Title
            DrawText( title.c_str() , NAME_COLECTOR_BOX_BG.x + ( NAME_COLECTOR_BOX_BG.width - MeasureText( title.c_str() ,title_fz ) ) /2 ,  NAME_COLECTOR_BOX_BG.y + 5 , title_fz , DARKBROWN );
            DrawText( sub_title.c_str() , NAME_COLECTOR_BOX_BG.x + ( NAME_COLECTOR_BOX_BG.width - MeasureText( sub_title.c_str() ,title_fz / 2 ) ) /2 ,  NAME_COLECTOR_BOX.y - title_fz - 3 , title_fz / 2 , DARKBROWN );

            // Filename
            DrawText( name.c_str() , NAME_COLECTOR_BOX_BG.x + ( NAME_COLECTOR_BOX_BG.width - MeasureText( name.c_str() ,font_size ) ) /2 ,  NAME_COLECTOR_BOX.y + ( NAME_COLECTOR_BOX.height - font_size ) / 2 , font_size , DARKBROWN );
            
        EndDrawing();

        if( IsKeyPressed( KEY_ENTER) )
            flag_coletado = true;
        if( IsKeyPressed( KEY_ESCAPE) 
        || IsMouseButtonPressed( MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(GetMousePosition(),NAME_COLECTOR_BOX_BG) )
            flag_cancelar = true;
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

    folder = opendir( FOLDER_DEFAULT_SAVES );

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


void save_function( HSMProgram *prog ){
    string file_name = "saves\\" + get_name();
    if( !contain(file_name, ".hsm")   )
        if( !contain(file_name, ".txt") )
            file_name += ".hsm";

    ofstream file( file_name , ios::out | ios::trunc );

    if( !file.is_open() ){
        cout << "\aCan't save file!!" << endl;
        BeginDrawing();
            DrawText( "\aERROR!!" , ( WINDOW_WIDTH - MeasureText("\aERROR!!" , 50 ) ) / 2 , ( WINDOW_HEIGHT - 50 ) / 2 , 50 , MAROON );
        EndDrawing();
    }else{
        for( int i = 0 ; i < prog->get_qtd_lines() ; i++ )
            file << ( prog->get_program_line( i ) ) << endl;

        file.close();
        cout << endl << "File saved: " << file_name << endl;
    }

}

template< typename T>
void play_function( HSMProgram *prog , HSM<T> *machine ){
    machine->line_execution = -1;
    machine->flag_execution = true;
}


#endif   // __BUTTONS_FUNCTIONS_HPP_
