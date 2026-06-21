#ifndef FUNCIONES_H
#define FUNCIONES_H


typedef struct {
    int id;
    char marca[20];
    char modelo[20];
    char tipo[20];    
    char estado[20];  
    float precio;
    int disponible;   
} Vehiculo;


int menu();
void crearVehiculo(int pos);
void listarVehiculos();     
void editarVehiculo();      
void guardarVehiculo(Vehiculo *vehiculo, int pos);
int buscarVehiculo();
void buscarPorPreferencia(); 
void registrarVenta();       
void eliminarVehiculo();
void leerCadena(char *cadena, int n);
float validarFloatRango(float a, float b);

#endif