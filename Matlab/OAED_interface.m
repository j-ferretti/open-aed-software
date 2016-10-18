s1 = serial('COM9', 'Baudrate', 9600);
count = 0;

while(true)
    
    in = input('in ','s');
    switch in
        case 'e'
            break;
        case 'o'
            fopen(s1);
            disp(s1);
            continue;
        case 'c'
            fclose(s1);
            disp(s1);
            continue;
        case 's'
            disp(s1);
            continue;
        case 'r'
            fclose(s1); fopen(s1); disp(s1);
            continue;
        case 'i'
            c = input('c ','s');
            while(c ~= 'e')
                fwrite(s1,c);
                while(s1.BytesAvailable == 0) end
                n = 0;
                count = count + 1;
                while(s1.BytesAvailable ~= 0)
                    data = fread(s1,s1.BytesAvailable);
                    if(c == 'E')
                        ecg(count,(n + 1):floor(n + length(data)/2) ) = data(1:2:end) + bitshift(data(2:2:end),8);
                        ecg(count, floor(n + length(data)/2):(end)) = 0;
                        n = n + length(data)/2;
                    end
                    if(c == 'Z')
                        z(count,(n + 1):floor(n + length(data)/2) ) = data(1:2:end) + bitshift(data(2:2:end),8);
                        z(count, floor(n + length(data)/2):(end)) = 0;
                        n = n + length(data)/2;
                    end
                    disp(char(data)');
                    pause(0.1);
                end
                c = input('c ','s');
            end
            continue;
    end
    
    
    
end