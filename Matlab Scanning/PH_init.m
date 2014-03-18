function [res, dev, fid] = PH_init(name, Tacq, a0, b0, na, nb, dr, dt, fa, sa)

%  Prepare PicoHarp 300 for T3R-Measurement

% (c) Ingo Gregor 2006

% Defininitions

dev = [];
fid = [];

MAXDEVNUM  =          7;
MODE_T3	   =          3;

ACQTMIN	   =          1;		% ms
ACQTMAX	   =   36000000;	    % ms  (10*60*60*1000ms = 10h)


PH_ERROR_DEVICE_OPEN_FAIL	=  -1;
FILE_OPEN_ERROR             =  -9;

PH_INIT_ERROR               = -10;
PH_GETLIBVERSION_ERROR      = -11;
PH_GETHWVERSION_ERROR       = -12;
PH_SYNCDIV_ERROR            = -14;
PH_CFDLEVEL_ERROR           = -15;
PH_CFDZERO_ERROR            = -16;
PH_SETRANGE_ERROR           = -17;
PH_SETOFFSET_ERROR          = -18;
PH_CALIBRATION_ERROR        = -19;
PH_GETRESOLUTION_ERROR      = -20;
PH_SETMARKEREDGES_ERROR     = -31;
PH_GETROUTERVERSION_ERROR   = -21;
PH_GETCOUNTRATE_ERROR       = -23;
PH_GETROUTINGCHANNELS_ERROR = -41;
 

% dummy = '123456789012345678901234567890123456789012345678901234567890';
% ret   = int32(0);

Offset       = 0;       %  you can change this
CFD_Zero0    = 10;      %  you can change this
CFDLevel0    = 150;     %  you can change this
CFD_Zero1    = 10;      %  you can change this
CFDLevel1    = 50;      %  you can change this
SyncDiv      = 4;       %  you can change this
Range        = 1;       %  you can change this

err = -99999;

% Load library if needed

if (~libisloaded('PHLib'))
    loadlibrary('PHLib.dll', 'PHLib.h');
end;

if (~libisloaded('PHLib'))
    fprintf('Could not open PHLib');
else

    err       = -999;

    CreatorName = zeros(1,18);
    
    if (nargin > 1)
        if (Tacq>=ACQTMIN)&&(Tacq<=ACQTMAX)
            err = 0;
            isScan = false;
            CreatorStr  = 'dummy';
            if ~isempty(name)
                CreatorStr = 'PH_FCS.m';
            end;
        end;
    end;

    if (nargin == 10)
            isScan = true;
            CreatorStr = 'PH_Scan2D.m';
    end;

    CreatorName(1:length(CreatorStr)) = CreatorStr;
    
    LibVersion    = '12345678';
    LibVersionPtr = libpointer('cstring', LibVersion);

    if (err == 0)
        [ret, LibVersion] = calllib('PHLib', 'PH_GetLibraryVersion', LibVersionPtr);
        if (ret<0)
            fprintf('\nError in GetLibVersion. Aborted.\n');
            err = PH_GETLIBVERSION_ERROR;
        end;
    end;

    fprintf('\nInitialising photon counting instrumentation...');

    Model      = '1234567890123456';
    Version    = '12345678';
    Serial     = '12345678';
    ModelPtr   = libpointer('cstring', Model);
    VersionPtr = libpointer('cstring', Version);
    SerialPtr  = libpointer('cstring', Serial);

    anzdev     = 0;
    HW_Model   = [];
    HW_Version = [];
    RT_Model   = [];
    RT_Version = [];


    for k=0:MAXDEVNUM
        if (err == 0)

            [ret, Serial] = calllib('PHLib', 'PH_OpenDevice', k, SerialPtr);

            if (ret==0)
                
                [ret] = calllib('PHLib', 'PH_Initialize', k, MODE_T3);
                
                if (ret<0)
                    fprintf('\nError during Initialization. Aborted.\n');
                    err = PH_INIT_ERROR;                    
                else
                    
                    [ret, Model, Version] = calllib('PHLib', 'PH_GetHardwareVersion', k, ModelPtr, VersionPtr);

                    if (ret<0)
                        fprintf('\nError in GetHWVersion. Aborted.\n');
                        err = PH_GETHWVERSION_ERROR;
                    else
                        anzdev             = anzdev + 1;
                        dev(anzdev)        = k;
                        SerNo(anzdev)      = uint32(str2double(Serial));
                        HW_Model           = strvcat(HW_Model, Model);
                        HW_Version         = strvcat(HW_Version, Version);
                    end;
                end;
            else
                if(ret~=PH_ERROR_DEVICE_OPEN_FAIL)
                    fprintf('\nPH init error %d. Aborted.\n',ret);
                    err = PH_INIT_ERROR;
                end;
            end;
        end;
    end;

    Resolution = zeros(anzdev, 1);
    anz_chan   = zeros(anzdev, 1);
    
    for k = 1:anzdev   % Prepare Hardware for measurement

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_SetSyncDiv', dev(k), SyncDiv);
            if (ret<0)
                fprintf('\nIllegal Sync. Divider (%ld). Aborted.\n',SyncDiv);
                err = PH_SYNCDIV_ERROR;
            end;
        end;

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_SetCFDLevel', dev(k), 0, CFDLevel0);
            if (ret<0)
                fprintf('\nIllegal CFDDiscriminMin (%ld). Aborted.\n', CFDLevel1);
                err = PH_CFDLEVEL_ERROR;
            end;
        end;

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_SetCFDLevel', dev(k), 1, CFDLevel1);
            if (ret<0)
                fprintf('\nIllegal CFD Level (%ld). Aborted.\n', CFDLevel1);
                err = PH_CFDLEVEL_ERROR;
            end;
        end;

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_SetCFDZeroCross', dev(k), 0, CFD_Zero0);
            if (ret<0)
                fprintf('\nIllegal CFDZeroCross (%ld). Aborted.\n', CFD_Zero0);
                err = PH_CFDZERO_ERROR;
            end;
        end;

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_SetCFDZeroCross', dev(k), 1, CFD_Zero1);
            if (ret<0)
                fprintf('\nIllegal CFDZeroCross (%ld). Aborted.\n', CFD_Zero1);
                err = PH_CFDZERO_ERROR;
            end;
        end;

        if (err == 0)
            [Offset] = calllib('PHLib', 'PH_SetOffset', dev(k), Offset);
            if (Offset<0)
                fprintf('\nError in SetOffset. Aborted.\n');
                err = PH_SETOFFSET_ERROR;
            end;
        end;

        if (err == 0)
            [Resolution(k)] = calllib('PHLib', 'PH_GetBaseResolution', dev(k));
            if (Resolution(k)<0)
                fprintf('\nError in GetBaseResolution. Aborted.\n');
                err = PH_GETRESOLUTION_ERROR;
            else
                Resolution(k) = Resolution(k)/1000;
            end;
        end;

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_TTSetMarkerEdges', dev(k), 1, 1, 1, 1);
            if (ret<0)
                fprintf('\nError in TTSetMarkerEdges. Aborted.\n');
                err = PH_SETMARKEREDGES_ERROR;
            end;
        end;

        if (err == 0)
            fprintf('\n Calibrating device %d : %s %s (%d)  ...', dev(k), HW_Model(k,:), HW_Version(k,:), SerNo(k));
            [ret] = calllib('PHLib', 'PH_Calibrate', dev(k));
            if (ret<0)
                fprintf('\nCalibration Error %d. Aborted.\n',ret);
                err = PH_CALIBRATION_ERROR;
            end;
        end;

        pause(1);

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_GetCountRate', dev(k), 0);
            if (ret<0)
                fprintf('\nError in GetCountRate %d. Aborted.\n',ret);
                err = PH_GETCOUNTRATE_ERROR;
            else
                Range = 0;
                while (Resolution(k)*2^(12+Range) < 1e9/ret)
                    Range = Range + 1;
                end;
            end;
        end;
       
        if (err == 0)
            [ret] = calllib('PHLib', 'PH_SetRange', dev(k), Range);
            if (ret<0)
                fprintf('\nError in SetRange. Aborted.\n');
                err = PH_SETRANGE_ERROR;
            end;
        end;

        if (err == 0)
            [Resolution(k)] = calllib('PHLib', 'PH_GetResolution', dev(k));
            if (Resolution(k)<0)
                fprintf('\nError in GetResolution. Aborted.\n');
                err = PH_GETRESOLUTION_ERROR;
            else
                Resolution(k) = Resolution(k)/1000;
            end;
        end;

        if (err == 0)
            [ret] = calllib('PHLib', 'PH_EnableRouting', dev(k), 1);
            if (ret<0)
                fprintf('\nNo router found!\n');
            else
                [ret, Serial, Version] = calllib('PHLib', 'PH_GetRouterVersion', dev(k), SerialPtr, VersionPtr);
                if (ret<0)
                   fprintf('\nError in GetRouterVersion. Aborted.\n');
                   err = PH_GETROUTERVERSION_ERROR;
                else
                   if (ret == 0)
                      [ret] = calllib('PHLib', 'PH_GetRoutingChannels', dev(k));
                      if (ret<0)
                         fprintf('\nPH_GetRoutingChannels error %ld. Aborted.\n', ret);
                         err = PH_GETROUTINGCHANNELS_ERROR;
                      end;
                   end;
                   anz_chan(k) = ret;
                   RT_Model    = strvcat(RT_Model, Serial);
                   RT_Version  = strvcat(RT_Version, Version);
                end;
            end;
        end;


        if (err == 0)
            fprintf('\n\n');
            fprintf('Lib. Version     : %16s\n', LibVersion);
            fprintf('HW Model         : %16s\n', HW_Model(k,:));
            fprintf('HW Version       : %16s\n', HW_Version(k,:));
            fprintf('Serial-#         : %16d\n', SerNo(k));

            fprintf('RT Model         : %16s\n', RT_Model(k,:));
            fprintf('RT Version       : %16s\n', RT_Version(k,:));
            fprintf('RT channels      : %16d\n',  anz_chan(k));
            
            fprintf('\n');
            fprintf('Range            : %12d   \n', Range);
            fprintf('Offset           : %12d ns\n', Offset);
            fprintf('CFD ZeroCross 0  : %12d mV\n', CFD_Zero0);
            fprintf('CFD Level     0  : %12d mV\n', CFDLevel0);
            fprintf('CFD ZeroCross 1  : %12d mV\n', CFD_Zero1);
            fprintf('CFD Level     1  : %12d mV\n', CFDLevel1);
            fprintf('Resolution       : %12.3f ns\n', Resolution(k));
        end;

    end;

    fid = zeros(1, anzdev);
    if ~isempty(name)
        if (err == 0)
            for n = 1:anzdev
                fname  = sprintf('%s_%d.pt3',name, n);
                fid(n) = fopen(fname,'w');
                if (fid(n)<0)
                    err = FILE_OPEN_ERROR;
                end;
            end;
        end;

        %     Write file header.....


        if (err == 0)
            for n = 1:anzdev
                a = HW_Model(n,:);
                fwrite(fid(n), a, 'char');                     %   Ident (1-..)                  0
                l = 16 - length(a);
                if (l>0)
                    a = []; a(1:l) = 0;
                    fwrite(fid(n), a, 'char');                 %   Ident (..-16)
                end;

                a = HW_Version(n,:);
                fwrite(fid(n), a, 'char');                     %   FormatVersion (1-..)         16
                l = 6 - length(a);
                if (l>0)
                    a = []; a(1:l) = 0;
                    fwrite(fid(n), a, 'char');                 %   FormatVersion (..-6)
                end;

                fwrite(fid(n), CreatorName, 'char');           %   CreatorName (1-18)           22

                a = []; a(1:5) = 0;
                fwrite(fid(n), '1.0.0.0', 'char');             %   CreatorVersion (1-7)         40
                fwrite(fid(n), a, 'char');                     %   CreatorVersion (8-12)

                a = clock;
                FileTime = sprintf('%02d/%02d/%02d %02d:%02d:%02d', a(3), a(2), mod(a(1),100), a(4), a(5), floor(a(6)));
                fwrite(fid(n), FileTime, 'char');              %   FileTime    (1-17)           52
                fwrite(fid(n), 0, 'char');                     %   FileTime    (18)

                a = [13 10];
                fwrite(fid(n), a, 'char');                     %   'CR/LF'                      70

                a(1:256) = 0;
                fwrite(fid(n), a, 'char');                     %   Comment (256)                72

                fwrite(fid(n),      0, 'int32');               %   NCurves (1)                 330
                fwrite(fid(n),     32, 'int32');               %   BitsPerRecord (1)           332
                fwrite(fid(n),      1, 'int32');               %   RoutingChannels (1)         336
                fwrite(fid(n),      1, 'int32');               %   NumberOfBoards (1)          340
                fwrite(fid(n),      1, 'int32');               %   ActiveCurve (1)             344
                fwrite(fid(n),      3, 'int32');               %   MeasMode (1)                348
                fwrite(fid(n),      0, 'int32');               %   SubMode  (1)                352
                fwrite(fid(n),  Range, 'int32');               %   RangeNo (1)                 356
                fwrite(fid(n), Offset, 'int32');               %   Offset (1)                  360
                fwrite(fid(n),   Tacq, 'int32');               %   TAcq (1)                    364
                fwrite(fid(n),  65535, 'int32');               %   StopAt (1)                  368
                fwrite(fid(n),      0, 'int32');               %   StopOnOvfl (1)              372
                fwrite(fid(n),      1, 'int32');               %   Restart (1)                 376
                fwrite(fid(n),      1, 'int32');               %   DisplayLinLog (1)           380
                fwrite(fid(n),      0, 'int32');               %   DisplayTimeAxMin (1)        384
                fwrite(fid(n),     25, 'int32');               %   DisplayTimeAxMax (1)        388
                fwrite(fid(n),      1, 'int32');               %   DisplayCountAxMin (1)       392
                fwrite(fid(n),   1000, 'int32');               %   DisplayCountAxMax (1)       396

                for x = 1:8
                    fwrite(fid(n), x, 'int32');                %   DisplayCurve[x].MapTo (8x1) 400 408 416 424 432 440 448 456
                    fwrite(fid(n), 1, 'int32');                %   DisplayCurve[x].Show  (8x1) 404 412 420 428 436 444 452 460
                end;

                for x = 1:3
                    fwrite(fid(n), 0, 'float');                %   Param[x].Start        (3x1) 464 476 488
                    fwrite(fid(n), 0, 'float');                %   Param[x].Step         (3x1) 468 480 492
                    fwrite(fid(n), 0, 'float');                %   Param[x].End          (3x1) 472 484 496
                end;

                fwrite(fid(n), 1, 'int32');                    %   RepeatMode (1)              500
                fwrite(fid(n), 1, 'int32');                    %   RepeatsPerCurve (1)         504
                fwrite(fid(n), 1, 'int32');                    %   RepeatTime (1)              508
                fwrite(fid(n), 1, 'int32');                    %   RepeatWaitTime (1)          512

                a = []; a(1:7)  = 0;
                fwrite(fid(n), 'PH300 T3-Mode', 'char');       %   ScriptName (1-13)           516
                fwrite(fid(n), a, 'char');                     %   ScriptName (14-20)


                a = HW_Model(n,:);
                fwrite(fid(n), a, 'char');                     %   HardwareIdent (1-..)        536
                l = 16 - length(a);
                if (l>0)
                    a = []; a(1:l) = 0;
                    fwrite(fid(n), a, 'char');                 %   Ident (..-16)
                end;

                a = HW_Version(n,:);
                fwrite(fid(n), a, 'char');                     %   HardwareVersion (1-..)      552
                l = 8 - length(a);
                if (l>0)
                    a = []; a(1:l) = 0;
                    fwrite(fid(n), a, 'char');                 %  HardwareVersion (..-8)
                end;

                fwrite(fid(n), SerNo(n), 'int32');             %   BordSerial (1)              560
                fwrite(fid(n), SyncDiv, 'int32');              %   SyncDivider                 564
                fwrite(fid(n), CFD_Zero0, 'int32');            %   CFD_Zero 0 (1)              568
                fwrite(fid(n), CFDLevel0, 'int32');            %   CFDLevel 0 (1)              572
                fwrite(fid(n), CFD_Zero1, 'int32');            %   CFD_Zero 1 (1)              576
                fwrite(fid(n), CFDLevel1, 'int32');            %   CFDLevel 1 (1)              580
                fwrite(fid(n), Resolution(n), 'float');        %   Resolution (1)              584

                fwrite(fid(n),  0, 'int32');                   %   External Devices (1)        588
                fwrite(fid(n),  0, 'int32');                   %   Dummy (1)                   592
                fwrite(fid(n),  0, 'int32');                   %   Dummy (1)                   596

                fwrite(fid(n),  0, 'int32');                   %   Syncrate (1)                600
                fwrite(fid(n),  0, 'int32');                   %   Countrate (1)               604
                fwrite(fid(n),  0, 'int32');                   %   TTTRStopAfter (1)           608
                fwrite(fid(n),  0, 'int32');                   %   TTTRStopReason (1)          612

                fwrite(fid(n),  0, 'uint32');                  %   NCounts (1)                 616

                if (isScan)
                    fwrite(fid(n), 16, 'uint32');                  %   SpecHeaderLength (1)        620

                    fwrite(fid(n),  3, 'int32');                   %   Dimensions (3=area)    (1)  624
                    fwrite(fid(n),  1, 'int32');                   %   Ident  (1=E710)        (1)  628
                    fwrite(fid(n),  round(dt*5), 'int32');         %   TimePerPix /0.2ms      (1)  632
                    fwrite(fid(n),  16*fa+sa, 'int32');            %   Axis assignment        (1)  636
                    fwrite(fid(n),  1, 'int32');                   %   Pattern (1=bidir.)     (1)  640
                    fwrite(fid(n),  0, 'int32');                   %   dummy                  (1)  644
                    fwrite(fid(n),  a0, 'float');                  %   Start fast axis µm     (1)  658
                    fwrite(fid(n),  b0, 'float');                  %   Start slow axis µm     (1)  652
                    fwrite(fid(n),  na, 'int32');                  %   #pix on fast axis      (1)  656
                    fwrite(fid(n),  nb, 'int32');                  %   #pix on slow axis      (1)  660
                    fwrite(fid(n),  dr, 'float');                  %   Resolution µm/pix      (1)  664
                    fwrite(fid(n),  0.25, 'float');                %   TStartTo               (1)  669
                    fwrite(fid(n),  0.75, 'float');                %   TStopTo                (1)  672
                    fwrite(fid(n),  0.25, 'float');                %   TStartFro              (1)  676
                    fwrite(fid(n),  0.75, 'float');                %   TStopFro               (1)  680
                    fwrite(fid(n),  0, 'float');                   %   Scan-Offset            (1)  684
                else
                    fwrite(fid(n), 0, 'uint32');                   %   SpecHeaderLength (1)        620
                end;
            end;
        end;
    end;
end;

res = err;