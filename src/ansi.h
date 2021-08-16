#ifndef __ANSI__
#define __ANSI__

/* ____ _  _ ____ _    ____ ____ ___  ____ ____
   |__| |\ | [__  |    |    |  | |  \ |___ [__ 
   |  | | \| ___] |    |___ |__| |__/ |___ ___]
*/



#define CLEAR_UP()				\
  printf("\e[1J")

#define PLACE_ROWS_UP(n)			\
  printf("\e[%dK", n)


// Place cursor at pos counted from top of terminal emulator
#define PLACE_CURSOR(x, y)			\
  printf("\e[%d;%dHm", x, y); 

#define LINE       "\e[09m"
#define ITALIC     "\e[03m" 
#define NORMAL      "\e[00m"
#define BOLD       "\e[01m"
#define BOLD       "\e[01m"

#define FG_RED     "\e[31m"
#define FG_GREEN   "\e[32m"
#define FG_BLUE    "\e[34m"
#define FG_MAGENTA "\e[35m"
#define FG_CYAN    "\e[36m"
#define FG_WHITE   "\e[37m"
#define FG_BLACK   "\e[30m"


#define FG_BRIGHT_RED     "\e[91m"
#define FG_BRIGHT_GREEN   "\e[92m"
#define FG_BRIGHT_BLUE    "\e[94m"
#define FG_BRIGHT_MAGENTA "\e[95m"
#define FG_BRIGHT_CYAN    "\e[96m"
#define FG_BRIGHT_WHITE   "\e[97m"
#define FG_BRIGHT_BLACK   "\e[90m"


#define BG_RED     "\e[41m"
#define BG_GREEN   "\e[42m"
#define BG_BLUE    "\e[44m"
#define BG_MAGENTA "\e[45m"
#define BG_CYAN    "\e[46m"
#define BG_WHITE   "\e[47m"
#define BG_BLACK   "\e[40m"

#define BG_BRIGHT_RED     "\e[101m"
#define BG_BRIGHT_GREEN   "\e[102m"
#define BG_BRIGHT_BLUE    "\e[104m"
#define BG_BRIGHT_MAGENTA "\e[105m"
#define BG_BRIGHT_CYAN    "\e[106m"
#define BG_BRIGHT_WHITE   "\e[107m"
#define BG_BRIGHT_BLACK   "\e[100m"



//#define SAVE_CURSOR()				\
//  printf("\e[s");
//
//#define LOAD_CURSOR()				\
//  printf("\e[u")
//
//#define CLEAR_ROW()				\
//  printf("\e[K")
//



#endif
// `========================================================='
// `|______________ANSI_TEXT_UTILITIES______________________|'
// `¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨'
// 
// = Common Colors= + === ANSI Strings in UNIX
//                  |
// <Attr>      <Nr> |
// `Black'     `40' | Format  <Attr>: "\e[<Nr n>m<txt>"
// `Red'       `41' | Combine <Attr>: "\e[<Nr>;<Nr>...m<txt>"
// `Green'     `42' | 
// `Yellow'    `43' | Light-color: <Nr> + 60    
// `Blue'      `44' | Dark-color:  <Nr>      
// `Magenta'   `45' | 
// `Cyan'      `46' | Background:  <Nr>     
// `White'     `47' | Foreground:  <Nr> - 10  
// ...              |
//                  |
// === Special ===  | === String injection in UNIX
//                  |
// <Attr>  <Nr>     | $ "${HOME}"  or "$HOME"      ==> "/home/eva.lu.ator"
// `Normal'    `00' | $ "$[3 * 3]" or "$((3 * 3))" ==> "9"
// `Bold'      `01' | $ "$(uname -o)"              ==> "GNU/Linux"
// `Italic'    `03' | 
// `Underline' `04' | === Example of Substituting in colors  
// `Blinking'  `06' | * End with \e[0m 
// `Inverse'   `07' | * blinking works in XFCE4's term and xterm      
// `Hide'      `08' | 
// `Line'      `09' | $ export FMT="\e[7;41mRED FLASHING...\e[0m"  
//                  | $ printf "...${FMT}..."		      
//                  |   ==> <blinking red text "RED FLASHING...>
//                  |
//  === Cursor ===  | === Cursor Manipulation
//                  |
// <Name> <Code>    |
// `Place'$Y;$XH    | Place cursor at position, empty value means 0
// `Save'      s    | Save current cursor position
// `Pos'       u    | Return to saved cursor position
// `Clear'   $XJ    | Clear screen, <x> signifies below-,
//                  | above cursor or entire screen
// `Clear-row' K    | Clear current row
// `Up'      $NA    | Move cursor n rows up
//                  |
//  === Tools ===   + ===
//                  |
// `printf'         | Print with ANSI compatibility  [ GNU Coreutils ]
// `echo' -e        | Print with ANSI compatibility  [ GNU Coreutils ]
//                  | 
//   =============  + === End                                 

