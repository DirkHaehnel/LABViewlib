function [br] = PI_SetBaudRate(rate)

br  = 0;
dbr = [9600, 19200, 38400, 57600, 115200];

n = 1;
while (rate~=dbr(n))&&(n<6)
   n=n+1;
end;

if (n<6)
   nbr = dbr(n);
   s = serial('COM1');
   set(s,'Databits',8,'Parity','None','Stopbits',1,'Timeout',1);
   
   fopen(s);
   s.ReadAsyncMode = 'continuous';
   s.FlowControl = 'hardware';
   s.Terminator = 'LF';
   
   b  = 0;
   t2 = '';
   while isempty(t2)&&(b<5)
      b = b+1;
      s.baudrate = dbr(b);
      while s.BytesAvailable>0
         fscanf(s);
      end;
      
      t2 = PI_Query(s,'GI');
   end;
   if ~isempty(t2)
      PI_write(s,'0BR%d',nbr);
      br = str2double(PI_Query(s,'0BR'));
   end;
   fclose(s);
   clear s;
else
   disp(' ');
   disp('Valid Baudrates are: 9600, 19200, 38400, 57600 and 115200');
   disp(' ');
end;


