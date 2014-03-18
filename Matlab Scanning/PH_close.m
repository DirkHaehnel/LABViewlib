function err = PH_close(fid, dev, m_err, etime, Progress, Syncrate, Countrate)

%  Finalize PicoHarp 300 T3R-Measurement
%  Valid arguments are:
%  fid      : List of data file handles 
%  dev      : List of active devices
%  m_err    : Event code that stopped the measurement
%  Progress : Number of data records

% (c) Ingo Gregor 2006

% Defininitions

PH_SHUTDOWN_ERROR       =   -25;

% ret   = int32(0);
 
% Check if library was loaded

fprintf('\n');

if (~libisloaded('PHLib'))    
    fprintf(' Could not open PHLib.');
else
    for n = 1:length(dev)       
        if ~isempty(fid)
            fseek(fid(n),0,'bof');
            fseek(fid(n),600,'bof');
            fwrite(fid(n), Syncrate(n), 'int32');                %   Syncrate (1)
            fwrite(fid(n), Countrate(n), 'int32');               %   Countrate (1)
            fwrite(fid(n), round(etime*1000), 'int32');          %   StopAfter (1)
            fwrite(fid(n), m_err, 'uint32');                     %   StopReason (1)
            fwrite(fid(n), Progress(n), 'uint32');               %   NCounts (1)

            fclose(fid(n));
        end;
        [ret] = calllib('PHLib', 'PH_CloseDevice', dev(n));
        if (ret<0)
            fprintf(' Could not close device %d.\n', dev(n));
            err = PH_SHUTDOWN_ERROR;
        else
            fprintf(' Device %d closed. \n', dev(n));
        end;
    end;
    unloadlibrary('PHLib');  % release Libraray
end;

