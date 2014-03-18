function pos = PI_Pos(s)

while s.BytesAvailable
   fscanf(s);
end;

pos = [0 0 0];

for n=1:100
    t = PI_Query(s,'1TS,2TS,3TS');
    pos(2) = pos(2) + str2double(t(1:3:end));
    pos(1) = pos(1) + str2double(t(2:3:end));
    pos(3) = pos(3) + str2double(t(3:3:end));
end;

pos = round(pos*10)./1000 - 100;
pos(3) = -pos(3);
