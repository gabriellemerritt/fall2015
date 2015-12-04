%% Evaluation demo

I = imread('train_images_P1/253027.jpg');
I_bi = rgb2gray(I);
E = edge(I_bi, 'canny');
E = cannyEdge(I_bi);
load('ground_truth_P1/253027.mat');

[f precision recall] = edgeEval(E, groundTruth)

