#ifndef __DEFINITIONS_HPP_
#define __DEFINITIONS_HPP_

/// WINDOW
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define WINDOW_BG_COLOR CLITERAL(Color){ 100 , 100 , 100 , 255}

/// BUTTONS
#define BUTTONS_REC_ROUNDNESS .6
#define BUTTONS_LINE_THICK 4
#define BUTTONS_QTD_SEG 100
#define BUTTONS_COLOR_BG LIGHTGRAY
#define BUTTONS_GROW_EFFECT 3

#define BUTTON_LOAD (Rectangle){20 , 20 , 150 , 40 }
#define BUTTON_LOAD_COLOR GOLD
#define BUTTON_SAVE (Rectangle){ BUTTON_LOAD.x + BUTTON_LOAD.width + 20 , 20 , 150 , 40 }
#define BUTTON_SAVE_COLOR GOLD
#define BUTTON_PLAY (Rectangle){ WINDOW_WIDTH - 130 , 50 , 25 , 4 }
#define BUTTON_PLAY_COLOR DARKGREEN

// NAME COLECTOR BOX
#define NAME_COLECTOR_BOX_EXTERN_COLOR GOLD
#define NAME_COLECTOR_BOX_INTERN_COLOR RAYWHITE
#define NAME_COLECTOR_BOX_THICK 4
#define NAME_COLECTOR_BOX_WIDTH ( .7 * WINDOW_WIDTH )
#define NAME_COLECTOR_BOX_HEIGHT ( WINDOW_HEIGHT / 10 )

#define NAME_COLECTOR_BOX (Rectangle){                          \
            (WINDOW_WIDTH - NAME_COLECTOR_BOX_WIDTH) / 2 ,      \
            (WINDOW_HEIGHT - NAME_COLECTOR_BOX_HEIGHT) / 2,     \
            NAME_COLECTOR_BOX_WIDTH,                            \
            NAME_COLECTOR_BOX_HEIGHT                            \
        } 


#define NAME_COLECTOR_BOX_BG_COLOR CLITERAL(Color){ 120 ,120 ,120 , 45}
#define NAME_COLECTOR_BOX_BG (Rectangle){                          \
            (WINDOW_WIDTH - 1.2 * NAME_COLECTOR_BOX_WIDTH) / 2 ,      \
            (WINDOW_HEIGHT - 6 * NAME_COLECTOR_BOX_HEIGHT) / 2,     \
            1.2 * NAME_COLECTOR_BOX_WIDTH,                            \
            6 * NAME_COLECTOR_BOX_HEIGHT                            \
        } 

// FILE SELECTION BOX
#define FILE_SELECTION_BOX_EXTERN_COLOR DARKBROWN
#define FILE_SELECTION_BOX_INTERN_COLOR LIGHTGRAY
#define FILE_SELECTION_BOX_THICK 7
#define FILE_SELECTION_BOX_WIDTH ( WINDOW_WIDTH / 2 )
#define FILE_SELECTION_BOX_HEIGHT ( WINDOW_HEIGHT / 2 )

#define FILE_SELECTION_BOX (Rectangle){                          \
            (WINDOW_WIDTH - FILE_SELECTION_BOX_WIDTH) / 2 ,      \
            (WINDOW_HEIGHT - FILE_SELECTION_BOX_HEIGHT) / 2,     \
            FILE_SELECTION_BOX_WIDTH,                            \
            FILE_SELECTION_BOX_HEIGHT                            \
        } 

/// EDITOR
#define EDITOR_WIDTH_PERCENTUAL_WINDOW 55
#define EDITOR_MARGEM 15
#define EDITOR_BORDA_THICK 5
#define EDITOR_REC_Y (BUTTON_LOAD.y + BUTTON_LOAD.height + EDITOR_MARGEM)
#define EDITOR_REC (Rectangle){ EDITOR_MARGEM , EDITOR_REC_Y , WINDOW_WIDTH * EDITOR_WIDTH_PERCENTUAL_WINDOW / 100.0 , WINDOW_HEIGHT - EDITOR_REC_Y - EDITOR_MARGEM }

#define EDITOR_SPACING_TEXT_LEFT 3
#define EDITOR_TEXT_LINE_HEIGHT 20
#define EDITOR_TEXT_FZ EDITOR_TEXT_LINE_HEIGHT

#define EDITOR_DEFAULT_COLOR_TEXT BLACK
#define EDITOR_HIGHLIGHT_ARITHIMETIC_INSTRUCTION BLUE
#define EDITOR_HIGHLIGHT_LOGIC_INSTRUCTION BLUE
#define EDITOR_HIGHLIGHT_CONTROL_INSTRUCTION BLUE
#define EDITOR_HIGHLIGHT_IO_INSTRUCTION BLUE
#define EDITOR_HIGHLIGHT_REGISTER MAROON
#define EDITOR_HIGHLIGHT_CONSTANTE ORANGE
#define EDITOR_HIGHLIGHT_COMMENT CLITERAL(Color){ 180, 180, 180, 255 }

// STACK
#define STACK_QTD_PAG 28
#define STACK_MARGEM 15
#define STACK_INDEX_FZ 13
#define STACK_FZ 25
#define STACK_BORDA_THICK 3
#define STACK_TOS_THICK 3
#define STACK_TOS_FZ 22
#define STACK_BORDA_COLOR DARKBLUE
#define STACK_FIELD_BG_COLOR CLITERAL(Color){ 200 , 200 , 200 , 255 }
#define STACK_VALUE_COLOR DARKBROWN

#define STACK_REC_WIDTH 125
#define STACK_REC_HEIGHT STACK_FZ
#define STACK_REC_X ( WINDOW_WIDTH - STACK_REC_WIDTH - STACK_MARGEM )
#define STACK_LAST_FIELD_REC (Rectangle){            \
        STACK_REC_X,\
        WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT * STACK_QTD_PAG,\
        STACK_REC_WIDTH,\
        STACK_REC_HEIGHT\
    }
#define STACK_FIRST_FIELD_REC (Rectangle){            \
        STACK_REC_X,\
        WINDOW_HEIGHT - STACK_MARGEM - STACK_REC_HEIGHT,\
        STACK_REC_WIDTH,\
        STACK_REC_HEIGHT\
    }

// GENERAL
#define DELAY_EXECUTION 100

// FILES
#define FOLDER_DEFAULT_SAVES "./saves/"

// CORRECTION
#define DEFAULT_FONT_HEIGHT_ERROR_CORRECTION_FACTOR (27 / 10.0f)

// GEOMETRIC
#define VECNUL (Vector2){0,0}
#define VECm1 (Vector2){-1,-1}

#endif  //__DEFINITIONS_HPP_