//Eri Santiago, 1220610, Estructuras de datos, Sistema de inventario de biblioteca
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura del nodo = los libros
typedef struct Libro {
    char isbn[15]; 
    char titulo[100];
    char autor[50];
    int anio;
    int estado;          // 0= Disponible, 1= Prestado
    struct Libro *siguiente;
} Libro;

// Funciones obligatorias
Libro* buscarLibro(Libro *inicio, char isbn[]);
void insertarLibro(Libro **inicio, char isbn[], char titulo[], char autor[], int anio);
void eliminarLibro(Libro **inicio, char isbn[]);
void prestarLibro(Libro *inicio, char isbn[]);
void devolverLibro(Libro *inicio, char isbn[]);
void mostrarInventario(Libro *inicio);

// Funciones auxiliares
void limpiarBuffer();
void quitarSaltoLinea(char *cadena);
void liberarMemoriaTotal(Libro **inicio);

// Menu principal
int main() {
    Libro *inicio = NULL; 
    int opcion, anio;
    char isbn[15], titulo[100], autor[50];

    do {
        printf("\n--- SISTEMA DE INVENTARIO DE BIBLIOTECA ---\n");
        printf("1. Agregar libro\n");
        printf("2. Buscar libro por ISBN\n");
        printf("3. Prestar libro\n");
        printf("4. Devolver libro\n");
        printf("5. Eliminar libro\n");
        printf("6. Mostrar inventario\n");
        printf("7. Salir\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            opcion = 0;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                printf("Ingrese ISBN (ej. 978-3-16-1484): ");
                fgets(isbn, sizeof(isbn), stdin);
                quitarSaltoLinea(isbn);

                printf("Ingrese Titulo: ");
                fgets(titulo, sizeof(titulo), stdin);
                quitarSaltoLinea(titulo);

                printf("Ingrese Autor: ");
                fgets(autor, sizeof(autor), stdin);
                quitarSaltoLinea(autor);

                printf("Ingrese Anio de publicacion: ");
                scanf("%d", &anio);
                limpiarBuffer();

                insertarLibro(&inicio, isbn, titulo, autor, anio);
                break;
            case 2:
                printf("Ingrese el ISBN a buscar: ");
                fgets(isbn, sizeof(isbn), stdin);
                quitarSaltoLinea(isbn);
                
                Libro *encontrado = buscarLibro(inicio, isbn);
                if (encontrado != NULL) {
                    printf("\n--- Libro Encontrado ---\n");
                    printf("ISBN: %s\nTitulo: %s\nAutor: %s\nAnio: %d\nEstado: %s\n", 
                           encontrado->isbn, encontrado->titulo, encontrado->autor, 
                           encontrado->anio, encontrado->estado == 0 ? "Disponible" : "Prestado");
                } else {
                    printf("\nError: Libro con ISBN %s no encontrado.\n", isbn);
                }
                break;
            case 3:
                printf("Ingrese el ISBN del libro a prestar: ");
                fgets(isbn, sizeof(isbn), stdin);
                quitarSaltoLinea(isbn);
                prestarLibro(inicio, isbn);
                break;
            case 4:
                printf("Ingrese el ISBN del libro a devolver: ");
                fgets(isbn, sizeof(isbn), stdin);
                quitarSaltoLinea(isbn);
                devolverLibro(inicio, isbn);
                break;
            case 5:
                printf("Ingrese el ISBN del libro a eliminar: ");
                fgets(isbn, sizeof(isbn), stdin);
                quitarSaltoLinea(isbn);
                eliminarLibro(&inicio, isbn);
                break;
            case 6:
                mostrarInventario(inicio);
                break;
            case 7:
                printf("\nSaliendo del programa. Liberando memoria...\n");
                liberarMemoriaTotal(&inicio);
                break;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 7);

    return 0;
}

// Implementación de Funciones
// Buscar libro
Libro* buscarLibro(Libro *inicio, char isbn[]) {
    Libro *actual = inicio;
    while (actual != NULL) {
        if (strcmp(actual->isbn, isbn) == 0) {
            return actual; // Regresa el puntero si encuentra coincidencia
        }
        actual = actual->siguiente;
    }
    return NULL; // Regresa NULL si no se encuentra
}

// Agregar al final de la lista verificando que no sea duplicado
void insertarLibro(Libro **inicio, char isbn[], char titulo[], char autor[], int anio) {
    // Validar duplicado
    if (buscarLibro(*inicio, isbn) != NULL) {
        printf("\nError: Ya existe un libro registrado con el ISBN %s.\n", isbn);
        return;
    }

    // Reservar memoria dinámica para el nuevo nodo
    Libro *nuevoLibro = (Libro*)malloc(sizeof(Libro));
    if (nuevoLibro == NULL) {
        printf("\nError: No se pudo asignar memoria.\n");
        return;
    }

    // Inicializar datos
    strcpy(nuevoLibro->isbn, isbn);
    strcpy(nuevoLibro->titulo, titulo);
    strcpy(nuevoLibro->autor, autor);
    nuevoLibro->anio = anio;
    nuevoLibro->estado = 0; // Disponible por defecto
    nuevoLibro->siguiente = NULL;

    // Si la lista está vacía, el nuevo libro es el inicio
    if (*inicio == NULL) {
        *inicio = nuevoLibro;
    } else {
        // Recorrer hasta el último nodo para insertarlo al final
        Libro *actual = *inicio;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoLibro;
    }
    printf("\nLibro '%s' agregado exitosamente al inventario.\n", titulo);
}

// Eliminar libro y liberar su memoria
void eliminarLibro(Libro **inicio, char isbn[]) {
    if (*inicio == NULL) {
        printf("\nError: El inventario esta vacio.\n");
        return;
    }

    Libro *actual = *inicio;
    Libro *anterior = NULL;

    // Buscar el nodo a eliminar
    while (actual != NULL && strcmp(actual->isbn, isbn) != 0) {
        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no se encontró
    if (actual == NULL) {
        printf("\nError: Libro con ISBN %s no encontrado.\n", isbn);
        return;
    }

    // Si el nodo a eliminar es el primero
    if (anterior == NULL) {
        *inicio = actual->siguiente;
    } else {
        // Desenlazar nodo intermedio o final
        anterior->siguiente = actual->siguiente;
    }

    free(actual); // Liberar memoria
    printf("\nLibro con ISBN %s eliminado exitosamente.\n", isbn);
}

// Cambiar estado a prestado
void prestarLibro(Libro *inicio, char isbn[]) {
    Libro *libro = buscarLibro(inicio, isbn);
    if (libro == NULL) {
        printf("\nError: Libro con ISBN %s no encontrado.\n", isbn);
    } else if (libro->estado == 1) {
        printf("\nError: El libro '%s' ya se encuentra prestado.\n", libro->titulo);
    } else {
        libro->estado = 1;
        printf("\nLibro '%s' prestado exitosamente.\n", libro->titulo);
    }
}

// Cambiar estado a disponible
void devolverLibro(Libro *inicio, char isbn[]) {
    Libro *libro = buscarLibro(inicio, isbn);
    if (libro == NULL) {
        printf("\nError: Libro con ISBN %s no encontrado.\n", isbn);
    } else if (libro->estado == 0) {
        printf("\nError: El libro '%s' ya esta disponible, no se puede devolver.\n", libro->titulo);
    } else {
        libro->estado = 0;
        printf("\nLibro '%s' devuelto exitosamente.\n", libro->titulo);
    }
}

// Mostrar todos los libros
void mostrarInventario(Libro *inicio) {
    if (inicio == NULL) {
        printf("\nEl inventario esta vacio.\n");
        return;
    }

    printf("\nINVENTARIO DE LIBROS\n");
    Libro *actual = inicio;
    while (actual != NULL) {
        printf("ISBN: %-15s | Titulo: %-25s | Autor: %-20s | Anio: %d | Estado: %s\n", 
               actual->isbn, 
               actual->titulo, 
               actual->autor, 
               actual->anio, 
               actual->estado == 0 ? "Disponible" : "Prestado");
        actual = actual->siguiente;
    }
}
// Funciones Auxiliares

// Limpia el buffer de entrada para evitar saltos en los 'scanf' y 'fgets'
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Elimina el salto de línea que 'fgets' agrega al final de las cadenas
void quitarSaltoLinea(char *cadena) {
    size_t longitud = strlen(cadena);
    if (longitud > 0 && cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
    }
}

// Libera toda la memoria dinámica antes de cerrar el programa para evitar fugas de memoria
void liberarMemoriaTotal(Libro **inicio) {
    Libro *actual = *inicio;
    Libro *siguienteNodo;
    while (actual != NULL) {
        siguienteNodo = actual->siguiente;
        free(actual);
        actual = siguienteNodo;
    }
    *inicio = NULL;
}
