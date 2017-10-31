# game

Es necesario descargar e instalar la librería sfml, esta puede ser obtenida de la página oficial

https://www.sfml-dev.org/

Se debe compilar en c++ 11. Usar los siguientes comandos 

g++ -c Main.cpp -std=gnu++11

g++ Main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -std=gnu++11

Para correr el juego (con el mapa actual) ejecutar:

./sfml-app < MAP.txt

De esta forma se crea la aplicación con el nombre sfml-app
Los comandos para mover el jugador son los siguientes: <br />
W : arriba <br />
S : abajo <br />
A : izquierda <br />
D : derecha <br />

Los demas comandos son los siguientes: <br />
C : aparecer la moneda <br />
X : desaparecer la moneda <br />
M : mostrar el mapa <br />
N : desaparecer el mapa <br />

