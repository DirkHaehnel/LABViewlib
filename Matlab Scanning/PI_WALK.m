function PI_WALK

%  PI E710 Walk 'n' Track

% (c) Ingo Gregor 2008

% Defininitions

m_stop   = false;
allStep  = [0.05 0.1 0.2 0.5 1 2 5 10 20];
Step_idx = 7;
Step     = 5;
M_List   = [];
M_ind    = 0;

   function end_measurement(src, evnt)
      
      if nargin==2
         plot([-102 102], [-py -py],'-w');
         plot([-px -px], [-102 102],'-w');
         if numel(M_List)>1
            plot(-M_List(:,1),-M_List(:,2),'-ob');
         end;
         
         if evnt.Character == 'c'
            m_stop = true;
         end;
         
         if evnt.Character == 'm'
            M = [px py pz];
            M_List = [M_List; M];
            M_ind  = M_ind + 1;
         end;
         
         if evnt.Character == 'g'
            if M_ind<size(M_List,1);
               M_ind = M_ind + 1;
            end;
            if M_ind>0
               px = M_List(M_ind,1);
               py = M_List(M_ind,2);
               pz = M_List(M_ind,3);
            end
            PI_write(s,'1SS%07.3f,2SS%07.3f,3SS%07.3f', [py+100 px+100 pz+100]);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
         
         if evnt.Character == 'f'
            if M_ind>1;
               M_ind = M_ind - 1;
            end;
            if M_ind>0
               px = M_List(M_ind,1);
               py = M_List(M_ind,2);
               pz = M_List(M_ind,3);
            end
            PI_write(s,'1SS%07.3f,2SS%07.3f,3SS%07.3f', [py+100 px+100 pz+100]);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
         
         if evnt.Character == 'r'
            px = 0;
            py = 0;
            pz = 0;
            PI_write(s,'1SS%07.3f,2SS%07.3f,3SS%07.3f', [py+100 px+100 pz+100]);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
         
         if evnt.Character == 'a'
            if (Step_idx<numel(allStep))
               Step_idx = Step_idx + 1;
            end;
            Step = allStep(Step_idx);
            s1 = sprintf('Stepsize: % 5.2f µm', Step);
            title(s1);
         end;
         if evnt.Character == 's'
            if (Step_idx>1)
               Step_idx = Step_idx - 1;
            end;
            Step = allStep(Step_idx);
            s1 = sprintf('Stepsize: % 5.2f µm', Step);
            title(s1);
         end;
         if evnt.Character == 29
            px = px - Step;
            if px < -100
               px = -100;
            end;
            PI_write(s,'2SS%07.3f', px+100);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
         if evnt.Character == 28
            px = px + Step;
            if px > 100
               px = 100;
            end;
            PI_write(s,'2SS%07.3f', px+100);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
         if evnt.Character == 30
            py = py - Step;
            if py < -100
               py = -100;
            end;
            PI_write(s,'1SS%07.3f', py+100);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
         if evnt.Character == 31
            py = py + Step;
            if py > 100
               py = 100;
            end;
            PI_write(s,'1SS%07.3f', py+100);   % Set target pos
            PI_write(s,'MS');              % Start move
         end;
      end;
   end

err  = 0;

[s, err] = PI_init;

if err==0
   px = 0;
   py = 0;
   pz = 0;
   
   PI_write(s,'1SL1,2SL1,3SL1');                         % Switch all axes to servo-on mode
   PI_write(s,'1SV%08.4f,2SV%08.4f,3SV%08.4f',[1 1 1]);  % Set velocity of axes to 0.05 µm/ms
   
   figure('KeyPressFcn',@end_measurement);
   hold on
   axis([-105 105 -105 105]);
   axis square;
   box on;
   s1 = sprintf('Stepsize: % 5.2f µm', Step);
   title(s1);
   drawnow;
   
   while ~m_stop
      plot([-102 102], [-py -py],'-g');
      plot([-px -px], [-102 102],'-g');
      drawnow;
      pause(.01);
   end;
   
   PI_close(s);
   
   fprintf(' Done!\n');
   close;
end
end


