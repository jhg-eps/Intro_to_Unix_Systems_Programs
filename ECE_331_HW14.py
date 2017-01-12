# Joseph Garcia
# ECE 331 Sheaff
# Homework 14
# May 6, 2015

#Request: Write a PYGTK application that grabs data from the temperature sensor and prints it to the terminal when you press the "Temp" button.
#Program is run using $ python ECE_331_HW14.py

#Note: Code is adapted from http://www.pygtk.org/pygtk2tutorial/sec-ToggleButtons.html#togglefig
#ECE_331_HW11 script on the next page

import pygtk
pygtk.require('2.0')  # import proper pyGTK version
import gtk
import ECE_331_HW11  #import other python script that actually grabs the temperature from the i2c bus

class TempButton:          #creation and initialization of TempButton class
    def grabtemp(self, widget, data=None):   # function called when "Temp" is pressed.
		ECE_331_HW11.main()	    # grab the temperature data
	
    def delete_event(self, widget, event, data=None):   #Properly close out the GTK window and buttons when user hits "x" or "Quit"
        gtk.main_quit()
        return False

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)    # Create a new window
        self.window.set_title("Temperature Reader")     # Set the window title
        self.window.connect("delete_event", self.delete_event)  # set handler to immediately exit GTK if "x" or "Quit" buttons pressed

        self.window.set_border_width(20)    # Window has border width 20
        vbox = gtk.VBox(True, 10)    # creates vertical box, sets 10 pixels of spacing between them.
        self.window.add(vbox)   # Put the vbox in the main window
        button = gtk.Button("Temp")   #Create the "Temp" button so that users can get temperature whenever they desire
        button.connect("clicked", self.grabtemp, "Temp")  # do the temperature grabbing upon "Temp" button being pressed

        vbox.pack_start(button, True, True, 2)   # Insert the first button, "Temp"

        button.show()    #show "Temp"
        button = gtk.Button("Quit")   # Create "Quit" button

        button.connect("clicked", lambda wid: gtk.main_quit()) # popup closes out
        vbox.pack_start(button, True, True, 2)    # Insert the quit button

        button.show()
        vbox.show()              # show the final button, vertical box, and show the completed window.
        self.window.show()

def main():
    gtk.main()  # run the primary code (class init and event handling) in the script.
    return 0       

if __name__ == "__main__":
    TempButton()    # do the class initialization 
    main()            # execute the main code 
