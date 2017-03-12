[inputfilename,dirname] = uigetfile('*.*');
inputfilename = [dirname, inputfilename];
fid = fopen(inputfilename,'rb');
[A,COUNT] = fscanf(fid,'%f');
fclose(fid);
x = 1:1:COUNT;
plot(x,A);
