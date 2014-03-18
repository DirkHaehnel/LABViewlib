function [s] = PI_close(s)

if strcmp(s.Type, 'serial')

    PI_write(s,'TR1');   % Set sampling rate divisor to default

    PI_write(s,'0MD0');  % Reset Baseline Position

    pause(0.01);

    PI_write(s,'1SV%08.4f,2SV%08.4f,3SV%08.4f',[0.5 0.5 0.5]);  % Set velocity of axis 1..3 to 0.5 µm/ms

    PI_write(s,'1SS100,2SS100,3SS100');   % Set traget for ax to 100.000 µm

    PI_write(s,'MS');   % Start move

    for ax=1:3
        x = 1;
        while (x>0)
            t = PI_Query(s,'%dGI8', ax);
            x = bitand(floor(str2double(t(end-5:end))/256),4);
        end;
    end;
    
    fclose(s);
    delete(s);
    clear s;
    clear global glob_SER_COM1;
    s = '';
end;
