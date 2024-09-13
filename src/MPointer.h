#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <stdexcept>

#include "MPointerGC.h"

//dice de que tipo va a ser 
//esto permite que yo pueda cuando utlizo  Mpointer sea entero
template <typename T>

class MPointer
{
private:
    //creo un puntero para el int 
    T *ptr;
    //este id se utiliza para darle tener un identificador al Mpointer 
    int id;
    //estos permiten creara instancias de Mpointer
    //el mpointer GC es el que le da el identificador esta en las isntrucciones 
    // este es un construcotr que guarda el valor y le asigna un puntero 
    explicit MPointer(T *p) : ptr(p), id(MPointerGC::getInstance().registrarPuntero(p)) {}
    //para crear un pointer con nulo se pone con -1
    explicit MPointer(std::nullptr_t) : ptr(nullptr), id(-1) {}

public:
    // este es el metodo estatico que hace el new 
    // lo que hcae es crear Mpointer con un nuevo puntero al valor 
    // es decir va a crear va a crear un objeto mPointer y dentro de el va a contener un puntero 
    // ala direccion de memoria que el tipo de dato requiere eso hace el new T
    static MPointer<T> New()
    {
        return MPointer<T>(new T());
    }

    // aqui lo que hace es pasar un valor este crea un espacio y luego en ese espacio agrega el  valor  
    // y luego retorna el puntero 
    static MPointer<T> New(T valor)
    {
        MPointer<T> nuevoPuntero = MPointer<T>(new T());
        *nuevoPuntero.ptr = valor;
        return nuevoPuntero;
    }
    // otra forma de crear Mpointer pasando nulo
    static MPointer<T> New(std::nullptr_t)
    {
        return MPointer<T>(nullptr);
    }

    // otra forma de crear Mpointer pasando otro 
    static MPointer<T> New(MPointer<T> &otro)
    {
        return MPointer<T>(otro);
    }
    //IMPORTANTE
    // ESTA linea dice que cuando vaya en el archivo main
    // y haga *ptr1 =10 esto va a meter el valor 10 dentro del puntero en el espacio que el reservo para el entero 10 
    // es decir cuando yo haga Mpointer* le estoy hablando a ese puntero  
    T &operator*()
    {
        return *ptr;
    }
    // este retorna cual es el puntero que existe al espacio de memoria que yo reserve 
    // entonces si yo vengo y pido la direccionel me va a dar del puntero
    T *operator&()
    {
        return ptr;
    }
    // aqui el operador flecha es como puntero 
    T *operator->() const
    {
        return ptr;
    }
    //este boleano nos permite revisar que exista 
    // entonces cuando le piden un booleanoa nuestro Mpointer se va a definir con base en que el puntero exista

    operator bool() const
    {
        return ptr != nullptr;
    }
    // para esta de asignacion que cuando yo haga Mpointer 
    //   y le paso que sea igual a nulo 
    MPointer &operator=(std::nullptr_t)
    {
        // sino hay puntero quiero que le baje las referencias en el recolector de basura
        // para que en caso que el recolector de basura considera que ya no se utiliza entonces
        // lo borre en cierto momento 
        if (ptr)
        {
            MPointerGC::getInstance().decrementarContadorReferencias(id);
            //quitamos el espacio 
            ptr = nullptr;
        }
        return *this;
    }
    // aqui tenemos el Mpointer que es un constructor 
    // que lo que hcae es cuando le enviamos otro Mpointer verifica y copia los datos que este tiene 
    // Basicamente lo copia el mismo id y valor que tenia el otro 
    MPointer(const MPointer &otro) : ptr(new T(*otro.ptr)), id(otro.id)
    {
        // se incrementa el contador de referencias 
        MPointerGC::getInstance().incrementarContadorReferencias(id);
    }
    // aqui es lo que el valor anterior y guardar el valor dentro de el
    // esto permite el ptr3 =30 en el main 
    // lo q hace es que lo q se le pase por valor quiero que lo guarde en el espacio y retorne la referencia del mismo 
    MPointer<T> &operator=(T valor)
    {
        *ptr = valor;
        return *this;
    }
    // 
    MPointer &operator=(const MPointer &otro)
    {
        if (this->id == -1 || !this->ptr)
        {
            ptr = otro.ptr;
            id = otro.id;
            return *this;
        }
        //si son punteros diferentes decrementar al puntero referencia que le quito el valor 
        // y asignarlo a otro 
        // entonces le doy los datos a uno y a el otro lo boto 
        if (this != &otro)
        {
            if (ptr)
            {
                MPointerGC::getInstance().decrementarContadorReferencias(id);
            }

            ptr = otro.ptr;
            id = otro.id;
            MPointerGC::getInstance().incrementarContadorReferencias(id);
        }
        return *this;
    }

    int obtenerId() const
    {
        return id;
    }
    // el destructor dice que si hay un puntero id que liberar 
    // entonces decrementa el contador referencia usando el ID 
    ~MPointer()
    {
        if (ptr)
        {
            MPointerGC::getInstance().decrementarContadorReferencias(id);
        }
    }
};

#endif
