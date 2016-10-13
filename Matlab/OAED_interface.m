

s1 = serial('COM7', 'Baudrate', 9600);

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
            while(s1.BytesAvailable ~= 0)
                disp(char(fread(s1,s1.BytesAvailable))');
            end
            continue;            
        case 'i'
            c = input('c ','s');
            fwrite(s1,c);
            while(s1.BytesAvailable == 0) end
            while(s1.BytesAvailable ~= 0)
                disp(char(fread(s1,s1.BytesAvailable))');
                pause(0.1);
            end
            continue;
    end
    
    
    
end