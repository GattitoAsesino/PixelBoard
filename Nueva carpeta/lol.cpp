
#include <stdio.h> 
#include <dos.h>

char flag = 't';
int estado = 0;
int coordx = 0;
int coordy = 0;
int tamano_punto = 8;
char color_actual = 0x1F; // Blanco por defecto

void inicializa_mouse() {
    asm {
        mov ax, 000h
        int 33h
    }
}

void posicionar_cursor(char fila, char columna) {
    asm {
        mov ah, 02h
        mov bh, 00h
        mov dh, fila
        mov dl, columna
        int 10h
    }
}

void mostrar_mouse() {
    asm {
        mov ax, 01h
        int 33h
    }
}

void ocultar_mouse() {
    asm {
        mov ax, 02h
        int 33h
    }
}

void configurar_modo_grafico() {
    asm {
        mov ax, 0x13    // Modo gráfico 320x200, 256 colores
        int 0x10        // Llamar a la BIOS para cambiar el modo
    }
}

void pintar_pixel(int x, int y, char color) {
    if (x >= 0 && x < 320 && y >= 0 && y < 200) { // Validar rango
        asm {
            mov ah, 0x0C   // Función para pintar un pixel
            mov al, color  // Color del pixel
            mov cx, x      // Coordenada X
            mov dx, y      // Coordenada Y
            int 0x10       // Interrupción BIOS
        }
    }
}


void dibujar_cuadro(int x, int y, int ancho, int alto, char color) {
    for (int i = 0; i < ancho; i++) {
        for (int j = 0; j < alto; j++) {
            pintar_pixel(x + i, y + j, color); // Pintar cada pixel
        }
    }
}


void dibujar_X() {
    // Dibujar la "X" en la esquina superior derecha
    for (int i = 0; i < 10; i++) {
        pintar_pixel(310 + i, 10 + i, 0x1F); // Diagonal \
        pintar_pixel(310 + i, 19 - i, 0x1F); // Diagonal /
    }
}

void cls() {
    asm {
        mov ax, 0600h
        mov bh, 1Fh
        mov cx, 0000h
        mov dx, 184Fh
        int 10h
    }
}

void estado_mouse() {
    inicializa_mouse();
    mostrar_mouse();
    configurar_modo_grafico();
    posicionar_cursor(0, 75); // Esquina superior derecha
    printf("X");

    // Dibujar cuadros de colores más "cute"
dibujar_cuadro(10, 10, 20, 20, 0x0D); // Rosa pastel
dibujar_cuadro(40, 10, 20, 20, 0x0A); // Verde menta
dibujar_cuadro(70, 10, 20, 20, 0x09); // Celeste pastel


    // Dibujar la "X" para cerrar
    //dibujar_X();

    while (flag != 'f') {
        asm {
            mov ax, 03h
            int 33h
            mov estado, bx
            mov coordx, cx
            mov coordy, dx
        }

        // Mostrar coordenadas en pantalla
        posicionar_cursor(0, 0);
        printf("Coordenadas: (%d, %d)   ", coordx, coordy);

        // Mostrar el cursor en la posición actual
        mostrar_mouse();

        // Detectar clic en la "X" (cerrar programa)
        if (coordx >= 558 && coordx <= 674 && coordy >= 0 && coordy <= 20 && estado == 1) {
            flag = 'f';
            break;
        }

        // Detectar clic en los cuadros de colores
        if (estado == 1) {
            if (coordx >= 10 && coordx <= 60 && coordy >= 10 && coordy <= 30) {
                color_actual = 0x0D; // Rojo
            } else if (coordx >= 80 && coordx <= 120 && coordy >= 10 && coordy <= 30) {
                color_actual = 0x0A; // Verde
            } else if (coordx >= 140 && coordx <= 180 && coordy >= 10 && coordy <= 30) {
                color_actual = 0x09; // Azul
            }else{
                for (int dx = -tamano_punto / 2; dx <= tamano_punto / 2; dx++) {
                for (int dy = -tamano_punto / 2; dy <= tamano_punto / 2; dy++) {
                    pintar_pixel(coordx / 2 + dx, coordy + dy, color_actual);
                }
            }
            }
        }

     // Pintar con el color actual
        if ( estado == 2) {
            for (int dx = -tamano_punto/2; dx <= tamano_punto/2; dx++) {
                for (int dy = -tamano_punto/2; dy <= tamano_punto/2; dy++) {
                    pintar_pixel(coordx / 2 + dx, coordy + dy, 0x00);
                }
            }
        }

        delay(50);
    }

    // Restaurar modo texto
    asm {
        mov ax, 0x03
        int 0x10
    }
}

int main() {
    cls();
    estado_mouse();
    posicionar_cursor(23, 0);
    return 0;
} 