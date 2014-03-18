	   DT-OpenLayers 32 Bit Frame Grabber SDK for Windows 
	   Version 3.1   Source: Imaging OMNI CD 1.2
____________________________________________________________________

The DT-OpenLayers 32bit Frame Grabber Software Development Kit (SDK) provides a
programming framework for developing device-indepedent applications based on the
DT-OpenLayers Application Programming Interface (API) specification for monochrome
color frame grabbers.

____________________________________________________________________
*** IMPORTANT NOTE *** about Camera Files.

The example programs now support the use of Camera Files, which load the necessary
parameters to use a given camera type.  These are mostly used if using the DT3157
Digital Frame Grabber.  When modifying an existing file or creating a new one, it is
important to note that the DIO setting values are NOW entered as a DECIMAL value
in all example programs.

Note: DT3157 boards require you to load a camera file (see below) after opening a
board and before image acquisition. Other boards (DT3152/DT3155) do not require
these camera files.

Camera files (editable ASCII format):
fgsdk32\Camfiles\640_480.CAM		- Generic 640x480 camera
fgsdk32\Camfiles\KDK_ES10.CAM		- Kodak ES10 camera
fgsdk32\Camfiles\KDK_1P4I.CAM       	- Kodak 1P4I camera
fgsdk32\Camfiles\KDK_1P6I.CAM       	- Kodak 1P6I camera
fgsdk32\Camfiles\PLNX1001.CAM		- Pulnix 1001 camera
fgsdk32\Camfiles\PLNX9701.CAM		- Pulnix 9701 camera
fgsdk32\Camfiles\HAM474295.CAM		- Hamamatsu C4742-95 camera

____________________________________________________________________
*** IMPORTANT NOTE to Microsoft(tm) Visual C++ Version 4.0 and greater users***

The Acq2Hst and Passthru C example programs are shipped with Version 2.0 format
mak files. If you intend on converting these projects to version 4.0 or greater, make sure 
that you save the project back to the orginal name. Failure to do so will cause problems 
with the bitmaps file compiling into the executable correctly. This is a bug in MSVC 4.x.
Because of a bug in MSVC 4.0 you will not be able to view or edit the bitmaps contained
in these example programs unless your video adapter is in 256 color mode. You can upgrade
to version 4.1 or set your video adatper to 256 color mode to work around this problem.

____________________________________________________________________
*** IMPORTANT NOTE to MACH Series SP0595 DTOL95 Frame Grabber SDK users***

If you require to use the original Windows 95 Frame Grabber SDK(SP0595) with the new 32bit
Frame Grabber(SP0585) in Windows 95, please contact Data Translation Technical Support 
to receive special instructions.  508-481-3700

____________________________________________________________________
DTAcquire Example

This example program provides a means to quickly verify that your hardware and the
driver software were installed correctly and are operating normally and supports file
saving in various graphic formats.
____________________________________________________________________
Acq2Hst Example

The purpose of the Acq2Hst example is to give you a complete ready to use 
example of a program using the DT-OpenLayers style of programming.

Note: Active frame parameters must be entered before images can be acquired
with the DT3157 board.  This is automatically accomplished when opening the Device.
A dialog box appears asking the user to load a Camera File, which examples have
been provided.
____________________________________________________________________
Passthru Example

The purpose of Passthru example is to give you an example of how to
handle Windows resources, bitmaps, palettes, etc when using passthru,
and how to acquire a frame and update the windows when passthru has
been completed. In addition to these features, the Passthru example now 
demonstrates how to implement the overlay features on frame grabber boards
that support this feature.

Note that this example is not supported by the DT3120 board; for the 
DT3120 board, use the PassthruMFC example instead.
____________________________________________________________________
PassthruMFC Example

The PassthruMfc example allows you to perform a passthru operation with
the DT3120 board and any other future boards that use the WDM driver model.

____________________________________________________________________
LS-Acquire Example

LS-Acquire allows you to verify that your MACH Series board is working properly in line-scan mode.
