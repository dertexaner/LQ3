#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TPolyLine.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"
#include "TSpline.h"
#include "TFile.h"

using namespace std;

void myStyle();
void setTDRStyle();

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************

 // eejj limits
 Double_t m_eejj[50] = {200, 204.082, 208.163, 212.245, 216.327, 220.408, 224.49, 228.571, 232.653, 236.735, 240.816, 244.898, 248.98, 253.061, 257.143, 261.224, 265.306, 269.388, 273.469, 277.551, 281.633, 285.714, 289.796, 293.878, 297.959, 302.041, 306.122, 310.204, 314.286, 318.367, 322.449, 326.531, 330.612, 334.694, 338.776, 342.857, 346.939, 351.02, 355.102, 359.184, 363.265, 367.347, 371.429, 375.51, 379.592, 383.673, 387.755, 391.837, 395.918, 400};
 Double_t beta_eejj_observed[50] = {0.192489, 0.19659, 0.201503, 0.207225, 0.213753, 0.22109, 0.229236, 0.238191, 0.247955, 0.258521, 0.269878, 0.282008, 0.294884, 0.308466, 0.322705, 0.337534, 0.352872, 0.36862, 0.384659, 0.400849, 0.417033, 0.433163, 0.449359, 0.465779, 0.482607, 0.50005, 0.518231, 0.537171, 0.556886, 0.577391, 0.598696, 0.620759, 0.643492, 0.666788, 0.690524, 0.714573, 0.738955, 0.763793, 0.78923, 0.815419, 0.842533, 0.870765, 0.900329, 0.931232, 0.963129, 0.99559, 1.02813, 1.06019, 1.09116, 1.12037};
 Double_t beta_eejj_expected[50] = {0.245939, 0.253519, 0.261428, 0.269668, 0.278244, 0.287156, 0.296408, 0.306, 0.315931, 0.326199, 0.336802, 0.347735, 0.35899, 0.370562, 0.382438, 0.394609, 0.40706, 0.419774, 0.432735, 0.445919, 0.459306, 0.472926, 0.486888, 0.501318, 0.516357, 0.532157, 0.548729, 0.565929, 0.583583, 0.601491, 0.619447, 0.637443, 0.655653, 0.674277, 0.693538, 0.713666, 0.734725, 0.75667, 0.779442, 0.802968, 0.827162, 0.851925, 0.877147, 0.902767, 0.928813, 0.955325, 0.98235, 1.00994, 1.03814, 1.06702};
 // enujj limits
 Double_t m_enujj_observed[100] = {200, 202.45, 204.899, 207.349, 209.799, 212.249, 214.698, 217.148, 219.598, 222.048, 224.497, 226.947, 229.397, 231.846, 234.296, 236.746, 239.196, 241.645, 244.095, 246.545, 248.995, 251.444, 253.894, 256.344, 258.794, 261.243, 263.693, 266.143, 268.592, 271.042, 273.492, 275.942, 278.391, 280.841, 283.291, 285.741, 288.19, 290.64, 293.09, 295.539, 297.989, 300.439, 302.889, 305.338, 307.788, 310.238, 312.688, 315.137, 317.587, 320.037, 320.037, 317.587, 315.137, 312.688, 310.238, 307.788, 305.338, 302.889, 300.439, 297.989, 295.539, 293.09, 290.64, 288.19, 285.741, 283.291, 280.841, 278.391, 275.942, 273.492, 271.042, 268.592, 266.143, 263.693, 261.243, 258.794, 256.344, 253.894, 251.444, 248.995, 246.545, 244.095, 241.645, 239.196, 236.746, 234.296, 231.846, 229.397, 226.947, 224.497, 222.048, 219.598, 217.148, 214.698, 212.249, 209.799, 207.349, 204.899, 202.45, 200};
 Double_t beta_enujj_observed[100] = {0.0481969, 0.0466801, 0.0456334, 0.0450041, 0.0447534, 0.0448532, 0.0452845, 0.0460357, 0.0471008, 0.0484792, 0.0501742, 0.0521931, 0.0545457, 0.0572446, 0.060304, 0.0637395, 0.0675669, 0.0718019, 0.0764587, 0.0815493, 0.0870817, 0.0930582, 0.0994741, 0.106315, 0.113555, 0.121152, 0.129047, 0.137159, 0.145385, 0.153593, 0.16162, 0.169277, 0.176345, 0.182585, 0.187918, 0.192562, 0.196808, 0.200998, 0.20552, 0.210813, 0.21739, 0.225876, 0.236873, 0.250666, 0.26762, 0.288323, 0.31378, 0.345964, 0.389992, 0.5, 0.5, 0.610008, 0.654036, 0.68622, 0.711677, 0.73238, 0.749334, 0.763127, 0.774124, 0.78261, 0.789187, 0.79448, 0.799002, 0.803192, 0.807438, 0.812082, 0.817415, 0.823655, 0.830723, 0.83838, 0.846407, 0.854615, 0.862841, 0.870953, 0.878848, 0.886445, 0.893685, 0.900526, 0.906942, 0.912918, 0.918451, 0.923541, 0.928198, 0.932433, 0.936261, 0.939696, 0.942755, 0.945454, 0.947807, 0.949826, 0.951521, 0.952899, 0.953964, 0.954715, 0.955147, 0.955247, 0.954996, 0.954367, 0.95332, 0.951803};
 Double_t m_enujj_expected[100] = {200, 202.451, 204.902, 207.353, 209.804, 212.255, 214.706, 217.157, 219.608, 222.06, 224.511, 226.962, 229.413, 231.864, 234.315, 236.766, 239.217, 241.668, 244.119, 246.57, 249.021, 251.472, 253.923, 256.374, 258.825, 261.277, 263.728, 266.179, 268.63, 271.081, 273.532, 275.983, 278.434, 280.885, 283.336, 285.787, 288.238, 290.689, 293.14, 295.591, 298.042, 300.494, 302.945, 305.396, 307.847, 310.298, 312.749, 315.2, 317.651, 320.102, 320.102, 317.651, 315.2, 312.749, 310.298, 307.847, 305.396, 302.945, 300.494, 298.042, 295.591, 293.14, 290.689, 288.238, 285.787, 283.336, 280.885, 278.434, 275.983, 273.532, 271.081, 268.63, 266.179, 263.728, 261.277, 258.825, 256.374, 253.923, 251.472, 249.021, 246.57, 244.119, 241.668, 239.217, 236.766, 234.315, 231.864, 229.413, 226.962, 224.511, 222.06, 219.608, 217.157, 214.706, 212.255, 209.804, 207.353, 204.902, 202.451, 200};
 Double_t beta_enujj_expected[100] = {0.0609646, 0.0627263, 0.0645734, 0.0665095, 0.0685384, 0.0706638, 0.0728898, 0.0752207, 0.0776611, 0.0802155, 0.082889, 0.0856868, 0.0886142, 0.091677, 0.0948813, 0.0982333, 0.10174, 0.105408, 0.109245, 0.113259, 0.117458, 0.121851, 0.126448, 0.131258, 0.136293, 0.141564, 0.147084, 0.152867, 0.158927, 0.165281, 0.171947, 0.178944, 0.186296, 0.194026, 0.202163, 0.210738, 0.219789, 0.229358, 0.239497, 0.250267, 0.261746, 0.274031, 0.287265, 0.30168, 0.317612, 0.335569, 0.356396, 0.381717, 0.415655, 0.5, 0.5, 0.584345, 0.618283, 0.643604, 0.664431, 0.682388, 0.69832, 0.712735, 0.725969, 0.738254, 0.749733, 0.760503, 0.770642, 0.780211, 0.789262, 0.797837, 0.805974, 0.813704, 0.821056, 0.828053, 0.834719, 0.841073, 0.847133, 0.852916, 0.858436, 0.863707, 0.868742, 0.873552, 0.878149, 0.882542, 0.886741, 0.890755, 0.894592, 0.89826, 0.901767, 0.905119, 0.908323, 0.911386, 0.914313, 0.917111, 0.919784, 0.922339, 0.924779, 0.92711, 0.929336, 0.931462, 0.93349, 0.935427, 0.937274, 0.939035};
 // combined limits
 Double_t beta_comb_observed[22] = {0.050536, 0.06, 0.08, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1};
 Double_t m_comb_observed[22] = {200.000, 233.027, 245.546, 254.27, 271.625, 295.592, 308.938, 316.743, 322.712, 328.038, 333.486, 338.996, 344.965, 352.801, 359.229, 363.545, 367.646, 371.074, 374.197, 376.37, 379.614, 384.083};
 Double_t beta_comb_expected[21] = {0.062088, 0.08, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1};
 Double_t m_comb_expected[21] = {200.000, 220.141, 236.425, 264.034, 283.502, 298.99, 311.723, 322.039, 330.64, 337.833, 344.2, 349.617, 354.484, 358.892, 362.718, 366.636, 370.37, 374.594, 378.727, 383.93, 390.572};
 
 
 // PASTE INFORMATION HERE
//static int numbetas = 2;
//Double_t beta_vals[2] = {0.3, 0.5};


//Double_t m_expected_combo[2] = {564, 640};


//Double_t m_observed_combo[2] = {557, 636};


//Double_t m_1sigma_combo[4]={620, 680, 599, 509};


//Double_t m_2sigma_combo[4]={653, 717, 560, 463};


//Double_t m_expected_lljj[2] = {497, 614};


//Double_t m_observed_lljj[2] = {526, 631};


//Double_t m_1sigma_lljj[4]={540, 651, 580, 453};


//Double_t m_2sigma_lljj[4]={571, 685, 550, 418};


//Double_t m_expected_lvjj[2] = {533, 572};


//Double_t m_observed_lvjj[2] = {519, 540};


//Double_t m_1sigma_lvjj[4]={605, 628, 510, 485};


//Double_t m_2sigma_lvjj[4]={641, 660, 469, 442};


//Double_t beta_vals[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99};


//Double_t m_expected_combo[10] = {405, 500, 564, 611, 640, 668, 698, 727, 756, 779};
//Double_t m_observed_combo[10] = {411, 494, 557, 603, 636, 668, 702, 744, 790, 827};
//Double_t m_1sigma_combo[20]={461, 555, 620, 651, 680, 710, 740, 768, 794, 815, 739, 714, 685, 656, 628, 599, 559, 509, 451, 353};
//Double_t m_2sigma_combo[20]={502, 613, 653, 686, 717, 747, 775, 801, 825, 844, 703, 678, 650, 623, 595, 560, 517, 463, 407, 293};


//Double_t m_expected_lljj[10] = {0, 394, 497, 568, 614, 655, 693, 727, 757, 780};
//Double_t m_observed_lljj[10] = {0, 438, 526, 593, 631, 670, 710, 753, 793, 827};
//Double_t m_1sigma_lljj[20]={0, 434, 540, 601, 651, 695, 735, 768, 795, 816, 740, 715, 685, 653, 617, 580, 530, 453, 346, 0};
//Double_t m_2sigma_lljj[20]={0, 470, 571, 632, 685, 732, 770, 801, 826, 845, 703, 679, 651, 620, 588, 550, 492, 418, 280, 0};


//Double_t m_expected_lvjj[10] = {400, 493, 533, 561, 572, 561, 533, 493, 400, 250};
//Double_t m_observed_lvjj[10] = {408, 476, 519, 535, 540, 535, 519, 476, 408, 250};
//Double_t m_1sigma_lvjj[20]={457, 543, 605, 623, 628, 623, 605, 543, 457, 0, 0, 342, 444, 485, 504, 510, 504, 485, 444, 342};
//Double_t m_2sigma_lvjj[20]={498, 607, 641, 656, 660, 656, 641, 607, 498, 0, 0, 0, 393, 442, 463, 469, 463, 442, 393, 0};

//Double_t beta_vals[29]={0.02,0.04,0.06,0.1,0.12,0.14,0.18,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.82,0.84,0.86,0.88,0.9,0.94,0.96,0.98,0.9995};
//Double_t m_expected_combo[29]={250,250,334,405,434,455,486,500,531,564,592,611,626,640,654,668,683,698,713,727,733,739,745,750,756,766,771,776,781};
//Double_t m_observed_combo[29]={250,250,258,411,426,439,471,494,528,557,583,603,620,636,652,668,684,702,722,744,753,761,771,780,790,807,814,823,830};
//Double_t m_1sigma_combo[58]={250,328,382,461,482,501,536,555,598,620,636,651,665,680,695,710,725,740,754,768,774,779,784,789,794,803,808,813,817,741,736,731,725,714,708,703,697,691,685,671,656,642,628,614,599,582,559,534,509,481,451,438,400,378,353,0,0,0};
//Double_t m_2sigma_combo[58]={250,369,432,502,524,549,599,613,635,653,670,686,702,717,733,747,761,775,788,801,806,811,816,820,825,834,838,842,846,705,700,695,689,678,673,667,661,656,650,636,623,609,595,579,560,539,517,491,463,439,407,391,353,330,293,0,0,0};
//Double_t m_expected_lljj[29]={250,250,250,250,250,250,365,394,449,497,538,568,592,614,635,655,675,693,711,727,734,740,746,752,757,768,773,778,782};
//Double_t m_observed_lljj[29]={250,250,250,250,343,372,417,438,477,526,570,593,612,631,650,670,690,710,732,753,762,770,778,786,793,808,815,824,830};
//Double_t m_expected_lvjj[29]={250,250,310,400,429,451,482,493,516,533,548,561,570,572,570,561,548,533,516,493,482,468,451,429,400,310,250,250,250};
//Double_t m_observed_lvjj[29]={250,250,259,408,423,435,461,476,505,519,529,535,539,540,539,535,529,519,505,476,461,447,435,423,408,259,250,250,250};

static int numbetas = 146;
Double_t beta_vals[146]={0.003,0.006,0.009,0.012,0.015,0.018,0.021,0.024,0.027,0.03,0.033,0.036,0.039,0.042,0.045,0.048,0.051,0.054,0.057,0.06,0.063,0.066,0.069,0.072,0.075,0.078,0.081,0.084,0.087,0.09,0.093,0.096,0.099,0.102,0.112,0.122,0.132,0.142,0.152,0.162,0.172,0.182,0.192,0.202,0.212,0.222,0.232,0.242,0.252,0.262,0.272,0.282,0.292,0.302,0.312,0.322,0.332,0.342,0.352,0.362,0.372,0.382,0.392,0.402,0.412,0.422,0.432,0.442,0.452,0.462,0.472,0.482,0.492,0.502,0.512,0.522,0.532,0.542,0.552,0.562,0.572,0.582,0.592,0.602,0.612,0.622,0.632,0.642,0.652,0.662,0.672,0.682,0.692,0.702,0.712,0.722,0.732,0.742,0.752,0.762,0.772,0.782,0.792,0.802,0.812,0.822,0.832,0.842,0.852,0.862,0.872,0.882,0.892,0.902,0.905,0.908,0.911,0.914,0.917,0.92,0.923,0.926,0.929,0.932,0.935,0.938,0.941,0.944,0.947,0.95,0.953,0.956,0.959,0.962,0.965,0.968,0.971,0.974,0.977,0.98,0.983,0.986,0.989,0.992,0.995,0.9995};
Double_t m_expected_combo[146]={250,250,250,250,250,250,250,250,250,256,287,304,317,328,338,347,355,362,370,376,383,390,396,403,410,416,423,429,434,439,444,449,453,456,468,478,488,498,509,521,532,541,549,555,561,566,571,575,579,583,587,590,594,597,601,604,608,611,614,617,620,623,626,629,632,635,638,641,643,646,649,651,654,656,659,661,664,666,669,671,674,676,679,681,684,686,689,691,694,697,699,702,704,707,710,712,715,718,721,723,726,729,732,734,737,740,743,745,748,751,754,756,759,762,762,763,764,765,765,766,767,768,768,769,770,771,771,772,773,774,774,775,776,777,777,778,779,779,780,781,782,782,783,784,784,785};
Double_t m_observed_combo[146]={250,250,250,250,250,250,250,250,250,250,250,250,250,253,262,271,283,293,339,379,394,404,412,418,425,431,437,445,452,462,473,482,490,495,508,517,525,532,538,545,553,560,568,576,582,588,592,597,600,603,606,608,611,613,616,618,620,622,624,625,627,629,631,633,635,637,638,640,642,644,646,648,650,652,654,656,659,661,664,666,669,672,675,679,682,686,690,694,698,705,709,715,722,728,733,740,748,753,758,765,770,775,780,785,788,793,796,800,803,807,809,813,815,817,819,820,820,821,822,823,823,823,824,825,826,827,828,828,829,830,831,831,832,832,833,834,835,835,836,837,837,837,838,839,840,841};
Double_t m_1sigma_combo[292]={250,250,250,250,250,250,250,292,313,330,343,355,366,376,385,395,405,414,424,432,440,447,453,458,463,468,472,476,481,485,490,494,498,502,520,536,549,558,566,572,578,584,589,594,598,603,608,612,617,621,625,629,633,636,640,643,646,650,653,656,659,662,665,668,670,673,676,678,681,684,686,689,692,694,697,700,702,705,707,710,713,715,718,721,723,726,729,732,734,737,740,743,745,748,751,754,756,759,762,764,767,770,772,775,777,780,782,785,787,790,792,795,797,799,800,801,801,802,803,803,804,805,805,806,807,807,808,809,809,810,810,811,812,812,813,814,814,815,815,816,817,817,818,818,819,820,746,744,744,743,742,741,741,740,739,738,737,737,736,735,734,733,733,732,731,730,729,729,728,727,726,725,724,724,723,722,721,720,719,717,714,711,709,706,703,701,698,695,693,690,687,685,682,680,677,675,672,670,668,665,663,660,658,656,653,651,649,646,644,641,639,637,634,632,629,627,624,622,619,616,614,611,608,605,603,600,597,594,591,588,585,582,579,576,573,569,566,562,559,555,550,545,540,533,526,518,510,502,495,488,481,474,467,459,451,441,429,415,400,395,391,386,382,377,373,368,363,358,353,347,341,335,328,321,312,303,292,277,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Double_t m_2sigma_combo[292]={250,250,250,250,250,294,321,340,357,371,384,397,411,424,435,445,453,460,467,473,479,485,491,496,503,510,517,525,532,539,544,549,553,557,568,576,584,591,598,604,611,617,622,628,633,638,643,647,651,655,659,663,666,670,673,676,680,683,686,689,692,695,698,700,703,706,709,712,714,717,720,723,725,728,731,734,736,739,742,745,747,750,753,755,758,761,764,766,769,771,774,777,779,782,785,787,790,792,795,797,800,802,804,807,809,811,813,816,818,820,822,824,826,828,829,829,830,831,831,832,832,833,833,834,835,835,836,836,837,837,838,839,839,840,840,841,841,842,842,843,843,844,844,845,846,846,709,708,707,706,705,705,704,703,702,702,701,700,699,698,698,697,696,695,694,694,693,692,691,691,690,689,688,688,687,686,685,685,684,681,679,676,674,671,669,667,664,662,659,657,655,653,650,648,646,643,641,639,637,634,632,630,627,625,623,620,618,615,613,610,608,605,603,600,597,595,592,589,586,584,581,578,575,572,569,566,563,560,557,554,550,546,542,538,533,529,523,517,511,505,499,493,488,482,477,471,466,460,453,447,439,430,420,410,399,389,378,367,355,352,348,344,339,335,330,325,320,314,308,301,292,283,269,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Double_t m_expected_lljj[146]={250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,251,269,291,316,344,370,391,409,424,437,451,464,476,487,497,506,513,520,527,533,539,545,551,557,563,569,575,581,586,592,597,602,606,611,615,619,623,627,631,635,639,643,646,650,654,657,661,665,668,672,676,679,683,686,690,694,697,701,704,708,711,715,718,721,725,728,731,734,738,741,744,747,750,753,755,758,761,762,763,763,764,765,766,767,767,768,769,770,770,771,772,773,773,774,775,776,776,777,778,779,779,780,781,781,782,783,784,784,785};
Double_t m_observed_lljj[146]={250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,257,318,379,407,425,440,454,466,478,489,499,509,518,527,535,542,549,554,560,565,570,575,579,583,588,592,596,600,604,607,611,615,619,622,626,630,633,637,641,645,648,653,657,661,665,670,675,680,685,691,696,702,709,715,722,728,735,741,748,754,759,765,770,775,780,784,788,791,795,798,802,805,808,811,813,817,819,821,822,823,823,824,825,825,826,827,827,828,829,829,830,831,831,831,832,833,833,834,834,835,835,836,837,837,838,838,839,839,840,840};
Double_t m_expected_lvjj[146]={250,250,250,250,250,250,250,250,250,250,250,272,295,310,322,333,342,351,359,366,373,380,387,394,401,408,415,421,428,434,439,444,449,453,464,474,484,493,502,512,522,531,540,546,552,557,561,565,568,571,574,576,579,581,583,585,587,588,590,591,593,594,595,596,597,598,598,599,599,600,600,600,600,600,600,600,600,600,599,599,598,597,596,595,594,593,592,591,589,588,586,584,582,580,578,575,573,570,567,563,559,555,550,544,537,528,518,508,498,489,480,471,460,447,442,437,432,426,419,412,405,398,392,385,378,371,364,356,348,339,329,318,305,288,257,250,250,250,250,250,250,250,250,250,250,250};
Double_t m_observed_lvjj[146]={250,250,250,250,250,250,250,250,250,250,250,250,250,256,269,285,331,371,387,397,405,412,418,424,429,434,440,446,452,459,468,477,484,490,502,511,518,523,529,534,539,543,548,553,559,564,570,575,580,584,587,590,593,595,597,599,601,602,603,604,605,606,607,608,608,609,609,610,610,610,610,611,611,611,611,610,610,610,610,609,609,608,608,607,607,606,605,604,603,601,600,598,597,594,592,589,586,582,578,573,568,562,556,551,546,541,537,532,527,521,515,508,498,482,475,466,457,450,444,438,433,427,421,416,410,403,394,383,362,311,279,264,253,250,250,250,250,250,250,250,250,250,250,250,250,250};

// INFORMATION PASTING OVER

Double_t beta_valsdub[numbetas*2];
int ind = numbetas;
for (unsigned int ii = 0; ii < numbetas*2; ++ii){
	if (ii < numbetas) beta_valsdub[ii] = beta_vals[ii];
	else {
		ind = ind -1;
		beta_valsdub[ii] = beta_vals[ind];
	}
}



 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName0 = "LQ2Combination.eps";
 string fileName1 = "LQ2Combination.pdf";
 string fileName2 = "LQ2Combination.png";


 // axes labels for the final plot
 string title = ";M_{LQ} [GeV];#beta";

 // integrated luminosity
 string sqrts = "#sqrt{s} = 7 TeV";

 // region excluded by Tevatron limits (1 fb-1)
 Double_t x_excl_D0[11] = {214.39,235.13,254.08,268.12,275.92,283.95,289.08,293.09,295.99,297.10,298.89};
 Double_t y_excl_D0[11] = {0,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1};

 // region excluded by ATLAS limits (35 pb-1)
 Double_t x_excl_ATLAS[13] = {200,221.91,243.07,254.12,265.17,287.83,304.68,318.54,331.46,342.88,353.75,363.11,375.47};
 Double_t y_excl_ATLAS[13] = {0.095,0.10,0.126,0.151,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.0};
 
 // region excluded by Tevatron limits (5.4 fb-1)
 Double_t x_excl_D0_new[11] = {243.13,275.12,308.18,320.02,325.95,327.96,325.95,320.02,308.18,275.12,243.13};
 Double_t y_excl_D0_new[11] = {0.05,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,0.95};

Double_t AtlasComboMass[13] = {250,253.997194950912,278.541374474053,305.539971949509,341.374474053296,352.173913043478,362.973352033661,371.318373071529,385.063113604488,395.371669004208,413.534361851332,421.879382889201,250};
Double_t AtlasComboBeta[13] ={.1,0.12280701754386,0.14619883040936,0.19493177387914,0.34892787524366,0.39766081871345,0.51851851851852,0.60038986354776,0.74269005847953,0.79727095516569,0.93567251461988,0.99805068226121,1.0};

 // turn on/off batch mode
 gROOT->SetBatch(kTRUE);
 // set ROOT style
//  myStyle();
 setTDRStyle();
//  gStyle->SetPadLeftMargin(0.14);
 gROOT->ForceStyle();

 TH2F *bg = new TH2F("bg",title.c_str(), 20, 250., 950., 100, 0., 1.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.05,"Y");
//  bg->GetXaxis()->SetNdivisions(505);

 TCanvas *c = new TCanvas("c","",800,800);
 c->cd();

 bg->Draw();

 TGraph *gr_excl_D0 = new TGraph(11,x_excl_D0,y_excl_D0);
 gr_excl_D0->SetLineWidth(3);
 gr_excl_D0->SetLineColor(49);
 gr_excl_D0->SetLineStyle(6);
 //gr_excl_D0->Draw("C");

 TPolyLine*gr_excl_ATLAS = new TPolyLine(13,AtlasComboMass,AtlasComboBeta,"F");
 //gr_excl_ATLAS->SetLineWidth(3);
 gr_excl_ATLAS->SetLineColor(kBlack);
 gr_excl_ATLAS->SetFillColor(kBlack);
 gr_excl_ATLAS->SetFillStyle(3244);
 gr_excl_ATLAS->Draw();

 TGraph *gr_excl_D0_new = new TGraph(11,x_excl_D0_new,y_excl_D0_new);
 gr_excl_D0_new->SetLineWidth(3);
 gr_excl_D0_new->SetLineColor(30);
 gr_excl_D0_new->SetLineStyle(7);
 //gr_excl_D0_new->Draw("C");


// ------------------------------------ LLJJ CURVES  -------------------------------------//
 TPolyLine *beta_vs_m_lljj_observed = new TPolyLine(numbetas, m_observed_lljj, beta_vals);
 TPolyLine *beta_vs_m_lljj_expected = new TPolyLine(numbetas, m_expected_lljj, beta_vals);
 //TPolyLine *beta_vs_m_lljj_observed_shade = new TPolyLine(numbetas*2, m_2sigma_lljj, beta_valsdub);
 
 beta_vs_m_lljj_observed->SetLineWidth(3);
 beta_vs_m_lljj_observed->SetLineColor(kBlue);
 beta_vs_m_lljj_observed->SetLineStyle(1);

 beta_vs_m_lljj_expected->SetLineWidth(3);
 beta_vs_m_lljj_expected->SetLineColor(kBlue);
 beta_vs_m_lljj_expected->SetLineStyle(2);
 
 
 //beta_vs_m_lljj_observed_shade->SetLineWidth(3);
 //beta_vs_m_lljj_observed_shade->SetLineColor(kGreen+2);
 //beta_vs_m_lljj_observed_shade->SetLineStyle(4);
 //beta_vs_m_lljj_observed_shade->SetFillStyle(3425);
 //beta_vs_m_lljj_observed_shade->SetFillColor(kGreen+2);
 //beta_vs_m_lljj_observed_shade->Draw("f");




// ------------------------------------ LVJJ CURVES  -------------------------------------//

 TPolyLine *beta_vs_m_lvjj_observed = new TPolyLine(numbetas, m_observed_lvjj, beta_vals);
 TPolyLine *beta_vs_m_lvjj_expected = new TPolyLine(numbetas, m_expected_lvjj, beta_vals);
 //TPolyLine *beta_vs_m_lvjj_observed_shade = new TPolyLine(numbetas*2, m_2sigma_lvjj, beta_valsdub);
 
 beta_vs_m_lvjj_observed->SetLineWidth(3);
 beta_vs_m_lvjj_observed->SetLineColor(kRed);
 beta_vs_m_lvjj_observed->SetLineStyle(1);
 
 beta_vs_m_lvjj_expected->SetLineWidth(3);
 beta_vs_m_lvjj_expected->SetLineColor(kRed);
 beta_vs_m_lvjj_expected->SetLineStyle(2);
 
 //beta_vs_m_lvjj_observed_shade->SetFillStyle(3452);
 //beta_vs_m_lvjj_observed_shade->SetFillColor(kRed);
 //beta_vs_m_lvjj_observed_shade->Draw("f");




// ------------------------------------ COMBO CURVES  -------------------------------------//

 TPolyLine *beta_vs_m_comb_observed = new TPolyLine(numbetas, m_observed_combo, beta_vals);
 TPolyLine *beta_vs_m_comb_expected = new TPolyLine(numbetas, m_expected_combo, beta_vals);
 TGraph *beta_vs_m_comb_expected_shade2 = new TGraph(numbetas*2, m_2sigma_combo, beta_valsdub);
 TGraph *beta_vs_m_comb_expected_shade1 = new TGraph(numbetas*2, m_1sigma_combo, beta_valsdub);

 
 beta_vs_m_comb_observed->SetLineWidth(3);
 beta_vs_m_comb_observed->SetLineColor(kBlack);
 beta_vs_m_comb_expected->SetLineStyle(1);

 //beta_vs_m_comb_expected_shade->SetLineWidth(1);
 beta_vs_m_comb_expected_shade1->SetFillStyle(1001);
 beta_vs_m_comb_expected_shade1->SetFillColor(kGreen);
 beta_vs_m_comb_expected_shade2->SetFillStyle(1001);
 beta_vs_m_comb_expected_shade2->SetFillColor(kYellow);

 
 beta_vs_m_comb_expected->SetLineWidth(3);
 beta_vs_m_comb_expected->SetLineColor(kBlack);
 beta_vs_m_comb_expected->SetLineStyle(2);

// ------------------------------------ DRAW  -------------------------------------//

 beta_vs_m_comb_expected_shade2->Draw("f");

 beta_vs_m_comb_expected_shade1->Draw("f");

 beta_vs_m_lljj_expected->Draw("C");
 beta_vs_m_lvjj_expected->Draw("C");
 beta_vs_m_comb_expected->Draw("C");
 beta_vs_m_lljj_observed->Draw("C");
 beta_vs_m_lvjj_observed->Draw("C");
 beta_vs_m_comb_observed->Draw("C");




 gPad->RedrawAxis();

 TLegend *legend = new TLegend(.59,.15,.92,.32);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetTextFont(132);
 legend->SetMargin(0.15);
 legend->SetHeader("95% CL limits");
 legend->AddEntry(beta_vs_m_comb_observed,"CMS #mu#mujj + #mu#nujj (Obs.)","l");
 //legend->AddEntry(beta_vs_m_comb_expected,"CMS #mu#mujj + #mu#nujj (Exp.)","l");
 legend->AddEntry(beta_vs_m_lvjj_observed,"CMS #mu#nujj (Obs.), 4.7 fb^{-1}","l");
 legend->AddEntry(beta_vs_m_lljj_observed,"CMS #mu#mujj (Obs.), 4.7 fb^{-1}","l");
 //legend->AddEntry(gr_excl_D0_new,"D#oslash (Obs.), fb^{-1}","l");
 legend->AddEntry(gr_excl_ATLAS,"ATLAS (Obs.), 35 pb^{-1}","f");
 //legend->AddEntry(gr_excl_D0,"D#oslash (Obs.), 1 fb^{-1}","l");

 legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextFont(132);
 l1.SetNDC();
 l1.SetTextSize(0.04);
 l1.DrawLatex(0.68,0.41,"CMS Preliminary");
 l1.SetTextSize(0.04);
 l1.DrawLatex(0.68,0.36,sqrts.c_str());

 TLatex l2;
 l2.SetTextAlign(12);
 l2.SetTextFont(132);
 l2.SetNDC();
 l2.SetTextSize(0.04);
 l2.SetTextColor(kGreen+2);
 l2.SetTextAngle(35);
 //l2.DrawLatex(0.17,0.33,"#mu#mujj");
 l2.SetTextColor(kRed);
 l2.SetTextAngle(-35);
 //l2.DrawLatex(0.3,0.81,"#mu#nujj");
 l2.SetTextColor(kBlack);
 l2.SetTextAngle(75);
 //l2.DrawLatex(0.58,0.6,"#mu#mujj + #mu#nujj");

 c->SetGridx();
 c->SetGridy();
 c->SetLogy(0);
 c->SaveAs(fileName0.c_str());
 c->SaveAs(fileName1.c_str());
 c->SaveAs(fileName2.c_str());


 delete legend;
 delete beta_vs_m_lljj_observed;
 delete beta_vs_m_lvjj_observed;
 delete beta_vs_m_comb_observed;
 delete gr_excl_D0;
 delete gr_excl_ATLAS;
 delete gr_excl_D0_new;
 delete bg;
 delete c;
}

void myStyle()
{
 gStyle->Reset("Default");
 gStyle->SetCanvasColor(0);
 gStyle->SetPadColor(0);
 gStyle->SetTitleFillColor(10);
 gStyle->SetCanvasBorderMode(0);
 gStyle->SetStatColor(0);
 gStyle->SetPadBorderMode(0);
 gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
 gStyle->SetPadTickY(1);
 gStyle->SetFrameBorderMode(0);
 gStyle->SetPalette(1);

   //gStyle->SetOptStat(kFALSE);
 gStyle->SetOptStat(111110);
 gStyle->SetOptFit(0);
 gStyle->SetStatFont(42);
 gStyle->SetPadLeftMargin(0.13);
 gStyle->SetPadRightMargin(0.07);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
 gStyle->SetStatY(.9);
}

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  tdrStyle->SetHistFillColor(63);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

//  tdrStyle->SetEndErrorSize(0);
//   tdrStyle->SetErrorX(0.);
//  tdrStyle->SetErrorMarker(20);

  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:

  //  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(132, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(132, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();
}



