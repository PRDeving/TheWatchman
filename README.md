WHAT IS IT?

C++ daemon that monitorize CPU and RAM usage in the server and shares the real time status via cURL to other server

HOW DO I MAKE IT WORK?

clone and go to /serverside
$ make
$ ./configurate
$ sudo make install

CONFIGURATION

TheWatchman need a configuration file, all configurations files has to start with a commented line (gonna fix it ASAP)

There are 4 variables that you can change by configuration script or editing the config file:

-MID or server name: it's the name that the reciever server will have to identify the server side

-URL or Host to send: the url where the client server is listening

-GET_NAME or name of get var: the name of the url get variablepased

-INTERVAL: the miliseconds that the daemon waits to send another call to the listener
