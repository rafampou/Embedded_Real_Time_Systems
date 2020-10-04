fileID = fileread('cpuAll.txt');
[match,noMatch] = regexp(fileID, '([0-9]*[.][0-9])       \d*:\d','tokens');
cpu_usage=str2double([match{:}]);
[match,noMatch] = regexp(fileID, '[0-9]*[.][0-9]       (\d*):(\d*)','tokens');
cpu_time=str2double([match{:}]);
cpu_time = cpu_time(1:2:size(cpu_time,2))*60 + cpu_time(2:2:size(cpu_time,2));

plot(cpu_time,cpu_usage)
