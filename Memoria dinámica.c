#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Prototipo de la función LEER
void LEER(void);

// Definición de constantes
#define TOTAL 50

// Definición de la estructura ESTUDIANTES
typedef struct {
    char nombre[TOTAL];
    int edad;
    int nt; // Calificación
    char matricula[TOTAL];
} ESTUDIANTES;

// Función principal
int main() {
    int OP;
    bool fin = false;
    char seguir;
    ESTUDIANTES *estudiante;
    estudiante = (ESTUDIANTES*) malloc(sizeof(ESTUDIANTES));

    // Apertura del archivo en modo "append binary"
    FILE *disco = fopen("text.dat", "ab");

    // Bucle principal del programa
    do {
        // Mostrar menú de opciones
        printf("\t\tMenu:\n");
        printf("1- LECTURA DEL DISCO\n\n");
        printf("2- GRABAR DATO EN EL DISCO\n\n");
        printf("3- SALIR DEL PROGRAMA\n\n");
        printf("\tElige: ");
        scanf("%d", &OP);

        // Evaluar la opción seleccionada
        switch(OP) {
            case 1: // Leer datos del disco
                LEER();
                system("pause");
                printf("\n\n\n");
                break;

            case 2: // Grabar datos en el disco
                do {
                    // Solicitar y leer datos del estudiante
                    printf("Nombre: ");
                    scanf("%s", estudiante->nombre);
                    fflush(stdin);
                    printf("EDAD: ");
                    scanf("%d", &estudiante->edad);
                    fflush(stdin);
                    printf("MATRICULA: ");
                    scanf("%s", estudiante->matricula);
                    fflush(stdin);
                    printf("NOTA: ");
                    scanf("%d", &estudiante->nt);
                    fflush(stdin);

                    // Escribir datos del estudiante en el archivo
                    fwrite(estudiante, sizeof(ESTUDIANTES), 1, disco);

                    // Preguntar si desea ingresar otro registro
                    printf("\notro registro? (s/n): ");
                    scanf(" %c", &seguir); // Nota: espacio antes de %c para consumir el salto de línea
                } while (seguir == 's' || seguir == 'S');

                // Liberar memoria y cerrar archivo
                free(estudiante);
                fclose(disco);
                break;

            case 3: // Salir del programa
                printf("\n\n\t\tFIN\n\n");
                fin = true;
                break;

            default: // Opción no válida
                printf("Opcion no valida.\n");
                break;
        }
    } while(fin == false);

    return 0;
}

// Función para leer datos del archivo
void LEER(void) {
    
    FILE *disco = fopen("text.dat", "rb");

    // Asignar memoria para un estudiante
    ESTUDIANTES *estudiante;
    estudiante = (ESTUDIANTES*) malloc(sizeof(ESTUDIANTES));

    // Leer y mostrar datos del archivo
    while (fread(estudiante, sizeof(ESTUDIANTES), 1, disco)) {
        printf("Nombre: %s, Edad: %d, Matricula: %s, Nota: %d \n", estudiante->nombre, estudiante->edad, estudiante->matricula, estudiante->nt);
    }

    // Liberar memoria y cerrar archivo
    free(estudiante);
    fclose(disco);
}