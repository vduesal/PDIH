#include<stdio.h>
#include<dos.h>

unsigned char cfondo = 3;
unsigned char ctexto = 0;

// ---------- REQUISITOS MINIMOS ----------

// Funcion de pausa para el main
void pausa(){
    union REGS inregs, outregs;
	inregs.h.ah = 8;
	int86(0x21, &inregs, &outregs);
}

// Funcion para colocar el cursor en una posicion determinada
void go_to_xy(int x, int y){
	union REGS inregs, outregs;
	inregs.h.ah = 0x02;
	inregs.h.bh = 0x00;
	inregs.h.dh = y;
	inregs.h.dl = x;
	int86(0x10,&inregs,&outregs);
	return;
}

// Funcion para fijar el aspecto del cursor: INVISIBLE, NORMAL y GRUESO
void set_cursor_type(int tipo_cursor){
	union REGS inregs, outregs;
	inregs.h.ah = 0x01;
	switch(tipo_cursor){
		case 0: //invisible
			inregs.h.ch = 010;
			inregs.h.cl = 000;
			break;
		case 1: //normal
			inregs.h.ch = 010;
			inregs.h.cl = 010;
			break;
		case 2: //grueso
			inregs.h.ch = 000;
			inregs.h.cl = 010;
			break;
	}
	int86(0x10, &inregs, &outregs);
}

// Funcion para fijar el modo de video
void set_video_mode(unsigned char modo){
    union REGS inregs, outregs;
    inregs.h.ah = 0x00;
    inregs.h.al = modo;
    int86(0x10,&inregs,&outregs);
    return;
}

// Funcion para obtener el modo de video actual
void get_video_mode(){
    union REGS inregs, outregs;
    unsigned char modo;
    inregs.h.ah = 0x0f;
    int86(0x10,&inregs,&outregs);
    modo = outregs.h.al;
    if(modo == 0 || modo == 1 || modo == 2 || modo == 3 || modo == 7){
        printf("Estamos en modo texto");
    }
    else{
        printf("Estamos en modo grafico");
    }
}

// Funcion que obtiene un caracter de teclado y lo muestra en pantalla
int get_char(){
    union REGS inregs, outregs;
	int letra;
    inregs.h.ah = 1;
	int86(0x21,&inregs,&outregs);
    letra = outregs.h.al;
	return letra;
}

// Funcion que borra toda la pantalla
void borrar(){
    union REGS regs;
	regs.h.ah = 15; 
	int86(0x10,&regs,&regs);
	regs.h.ah = 0; 
	int86(0x10,&regs,&regs);
}

// Funcion para escribir un caracter
void put_char(char c){
	union REGS inregs, outregs;
    inregs.h.ah = 2;
	inregs.h.dl = c;
	int86(0x21, &inregs, &outregs);
}

// Funcion que modifica el color del primer plano con que se mostraran los caracteres
// y modifica el color de fondo con que se mostraran los caracteres
// Funcion que escribe un caracter en pantalla con el color indicado actualmente
void put_char_color(char letra){
    union REGS inregs, outregs;
    inregs.h.ah= 0x09;
    inregs.h.al= letra;
	inregs.h.bl= (cfondo << 4) | ctexto;
    inregs.h.bh= 0x00;
	inregs.x.cx = 1;
    int86(0x10, &inregs, &outregs);
}


// ---------- REQUISITOS AMPLIADOS ----------

// Funcion pixel, pone un pixel en color
void pixel(int x, int y, unsigned char color){
    union REGS inregs, outregs;
    inregs.x.cx = x;
    inregs.x.dx = y;
    inregs.h.al = color;
    inregs.h.ah = 0x0C;
    int86(0x10,&inregs,&outregs);
}

// Funcion que dibuja un cuadrado
void cuadrado(int xi, int yi, int xd, int yd, unsigned char color_texto, unsigned char color_fondo, char letra){
    int i;
    cfondo = color_fondo;
    ctexto = color_texto;
    for (i = xi; i <= xd; i++){
        go_to_xy(i,yi);
        put_char_color(letra);
    }
    for (i = xi; i <= xd; i++){
        go_to_xy(i,yd);
        put_char_color(letra);
    }
    for (i = yi; i <= yd; i++){
        go_to_xy(xi,i);
        put_char_color(letra);
    }
    for (i = yi; i <= yd; i++){
        go_to_xy(xd,i);
        put_char_color(letra);
    }
}

// Funcion que dibuja una carita sonriente
void carita_feliz(){
    int i;
    set_video_mode(4);

    // Circunferencia (lineas rectas)
    // Izquierda
    for (i = 35; i < 75; i++){
        pixel(40,i,2);
    }
    // Derecha
    for (i = 35; i < 75; i++){
        pixel(130,i,2);
    }
    // Arriba
    for (i = 65; i < 105; i++){
        pixel(i,10,2);
    }
    // Abajo
    for (i = 65; i < 105; i++){
        pixel(i,100,2);
    }

    // Circunferencia (lineas diagonales)
    // Arriba, derecha
    for (i = 0; i < 25; i++){
        pixel(105+i,10+i,2);
    }
    // Abajo, izquierda
    for (i = 0; i < 25; i++){
        pixel(40+i,75+i,2);
    }
    // Arriba, izquierda
    for (i = 0; i < 25; i++){
        pixel(65-i,10+i,2);
    }
    // Abajo, derecha
    for (i = 0; i < 26; i++){
        pixel(130-i,75+i,2);
    }

    // Ojos
    for (i = 0; i < 30; i++){
        pixel(71,30+i,1);
        pixel(99,30+i,1);
    }

    // Boca
    for (i = 70; i < 100; i++){
        pixel(i,80,1);
    }
    for (i = 0; i < 20; i++){
        pixel(100+i,80-i,1);
    }
    for (i = 0; i < 20; i++){
        pixel(70-i,80-i,1);
    }
}


// ---------- FUNCION MAIN ----------

// Funcion main
int main(){
    int tmp;
    char letra;
    int x,y;
    int x1,y1,x2,y2;
    unsigned char c1,c2;
    char c;

    // Borramos toda la pantalla antes de empezar
    borrar();

    // Comprobacion de la funcion para mover el cursor
    x = 25;
    y = 3;
	go_to_xy(x,y);
	printf("* Estamos en el punto x=%d, y=%d",x,y);
	pausa();

    // Cambiamos el color del caracter
    printf("\nPulsa un numero para elegir color de fondo: ");
    cfondo = get_char();
    pausa();
    printf("\nPulsa un numero para elegir color del texto: ");
    ctexto = get_char();
    pausa();
    printf("\nLetra que quieres mostrar: ");
    letra = get_char();
    pausa();
    printf("\n");
    put_char_color(letra);
    pausa();

    // Tipos de cursor
    printf("\nCursor invisible: ");
    set_cursor_type(0);
    pausa();
    printf("\nCursor grueso: ");
    set_cursor_type(2);
    pausa();
    printf("\nCursor normal: ");
    set_cursor_type(1);
    pausa();

    // Leer un caracter de teclado
    printf("\nPulsa una tecla: ");
    tmp = get_char();
    printf("\nTecla pulsada: ");
    put_char((char)tmp);
    pausa();

    // Borrar la pantalla entera
    printf("\nBorramos la pantalla...");
    pausa();
    borrar();

    // Obtenemos el modo de video
    get_video_mode();
    pausa();

    // Cambiamos el modo de video
    set_video_mode(4);  // modo 4 -> 320x200 (letras grandes, en modo grafico)

    // Volvemos a obtener el modo de video 
    get_video_mode();
    pausa();

    // Ponemos de nuevo el modo de video por defecto para salir del programa
    set_video_mode(3);  // modo 3 -> 80x25 (letras pequenias)
    pausa();


    // // ---------- REQUISITOS ADICIONALES ----------

    // Vamos a dibujar un cuadrado
    printf("\nVamos a dibujar un cuadrado");
    printf("\nColor de texto: ");
    c1 = get_char();
    pausa();
    printf("\nColor de fondo: ");
    c2 = get_char();
    pausa();
    printf("\nLetra: ");
    c = get_char();
    pausa();

    // Borramos toda la pantalla para mostrar un cuadrado en modo texto
    printf("\nBorramos toda la pantalla para mostrar un cuadrado en modo texto ");
    pausa();
    borrar();

    // Obtenemos el modo video
    get_video_mode();

    // Cuadrado en modo texto
    x1 = 2;
    y1 = 2;
    x2 = 52;
    y2 = 14;
    cuadrado(x1,y1,x2,y2,c1,c2,c);
    pausa();

    // Borramos toda la pantalla para mostrar un dibujo en modo grafico
    printf("\nBorramos toda la pantalla para mostrar un dibujo en modo grafico ");
    pausa();
    borrar();

    // Dibujo en modo grafico
    carita_feliz();
    pausa();

    // Fin del programa
    return 0;
}

