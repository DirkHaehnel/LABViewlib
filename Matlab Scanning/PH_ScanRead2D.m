function [tag, tim, tcspc, tau, ftau, filter] = PH_ScanRead2D(name, filter)

name  = sprintf('%s.pt3',name);

head = pt3Read_head(name);

Resolution = mean(head.Resolution);
sync       = 1e9/head.CntRate0;

NChannels = 4096;
tcspc     = zeros(NChannels,16);
tau       = (0:(NChannels-1));
nx        = head.ScanWidthX;
ny        = head.ScanWidthY;
dt        = head.ScanTimePerPix*2e5;  % in ns

offset    = floor(head.ScanOffset);
f1        = offset-head.ScanOffset;

tim  = [];
para = [dt, nx, ny]; 

h = waitbar(0,'Please wait ...');

nch = (0:15);

if (nargout>1)            % lifetime image required -> filter generation
    
    % build TCSPC-Hisogramm    
    
    cnts      = 1e5;
    tst       = 1;
    cnt       = 0;
    overcount = 0;

    while (tst>0)
        [tmpy, tmpx, flag, tst, overcount] = pt3Read(name, cnt+1, cnts, overcount, sync);

        cnt = cnt + tst;
        if (tst>0)
            for ch = 0:15
                tcspc(:,1+ch) = tcspc(:,1+ch) + mHist(tmpx(flag==ch), tau);
            end;
        end
        waitbar(cnt/head.NCounts);
        drawnow;

    end

    % clear empty tcspc channels
    ind = sum(tcspc,1)==0;
    nch(ind) = [];
    anzch    = numel(nch);
    tcspc(:,ind) = [];
    
    ind = sum(tcspc,2)==0;
    tau(ind)     = [];
    tcspc(ind,:) = [];

    if nargin==1                                         % no filter given
        for jch=1:anzch
            [m,m] = max(tcspc(:,jch));
            filter(:,jch) = [tau(m+9); tau(end-10)];     % [begin, end] of fluorescence decay
        end
    else        
        if  ~isempty(filter) && (size(filter,1)==nx) && (size(filter,2)==ny)  % index field given for TCSPC-Analysis            
            filter(:, 1:end)    = filter(end:-1:1, 1:end);
            filter = filter';
            filter(:,2:2:end) = filter(end:-1:1,2:2:end);
%             if (min(min(filter)) == 0)
%                 filter = 1 + filter;
%             end;
            find = sort(reshape(filter,numel(filter),1)); % min(min(filter)):max(max(filter));  
            tmp1 = [diff(find)==0; 0];
            find(tmp1 == 1) = [];
            tim  = zeros(find(end), NChannels+1, anzch);
        else

            if  ~isempty(filter) && (size(filter,1)~=length(tau))  % lifetime values given as filter
                ftau    = filter;
                maxftau = max(ftau);

                if length(ftau)>1
                    filter = exp(- (tau*Resolution)'*(1./ftau(:)'));
                    filter = reshape(repmat(filter, [anzch 1]), length(tau), size(filter,2)*anzch);

                    weight = tcspc;

                    for jch=1:anzch
                        [m, mi] = max(weight(:,jch));
                        filter(1:mi+9, jch:anzch:end) = 0;
                        weight(1:mi+9, jch)         = 0;

                        [m, mi] = max(tau(filter(:,jch)>0));
                        filter(mi-9:end, jch:anzch:end) = 0;
                        weight(mi-9:end, jch)         = 0;
                    end

                    filter = filter./(ones(size(filter,1),1)*sum(filter));
                    for jch = 1:anzch
                        ind = weight(:,jch)>0;
                        filter(ind,jch:anzch:end) = ( (filter(ind,jch:anzch:end)' ...
                            *diag(1./weight(ind,jch)) ...
                            *filter(ind,jch:anzch:end)) ...
                            \( diag(1./weight(ind,jch)) ...
                            *filter(ind,jch:anzch:end))')';
                    end
                else
                    filter = zeros(2,anzch);
                    for jch=1:anzch
                        [m,m] = max(tcspc(:,jch));
                        tmp = round(3*maxftau)+m+9;
                        filter(:,jch) = [tau(m+9); tau(min([end-10 tmp]))]*Resolution;
                    end
                end
            end
        end
    end

end;   % of filter generation

cnt       = 0;
overcount = 0;

tag = zeros(ny*nx,numel(nch)+1);

for k = 1:ny
    waitbar(k/ny);
    drawnow;

    %  read scan line
     
    [tmpy, tmpx, flag, tst, overcount] = pt3Read_s(name, cnt+1, overcount, sync);

    cnt = cnt + tst;

    if (tst>0)

        ttmpy = tmpy(flag==16)+tmpx(flag==16)*Resolution;

        tmpy(flag==16)  = [];
        tmpx(flag==16)  = [];
        flag(flag==16)  = [];
        
        dt = (ttmpy(end)-ttmpy(1))/nx; 
        ttmpy(end) = ttmpy(end)-1;
                
        tmptag  = zeros(nx,numel(nch)+1);
                
        tmptag(:,end)  = tttr2bin(ttmpy-ttmpy(1), dt);
        
        for jch=1:numel(nch)
            tmp             = [ttmpy(1); tmpy(flag==nch(jch)); ttmpy(end)]-ttmpy(1);
            tmptag(:,jch)   = tttr2bin(tmp, dt);
            tmptag(1,jch)   = tmptag(1,jch)-1;
            tmptag(end,jch) = tmptag(end,jch)-1;
        end;
                        
        tag(1+(k-1)*nx:k*nx,:) = tmptag;
      
        if nargout>1
            
            if (size(filter,1)==nx)&&(size(filter,2)==ny)
               n1 = 1;
               n2 = 1;
               for l = 1:nx
                   ftmp = filter(l,k);
                   if (ftmp>0)
                       tind = 1+ tmpx(flag==1);
                       for m = 1:tmptag(l,1)
                           tim(ftmp,tind(n1),1) = tim(ftmp,tind(n1),1) +1;
                           n1 = n1 +1;
                       end;
                       tind = 1+ tmpx(flag==2);
                       for m = 1:tmptag(l,2)
                           tim(ftmp,tind(n2),2) = tim(ftmp,tind(n2),2) +1;
                           n2 = n2 +1;
                       end;
                   end;
               end;

            else 
                
                if size(filter,1)==2
                    
                    tmp = cumsum([1; tmptag(:,1)]);
                    tst = tmpx(flag==1)>filter(1,1) & tmpx(flag==1)<=filter(2,1);

                    tct = cumsum([0; (tmpx(flag==1) - filter(1,1)).*tst]);
                    tct = tct(tmp(2:end)) - tct(tmp(1:end-1));

                    tst = cumsum([0; tst]);
                    tst = tst(tmp(2:end)) - tst(tmp(1:end-1));

                    ftcs = tct./(tst+(tst==0));

                    tmp = cumsum([1; tmptag(:,2)]);
                    tst = tmpx(flag==2)>filter(1,2) & tmpx(flag==2)<=filter(2,2);

                    tct = cumsum([0; (tmpx(flag==2) - filter(1,2)).*tst]);
                    tct = tct(tmp(2:end)) - tct(tmp(1:end-1));

                    tst = cumsum([0; tst]);
                    tst = tst(tmp(2:end)) - tst(tmp(1:end-1));

                    ftcs = [ftcs tct./(tst+(tst==0))];

                else
                    tmpx(tmpx<tau(1))   = tau(1);
                    tmpx(tmpx>tau(end)) = tau(end);

                    tmp = cumsum([1; tmptag(:,1)]);
                    ttcs = filter(tmpx(flag==1)-tau(1)+1,1:nch:end);
                    ttcs = cumsum([zeros(1,size(ttcs,2)); ttcs]);
                    ftcs = ttcs(tmp(2:end),:) - ttcs(tmp(1:end-1),:);

                    tmp = cumsum([1; tmptag(:,2)]);
                    ttcs = filter(tmpx(flag==2)-tau(1)+1,2:nch:end);
                    ttcs = cumsum([zeros(1,size(ttcs,2)); ttcs]);
                    ftcs = [ftcs ttcs(tmp(2:end),:) - ttcs(tmp(1:end-1),:)];
                end
                tim = [tim; ftcs];
            end;
        end;
    end;
end;
close(h);

ind = sum(tag)==0;

tag(:,ind) = [];
nch(ind)   = [];

tag = reshape(tag, para(2), para(3), numel(nch)+1);

tag_a = tag(:,1:2:end,:);
tag_b = tag(end:-1:1, 2:2:end, :);

ind = 1:min([size(tag_a,2) size(tag_b,2)]);

k_max = 20;
tmp   = zeros(1,k_max+1);

for k = 0:k_max
    tmp(1+k) = sqrt(sum(sum(sum(tag_a(1+k:end-k_max+k,ind(1:end-1),1:end-1).*tag_b(1:end-k_max,ind(1:end-1),1:end-1)))))+ ...
               sqrt(sum(sum(sum(tag_a(1+k:end-k_max+k,ind(2:end  ),1:end-1).*tag_b(1:end-k_max,ind(1:end-1),1:end-1)))));
end;

bini = 0:0.1:k_max;
tmpi = interp1(0:k_max, tmp, bini, 'cubic');
[tmpi, tmpi] = max(tmpi);
offset = bini(tmpi);
f1     = offset-floor(offset);
offset = floor(offset);

%offset=0;

if offset ~= 0
    if f1 == 0
        tag_a =    tag_a(offset+1:end,:,:);
        tag_b =    tag_b(1:end-offset,:,:);
    else
        tag_a = (1-f1)*tag_a(offset+1:end-1,:,:) +    f1 *tag_a(offset+2:end,:,:);
        tag_b =    f1 *tag_b(1:end-offset-1,:,:) + (1-f1)*tag_b(2:end-offset,:,:);
    end
end;

tag = [];
tag(:,1:2:ny,:) = tag_a;
tag(:,2:2:ny,:) = tag_b;

fa = floor(head.ScanAxes/16);
sa = bitand(head.ScanAxes,15);

if (fa==1)                        % 'xy' or 'xz'
    tag = permute(tag, [2 1 3]);
    tag = flipdim(tag,1);
    x0  = head.ScanStartX;
    y0  = head.ScanStartY;
elseif (sa==1)                    % 'yx' or 'zx'
    tag = flipdim(tag,1);
    x0  = head.ScanStartY;
    y0  = head.ScanStartX;
elseif (fa==2)                    % 'yz'
    tag = permute(tag, [2 1 3]);
else                              % 'zy'
    ;
end;

% if ~isempty(tim)&&(size(tim,1)==nx*ny)
%     tim = reshape(tim, para(2)+1, para(3), numel(tim)/prod(para(2:3))/nch, nch);
% 
%     tim(:, 2:2:end, :, :) = tim(end:-1:1, 2:2:end, :, :);
% 
%     tim = permute(tim, [2 1 3:ndims(tim)]);
% 
%     tim(:, 1:end, :, :) = tim(end:-1:1, 1:end, :, :);
% 
%     tim = squeeze(tim);
% end;


figure;
x = x0+(1:size(tag,2))*head.ScanResolution;
y = y0+(1:size(tag,1))*head.ScanResolution;
%imagesc(x,y,Fgauss(sum(tag(:,:,1:nch),3),0.5));
imagesc(x,y,sum(tag(:,:,1:nch),3));
set(gca,'DataAspectRatio', [1,1,1], ...
        'PlotBoxAspectRatio',[1 1 1], ...
        'XDir','reverse', ...
        'YDir','normal');
%axis image;

xlabel('x / �m');
ylabel('z / �m');
if (sa+fa)==5
    xlabel('y / �m');
end;
if (sa+fa)==3
    ylabel('y / �m');
end;

