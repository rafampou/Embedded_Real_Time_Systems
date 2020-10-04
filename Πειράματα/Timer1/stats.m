fileID = fileread('timestamps_timer1.txt');
[match,noMatch] = regexp(fileID, '(\d*) usec','tokens');
drifting=str2double([match{:}]);

fprintf("Timer drifting(msec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(drifting),max(drifting),mean(drifting),std(drifting),var(drifting));
boxplot(drifting,'DataLim',[0 200],'ExtremeMode','compress')

fileID = fileread('timestamps_timer1.txt');
[match,noMatch] = regexp(fileID, 'timestamp (\d*) sec','tokens');
timestamp_sec=str2double([match{:}]);
[match,noMatch] = regexp(fileID, 'timestamp \d* sec [ ]* (\d*) sec','tokens');
timestamp_usec=str2double([match{:}]);

timestamp_usec = timestamp_usec + timestamp_sec*1000000;
timestamp_usec_start = [timestamp_usec(1:size(timestamp_usec,2)-1)];
timestamp_usec_end = [timestamp_usec(2:size(timestamp_usec,2))];

time_usec = timestamp_usec_end - timestamp_usec_start;
time_usec = time_usec - 1000000;

fprintf("Καθυστέριση εκτέλεσης(μsec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(time_usec),max(time_usec),mean(time_usec),std(time_usec),var(time_usec));
boxplot(time_usec,'DataLim',[100 500],'ExtremeMode','compress')
