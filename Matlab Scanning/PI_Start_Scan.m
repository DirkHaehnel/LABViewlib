function PI_Start_Scan(s, fa, sa)

%  PI_Start_Scan: PI_Start_Scan(s, fa, sa)
%
%  Start scanner wit programmed waveform
%
%  s          :  handle for assigned Com-port. 
%  fa         :  assignment of fast axis 1:x, 2:y, 3:z
%  sa         :  assignment of slow axis 1:x, 2:y, 3:z

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

%PI_write(s,'0MC%d',bitshift(8,(fa-1)*4)+bitshift(10,(sa-1)*4));   % Start synchronous waveform output for both generators
PI_write(s,'0MC%d',bitshift(2,(sa-1)*4));   % Start synchronous waveform output for both generators

