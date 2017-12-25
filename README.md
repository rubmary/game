# game

Es necesario descargar e instalar la librería sfml, esta puede ser obtenida de la página oficial

https://www.sfml-dev.org/

Se debe compilar en c++ 11. Usar los siguientes comandos 

g++ -c game.cpp -std=gnu++11

g++ game.o -o game -lsfml-graphics -lsfml-window -lsfml-system -std=gnu++11

Para correr el juego (con el mapa actual y archivos actuales) ejecutar:

./game

De esta forma se crea la aplicación con el nombre sfml-app
Los comandos para mover el jugador son los siguientes: <br />
W : arriba <br />
S : abajo <br />
A : izquierda <br />
D : derecha <br />

Los comandos para aparecer y desaparecer el mapa son: <br />
M : mostrar el mapa <br />
N : desaparecer el mapa <br />

