clc

%%
n = 0;
m = 0;
while(true)

    in = input('in ');

    if(~psoc.reload)
        disp('Cannot reload psoc');
        break;
    end

    for k = 1:in
        psoc.message = 'a';
        pause(0.05);
        psoc.send;
        pause(0.1);
        tmp = psoc.receive;
        ecg = [ecg tmp];
        %tmp = psoc.receive;
        %ecg = [ecg tmp];
        n = n+1;

        if(psoc.message == 'A')
            tmp = psoc.receive;
            raw = [raw tmp];
            m = m+1;
        end

        disp(n);
        t = [1:length(ecg)]/500;
        plot(t, ecg)
    end

    break;
end
