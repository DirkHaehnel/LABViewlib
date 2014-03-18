function [s, res, dr, nfa, nsa] = PI_Scan_prepare2D(a0, b0, c0, da, db, dr, dt, fa, sa)

%
%  PI_Scan_prepare2D: PI_Scan_prepare2D(a0, b0, c0, da, db, dr, dt, fa, sa)
%
%  Adjust PI Scanner controller according to scan parameters
%
%  a0, b0, c0 :  start position for scanning im µm. 
%   (limits)       a0 : fast axis     (-98..98) 
%                  b0 : slow axis     (-100..100)
%                  c0 : fixed axis    (-100..100) 
%
%  da, db     :  width of movement in µm.
%   (limits)      da : 98-a0
%                 db : 100-b0 
%  dr         :  resolution of movement in µm
%  dt         :  acquisition-time per pixel in ms
%  fa         :  assignmaent of fast axis 1:x, 2:y, 3:z
%  sa         :  assignmaent of slow axis 1:x, 2:y, 3:z

err = 0;
res = -1;
nfa = 0;
nsa = 0;

%% check input parameters

if nargin < 9
    disp(' ');
    disp('Syntax: PI_Scan_prepare2D(a0, b0, c0, da, db, dr, dt, fa, sa)');
    disp(' ');
    disp('Use "help PI_Scan_prepare2D" to get more information.');
    disp(' ');
    err = -1;
end

if err==0
    fa = floor(fa);
    if (fa<1)||(fa>3)
        fprintf('\n Error in PI_Scan_prepare2D! \n');
        fprintf('   Valid optins for <fa> are: 1,2, or 3.');
        err = -1;
    end;
end;

if err==0
    sa = floor(sa);
    if (sa<1)||(sa>3)
        fprintf('\n Error in PI_Scan_prepare2D! \n');
        fprintf('   Valid optins for <sa> are: 1,2, or 3.');
        err = -1;
    end;
end;

if err==0
    if (sa==fa)
        fprintf('\n Error in PI_Scan_prepare2D! \n');
        fprintf('   <fa> and <sa> must be unique.');
        err = -1;
    end;
end;

if err==0
    ca = 6 - sa - fa;     % stationary axis

    if (fa==1)
        fa = 2;
    elseif (fa==2)
        fa = 1;
    end;
    if (sa==1)
        sa = 2;
    elseif (sa==2)
        sa = 1;
    end;
    if (fa==3)
        a0 = 100-a0;
    else
        a0 = 100+a0;
    end;
    if (sa==3)
        b0 = 100-b0;
    else
        b0 = 100+b0;
    end;
    if (ca==3)
        c0 = 100-c0;
    else
        c0 = 100+c0;
    end;
end;

if err==0
    tmp = 0.01*ceil(dr/0.01);
    if tmp~=dr
        dr = tmp;
        fprintf('\n Warning: Scanning resolution has to be a multiple of 10 nm!');
        fprintf('\n Resolution was adjusted to %5.2f µm. \n', dr);
    end;
end;

if (err==0)
    tmp = min([2, ceil(da*20)/2000]);
    xa  = a0 - tmp;              
    xa  = round(100*xa)/100;     % Offset (beginning) of fast axis
    xe  = a0 + da + tmp;
    xe  = round(100*xe)/100;     % End point of fast axis
    dfa = xe-xa;                 % Amplitude of fast axis
    if (xa<0)||(xe>200)
        fprintf('\n Error in PI_Scan_prepare2D! \n');
        fprintf('   Scan range exceeded. start and end points of fast axis should lie within 2..198 µm.');
        err = -1;
    end;
end;

if (err==0)
    

    rate = 5;             % clock rate of scanner controller in kHz

    nfa = round(da/dr);   % number of pixels on fast axis (integer)
    dr  = da/nfa;         % real pixel size 
    nsa = ceil(db/2/dr);  % number of scan lines (pixels on of slow axis)

    tfa = nfa*dt;         % time per line on fast axis in ms

    tr = 0;
    ttp = 1e6;
    ns  = 1e6;
    
    while (ttp>62000)&&(ns>10)
        tr   = tr +1;
        nd   = round(tfa*rate/tr);   % data points in linear scan range of fast axis
        ns   = round(nd*(dfa/da-1)); % number of acceleration points;
        nr   = ns;
        ttp  = 10*ns + 4*nd + 4*nr;  % total data points to store in controller
        Tacq = nsa*ttp*tr/10;        % total time for scan in ms
    end;

    if ttp > 62000
        fprintf('Number of scan points (%6d) too large!\n', ttp);
        fprintf('Select smaller area or faster scan.\n\n');
        res = -2;
        err = -1;
    end;
end;

%% Initialize hardware

if err==0
   [s, err] = PI_init;
end

if err==0 
    while s.BytesAvailable
        fscanf(s);
    end;

    PI_Query(s,'GI');

    PI_write(s,'1SL1,2SL1,3SL1');                               % Switch axis 1..3 to servo-on mode
    PI_write(s,'1SV%08.4f,2SV%08.4f,3SV%08.4f',[0.5 0.5 0.5]);  % Set velocity of axis 1..3 to 0.5 µm/ms

    p1 = dfa;     % Amplitude of fast-axis
    p0 = xa;      % Offset of fast axis

    PI_write(s,'%dSS%07.3f,%dSS%07.3f,%dSS%07.3f', [fa p0 sa b0 ca c0]);   % Move scanner to start pos
    PI_write(s,'MS');   % Start move

    for ax=1:3
        x=1;
        while (x>0)
            t = PI_Query(s,'%dGI8', ax);
            x = bitand(floor(str2double(t(end-5:end))/256),4);
        end;
    end;

    PI_write(s,'0MD0');        % Reset Baseline Position

    PI_write(s,'%dSV%08.4f,%dSV%08.4f',[fa dr/dt sa dr/dt]);  % Set velocity of fast and slow axis

    PI_write(s,'TR%03d', tr);  % Set sampling rate divisor

    PI_write(s,'0CL0');  % Clear all DDL tables

    PI_write(s,'0PT0');  % Switch to segment definition mode

    pause(0.1);

    pt = ns + nd + ns + nr;
    cp = ns + nd + ns;
    pc = round(cp/2);
    ps = ns;
    pa = 0;

    %% Define waveform segment of fast axis

    PI_write(s,'1PT%d', pt);  % total points in segment 1
    PI_write(s,'1CP%d', cp);  % curve points in segment 1
    PI_write(s,'1PC%d', pc);  % center point in segment 1
    PI_write(s,'1PS%d', ps);  % acceleration points in segment 1
    PI_write(s,'1PA%d', pa);  % start point of curve
    PI_write(s,'1FO%08.3f',  0); % offset of segment 1 ( start pos of scan line)
    PI_write(s,'1GL%08.3f', p1); % amplitude of curve   (end pos of scan line)

    pause(0.1);

    seg1(1:pa)         = p0;
    n    = 0:ps-1;
    seg1(pa+n+1)       = p0 + p1/(cp/ps-1)*(n/(2*ps) - sin(n*pi/ps)/(2*pi));
    n    = 0:(cp-2*ps-1);
    seg1(pa+ps+n+1)    = p0 + p1/(cp/ps-1)*(1/2+n/ps);
    n    = 0:ps-1;
    seg1(pa+cp-ps+n+1) = p0 + p1/(cp/ps-1)*((cp-2*ps)/ps + (n+ps)/(2*ps) - sin((n+ps)*pi/ps)/2/pi);
    n    = (pa+cp):(pt-1);
    seg1(n+1)       = p0 + p1;

    pt = ns + nd + ns + nr + ns;
    cp = ns + nd + ns;
    pc = round(cp/2);
    ps = ns;
    pa = 0;

    PI_write(s,'2PT%d', pt);
    PI_write(s,'2CP%d', cp);
    PI_write(s,'2PC%d', pc);
    PI_write(s,'2PS%d', ps);
    PI_write(s,'2PA%d', pa);
    PI_write(s,'2FO%08.3f',  p1);
    PI_write(s,'2GL%08.3f', -p1);

    pause(0.1);

    seg2(1:pa)      = p1+p0;
    n    = 0:ps-1;
    seg2(pa+n+1)    = p1+p0 - p1/(cp/ps-1)*(n/(2*ps) - sin(n*pi/ps)/(2*pi));
    n    = 0:(cp-2*ps-1);
    seg2(pa+ps+n+1) = p1+p0 - p1/(cp/ps-1)*(1/2+n/ps);
    n    = 0:ps-1;
    seg2(pa+cp-ps+n+1) = p1+p0 - p1/(cp/ps-1)*((cp-2*ps)/ps + (n+ps)/(2*ps) - sin((n+ps)*pi/ps)/2/pi);
    n    = (pa+cp):(pt-1);
    seg2(n+1)       = p0;


    %% Define waveform segment of slow axis

    pt = ns + nd + ns + nr + ns;
    cp = ns + nr + ns;
    pc = ns + nd + round(cp/2);
    ps = ns;
    pa = ps + nd;

    PI_write(s,'3PT%d', pt);
    PI_write(s,'3CP%d', cp);
    PI_write(s,'3PC%d', pc);
    PI_write(s,'3PS%d', ps);
    PI_write(s,'3PA%d', pa);
    PI_write(s,'3FO%08.4f', 0);
    PI_write(s,'3GL%08.4f', dr);

    pause(0.1);

    seg3(1:pa)      = 0;
    n    = 0:ps-1;
    seg3(pa+n+1)    = 0 + dr/(cp/ps-1)*(n/(2*ps) - sin(n*pi/ps)/(2*pi));
    n    = 0:(cp-2*ps-1);
    seg3(pa+ps+n+1) = 0 + dr/(cp/ps-1)*(1/2+n/ps);
    n    = 0:ps-1;
    seg3(pa+cp-ps+n+1) = 0 + dr/(cp/ps-1)*((cp-2*ps)/ps + (n+ps)/(2*ps) - sin((n+ps)*pi/ps)/2/pi);
    n    = (pa+cp):(pt-1);
    seg3(n+1)       = 0 + dr;


    pt = nd + ns + nr + ns;
    cp = ns + nr + ns;
    pc = nd + round(cp/2);
    ps = ns;
    pa = nd;

    PI_write(s,'4PT%d', pt);
    PI_write(s,'4CP%d', cp);
    PI_write(s,'4PC%d', pc);
    PI_write(s,'4PS%d', ps);
    PI_write(s,'4PA%d', nd);
    PI_write(s,'4FO%08.4f', dr);
    PI_write(s,'4GL%08.4f', dr);

    pause(0.1);

    seg4(1:pa)      = dr;
    n    = 0:ps-1;
    seg4(pa+n+1)    = dr + dr/(cp/ps-1)*(n/(2*ps) - sin(n*pi/ps)/(2*pi));
    n    = 0:(cp-2*ps-1);
    seg4(pa+ps+n+1) = dr + dr/(cp/ps-1)*(1/2+n/ps);
    n    = 0:ps-1;
    seg4(pa+cp-ps+n+1) = dr + dr/(cp/ps-1)*((cp-2*ps)/ps + (n+ps)/(2*ps) - sin((n+ps)*pi/ps)/2/pi);
    n    = (pa+cp):(pt-1);
    seg4(n+1)       = dr + dr;

    %% Write waveforms to scanner controller

    PI_write(s,'0PT0');                        % Switch to waveform definition mode
    PI_write(s,'1PT%d', length([seg1 seg2]));  % Assign segments 1 and 2 to waveform 1
    PI_write(s,'2PT%d', length([seg3 seg4]));  % Assign segments 3 and 4 to waveform 2

    pause(0.1);

    seg = [[seg1 seg2]; [seg3 seg4]];

    %% verify waveform transmission
    x = 0;
    for wf = 1:2
        t = PI_Query(s,'%dPT', wf);
        x = x + str2double(t)-length(seg(wf,:));
    end;

    if (x==0)

        %% Setup generators

        PI_write(s,'1SF1');       % Assign waveform 1 to generator 1
        PI_write(s,'2SF2');       % Assign waveform 2 to generator 2
        PI_write(s,'%dCF1', fa);  % Connect generator 1 to fast axis
        PI_write(s,'%dCF2', sa);  % Connect generator 2 to slow axis

        PI_write(s,'%dLT1', fa);  % Connect fast axis to DDL table 1
        PI_write(s,'$dLS1', fa);  % Enable DDL usage for fast axis
        PI_write(s,'%dLT2', sa);  % Connect slow axis to DDL table 2
        PI_write(s,'%dLS1', sa);  % Enable DDL usage for slow axis
        PI_write(s,'0CL0');       % Clear all DDL tables
        

        pause(0.1);

        %% Setup trigger pulses

        PI_write(s,'1KT1');                        % Set trigger kind of line 1 to positive short pulse
        PI_write(s,'2KT1');                        % Set trigger kind of line 2 to positive short pulse
        PI_write(s,'0FT0');                        % Clear all trigger events
        PI_write(s,'%dFT1',ns+round(0.25*nd)+1);              % Set trigger event for line 1 at 25% of 1st line
        PI_write(s,'%dFT1',ns+round(0.75*nd)+1);              % Set trigger event for line 1 at 75% of 1st line
        PI_write(s,'%dFT2',length(seg1)+ns+round(0.25*nd)+2); % Set trigger event for line 2 at 25% of 2nd line
        PI_write(s,'%dFT2',length(seg1)+ns+round(0.75*nd)+2); % Set trigger event for line 2 at 75% of 2nd line

        %% Start test scan for linearisation

        PI_write(s,'RN%d',5);   
        PI_write(s,'0MC%d',bitshift(9,(fa-1)*4)+bitshift(9,(sa-1)*4));   % Start synchronous waveform output for both generators
        
        x=1;
        while (x>0)
            t = PI_Query(s,'1GI8');
            if ~isempty(t)
                x = bitand(floor(str2double(t(end-5:end))/256),64);
            end;
        end;

        %% Set number of repetitions for real scan

        PI_write(s,'RN%d',nsa);                     % Set number of repetitions

        res = Tacq;

    else
        fprintf('Unable to verify waveform definition.\n');
        fprintf('Scan aborted.\n\n');
    end;

end;

nsa = 2*nsa;
