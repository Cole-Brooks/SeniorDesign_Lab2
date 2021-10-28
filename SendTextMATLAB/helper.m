hasBeenAlerted = false;
s = serialport('COM6', 115200);
while true
   res = str2double(readline(s)); 
   if res == 1
       hasBeenAlerted = false;
   else
       if ~hasBeenAlerted
           c = fix(clock);
           hour = c(4);
           minue = c(5);
           ampm = "AM";
           if hour > 14
               hour = mod(hour, 12);
               ampm = "PM";
           end

           warning = sprintf("Critical safety event at %02d:%02d  %s", hour, minue, ampm);
           send_text_message("319-383-9547", "att", "WARNING", warning);
           hasBeenAlerted = true;
       end
   end
end
