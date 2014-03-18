function [y, tcspc, flag, num, overcount, head] = pt3Read_s(name, start_cnt, start_time, sync)

%        [y, tcspc, flag, num, overcount, head] = pt3Read_s(name, start_cnt, start_time, sync)


if strcmp(name(end-3:end),'.pt3')
    hlength = 0;

    if nargin>1

        [head, hlength] = pt3Read_head(name);
        sync            = 1e9/head.CntRate0;

        if nargin<3
            start_time = 0;
        end;
        
        if start_cnt<1
            start_cnt = 1;
        end;

    end;

    fin = fopen(name,'r');
    if (fin==-1)
        errordlg('Cannot open specified file. Please try again.');
    else

        dt = head.ScanWidthX*head.ScanTimePerPix*2e5;  % time/line in ns

        fseek(fin, hlength+4*(start_cnt-1), 'bof');

        valid     = [];
        overflow  = [];
        y         = [];
        tcspc     = [];
        flag      = [];

        overcount = start_time;

        mark = [];
        
        cntread = round(2*head.NCounts/head.ScanWidthY);

        while (numel(mark)<2)&&(~feof(fin))

           [tmpy num] = fread(fin, cntread, 'uint32');

           tmpy     = uint32(tmpy);
           numsync  = bitand(tmpy,65535);
           chan     = bitshift(tmpy, -28);
           tmpt     = double(bitshift(bitand(tmpy,268369920),-16));                      
           valid    = [valid;    (chan~=1)&(chan~=2)&(chan~=3)&(chan~=4)&(chan~=15)];
           overflow = [overflow; (chan==15&tmpt==0)];
           flag     = [flag;      chan + uint32((chan==15)&(tmpt~=0))];
            
           mark = find(flag==16);

           cs_ovl = start_time + 2^16*cumsum(overflow);
           cs_ovl(1:numel(y)) = [];
           y      = [y; sync*(double(numsync) + cs_ovl)];
           tcspc  = [tcspc; double(tmpt)];

        end;
        
        if numel(mark)>1
            t1 = tcspc(mark(1));
            if tcspc(mark(2))==t1
                dt1 = y(mark(2))-y(mark(1));
                dt0 = (dt1/2+dt/4)/2;
                y0 = y(mark(1))-dt0;
                y1 = y(mark(2))+dt0;
            else
                fprintf('\n Warning! only one marker found in this line! \n');
                fprintf('   Trying to reconstruct the data... \n');
                dt1 = y(mark(2))-y(mark(1));
                if dt1>dt                      % so likely the first marker is present ....
                    y0 = y(mark(1)) -   dt/4;
                    y1 = y(mark(1)) + 3*dt/4;
                else                           % otherwise it is the second
                    y0 = y(mark(1)) - 3*dt/4;
                    y1 = y(mark(1)) +   dt/4;
                end;
            end;
            while (y(end)<=y1)&&(~feof(fin))

                [tmpy num] = fread(fin, cntread, 'uint32');

                tmpy     = uint32(tmpy);
                numsync  = bitand(tmpy,65535);
                chan     = bitshift(tmpy, -28);
                tmpt     = double(bitshift(bitand(tmpy,268369920),-16));
                valid    = [valid;    (chan~=1)&(chan~=2)&(chan~=3)&(chan~=4)&(chan~=15)];
                overflow = [overflow; (chan==15&tmpt==0)];
                flag     = [flag;      chan + uint32((chan==15)&(tmpt~=0))];

                cs_ovl = start_time + 2^16*cumsum(overflow);
                cs_ovl(1:numel(y)) = [];
                y      = [y; sync*(double(numsync) + cs_ovl)];
                tcspc  = [tcspc; double(tmpt)];
            end;
        end;
        num = sum(y<y1);
        ind = sum(y<y0);

        y        = [y0;        y(ind+1:num); y1];
        tcspc    = [t1;    tcspc(ind+1:num); t1];
        valid    = [ 0;    valid(ind+1:num);  0];
        overflow = [ 0; overflow(ind+1:num);  0];
        flag     = [16;     flag(ind+1:num); 16];

        cs_ovl    = start_time + 2^16*cumsum(overflow);
        if ~isempty(cs_ovl)
            overcount = cs_ovl(end);
        end;

        y(valid==1)        = [];
        tcspc(valid==1)    = [];
        flag(valid==1)     = [];
        overflow(valid==1) = [];

        y(overflow==1)     = [];
        tcspc(overflow==1) = [];
        flag(overflow==1)  = [];

    end
    fclose(fin);
else
    disp('Not a pt3-file!');
end
