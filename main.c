#include <stdio.h>
#include "funciones.h"

int main(int argc, char *argv[]) {
    int opc, opc2;
    
    do {
        opc = menu();
        switch (opc) {
            case 1:
                crearVehiculo(-1);
                break;
            case 2:
                listarVehiculos();
                break;
            case 3:
                editarVehiculo();
                break;
            case 4:
                buscarPorPreferencia(); 
                break;
            case 5:
                registrarVenta();       
                break;
            case 6:                     
                eliminarVehiculo();     
            break;
            default:
                printf("No existe la opcion\n");
                break;
        }
        
        printf("Desea seleccionar otra opcion 1.Si/2.No: ");
        opc2 = validarFloatRango(1, 2);
    } while (opc2 == 1);

    return 0;
}