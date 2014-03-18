function pos = PI_goto(s, x0, y0, z0)

%  PI-PiezoTranslationController: Go to position (x0, y0, z0) (in microns)

% (c) Ingo Gregor 2006

while s.BytesAvailable>0
       fscanf(s);
end;

PI_write(s,'1SL1,2SL1,3SL1');                               % Switch all axes to servo-on mode
PI_write(s,'1SV%08.4f,2SV%08.4f,3SV%08.4f',[0.5 0.5 0.5]);  % Set velocity of axes to 0.5 µm/ms

PI_write(s,'1SS%07.3f,2SS%07.3f,3SS%07.3f', 100+[y0 x0 z0]);   % Set target pos

PI_write(s,'MS');   % Start move

for n = 1:3
    x = 1;
    while (x>0)
        t = PI_Query(s,'%1dGI8',n);
        x = bitand(floor(str2double(t(end-5:end))/256),4);
    end;
end;
pos = PI_Pos(s);
