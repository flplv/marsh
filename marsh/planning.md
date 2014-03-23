## This is the planning of the project ##

### Adopted techinques and processess ###


### Pending Tasks ###

1) New compositon structure

 - Remove the figure of a container to contain widgets, instead,
   use the pattern of compositon on the widget class.
   Widgets must contain other widgets by itself, creating a 
   tree of widgets, each parent can have many childs, each child
   can have only one parent (based on Qt).
	
		added:       19 / 03 / 2014
		implemented: not yet.
		

2) Canvas painting model

 - Should be necessary to use a canvas object in order to paint
   anything to the framebuffer. This canvas object will be
   responsible to map framebuffer area and decode what should
   and should not be painted based on the intersection of the 
   screen and the canvas position and dimensions.
   
 - The current framebuffer painting system is poorly planned
   and must not be incremented, but replaced.
	
		added:       19 / 03 / 2014
		implemented: not yet.

3) Drawing Interface model

 - TBD: A drawing interface model must be described, that seemless 
   couple to the Canvas Painting model.
 - Drawing algorithms (rectangle, borders, etc) has nothing to do
   with the Drawing Interface model, and must be placed inside an
   algorithms module.

		added:       19 / 03 / 2014
		implemented: not yet.
		
		
4) Rename object creation and destruction

 - Objects are allocad within create functions, rename these
   functions to object_new and object_delete.
 - If alloc and free is not used in object creation an destruction,
   than the functions shall be named object_init and object_deinit.
 - It is not expect to marsh stop using malloc, or provide a static
   allocation choice. It's planned to write a better allocation 
   algorithm to work well on small memory processors.
   
		added:       22 / 03 / 2014
		implemented: 23 / 03 / 2014 by Felipe Lavratti
		
5) Add widget collection to improve rubstness of memory management

 - It is important to keep a widget collection since widgets will
   be accessed from multiple parts of the code and they might be
   deleted and tried to be accessed later.
 - The widget collection shall be used to avoid access to already
   deleted widgets, or, even better, to avoid deletation of widgets
   already in use.

		added:       23 / 03 / 2014
		implemented: not yet.	
		