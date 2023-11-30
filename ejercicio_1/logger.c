#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

int hide_logs = 0;
int use_file = 0;
int skip_logs = 0;

//https://www.ozzu.com/wiki/504927/writing-a-custom-printf-wrapper-function-in-c
void console_log(  int hiddeable, char* format, ... ) {
    //saltar logs
    if(skip_logs){
        return;
    }

    //validacion de no mostrar
    if(hiddeable && hide_logs){
        return;
    }

    va_list args;
    va_start( args, format );

    if(use_file){
        FILE *f;
        f = fopen("1.log", "a+"); // a+ (create + append) option will allow appending which is useful in a log file
        if (f == NULL) { 
            /* Something is wrong   */
            return;
        }
        vfprintf(f, format, args);

        fclose(f);

    }else{
        vprintf( format, args );

    }

    va_end( args );

}

int set_variable( int status, int *variable ){
     //validacion
    if(status > 1 || status < 0){
        return 1;
    }

    *variable = status;
}


//recibe el estatus de los hiddeables
int set_hide_logs(int status){
    set_variable(status, &hide_logs);
}

int set_use_file(int status){
    set_variable(status, &use_file);
}

int set_skip_logs( int status){
    set_variable(status, &skip_logs);
}