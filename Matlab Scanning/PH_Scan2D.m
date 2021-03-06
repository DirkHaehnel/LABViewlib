function [seg] = PH_Scan2D(name, mode, a0, b0, c0, da, db, dr, dt)

%
%  PH_Scan: PH_Scan2D(name, mode, a0, b0, c0, da, db, dr, dt)
%
%  Perform sample scanning
%
%  name       :  filename for pt3-files
%  mode       : Two character string defingig fast and slow scan axis 
%                  e.g. 'xy', 'xz', 'zx' etc.
%  a0, b0, c0 :  start position for scanning im �m. 
%   (limits)       a0 : slow axis     (-100..100) 
%                  b0 : fast axis     (-100..100)
%                  c0 : fixed axis    (-100..100) 
%
%  da, db     :  width of movement in �m.
%   (limits)      da : 100-x0
%                 db : 100-y0 
%  dr         :  resolution of movement in �m
%  dt         :  acquisition-time per pixel in ms
%

if nargin < 9
    disp(' ');
    disp('Syntax: PH_Scan2D(name, mode, a0, b0, c0, da, db, dr, dt)');
    disp(' ');
    disp('Use "help PH_Scan2D" to get more information.');
    disp(' ');
    seg = -1;
    return
end

%% Definitions

FLAG_OVERFLOW     = hex2dec('0040');
FLAG_FIFOFULL     = hex2dec('0003');

TTREADMAX         =     131072;     % 128K event records

PH_STARTMEAS_ERROR       =   -22;
PH_READDATA_ERROR        =   -23;
PH_STOPMEAS_ERROR        =   -24;
PH_GETCOUNTRATE_ERROR    =   -21;

WRITEDATA_ERROR          =     4;
READDATA_ERROR           =     3;
FIFOFULL                 =     2;

%% Check input parameters

a0 = round(1000*a0)/1000;
b0 = round(1000*b0)/1000;
c0 = round(1000*c0)/1000;
da = round(1000*da)/1000;
db = round(1000*db)/1000;

tmp = 0.2*ceil(dt/0.2);

if tmp~=dt
    dt = tmp;
    fprintf('\n Warning: Acquisition time per pixel has to be a multiple of 0.2 ms!');
    fprintf('\n Acquisition time was adjusted to %5.1f ms. \n', dt);
end;

tmp = 0.01*ceil(dr/0.01);
if tmp~=dr 
    dr = tmp;
    fprintf('\n Warning: Scanning resolution has to be a multiple of 10 nm!');
    fprintf('\n Resolution was adjusted to %5.2f �m. \n', dr);
end;

fa = 0;
sa = 0;
if numel(mode)>2
    mode = mode(1:2);
    fprintf('\n Warning: Scanmode has to be a two character string!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
end;    
if isempty(mode)
    fa = 1;
    sa = 2;
    mode = 'xy';
    fprintf('\n Warning: Scanmode has to be a two character string!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
else
    if strcmp(mode(1),'x')
        fa = 1;
    elseif strcmp(mode(1),'y')
        fa = 2;
    elseif strcmp(mode(1),'z')
        fa = 3;        
    end;    
end;
if numel(mode)==1
    sa = 1;
    if fa==1
        sa = 2;
    end;
    tmp = 'xyz';    
    mode = [tmp(fa) tmp(sa)];
    fprintf('\n Warning: Scanmode has to be a two character string!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
else
    if strcmp(mode(2),'x')
        sa = 1;
    elseif strcmp(mode(2),'y')
        sa = 2;
    elseif strcmp(mode(2),'z')
        sa = 3;        
    end;
end;
if (fa==0)&&(sa==0)
    fa = 1;
    sa = 2;
    mode = 'xy';
    fprintf('\n Warning: Scanmode has to consist uniquely of x, y, and z characters!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
elseif (fa==0)
    fa = 1;
    if sa==1
        fa = 2;
    end;
    tmp = 'xyz';    
    mode = [tmp(fa) tmp(sa)];
    fprintf('\n Warning: Scanmode has to consist uniquely of x, y, and z characters!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
elseif (sa==0)
    sa = 1;
    if fa==1
        sa = 2;
    end;
    tmp = 'xyz';    
    mode = [tmp(fa) tmp(sa)];
    fprintf('\n Warning: Scanmode has to consist uniquely of x, y, and z characters!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
end;

if fa==sa
    sa = 1;
    if fa==1
        sa = 2;
    end;
    tmp = 'xyz';
    mode = [tmp(fa) tmp(sa)];
    fprintf('\n Warning: Scanmode must define two different axes!');
    fprintf('\n Scanmode was adjusted to %s. \n', mode);
end;


%% Prepare scanner

fprintf('\n Preparing scanning device ...');

[s, Tacq, dr, nfa, nsa] = PI_Scan_prepare2D(a0, b0, c0, da, db, dr, dt, fa, sa);

if Tacq > 0

    fprintf('\n Ready \n');

    Tacq = Tacq + 5000;

    fname = sprintf('%s',name);

    [err, dev, fid] = PH_init(fname, Tacq, a0, b0, nfa, nsa, dr, dt, fa, sa);

    if (err == 0)

        buffer    = uint32(zeros(1,TTREADMAX));
        bufferptr = libpointer('uint32Ptr', buffer);
        Syncrate  = zeros(size(dev));
        Countrate = zeros(size(dev));
        rate      = zeros(1,numel(dev),2);
        Progress  = zeros(size(dev));

        fprintf('\n Scan will last %10.3f s = %02d:%02d:%02d.%03d\n', ...
            Tacq/1000,fix(Tacq/(60*60*1000)), ...
            mod(fix(Tacq/(60*1000)),60), ...
            mod(fix(Tacq/1000),60), ...
            mod(Tacq,1000));

        fprintf('\n Done: 00:00:00 s  ---  Progress:');
        for n = 1:length(dev)
            fprintf(' %9d ', Progress(n));
        end;

        for n = 1:length(dev)
            if (err==0)
                [ret] = calllib('PHLib', 'PH_StartMeas', dev(n), Tacq);
                if (ret<0)
                    fprintf('\nError in StartMeas. Aborted.\n');
                    err = PH_STARTMEAS_ERROR;
                end;
            end;
        end;

        if (err==0)
            pause(0.5);
            PI_Start_Scan(s, fa, sa);                    % Start Scan

            tic;
            m_err       = 0;
            rtm         = 0;
            dev_running = true(size(dev));

            while((m_err==0) && any(dev_running))

                if (toc>rtm)
                    rtm = rtm + 1;
                    for n = 1:length(dev)
                        if (dev_running(n))
                            [CTCDone]   = calllib('PHLib', 'PH_CTCStatus', dev(n));
                            if (CTCDone~=0)
                                dev_running(n) = false;
                            end;

                            [flags]     = calllib('PHLib', 'PH_GetFlags', dev(n));
                            if (bitand(flags, FLAG_FIFOFULL))
                                m_err = FIFOFULL;
                                fprintf('\n FIFO-buffer full. Aborted.\n');
                            end;

                            if (m_err == 0)
                                [counts, buffer] = calllib('PHLib','PH_TTReadData', dev(n), bufferptr, TTREADMAX);
                                if (counts > 0)
                                    if (fwrite(fid(n), buffer(1:counts), 'uint32')~= counts)
                                        m_err = WRITEDATA_ERROR;
                                        fprintf('\n WriteData Error. Aborted.\n');
                                    end;
                                else
                                    m_err = READDATA_ERROR;
                                    fprintf('\n ReadData Error. Aborted.\n');
                                end;
                            end;

                            [m_rate0]   = calllib('PHLib', 'PH_GetCountRate', dev(n), 0);
                            if (m_rate0<0)
                                fprintf('\nError in GetCountRate: %f. Aborted.\n', m_rate0);
                                err = PH_GETCOUNTRATE_ERROR;
                            end;
                            [m_rate1]   = calllib('PHLib', 'PH_GetCountRate', dev(n), 1);
                            if (m_rate1<0)
                                fprintf('\nError in GetCountRate: %f. Aborted.\n', m_rate1);
                                err = PH_GETCOUNTRATE_ERROR;
                            end;

                            if (err==0)
                                rate(rtm, n, 1) = m_rate0; %#ok<AGROW>
                                rate(rtm, n, 2) = m_rate1; %#ok<AGROW>
                            end;


                            if (m_err==0)
                                Progress(n) = Progress(n) + counts;
                                tau = round(toc*1000);
                                for m = 1:length(dev)
                                    fprintf('\b\b\b\b\b\b\b\b\b\b\b');
                                end;
                                fprintf('\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b');
                                fprintf('%02d:%02d:%02d s  ---  Progress:', ...
                                    fix(tau/3600000), ...
                                    mod(fix(tau/60000),60), ...
                                    mod(fix(tau/1000),60));
                                for m = 1:length(dev)
                                    fprintf(' %9d ', Progress(m));
                                end;
                            end;
                        end;
                    end;
                end;
            end;
            etime = toc;

            fprintf('\n');

            for n = 1:length(dev)
                [ret] = calllib('PHLib','PH_StopMeas', dev(n));
                if (ret < 0)
                    fprintf(1,'\n Could not stop measurement.\n');
                    err = PH_STOPMEAS_ERROR;
                else
                    fprintf(1,' Device %d stopped.\n', dev(n));
                end;
            end;


        end;  % of StartMeas

        if (size(rate,1) > 3)
            Syncrate  = mean(rate(2:end-1,1:numel(dev),1));
            Countrate = mean(rate(2:end-1,1:numel(dev),2));
        end;

        PH_close(fid, dev, m_err, etime, Progress, Syncrate, Countrate);

        x=1;
        while (x>0)
            q = sprintf('1GI8');
            t = PI_Query(s,q);
            if ~isempty(t)
                x = bitand(floor(str2double(t(end-5:end))/256),64);
            end;
        end;
    end;


end;

fprintf('\n Closing scanning device ...');

PI_goto(s, 0, 0, 0);
PI_close(s);

fprintf(' Done!\n');
