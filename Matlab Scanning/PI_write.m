function PI_write(fid, qstr1, qstr2)

if nargin>2
   if ~isempty(qstr2)
      qstr1 = sprintf(qstr1,qstr2);
   end;
end;

fid.RequestToSend = 'on';
while ~strcmp(fid.PinStatus.ClearToSend,'on')
   pause(0.01);
end;

fprintf(fid,'%s\n',qstr1,'async');
n = 0;
while ~strcmp(fid.TransferStatus,'idle')
   pause(0.01);
   n = n+1;
end;


