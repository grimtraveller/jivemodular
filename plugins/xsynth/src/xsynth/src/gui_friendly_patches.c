/* Copyright? Hmm.  Sean's patches are public domain, Steve's patches are
 * GPLed?, Silvan's are ? */

#include "xsynth_voice.h"

#define FRIENDLY_PATCH_COUNT  57

int friendly_patch_count = FRIENDLY_PATCH_COUNT;

xsynth_patch_t friendly_patches[FRIENDLY_PATCH_COUNT] = {
    {
        "strings",
        0.996955, 2, 0.5,
        1.00305, 2, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.0002, 0.1, 1, 1e-04, 0, 0, 14.5722,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        11.4258, 0.388616, 1,
        0.984375, 0.5
    },
    {
        "A-ha",
        1, 2, 0.5,
        2.00277, 2, 0,
        0, 0.53,
        0.18197, 0, 0, 0,
        0.057544, 0.000912, 0.65, 0.000132, 0, 0, 0,
        0.075858, 0.005248, 0.59, 0.000331, 0, 0, 31,
        11, 0.37905, 0,
        1, 0.6296
    },
    {
        "A-ha VS",
        1, 2, 0.5,
        2.00277, 6, 0.0859207,
        0, 0.53,
        0.18197, 0, 0, 0,
        0.057544, 0.000912, 0.65, 0.000128351, 0.234944, 0, 0,
        0.075858, 0.005248, 0.59, 0.000331, 0.774276, 0, 22.9056,
        4.91945, 0.37905, 0,
        1, 0.682958
    },
    {
        "Brass",
        0.997923, 2, 0.21,
        1.00208, 2, 0.25,
        0, 0.56,
        2.51189, 0, 0, 0.05,
        0.083176, 5.1e-05, 0.67, 0.000191, 0, 0, 0,
        0.000912, 0.000398, 0.73, 6.9e-05, 0, 0, 14.5,
        0, 0.89775, 1,
        0.984375, 0.530111
    },
    {
        "Xylophone",
        1, 0, 0.5,
        4, 0, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.069183, 0.000275, 0, 0.000275, 0, 0, 0,
        0.091201, 2.8e-05, 0, 2.3e-05, 0, 0, 50,
        0, 1.98303, 0,
        0.984375, 0.49
    },
    {
        "Shimmer Pad",
        1, 6, 0.0682776,
        1.00069, 6, 0.08875,
        0, 0.440943,
        0.70426, 0, 0, 0.193056,
        0.000215443, 0.1, 1, 0.000163263, 0.333333, 0, 0,
        0.000533199, 0.1, 1, 0.000168958, 0.774276, 0, 37.3706,
        4.91945, 0.748568, 2,
        0.984375, 0.84622
    },
    {
        "Sun Dog",
        0.998615, 3, 0.5,
        1.00139, 5, 0.598389,
        0, 0.5,
        0.382727, 1, 0, 0,
        0.0168958, 0.0215443, 1, 0.000267462, 0, 0, 0,
        0.037116, 0.1, 1, 0.000250322, 0, 0, 25,
        0, 0.861286, 2,
        0.0539244, 0.663001
    },
    {
        "Organ 2",
        1, 0, 0.5,
        2.00277, 4, 0.5,
        0, 0.41,
        6.62217, 0, 0, 0.071,
        0.047863, 0.1, 1, 0.013183, 0, 0, 1e-06,
        0.063096, 0.1, 1, 0.015849, 0, 0, 1e-06,
        6.5, 0.05985, 1,
        1, 0.51
    },
    {
        "French Horn",
        1, 5, 0.21,
        1, 2, 0.25,
        0, 0.56,
        3.98107, 0, 0, 0.17,
        0.043652, 5.1e-05, 0, 0.000142, 0, 0, 0,
        1e-04, 4.4e-05, 0, 8.3e-05, 0, 0, 12.5,
        0, 1.197, 1,
        0.984375, 0.7
    },
    {
        "plucked",
        0.996955, 2, 0.5,
        1.00305, 2, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.013347, 0.000198, 0, 0.000215, 0, 0, 1e-06,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        10.9326, 1.39007, 1,
        0.984375, 0.509221
    },
    {
        "bell",
        1, 1, 0.5,
        4, 1, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.1, 5.1e-05, 0, 0.000142, 0, 0, 1e-06,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        15.4926, 1.79871, 1,
        0.984375, 0.505
    },
    {
        "bell VS",
        1, 1, 0.5,
        4, 1, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.1, 5.1e-05, 0, 0.000142, 0.314095, 0, 1e-06,
        0.1, 0.1, 1, 1e-05, 0.708555, 0, 20.0805,
        3.325, 1.6625, 1,
        0.984375, 0.5155
    },
    {
        "sync_bass",
        1, 4, 0.5,
        2.01737, 2, 0.507856,
        1, 0.5,
        0.1, 1, 0, 0,
        0.1, 4.6e-05, 0, 0.00011, 0, 0.275399, 50,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        1e-06, 1.37958, 1,
        0.984375, 0.5
    },
    {
        "sync_sweep",
        1, 2, 0.5,
        1, 2, 0.5,
        1, 0.5,
        0.1, 1, 0, 0,
        0.002475, 0.1, 1, 8.3e-05, 0, 0, 1e-06,
        2.6e-05, 0.1, 1, 2.4e-05, 0, 1, 1e-06,
        50, 0.1, 1,
        0.984375, 0.531011
    },
    {
        "Gramma's Organ",
        1, 1, 0.5,
        2, 1, 0.5,
        0, 0.43,
        7.58577, 0, 0.003, 0,
        0.047863, 0.1, 1, 0.013183, 0, 0, 1e-06,
        0.063096, 0.1, 1, 0.015849, 0, 0, 1e-06,
        15, 0.09975, 1,
        1, 0.640712
    },
    {
        "Long Portamento",
        1, 1, 0.5,
        1.62366, 2, 0.5,
        1, 0.5,
        0.1, 0, 0.714795, 0,
        0.017378, 0.1, 1, 0.000275, 0, 0, 1e-06,
        0.052481, 0.000331, 0, 0.000331, 0, 0, 0,
        10, 1.3566, 1,
        0.002509, 0.5
    },
    {
        "Owww!",
        1, 1, 0.5,
        1.41929, 2, 0.791445,
        1, 0.488842,
        0.247489, 0, 0, 0,
        0.1, 0.000146, 0.885319, 0.000174, 0, 0.295167, 1e-06,
        0.1, 7.1e-05, 0, 0.000102, 0, 0, 50,
        1e-06, 1.50141, 1,
        0.984375, 0.36
    },
    {
        "Tube Growl",
        1, 2, 0.5,
        1.81938, 5, 0.5,
        1, 0.5,
        0.1, 1, 0, 0,
        0.012188, 0.1, 1, 5.8e-05, 0, 0, 17.4812,
        2.1e-05, 2.2e-05, 0.631621, 2.4e-05, 0, 1, 1e-06,
        1e-06, 1.59258, 1,
        0.984375, 0.3325
    },
    {
        "squelchy_bass",
        1, 2, 0.516291,
        1, 2, 0.492144,
        1, 0.5,
        0.1, 0, 0.114681, 0.800101,
        0.1, 4.6e-05, 0, 1e-04, 0, 0.154198, 50,
        0.1, 5.4e-05, 0, 4.6e-05, 0, 0, 50,
        1e-06, 1.6625, 1,
        0.984375, 0.45
    },
    {
        "reso_sweep",
        1, 2, 0.5,
        2.00967, 2, 0.482832,
        0, 0.505304,
        0.1, 1, 0, 0,
        0.002475, 0.1, 1, 8.3e-05, 0, 0, 1e-06,
        0.005541, 2.3e-05, 0, 2.4e-05, 0, 0, 50,
        11.9333, 1.83391, 1,
        0.984375, 0.2108
    },
    {
        "theremin",
        1, 0, 0.5,
        4, 0, 0.5,
        0, 1,
        10, 0, 0.050261, 0,
        4.6e-05, 0.1, 1, 1e-05, 0, 0, 1e-06,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0, 0,
        0.009709, 0.5146
    },
    {
        "Twub",
        1, 3, 0.5,
        1.53656, 5, 0.255417,
        1, 0.488842,
        0.247489, 0, 0.193056, 0,
        0.1, 5.1e-05, 0, 0.000142, 0, 0, 1e-06,
        0.006125, 0.040406, 0.610166, 0.00018, 0, 0, 7.01389,
        0.623417, 1.50141, 1,
        0.984375, 0.289927
    },
    {
        "Wavey",
        1, 2, 0.5,
        1.81938, 5, 0.5,
        1, 0.5,
        0.256842, 1, 0, 0.76999,
        0.012188, 0.1, 1, 4e-05, 0, 0, 17.4812,
        2.1e-05, 7.3e-05, 0.631621, 2.4e-05, 0, 1, 1e-06,
        10.6535, 1.51061, 1,
        0.087247, 0.339089
    },
    {
        "heart_beat",
        1, 0, 0.5,
        2.01737, 0, 0.5,
        0, 1,
        0.932825, 0, 0.923867, 0,
        0.1, 0.1, 1, 2.3e-05, 0, 0, 1e-06,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        0.539044, 1.70088, 1,
        0.984375, 0.18
    },
    {
        "voice",
        1, 2, 0.5,
        1.63237, 5, 0.931722,
        0, 1,
        0.1, 0, 0, 0,
        0.000317, 0.000342, 0.794001, 0.000317, 0, 0, 1e-06,
        0.000236, 0.1, 1, 1e-05, 0, 0.225724, 1e-06,
        15.3472, 1.35487, 1,
        0.984375, 0.291445
    },
    {
        "VS Bass 1",
        0.997231, 2, 0.5,
        1.00264, 2, 0.261538,
        0, 0.5,
        0.1, 0, 0, 0,
        0.045595, 0.000464, 0.651163, 0.023997, 0.166255, 0, 0,
        0.048661, 0.000287, 0.226667, 0.001226, 0.758072, 0, 41.2564,
        5.38461, 0.736615, 1,
        1, 0.586264
    },
    {
        "VS Bass 2",
        0.5, 1, 0.24,
        1.5874, 5, 0.856816,
        1, 0.303222,
        0.215443, 0, 0, 0,
        0.0330328, 1e-05, 0, 0.000475353, 0.276833, 0, 0,
        0.0347758, 0.000565723, 0, 0.000185425, 0.614681, 0, 37.6294,
        8.33333, 0.561288, 2,
        1, 0.80025
    },
    {
        "Klaxon",
        1, 2, 0.5,
        4, 2, 0.21,
        1, 1,
        1.97177, 2, 0.89239, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5082
    },
    {
        "======= Contributed by Silvan:",
        1, 0, 0.5,
        1, 0, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        50, 0, 0,
        0.984375, 0.5
    },
    {
        "Hammond pedals",
        1, 5, 0.49,
        1, 0, 0,
        0, 0,
        6.139, 0, 0, 0,
        0.030007, 1e-05, 0.01, 0.039006, 0, 0, 0,
        0.1, 1e-05, 0.01, 0.093001, 0, 0, 0,
        8.5, 0, 1,
        1, 0.51
    },
    {
        "space race",
        2, 5, 0.64,
        4, 2, 0.55,
        1, 0.76,
        2.575, 5, 0.3, 0.17,
        0.031653, 0.016008, 0.66, 0.027139, 0, 0.01, 14,
        0.017008, 4.4e-05, 0.28, 0.024008, 0, 1, 16.5,
        15.5, 1.197, 1,
        0.28144, 0.53
    },
    {
        "Night Sweats",
        1, 0, 0.25,
        1, 2, 0,
        0, 0.17,
        10, 3, 0.26, 0.08,
        3.3e-05, 0.039811, 0, 0.017378, 0, 0, 32.5,
        1e-05, 0.00631, 0.15, 0.000174, 0, 0.29, 49.5,
        3.5, 1.995, 1,
        1, 0.0327776
    },
    {
        "Night Sweats 2",
        1, 0, 0.25,
        1, 2, 0,
        0, 0.17,
        10, 3, 0.26, 0.08,
        3.3e-05, 0.039811, 0, 0.017378, 0, 0, 32.5,
        1e-05, 0.00631, 0.15, 0.000174, 0, 0.29, 49.5,
        3.5, 1.90622, 1,
        1, 0.302555
    },
    {
        "Chalk Board",
        1, 0, 0.25,
        1, 2, 0,
        0, 0.17,
        10, 3, 0.26, 0.08,
        0.1, 0.039811, 0.84, 0.017378, 0, 0, 32.5,
        0.1, 0.001096, 0.87, 0.000174, 0, 0.29, 38.5,
        3.5, 1.995, 1,
        1, 0.0268887
    },
    {
        "Tweetie",
        1, 0, 0.25,
        1, 2, 0,
        0, 0.17,
        10, 3, 0.26, 0.08,
        0.1, 0.000912, 0, 0.017378, 0, 0, 32.5,
        0.1, 0.036308, 0.22, 0.000174, 0, 0.29, 38.5,
        3.5, 1.995, 1,
        1, 0.080246
    },
    {
        "Centipede",
        1, 0, 0.25,
        1, 2, 0,
        0, 0.17,
        10, 3, 0.26, 0.08,
        0.1, 0.019055, 0.17, 0.000251, 0, 0.42, 32.5,
        0.1, 0.1, 0, 0.000174, 0, 0.21, 38.5,
        3.5, 1.995, 1,
        1, 0.0663
    },
    {
        "Vitreous Humor",
        1, 0, 0,
        4, 0, 0.45,
        1, 0.3,
        0.158489, 1, 0.2, 0.75,
        7.6e-05, 0.1, 1, 3.6e-05, 0, 0.58, 16.5,
        0.003311, 6.9e-05, 0.81, 4.4e-05, 0, 0.44, 19,
        27, 0, 1,
        1, 0.535501
    },
    {
        "Vibrothingie",
        1, 2, 0.35,
        1, 0, 0.21,
        1, 0.8,
        0.630957, 0, 0.74, 0.36,
        0.1, 1e-05, 0.65, 0.00912011, 1, 0.02, 23,
        0.0057544, 1e-05, 0, 0.000120226, 0.66, 0.44, 24,
        33.5, 1.596, 1,
        0.002, 0.414116
    },
    {
        "Vibrato Hammond",
        4, 1, 0.5,
        2, 5, 0.5,
        1, 0.3,
        7.58578, 0, 0.02, 0.6,
        0.047863, 0.1, 1, 0.013183, 0.6, 0.21, 18.5,
        0.063096, 0.1, 1, 0.015849, 1, 1, 1e-06,
        10, 0.01995, 1,
        1, 0.615634
    },
    {
        "Music Box",
        4, 0, 0.12,
        1, 5, 0,
        0, 0,
        0.288403, 0, 0.36, 0.02,
        0.1, 0.000190546, 0.18, 4.36516e-05, 1, 0.69, 39.5,
        0.1, 0.1, 0, 0.1, 0.25, 0.08, 0,
        50, 1.9551, 1,
        1, 0.298288
    },
    {
        "Ploink Box",
        4, 1, 0.12,
        2, 0, 0.15,
        1, 0.39,
        0.275423, 0, 0, 0,
        0.1, 0.000436516, 0.18, 4.36516e-05, 0.72, 1, 39.5,
        0.0047863, 1e-04, 0.67, 0.00057544, 0.4, 0.5, 12.5,
        30, 1.7955, 0,
        1, 0.504388
    },
    {
        "Polkenstein",
        1, 0, 0.46,
        2, 0, 0.17,
        1, 0.16,
        6.60694, 3, 0, 0.09,
        0.000912011, 0.0047863, 0.68, 3.98107e-05, 0.58, 0.56, 0,
        1.09648e-05, 0.000251189, 1, 1e-05, 0.82, 0.49, 40.5,
        4.5, 1.05735, 0,
        1, 0.580002
    },
    {
        "================ Test Patches:",
        1, 0, 0.5,
        1, 0, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        50, 0, 0,
        0.984375, 0.5
    },
    {
        "sweep alias test (key=100)",
        1, 2, 0.5,
        2.09957, 1, 0.21,
        0, 1,
        0.424805, 0, 0.863445, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "sweep alias test 2 (key=108)",
        1, 2, 0.5,
        1, 1, 0.21,
        0, 1,
        0.1, 0, 0.57, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "sync alias test (key=108)",
        1, 2, 0.5,
        2.09957, 1, 0.21,
        1, 1,
        0.424805, 0, 0.863445, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "mod discontinuity test",
        1, 2, 0.5,
        2.09957, 1, 0.21,
        0, 1,
        2.73719, 2, 0.8634, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "subsample alias test (key=94)",
        1, 2, 0.5,
        2.0144, 1, 0.21,
        1, 1,
        0.136948, 0, 0.431722, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "subsample alias 2 (key=60)",
        1, 2, 0.5,
        2.00139, 6, 0.5,
        1, 1,
        0.247489, 0, 0.166667, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5185
    },
    {
        "subsample alias 3 (key=96)",
        1.02285, 2, 0.5,
        1.00628, 6, 0.0682776,
        1, 1,
        0.164142, 0, 0.791445, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.103712, 0,
        0.984375, 0.5
    },
    {
        "startup thump test (key=108)",
        1, 2, 0.5,
        1.47427, 5, 0.06,
        1, 1,
        0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "tri sync thump test (key=77)",
        1, 2, 0.5,
        2.20381, 6, 0.08,
        1, 1,
        0.416869, 0, 0.57, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "pulse click! (key=106)",
        1, 5, 0.5,
        2.09957, 5, 0.21,
        0, 1,
        0.424805, 0, 0.863445, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "pulse click! (key=103)",
        1, 5, 0.5,
        2.09957, 5, 0.21,
        0, 1,
        0.424805, 0, 0.863445, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 1e-06,
        50, 0.0399, 0,
        0.984375, 0.5
    },
    {
        "============= New after 0.9.0:",
        1, 0, 0.5,
        1, 0, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        50, 0, 0,
        0.984375, 0.5
    },
    {
        "Quick Strings",
        0.997923, 3, 0.5,
        1.00208, 0, 0.114681,
        0, 0.5,
        0.533199, 0, 0, 0.0350456,
        0.000533199, 0.1, 1, 1e-04, 0.291445, 0, 0,
        0.00287556, 0.1, 1, 0.000267462, 0.401611, 0, 7.15922,
        8.33333, 0, 2,
        0.984375, 0.5
    },
    {
        "- new voice -",
        1, 0, 0.5,
        1, 0, 0.5,
        0, 0.5,
        0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        0.1, 0.1, 1, 0.1, 0, 0, 0,
        50, 0, 0,
        0.984375, 0.5
    }
};

