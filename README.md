**WHAT IS IT?**

C++ daemon that monitorize CPU, RAM and HHD usage in the server and shares the real time status via cURL to other server.

You can have the statics in real time anywhere you want to.

**HOW DO I MAKE IT WORK?**

**IN SERVER**

* clone and go to /serverside
* '$ make'
* '$ ./configurate' and follow the steps (explained bellow)
* '$ sudo make install'
* run the daemon by '$ make run' or '$ thewatchman'

**CONFIGURATION**

TheWatchman needs a configuration file, all configurations files has to start with a commented line (gonna fix it ASAP)

TheWatchman config file will be automaticly created by runing the 'configurate' script or, once installed, runing thewatchman-conf in your shell;

The configuration script will lead you by some simple steps to configure the daemon.

* **Server name (MID)**: An identificator that will be sent as key in the json.
* **Host to send data (URL)**: The url where the json will be sent, it has to include http://, port and path (ex: www.google.com/thewatchman/hellomachine.php)
* **Get variable name for data (GET_NAME)**: The name of the get variable that will be sent with the json
* **Interval (INTERVAL)**: The time in miliseconds that the daemon will wait to make another call

You can change or create another config files editing the config file, you can have a peek at the automatcly created in ~/.config/thewatchman.conf

Yo can pass custom config files when running the daemon as first parameter:
`thewatchman PATH/TO/CONF/FILE`

**IN CLIENT**

There's an application folder that contents some sample files that stores the get call made from the server and displays it in real time.

Just host the application folder somewhere publicly and point to the hellomachine.php script once you are configuring the server daemon.

**WHAT DO I HAVE TO KNOW?**
* TheWatchman will use libcurl eventually, i brainfarted so bad trying to make it portable, so, i gonna work in it once the core is stable and works as i want it to.
* TheWatchman displays data in MB
* TheWatchman creates a log in the HOME folder named thewatchman.log, you can look there to spot errors and missfunctions

**MULTIPLATFORM**
This software has been developed and tested in Arch linux x64 and Ubuntu server, i tried to make it as portable as it could be but i cant assure it works in other platforms
By default it may work well in \*nix systems.

If you have any question, advice or kisses to send, text me pablo.deving@gmail.com
