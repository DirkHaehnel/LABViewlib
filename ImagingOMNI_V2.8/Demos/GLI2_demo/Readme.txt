GLOBAL LAB Image/2                                 Version 3.6
Release Notes Document                          February, 2004
Data Translation - 100 Locke Drive, Marlboro, MA  01752


CONTENTS OF THIS FILE
-- Known Limitations of the Software
-- Hardware Issues
-- Programming Language Issues
-- Script and Code Examples
-- Custom Application/Tools Issues


KNOWN LIMITATIONS OF THE SOFTWARE

1. GENERAL: If you are using the same ROI with multiple tools, 
you may have to deselect the ROI and then reselect it to ensure
that the ROI appears in a tool. 

2. GENERAL: Tools that allow DDE transfer of data to Excel 
(such as the Blob Analysis tool) do not allow you to increment 
row and column numbers. If you want to use DDE transfer in a 
point & click script and do not want your data to be overwritten 
each time the point & click script is run, it is recommended 
that you use the GLI/2 Export tool. For more information, refer 
to the GLI/2 documentation. 

3. GENERAL: If a tool has AUTOSTART enabled, the tool may remain
in the Windows task after you close the GLI/2 application. If 
this happens, close the tool manually. 

4. GENERAL: Under certain circumstances, the file ILERR.OUT may 
appear on your desktop. If this happens, you can just delete 
this file from the desktop. 

5. POINT & CLICK SCRIPT TOOL: Point & click scripts support 
relative paths in GLI/2 Ver. 3.5 and higher. If you have a 
point & click script created in a previous version of GLI/2, 
run the point & click script from the location where it was 
created, then save the script. The script will now support 
relative paths. 

6. POINT & CLICK SCRIPT TOOL: For some tools (such as the 
Gauge and Prompt tools), the tool's point & click script 
variables are not immediately brought into the Point & Click 
Script tool. If you have enabled the point & click script 
variables, but the variables do not appear in the Point & 
Click Script tool, click the Step button until you have 
stepped past the appropriate tools's script item. The 
variables should then appear. 

7. DIGITAL I/O TOOL: The Digital I/O tool provided with GLI/2 
Ver. 3.5 is not compatible with the DT2820 (Digital I/O) tool 
provided in previous versions of GLI/2. If you created a 
point & click script using the DT2820 Digital I/O tool, you 
must edit your script to include the new Digital I/O tool. 

8. DIGITAL I/O TOOL: At the present time, the Digital I/O tool 
always operates synchronously. You must wait until the timeout 
period expires before you can perform another operation. 

9. DISPLAY TOOL: The two left-most Curve Type buttons (highest 
point and lowest point) do not work properly. If you click one 
of these two buttons, you must click the Restore Original LUT 
button before you can change the curve style again. 

10. DISPLAY TOOL: The X-Axis End Points option does not work 
properly. If this option is disabled when you open the Display 
tool (e.g., you had an 8-bit grayscale image open at the time), 
the option does not become enabled when you open a 32-bit or 
floating-point grayscale image. You must open a new instance of 
the Display tool to enable this option. 

11. EDGE FINDER TOOL: When editing the Edge Finder tool from the 
Point & Click Script tool, you may find that the Edge Finder 
Settings dialog box is not available. If this occurs, close the 
Edge Finder tool, click Edit in the Point & Click Script again, 
and then reselect Edge Finder Settings from the Options menu. 

12. EXPORT TOOL: The Selected Variables list cannot hold more than 
100 variables. If you want to use more than 100 variables from the 
P & C Script Variables window, try using a wildcard (* or ?) to 
specify multiple variables in a single item. Refer to the Export 
tool documentation for more information. 

13. HISTOGRAM TOOL: Make sure that you select the Calculate 
Histogram Stats checkbox immediately before you add the Histogram 
tool to the script. If you select the Calculate Histogram Stats 
checkbox too early, the histogram statistics may not update 
correctly. 

14. HISTOGRAM TOOL: In some situations, the Histogram tool may not 
respond as expected. If this happens, you must close the Histogram 
tool, delete the Histogram step from the point & click script, 
open the Histogram tool again, then create a new Histogram script 
step. Make sure that you delete the script step BEFORE you open a 
new Histogram tool. If you create a second script step before you 
delete the original Histogram script step, the Histogram statistics 
variables may contain invalid data. 

15. LINE PROFILE TOOL: If the measurement bars on the graph 
disappear from view and you want them to reappear, deselect the 
Func\Keep Bars in View option and then reselect it. You can also
double-click in the graph with the right mouse button to redraw 
the bars. 

16. PICTURE TOOL: If you specify a long time delay between image 
acquisitions (typically, longer than one minute), your system may 
hang. If this happens, try the operation again with a shorter 
time delay. 

17. PICTURE TOOL: If you specify a large number of frames to 
acquire (typically, more than 100), your system may appear to 
hang. If this happens, try the operation again with fewer frames. 

18. PICTURE TOOL: Date/time stamping is supported for 8-bit 
grayscale images, 24-bit RGB color images, and 24-bit HSL color 
images only. It is not supported for 16-bit, 32-bit, or 
floating-point grayscale images. 

19. THRESHOLD TOOL: If you want to provide your own name for the 
output image, you can enter the name in the Output Image text box. 
However, if you do, make sure that you do not close the Threshold 
Controls dialog box (by clicking OK in the Threshold Controls 
dialog box) before you threshold the image (by clicking Threshold 
in the Threshold display graph). If the Threshold Controls dialog 
box is closed when you threshold the image, the software 
automatically gives the output image the default name Output:x. 


HARDWARE ISSUES

If your monitor is set to 256 colors, binary images show black 
pixels as black, not magenta. If you cannot discern the transition
between black and white clearly enough for your needs, try 
adjusting the monitor settings.  

At the present time, the firewire plugin supports firewire and USB
video cameras that are DirectShow compatible. (Microsoft® DirectShow® is
an architecture for streaming media on the Microsoft Windows® 
platform.) DirectX 9.0 is required to use this firewire plugin.
Note also that the firewire plugin supports only uncompressed data 
from the camera in either 8-bit monochrome or 24-bit RGB format; it 
does not support any firewire DV devices.

If your firewire/USB camera does not appear in the Device Manager:
 
 o Ensure that the camera is connected and powered up
 o Ensure that the device driver for your camera is properly installed
 o Ensure that DirectShow is installed on your computer. You can
   download the latest DirectShow software from the Microsoft website. 



PROGRAMMING LANGUAGE ISSUES

The product supports application and custom tool development using 
Microsoft Visual C++ 6.0. 


SCRIPT AND CODE EXAMPLES

GLI/2 is shipped with several examples to help you get started 
creating either point & click scripts or your own custom tools 
and application programs. 

-- The C:\PROGRAM FILES\DATA TRANSLATION\GLOBAL LAB Image2\SCRIPTS 
   directory contains several point & click scripts. These are 
   intended for GLI/2 users who want to use the Point & Click 
   Script tool to create simple scripts to automate repetitive 
   tasks within the GLI/2 environment. Programming experience is 
   helpful, but not required. 

-- The C:\PROGRAM FILES\DATA TRANSLATION\GLOBAL LAB Image2\
   c++ DEVEL\EXAMPLES directory contains several Visual C++ 6.0 
   projects. These are intended for experienced programmers who 
   want to develop custom tools and applications using the 
   GLI/2 API within the Visual C++ 6.0 environment. 


CUSTOM APPLICATION/TOOLs ISSUES

1. If you create your own custom tools, you must have the complete 
Microsoft Visual Studio, Ver. 6.0 or later, installed on your 
system. If necessary, you can use the debug version of GLI/2 
(GLI2D.EXE) to debug your custom tools. GLI2D.EXE is located in 
C:\PROGRAM FILES\DATA TRANSLATION\GLOBAL LAB Image2\BIN, by default. 

2. Before you compile a custom tool (project) that you created 
using the GLI/2 APIs, make sure that you add the files DT_TOOL.CPP 
and DT_TOOL.H to the project. DT_TOOL.CPP and DT_TOOL.H are located 
in C:\PROGRAM FILES\DATA TRANSLATION\GLOBAL LAB Image2\
C++ DEVEL\EXAMPLES\TOOLS\CHANGE, by default. 

3. To run a custom application that you created using the GLI/2 
APIs, the files DTAPI.DLL, DTDEVMAN.DLL, and DTTOOLS.DLL must be 
located in the same directory as your custom application. These 
files are located in C:\PROGRAM FILES\DATA TRANSLATION\GLOBAL LAB 
Image2, by default. If your custom application is located in a 
different directory, copy these files and place it in the directory 
containing the custom application. 


