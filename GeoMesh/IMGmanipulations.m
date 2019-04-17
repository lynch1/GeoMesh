% Written by Daniel Lynch
% Based on tutorials in the documentation
% for the MATLAB Image Toolbox.

% Parameters
h = 200;
w = 200;
sigma = 0.1;
alpha = 15;
gbFactor = 7;
lgbLevels = 100;

% For input / output
inputfile = 'photos/utah.jpg';
outputfile = 'tfiles/utah.txt';

% Get photo
CL = imread(inputfile);

% Seperate into color channels
R = CL(:,:,1); % Red channel
G = CL(:,:,2); % Green channel
B = CL(:,:,3); % Blue channel

% Blur photo
GBF = imgaussfilt(CL, gbFactor);

% Contrast photo
% S = im2single(CL);
H = histeq(GBF);

% Process image
% A = im2single(BL);

%DN = localpfilt(GBF, sigma, alpha, 'NumIntensityLevels', lgbLevels);
%PR = psnr(DN, A);

%Convert to grayscale
GS = rgb2gray(GBF);
%GS = DN(:,:,1);

% for i = 1:750
%     for j = 1:750
%         temp = P_gs(i,j);
%         P_gs(i,j) = (255 - temp) - 20;
%     end
% end

% Resize image
EL = imresize(GS, [h w]);
% Find out the size of photo
% [hgs, wgs] = size(EL);

% Contrast again
%EL = histeq(EL);
%EL = locallapfilt(EL, sigma, alpha, 'NumIntensityLevels', lgbLevels);

% Display photo data
fig = figure('Name','Photo Manipulations');

% Show original image and readied image
subplot(3, 3, 1), imshow(CL), title('Original');
subplot(3, 3, 2), imshow(R), title('Red Channel');
subplot(3, 3, 3), imshow(B), title('Blue Channel');
subplot(3, 3, 4), imshow(G), title('Green Channel');
subplot(3, 3, 5), imshow(GBF), title('Gaussian Blur');
subplot(3, 3, 6), imshow(H), title('Hist. Contrasted');
%subplot(3, 3, 7), imshow(DN), title('De-Noised');
subplot(3, 3, 8), imshow(GS), title('Graycale');
subplot(3, 3, 9), imshow(EL), title('Resized');

fig2 = figure('Name', '2D Bitmap');
imshow(EL);

% EL2 = transpose(EL);
% fig3 = figure('Name', '2D Bitmap transposed');
% imshow(EL2);

% Write CSV file
fp = fopen(outputfile, 'wt');
fprintf(fp, '%d %d \n', h, w);
for i = 1:(h)
    for j = 1:(w)
        fprintf(fp, '%6f ', EL(i,j));
    end
    fprintf(fp, '\n');
end
fclose(fp);