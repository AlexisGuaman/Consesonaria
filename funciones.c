#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu() {
    printf("\n--- CONCESIONARIA RUEDAS DE ORO ---\n");
    printf("Seleccione una opcion:\n");
    printf("1. Ingresar nuevo vehiculo\n");
    printf("2. Listar vehiculos en inventario\n");
    printf("3. Editar datos de un vehiculo\n");
    printf("4. Buscar vehiculo por preferencia (Presupuesto/Marca)\n");
    printf("5. Registrar venta\n");
    printf("6. Eliminar un vehiculo del sistema\n");
    printf(">> ");
    
    
    int opc = validarFloatRango(1, 6); // Validamos que la opción esté entre 1 y 6
    
    return opc;
}


float validarFloatRango(float a, float b) {
    int aux;
    float n;
    do {
        aux = scanf("%f", &n);
        while((getchar()) != '\n'); 
        
        if(aux != 1 || n < a || n > b) {
            printf("Error: valor ingresado incorrecto\n");
            printf(">> ");
        }
    } while (aux != 1 || n < a || n > b);
    
    return n;
}


void leerCadena(char *cadena, int n) {
    int len;
    fgets(cadena, n, stdin);
    len = strlen(cadena) - 1; 
    cadena[len] = '\0';       
}


void crearVehiculo(int pos) {
    Vehiculo vehiculo;
    
    printf("\n--- INGRESO DE DATOS DEL VEHICULO ---\n");

    printf("Ingrese el ID del vehiculo: ");
    vehiculo.id =(int) validarFloatRango(1, 99999); 
    
    printf("Ingrese la marca del vehiculo (Ej. Chevrolet Todo en minusculas o mayusculas): ");
    leerCadena(vehiculo.marca, 20);
    
    printf("Ingrese el modelo (Ej. D-Max): ");
    leerCadena(vehiculo.modelo, 20);
    
    printf("Ingrese el tipo (Ej. Camioneta): ");
    leerCadena(vehiculo.tipo, 20);
    
    printf("Ingrese el estado (Ej. Usado): ");
    leerCadena(vehiculo.estado, 20);
    
    printf("Ingrese el precio del vehiculo: ");
    vehiculo.precio = validarFloatRango(1, 9999999); 
    
    
    vehiculo.disponible = 1; 
    
    // Llamamos a la función que guarda la estructura en el archivo binario
    guardarVehiculo(&vehiculo, pos);
}


void guardarVehiculo(Vehiculo *vehiculo, int pos) {
    FILE *f;
    
    // (editar/vender)
    if (pos != -1) {
        f = fopen("vehiculos.dat", "rb+");
    } else {
        // Si pos es -1, abrimos para añadir datos al final del archivo (nuevo vehículo)
        f = fopen("vehiculos.dat", "ab+");
    }
    
    if (f == NULL) {
        printf("Error al abrir el archivo de inventario.\n");
        return;
    }
    
    // Si es una edición, movemos el cursor del archivo a la posición exacta
    if (pos != -1) {
        fseek(f, pos, SEEK_SET);
    }
    
    // Escribimos la estructura completa en el archivo
    fwrite(vehiculo, sizeof(Vehiculo), 1, f);
    fclose(f);
}


void listarVehiculos() {
    FILE *f;
    Vehiculo vehiculo;
    
    f = fopen("vehiculos.dat", "rb"); 
    
    if (f == NULL) {
        printf("El inventario esta vacio o no existe.\n");
        return;
    }
    
    printf("\n--- INVENTARIO DE VEHICULOS ---\n");
    printf("ID\tMarca\t\tModelo\t\tTipo\t\tEstado\t\tPrecio\t\tDisponible\n");
    printf("------------------------------------------------------------------------------------------\n");
    
    // Leemos registro por registro hasta que se acabe el archivo
    while (fread(&vehiculo, sizeof(Vehiculo), 1, f)) {
        // Usamos \t para tabular y %-10s para alinear los textos y que se vea como una tabla bonita
        if (vehiculo.id != -1) { // Solo mostramos los vehículos que no han sido eliminados (id != -1)
            printf("%d\t%-10s\t%-10s\t%-10s\t%-10s\t$%.2f\t%s\n", 
                   vehiculo.id, vehiculo.marca, vehiculo.modelo, vehiculo.tipo, vehiculo.estado, vehiculo.precio,
                   vehiculo.disponible == 1 ? "SI" : "NO (Vendido)");
        }
    }
    fclose(f);
}

int buscarVehiculo() {
    Vehiculo vehiculo;
    int idBuscar;
    int pos = 0;
    int flag = 0;
    FILE *f;
    
    printf("\nIngrese el ID del vehiculo a buscar: ");
    idBuscar =(int) validarFloatRango(1, 99999);
    
    f = fopen("vehiculos.dat", "rb");
    if (f == NULL) {
        return -1;
    }
    
    while (fread(&vehiculo, sizeof(Vehiculo), 1, f)) {
        if (vehiculo.id == idBuscar) {
            flag = 1;
            pos = ftell(f) - sizeof(Vehiculo); 
            break;
        }
    }
    fclose(f);
    
    if (flag == 0) {
        pos = -1;
    }
    return pos;
}


void editarVehiculo() {
    int pos = buscarVehiculo();
    
    if (pos == -1) {
        printf("No existe el vehiculo buscado o el inventario esta vacio.\n");
        return;
    }
    
    printf("\nVehiculo encontrado. Por favor, ingrese los nuevos datos:\n");
    crearVehiculo(pos); 
}


void buscarPorPreferencia() {
    FILE *f;
    Vehiculo vehiculo;
    char marcaBuscada[20], tipoBuscado[20], estadoBuscado[20];
    float presupuestoMax;
    int encontrados = 0;
    
    printf("\n--- BUSQUEDA POR PREFERENCIAS DEL CLIENTE ---\n");
    printf("Ingrese la marca de preferencia (Ej. Chevrolet): ");
    leerCadena(marcaBuscada, 20);
    
    printf("Ingrese el tipo de vehiculo (Ej. Camioneta): ");
    leerCadena(tipoBuscado, 20);
    
    printf("Ingrese el estado (Ej. Usado): ");
    leerCadena(estadoBuscado, 20);
    
    printf("Ingrese el presupuesto maximo del cliente: $");
    presupuestoMax = validarFloatRango(1, 10000000);
    
    f = fopen("vehiculos.dat", "rb");
    if (f == NULL) {
        printf("El inventario esta vacio.\n");
        return;
    }
    
    printf("\nResultados encontrados para sus preferencias:\n");
    printf("ID\tMarca\t\tModelo\t\tTipo\t\tEstado\t\tPrecio\n");
    printf("------------------------------------------------------------------------------------------\n");
    
    
    while (fread(&vehiculo, sizeof(Vehiculo), 1, f)) {
        
        if (strcmp(vehiculo.marca, marcaBuscada) == 0 &&
            strcmp(vehiculo.tipo, tipoBuscado) == 0 &&
            strcmp(vehiculo.estado, estadoBuscado) == 0 &&
            vehiculo.precio <= presupuestoMax &&
            vehiculo.disponible == 1 &&
            vehiculo.id != -1) { 

            printf("%d\t%-10s\t%-10s\t%-10s\t%-10s\t$%.2f\n", 
                   vehiculo.id, vehiculo.marca, vehiculo.modelo, vehiculo.tipo, vehiculo.estado, vehiculo.precio);
            encontrados = 1;
        }
    }
    fclose(f);
    
    if (encontrados == 0) {
        printf("Lo sentimos, no hay vehiculos disponibles que coincidan con esas preferencias.\n");
    }
}


void registrarVenta() {
    FILE *f;
    Vehiculo vehiculo;
    int pos;
    
    printf("\n--- REGISTRAR VENTA DE VEHICULO ---\n");
    
    pos = buscarVehiculo();
    
    if (pos == -1) {
        printf("El vehiculo no existe en el inventario.\n");
        return;
    }
    
    
    f = fopen("vehiculos.dat", "rb+");
    if (f == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    
    
    fseek(f, pos, SEEK_SET);
    fread(&vehiculo, sizeof(Vehiculo), 1, f); 
    
    
    if (vehiculo.disponible == 0) {
        printf("Atencion: Este vehiculo ya ha sido vendido previamente.\n");
    } else {
        
        vehiculo.disponible = 0;
        
        
        fseek(f, pos, SEEK_SET);
        fwrite(&vehiculo, sizeof(Vehiculo), 1, f); 
        
        printf("\n¡Venta registrada con exito! El %s %s ha sido descontado del inventario.\n", vehiculo.marca, vehiculo.modelo);
    }
    
    fclose(f);
}

void eliminarVehiculo() {
    FILE *f;
    Vehiculo vehiculo;
    int pos;
    
    printf("\n--- ELIMINAR VEHICULO ---\n");
    
    
    pos = buscarVehiculo(); 
    
    if (pos == -1) {
        printf("El vehiculo no existe en el inventario.\n");
        return;
    }
    
    
    f = fopen("vehiculos.dat", "rb+");
    if (f == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    
    
    fseek(f, pos, SEEK_SET);
    fread(&vehiculo, sizeof(Vehiculo), 1, f);
    
    
    vehiculo.id = -1;
    
    
    fseek(f, pos, SEEK_SET);
    fwrite(&vehiculo, sizeof(Vehiculo), 1, f);
    
    printf("\n¡El vehiculo fue eliminado del sistema con exito!\n");
    fclose(f);
}