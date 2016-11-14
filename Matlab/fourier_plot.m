function fourier_plot(sig, f)
    
    if(nargin == 1)
        f = 500;
    end
    sig_fshift = fftshift(abs(fft(sig')));
    nl = length(sig_fshift);
    df = f / (nl);
    fk = df * (-nl/2:nl/2-1);
    % figure, plot(fk,sig_fshift)
    figure, plot(fk,20*log10(sig_fshift))
end