fileID = fileread('timestamps_timerAll.txt');
[match,noMatch] = regexp(fileID, '  1 \|drifting [ ]* (\d*) usec','tokens');
drifting_1=str2double([match{:}]);
[match,noMatch] = regexp(fileID, '  2 \|drifting [ ]* (\d*) usec','tokens');
drifting_2=str2double([match{:}]);
[match,noMatch] = regexp(fileID, '  3 \|drifting [ ]* (\d*) usec','tokens');
drifting_3=str2double([match{:}]);

figure(1)
fprintf("Timer 1 drifting(msec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(drifting_1),max(drifting_1),mean(drifting_1),std(drifting_1),var(drifting_1));
subplot(1,3,1)
boxplot(drifting_1,'DataLim',[0 200])
fprintf("Timer 2 drifting(msec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(drifting_2),max(drifting_2),mean(drifting_2),std(drifting_2),var(drifting_2));
subplot(1,3,2)
boxplot(drifting_2,'DataLim',[0 5000])
fprintf("Timer 3 drifting(msec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(drifting_3),max(drifting_3),mean(drifting_3),std(drifting_3),var(drifting_3));
subplot(1,3,3)
boxplot(drifting_3,'DataLim',[0 1200])

figure(2)
fileID = fileread('timestamps_timerAll.txt');
[match,noMatch] = regexp(fileID, '  1 \| timefnc timestamp (\d*) sec','tokens');
timestamp_sec=str2double([match{:}]);
[match,noMatch] = regexp(fileID, '  1 \| timefnc timestamp \d* sec [ ]* (\d*) sec','tokens');
timestamp_usec=str2double([match{:}]);
timestamp_usec = timestamp_usec + timestamp_sec*1000000;
timestamp_usec_start = [timestamp_usec(1:size(timestamp_usec,2)-1)];
timestamp_usec_end = [timestamp_usec(2:size(timestamp_usec,2))];

time_usec_1 = timestamp_usec_end - timestamp_usec_start;
time_usec_1 = time_usec_1 - 1000000;

[match,noMatch] = regexp(fileID, '  2 \| timefnc timestamp (\d*) sec','tokens');
timestamp_sec=str2double([match{:}]);
[match,noMatch] = regexp(fileID, '  2 \| timefnc timestamp \d* sec [ ]* (\d*) sec','tokens');
timestamp_usec=str2double([match{:}]);
timestamp_usec = timestamp_usec + timestamp_sec*1000000;
timestamp_usec_start = [timestamp_usec(1:size(timestamp_usec,2)-1)];
timestamp_usec_end = [timestamp_usec(2:size(timestamp_usec,2))];

time_usec_2 = timestamp_usec_end - timestamp_usec_start;
time_usec_2 = time_usec_2 - 100000;

[match,noMatch] = regexp(fileID, '  3 \| timefnc timestamp (\d*) sec','tokens');
timestamp_sec=str2double([match{:}]);
[match,noMatch] = regexp(fileID, '  3 \| timefnc timestamp \d* sec [ ]* (\d*) sec','tokens');
timestamp_usec=str2double([match{:}]);
timestamp_usec = timestamp_usec + timestamp_sec*1000000;
timestamp_usec_start = [timestamp_usec(1:size(timestamp_usec,2)-1)];
timestamp_usec_end = [timestamp_usec(2:size(timestamp_usec,2))];

time_usec_3 = timestamp_usec_end - timestamp_usec_start;
time_usec_3 = time_usec_3 - 10000;

fprintf("Καθυστέριση εκτέλεσης 1(usec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(time_usec_1),max(time_usec_1),mean(time_usec_1),std(time_usec_1),var(time_usec_1));
fprintf("Καθυστέριση εκτέλεσης 2(usec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(time_usec_2),max(time_usec_2),mean(time_usec_2),std(time_usec_2),var(time_usec_2));
fprintf("Καθυστέριση εκτέλεσης 3(usec):\n Ελάχιστο:%d\n Μέγιστο:%d\n Μέση τιμή:%f\n Τυπική απόκλιση:%f\n Διασπορά:%e\n",min(time_usec_3),max(time_usec_3),mean(time_usec_3),std(time_usec_3),var(time_usec_3));


