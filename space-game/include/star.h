#ifndef STAR_H
#define STAR_H

enum star_class {
    HZPR_NOMASEQ_G,  // GIANT
    HZPR_NOMASEQ_SG, // SUPER GIANT
    HZPR_NOMASEQ_BH, // BLACK HOLE
    HZPR_NOMASEQ_WD, // WHITE DWARF
    HZPR_NOMASEQ_N,  // NEUTRON
    HZPR_NOMASEQ_P,  // PULSARE
    HZPR_NOMASEQ_Q,  // QUASAR
    HZPR_MAINSEQ_O,
    HZPR_MAINSEQ_B,
    HZPR_MAINSEQ_A,
    HZPR_MAINSEQ_F,
    HZPR_MAINSEQ_G,
    HZPR_MAINSEQ_K,
    HZPR_MAINSEQ_M
};

struct star {
    enum star_class hzprclass;
    float mass;
    float luminosity;
    float diameter;
    float surfacetemp;
    float lifetime;
    float habitzonemin;
    float habitzonemax;
};

#endif