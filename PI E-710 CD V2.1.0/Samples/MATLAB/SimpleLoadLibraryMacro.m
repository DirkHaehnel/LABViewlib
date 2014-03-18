% Load the E7XX GCS dll
%  The files E7XX_GCS_DLL.dll and E7XX_GCS_DLL.h must be accessible
loadlibrary 'E7XX_GCS_DLL.dll'
% Show all functions of the DLL
libfunctionsview('E7XX_GCS_DLL')
% Connect to the controller (COM 1, 57600 baud)
ID = calllib('E7XX_GCS_DLL','E7XX_ConnectRS232',1,57600)
% Query the identification string
[ret,ControllerName] = calllib('E7XX_GCS_DLL','E7XX_qIDN',ID,blanks(100),100)
% Query connected axes
[ret,AxesNames] = calllib('E7XX_GCS_DLL','E7XX_qSAI_ALL',ID,blanks(100),100)
% Initialize stage on axis 1
[ret,axes] = calllib('E7XX_GCS_DLL','E7XX_INI',ID,'1')
% Query servo status
[ret,axes,svo] = calllib('E7XX_GCS_DLL','E7XX_qSVO',ID,'1',libpointer('int32Ptr',1))
% Close servo loop
[ret] = calllib('E7XX_GCS_DLL','E7XX_SVO',ID,'1',libpointer('int32Ptr',1))
% Command absolute motion
[ret] = calllib('E7XX_GCS_DLL','E7XX_MOV',ID,'1',libpointer('doublePtr',50.0))
% Query absolute position
[ret,axes,pos] = calllib('E7XX_GCS_DLL','E7XX_qPOS',ID,'1',libpointer('doublePtr',0.0))
disp('Type commands now or "return" to quit\n");
keyboard;
% Close down the connection
calllib('E7XX_GCS_DLL','E7XX_CloseConnection',ID)