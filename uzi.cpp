#include <stdio.h>
#include <dos.h>

char flag = 't';
int estado = 0;
int coordx = 0;
int coordy = 0;
int tamano_punto = 3;

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

void configurar_modo_grafico() {
    asm {
        mov ax, 0x13
        int 0x10
    }
}

void pintar_pixel(int x, int y, char color) {
    asm {
        mov ah, 0x0C
        mov al, color
        mov cx, x
        mov dx, y
        mov bh, 0
        int 0x10
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

void dibujar_X() {

}

void estado_mouse() {
    inicializa_mouse();
    mostrar_mouse();
    configurar_modo_grafico();
    posicionar_cursor(0, 75); // Esquina superior derecha
    printf("X");

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

        // Detectar clic en la "X" (cerrar programa)
        if (coordx >= 558 && coordx <= 674 && coordy >= 0 && coordy <= 20 && estado == 1) {
            flag = 'f';
            break;
        }

         if (estado == 2) { // Botón izquierdo: negro
            for (int dx = -tamano_punto/2; dx <= tamano_punto/2; dx++) {
                for (int dy = -tamano_punto/2; dy <= tamano_punto/2; dy++) {
                    pintar_pixel(coordx / 2 + dx, coordy + dy, 0x00);
                }
            }
        } else if (estado == 1) { // Botón derecho: blanco
            for (int dx = -tamano_punto/2; dx <= tamano_punto/2; dx++) {
                for (int dy = -tamano_punto/2; dy <= tamano_punto/2; dy++) {
                    pintar_pixel(coordx / 2 + dx, coordy + dy, 0x1F);
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
    dibujar_X();
    estado_mouse();
    posicionar_cursor(23, 0);
    return 0;
}