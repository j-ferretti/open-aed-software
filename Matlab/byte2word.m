function W = byte2word(B)
    W = B(1:2:end) + bitshift(B(2:2:end),8);
    return;
end