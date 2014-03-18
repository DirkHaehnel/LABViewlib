# Microsoft Developer Studio Project File - Name="ACQ2HST" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ACQ2HST - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ACQ2HST4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ACQ2HST4.mak" CFG="ACQ2HST - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACQ2HST - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ACQ2HST - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACQ2HST - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# ADD BASE CPP /nologo /MT /W3 /GX /Ox /I "c:\dtol\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /Od /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x8192 /subsystem:windows /machine:IX86
# ADD LINK32 olimg32.lib olfg32.lib oldnames.lib gdi32.lib user32.lib comdlg32.lib /nologo /stack:0x8192 /subsystem:windows /map /machine:IX86 /out:".\WinRel\ACQ2HST.exe" /libpath:"..\..\lib"

!ELSEIF  "$(CFG)" == "ACQ2HST - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /I "c:\dtol\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /stack:0x8192 /subsystem:windows /debug /machine:IX86
# ADD LINK32 olimg32.lib olfg32.lib oldnames.lib gdi32.lib user32.lib comdlg32.lib /nologo /stack:0x8192 /subsystem:windows /map /debug /machine:IX86 /out:".\WinDebug\ACQ2HST.exe" /libpath:"..\..\lib"

!ENDIF 

# Begin Target

# Name "ACQ2HST - Win32 Release"
# Name "ACQ2HST - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\A2D.C
# End Source File
# Begin Source File

SOURCE=.\ACQ2HST.C
# End Source File
# Begin Source File

SOURCE=.\ACQ2HST.DEF
# End Source File
# Begin Source File

SOURCE=.\ACQ2HST.RC
# End Source File
# Begin Source File

SOURCE=.\FRAMESIZ.C
# End Source File
# Begin Source File

SOURCE=.\FRAMETYP.C
# End Source File
# Begin Source File

SOURCE=.\INP_FILT.C
# End Source File
# Begin Source File

SOURCE=.\INP_SRC.C
# End Source File
# Begin Source File

SOURCE=.\PIXELCLK.C
# End Source File
# Begin Source File

SOURCE=.\SYNCSENT.C
# End Source File
# Begin Source File

SOURCE=.\TIMEOUT.C
# End Source File
# Begin Source File

SOURCE=.\UTILS.C
# End Source File
# Begin Source File

SOURCE=.\VIDEOSEL.C
# End Source File
# Begin Source File

SOURCE=.\VIDEOSIZ.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Acq2hst.h
# End Source File
# Begin Source File

SOURCE=.\RESOURCE.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
