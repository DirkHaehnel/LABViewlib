function pos = PI_Set_z(s, z1)

%  PI-PiezoTranslationController: sets z_position to z1 (in microns)

% (c) Ingo Gregor 2006

while s.BytesAvailable>0
   fscanf(s);
end;

PI_write(s,'3SL1');            % Switch axis 3 to servo-on mode
PI_write(s,'3SV%08.4f',0.5);   % Set velocity of axis 3 to 0.5 µm/ms

PI_write(s,'3SS%08.4f', 100-z1);   % Set traget for axis 3 to 100 - z1 µm

PI_write(s,'MS');   % Start move

x = 1;
while (x>0)
   t = PI_Query(s,'3GI8');
   x = bitand(floor(str2double(t(end-5:end))/256),4);
end;

pos = 0;

for n=1:100
   t = PI_Query(s,'3TS');
   pos = pos + str2double(t);
end;

pos = 100 - round(pos*10)./1000;
