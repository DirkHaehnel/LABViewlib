function [s, err] = PI_init

global glob_SER_COM1;

err = -1;

if isempty(glob_SER_COM1)
    br = 0;
    while br == 0
        br = PI_SetBaudRate(115200);
    end;
%    out = instrhwinfo('serial');
%    x   = out.AvailableSerialPorts;

%    if length(x)>0
%        if (x{1}(4)=='1')
            s = serial('COM1');
            s.BaudRate      = 115200;
            s.Databits      = 8;
            s.Parity        = 'None';
            s.Stopbits      = 1;
            s.Timeout       = 1;
            s.ReadAsyncMode = 'continuous';
            s.FlowControl   = 'hardware';
            s.Terminator    = 'LF';
            s.OutputBufferSize = 2048;
            fopen(s);

            glob_SER_COM1 = s;
            
            err = 0;
%        end;
%    else
%        fprintf('COM1 is not available!\n\n');
%        s   = '';
%    end;
else 
    if isa(glob_SER_COM1,'serial')
        if strcmp(glob_SER_COM1.Port,'COM1')
            s = glob_SER_COM1;
            fclose(s);

            s.BaudRate      = 115200;
            s.Databits      = 8;
            s.Parity        = 'None';
            s.Stopbits      = 1;
            s.Timeout       = 1;
            s.ReadAsyncMode = 'continuous';
            s.FlowControl   = 'hardware';
            s.Terminator    = 'LF';
            s.OutputBufferSize = 2048;
            fopen(s);
            
            err = 0;
        end;
    end;
end;
    
if (err == 0)

    while s.BytesAvailable>0
       fscanf(s);
    end;
    PI_Query(s,'GI');

    PI_write(s,'1SL1,2SL1,3SL1');           % Switch axis 1..3 to servo-on mode
    PI_write(s,'1SV%08.4f,2SV%08.4f,3SV%08.4f',[0.5 0.5 0.5]);  % Set velocity of axis 1..3 to 1.0 µm/ms

    for ax = 1:3
        PI_write(s,'%dGH', ax);   % Move ax to 0.000 µm

        x=1;
        while (x>0)
            t = PI_Query(s,'%dGI8', ax);
            x = bitand(floor(str2double(t(end-5:end))/256),4);
        end;
    end;

    PI_write(s,'0MD0');                     % Reset Baseline Position
    PI_write(s,'TR1');                      % Reset sampling rate divisor

    PI_write(s,'1SS100,2SS100,2SS100');   % Set traget for ax to 100.000 µm

    PI_write(s,'MS');   % Start move

    x = 1;
    while (x>0)
        t = PI_Query(s,'%dGI8', ax);
        x = bitand(floor(str2double(t(end-5:end))/256),4);
    end;

    err = 0;
end;