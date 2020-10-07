% SXEDIASI TELESTIKOU ENISXITI 2019-2020
% ONOMA : STASINOS ALKIVIADIS
% AEM : 9214


% PARAMETROI


% CL = 2.14 pF
% SR > 18.14 V/µs
% Vdd = 1.842 V
% vss = -1.842 V
% GB > 7.14 MHz
% A > 20.14 dB
% P < 50.14 mW


CL = 2.14 * 10^-12 
GBmin = 7.14 * 10^6 % Minimum GB
Vdd = 1.842
Vss = -Vdd
minSR = 18.14 * 10^6
Pmax = 50.14 * 10^-3
Amin = 20.14

% Algorithm Steps

% 1st step
L = 1 * 10^-6 

% 2nd step
% Cc > 0.22 * CL 
Cc = 3 * 10^-12 % Set at 3 pF

% 3rd step
I5 = minSR * Cc

% 4th step

VINmax = 0.1
VINmin = -0.1
VToP = -0.9056
VToN = 0.786
kp = 60 * 10^-6
kn = 150 * 10^-6
s3 = I5/(kp * (Vdd - VINmax -abs(VToP) + VToN)^2)

% s3 was 0.344 < 1 so it was set to s3=1
s3 = 1
s4 = s3

% 5th step
% Checking if p3 > 10GB
Cox = 4.6 * 10^-3
ln = 0.04
lp = 0.05
I3 = I5/2
W3 = s3 * L
p3 = sqrt(2*kp*s3*I3)/(2*0.667*W3*L*Cox)
p3 > 10*GBmin * 2 *pi

% 6th step
gm1 = GBmin * 2*pi * Cc
gm2 = gm1
s1 = (gm1^2)/(kn*I5)
s2 = s1

% 7th step

beta1 = kn * s1
Vds5 = VINmin - Vss - sqrt(I5/beta1) - VToN
s5 = (2*I5)/(kn * (Vds5)^2)

% 8th-9th step
I4 = I5/2
gm4 = sqrt(2*kp*s4*I4)
gm6 = 2.2 * gm2 * (CL/Cc)
s6 = s4 * (gm6/gm4)
I6 = (gm6^2)/(2*kp*s6)

% 10th step
s7 = (I6/I5)*s5

% 11th step

Av = (2*gm2*gm6)/(I5*(ln+lp)*I6*(ln+lp))
gain = 20*log10(Av)
% Check gain 
gain > Amin

Pdiss = (I5+I6)*(Vdd + abs(Vss))
% Check Pdiss < Pmax
Pdiss < Pmax

% W

W1 = s1 * L
W2 = s2 * L
W3 = s3 * L
W4 = s4 * L
W5 = s5 * L
W6 = s6 * L
W7 = s7 * L
W8 = W5


