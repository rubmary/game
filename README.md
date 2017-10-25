# game

Es necesario descargar e instalar la librería sfml, esta puede ser obtenida de la págin oficial

https://www.sfml-dev.org/

Se debe compilar en c++ 11. Usar los siguientes comandos 

g++ -c Main.cpp -std=gnu++11

g++ Main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -std=gnu++11

De esta forma se crea la aplicación con el nombre sfml-app
Al correr el programa se debe ingresar un número (entre el 1 y 5) para seleccionar el tipo de movimiento deseado.
Los comandos para mover el jugador son los siguientes:
W : arriba
S : abajo
A : izquierda
D : derecha
