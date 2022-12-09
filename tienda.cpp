//Para la entrada y salida de datos.
#include<iostream>
//Para la lectura y escritura de ficheros.
#include<fstream>
//Complemento para lectura y escritura de ficheros.
#include<sstream>

using namespace std;

template<typename T>
using pointer = T*;

template<typename T>
struct node{
    T value;
    pointer<node<T>> next;
    node(T value) : value{value}, next{nullptr} {} 
};

//Estructura tipo Pila
template<typename T>
class stack{
    //Puntero que señala a la cima de la pila.
    pointer<node<T>> _top;
    int size;
    public:
        //Constructor de la pila.
        stack(): _top{nullptr}, size{0} {}

        //Retorna verdadero si la pila está vacía y falso si contiene algún elemento.
        auto empty() -> bool {
            return not _top;
        }
        
        //Agrega un valor a la cima de la pila.
        auto push(T value) -> void {
            auto n = new node<T>(value);
            n->next = _top;
            _top = n;
            size++;
        }

        //Elimina un valor de la cima de la pila.
        auto pop() -> void {
            if(empty()) return;
            auto n = _top;
            _top = n -> next;
            delete n;
            size--;
        }
        //Retorna el contenido del apuntador de la cima de la pila.
        auto top() -> T {
            return _top -> value;
        }

        //Retorna el numero de elementos que tiene guardada la pila.
        auto length() -> int {
            return size;
        }

};


//Estructura que da formato a los articulos segun el archivo de articulos.txt
struct articulo{
    int key;
    string nombre;
    int cantidad;
    float precioBase;
    string caducidad;
};

//Estructura que da el formato a los renglones de la tabla hasg
struct hashrow{
    int key;
    articulo data;
    int cupo;
    hashrow(): cupo{0}{}
};

class hashTable{
    //Arreglo que contiene los 75 articulos del archivo articulos.txt
    hashrow hash[75];
    public:
        //Constructor de la tabla hash.
        hashTable(){
            //Lectura y almacenamiento de los elementos del archivo.
            articulo data[75];
            ifstream fuente;
            fuente.open("articulos.txt", ios_base::in);
            int contador = 0;
            for(string line; getline(fuente, line); ){
                istringstream in(line);
                int clave, cantidad;
                float precio;
                string nombre, caducidad;
                in >> clave >> nombre >> cantidad >> precio >> caducidad;
                data[contador].key = clave;
                data[contador].nombre = nombre;
                data[contador].cantidad = cantidad;
                data[contador].precioBase = precio;
                data[contador].caducidad = caducidad;
                contador++;
            }

            //Llenado de la tabla hash con los elementos leídos.
            for(int i = 0; i<75;i++){

                int res = data[i].key % 75;
                while(hash[res].cupo==1){
                    res++;
                    if(res == 75) res = 0;
                }

                hash[res].data = data[i];
                hash[res].key = data[i].key;
                hash[res].cupo = 1;
            }

        }

        //Funcion Hash utilizada para almacenar y encontrar los artículos.
        auto hasheo(int value) -> int {
            return value%75;
        }
        //Retorna un articulo dada la clave que lo identifica, o retorna un artículo vacío si no la encuentra.
        auto find(int key) -> articulo {
            int res = hasheo( key );
            int notfound = 0;
            //Utilizar busqueda secuencial para manejar las colisiones y encontrar el articulo.
            while(hash[res].key != key){
                res++;
                notfound++;
                if(notfound > 75) {
                    articulo vacio;
                    vacio.nombre = "articulo no encontrado";
                    vacio.caducidad = " ";
                    vacio.key = 404;
                    vacio.cantidad = 0;
                    vacio.precioBase = 0;
                    return vacio;
                    };
                if(res == 75) res = 0;
            }
            return hash[res].data;
        }

};


auto main() -> int {

    hashTable articulos;

    stack<articulo> venta;


    int temp;
    do{
        cout << "\n\n\nBienvenido.\n";
        cout << "\t1. Realizar venta.\n\t2. Buscar articulo por clave.\n\t\n\t0. Salir.\nSeleccione una opcion: ";
        cin.clear();
        cin >> temp;
        articulo producto;
        int clave = -1;
        float total = 0.0;
        switch(temp){
            //Realizar venta
            case 1:
                //Almacena los productos en la pila
                do{
                    cout << "Inserte la clave del producto (-1 para terminar): ";
                    cin >> clave;
                    //Busca el articulo por la clave, lo almacena (si existe) y lo muestra en la pantalla
                    if(clave > 0){
                        producto = articulos.find(clave);
                        venta.push(producto);
                        cout << "\tClave: " << producto.key << "\tArticulo: " << producto.nombre << "\t\tPrecio: $" << producto.precioBase*1.60 << endl << endl;
                        if(producto.key == 404) venta.pop();
                    }
                }while(clave > 0);
                //calcula el total a pagar y lo muestra en la pantalla
                cout << endl << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<< endl;
                cout << endl << endl << "\t\tInformacion de la venta: " << endl<< endl;
                while(venta.length() > 0){
                    producto = venta.top();
                    cout << "Clave: " << producto.key << "\tNombre: " << producto.nombre << "\t\tPrecio: $" << producto.precioBase*1.60 << endl;
                    total += venta.top().precioBase*1.60;
                    venta.pop();
                }
                cout << "\n\n\tTotal a pagar: $" << total<< endl;
                cout << endl << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<< endl;


                break;
            
            //Busca un articulo en la tabla hash mediante su identificador y lo muestra en la pantalla.
            case 2:
                cout << "\n\n\tIngrese la clave del articulo: ";
                cin.clear();
                cin >> clave;
                producto = articulos.find(clave);
                cout << endl << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<< endl;
                cout << "\tClave: " << producto.key << "\n\tNombre: " << producto.nombre << "\n\tCantidad: "  << producto.cantidad << "\n\tPrecio base: $" << producto.precioBase<< "\n\tCaducidad: "<< producto.caducidad << endl;
                cout << endl << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<< endl;
                break;

            case 0:
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }
    }while(temp != 0);

    return 0;
}