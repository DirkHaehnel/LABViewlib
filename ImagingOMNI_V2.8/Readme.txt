IMAGING OMNI CD CONTENTS

CONTENTS
- PassthruEx in DT-Active Open Layers
- Sharing Components Among DT-Active Frame Grabber Controls
- Known Issues with the DT3152, DT3152-LS, DT3153, DT3154, 
     DT3155, and DT3157 
- Known Issues with the DT3120 
- Known Issues with the DT3145


PASSTHRUEX IN DT-ACTIVE OPEN LAYERS

PassthruEx is not fully implemented in DT-Active Open Layers 
version 2.0. Therefore, if your board supports passthruex and 
you need this functionality, it is recommended that you use the 
Data Acq SDK instead of DT-Active Open Layers.


SHARING COMPONENTS AMONG DT-ACTIVE FRAME GRABBER CONTROLS

The DT-Active Frame Grabber controls use some components that 
are also used by other DT-Active Frame Grabber controls. If you 
are programming in Visual Basic, if both a DT3162 frame grabber 
board and a DT3145 frame grabber board are installed in your 
system, and if you are using both the DT3162 and DT3145 in the 
same application (both controls are on the form), you may see 
two instances of the shared components (DTBuffer object, DTOverlay 
object, and DTError object). To ensure that you are using the 
correct instance of the component, check the bottom pane of the 
Object Browser and make sure that the component is a member of 
DTFGCLNKLibCtl. If you are programming in Visual C++, whenever 
you insert a new instance of a shared component into your 
project, Visual C++ automatically renames the new component. 
For example, if you insert a second instance of the DTBuffer 
object, Visual C++ renames the second component DTBuffer1. 


KNOWN ISSUES WITH THE DT3152, DT3152-LS, DT3153, DT3154, DT3155, 
AND DT3157 

1. If you are using multiple DT3152, DT3152-LS, DT3153, DT3154, 
DT3155, or DT3157 boards in your system, the Control Panel may 
display the board aliases incorrectly (as swapped). However, 
the boards will work properly in your application.

2. If you are using multiple DT3152, DT3152-LS, DT3153, DT3154, 
DT3155, or DT3157 boards in your system and your system does 
not restart after you add an additional board, remove the last 
board, reconfigure the first board to use less memory, add the 
board again, then restart the system.


KNOWN ISSUES WITH THE DT3120 

1. If you have an infinite timeout period (you set the timeout 
period to 0), you are using an external trigger, and you decide 
to terminate the application before the external trigger occurs 
with CTRL-ALT-DELETE, you will need to reboot your system before 
you can use the driver. To avoid this problem, use asynchronous 
acquisitons and then close the device with OlImgCloseDevice 
before exiting the application, or use a specified timeout period.

2. If you are using multiple DT3120 boards in your system, the 
Control Panel may display the board aliases incorrectly 
(as swapped). However, the boards will work properly in your 
application.


KNOWN ISSUES WITH THE DT3145 

1. The current release of the DT-Active Camera Link Frame Grabber 
control does not support user-programmed LUTs (look-up tables). 
You can use the LoadLUTFromFile method to load the LUT files 
provided on the Imaging OMNI CD. However, you cannot use the 
LoadLUTFromArray, SaveLUTToFile, and SaveLUTToArray methods. 

2. If you are displaying very large ROIs or frames of lines 
(typically, larger than 2K x 2K), you may receive an error if 
your graphics card does not have enough memory. If this happens, 
you can try reducing the size of the ROI/frame that you are 
acquiring, adjusting the source origin of the ROI/frame to 
display another part of the ROI/frame, increasing the memory in 
your graphics card, or updating the driver for your graphics 
card. If you still have problems, you can change the display 
mode to GDI (Graphics Display Interface) using either the 
DT3145 property pages or the DisplayMode property. Note that 
typically, you will use DDI (DirectDraw Interface) mode; it is 
faster and requires less system memory. However, if you are 
having display problems, you can change to GDI mode. 

3. For best display performance, you should set your system 
color to True Color (32 bit). 

4. The list of cameras supported for use with the DT-Active Camera 
Link Frame Grabber control is constantly expanding. Refer to the 
Data Translation web site 
(http://www.datx.com/imaging/CameraLinkInfo/default.htm) for 
information on the cameras supported. 
