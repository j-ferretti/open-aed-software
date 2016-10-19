function ecg_plot(ecg)
    figure
    n = size(ecg,1);
    te = [1:2000]/500;
    if(n == 1)
        plot(te,ecg);
        return;
    end
    for l = 1:round(n/9)
        figure(l)
        m = l:min([l+9 n]);
        for k = 1:9
            subplot(3,3,k), plot(te,ecg(m(k),:))
        end
    end
    return;
end