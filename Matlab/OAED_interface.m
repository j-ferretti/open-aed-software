%%
clear
close all
clc

%%
ecgn = 0;
zn = 0;
bn = 0;
sn = 0;
rn = 0;

recg = zeros(1,2000);
ecg = zeros(1,2000);
z = zeros(1,1000);
ecgb = zeros(1,2000);
zb = zeros(1,1000);
s = 0;
te = [1:2000]/500;

%%

s1 = serial('COM9', 'Baudrate', 9600);
count = 0;

while(true)
    
    in = input('in ','s');
    switch in
        case 'e'
            fclose(s1);
            break;
        case 'o'
            fopen(s1);
            continue;
        case 'c'
            fclose(s1);
            continue;
        case 's'
            disp(s1);
            continue;
        case 'r'
            fclose(s1); fopen(s1);
            continue;
        case 'p'
            ecg_plot(ecg)
            continue;
        case 'i'
            while(true)
                
                if( 0 ~= s1.BytesAvailable )
                    data = fread(s1,s1.BytesAvailable);
                end
                
                c = input('c ','s');
                
                if(c == 'e')
                    break;
                end
                if(c == 'A' || c == 'a')
                    count = 0;
                    while( count<5 )
                        disp(count);
                        data = get_data('S', s1);
                        if(isnan(data))
                            continue;
                        end
                        disp(dec2bin(data));
                        if(~bitget(data,1) && ~bitget(data,2))
                            data = get_data('A', s1);
                            ecgn = ecgn+1;
                            ecg(ecgn,:) = data(1:2000);
                            rn = rn+1;
                            recg(rn,:) = data(2001:4000);
                            count = count+1;
                        end
                    end
                    ecg_plot([ecg]);
                    continue;
                end
                
                data = get_data(c, s1);
                if(isnan(data))
                    disp('Timeout');
                end
                
                switch c
                    case 'E'
%                         if(ecgn == 0)
%                             ecgn = ecgn + 1;
%                             ecg(ecgn,:) = data;
%                             continue;
%                         end
%                         if(ecg(ecgn,:) ~= data)
                            ecgn = ecgn + 1;
                            ecg(ecgn,:) = data;
%                         end
                        continue;
                    case 'R'
%                         if(rn == 0)
                            rn = rn + 1;
                            recg(rn,:) = data;
                            continue;
%                         end
%                         if(recg(rn,:) ~= data)
%                             rn = rn + 1;
%                             recg(rn,:) = data;
%                         end
%                         continue;
                    case 'Z'
                        zn = zn + 1;
                        z(zn,:) = data;
                        continue;
                    case 'B'
                        bn = bn + 1;
                        ecgb(bn,:) = data(1:1000);
                        zb(bn,:) = data(1001:2000);
                        continue;
                    case 'S'
                        sn = sn + 1;
                        s(sn) = data;
                        disp(dec2bin(s(sn)));
                        continue;
                    otherwise
                        disp(char(data));
                        continue;
                end
                
            end
            continue;
    end
    
    
    
end