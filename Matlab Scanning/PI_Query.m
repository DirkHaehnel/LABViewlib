function ret = PI_Query(fid, qstr1, qstr2)

ret = '';
t = '';
m = 0;

if nargin>2
   if ~isempty(qstr2)
      qstr1 = sprintf(qstr1,qstr2);
   end;
end;

while (isempty(t))&&(m<5)
    m = m+1;
    fid.RequestToSend = 'on';
    while ~strcmp(fid.PinStatus.ClearToSend,'on')
       pause(0.01);
    end;
    
    fprintf(fid,'%s\n',qstr1);
    n = 0;
    while (fid.BytesAvailable == 0)&&(n<10)
        pause(0.1);
        n = n+1;
    end;

    while(fid.BytesAvailable>0)
        t = fgetl(fid);
        ret = strvcat(ret,t);
    end;
end;

