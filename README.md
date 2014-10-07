marsh
=====

    Pure C Graphic Library
    Platform Independent
    With a widget tree able to propagate custom events and install custom event handlers to widgets.


You need to build cpputest (not install) in order to build and run
marsh.

	1) unzip cpputest-3.5.zip
	2) cd cpputest-3.5
	3) ./configure
	4) make

To build marsh:
	
	1) cd marsh
	2) make all

Marsh simulator has dependency with libgtkmm-3.0-dev, you may need to install
it in order to build.

	In Ubuntu:

	1) sudo apt-get install libgtkmm-3.0-dev

You will have two executables:
	- marsh-tests runs the CppUTest tests
	- marsh-simu runs a linux example
