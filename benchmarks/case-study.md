## Mibench

# typeset

* ParentFlush

src/z20.c
236 -> 168,  28.8% reduction
whole function contains an if/else branch with lots of similar computations.
large isomorphic regions for CFM

macro function calls, so identical computations (high similarity)

* FilterFlush

src/z40.c
116 -> 100  13.79%
if else branch with identical computations

* TransferEnd

src/z18.c
large if else branch with similar computations
use of macro functions which translate to identical computations
693 -> 630 9%

* Meld

src/z07.c
multiple if/else branches
914 -> 885 3.17%


# ghostscript

* cmd_put_color_map

src/gxclblits.c
52 -> 48 7.69%
nested if within if/else

* gs_type2_charstring_interpret

1401 -> 1310 6.49%
this is long complex functions

lots of if elses with similar code inside switch cases!
src/gstype2.c

* gx_default_fill_triangle

134 -> 126 5.97%
if else merging
src/gdevddrw.c

* gx_default_strip_tile_rectangle

src/gdevdflt.c
if else within if else
426 -> 384 9.86% 

* gs_currenthsbcolor: 

97 to  91 (6.185567e+00%)
if else if else chain (diamond shape)
./src/gshsb.c

# blowfish_d

* BF_encrypt

658 to  359 (4.544073e+01%)
large if else branch with identical computations
why BF does not catch this??
src/bf_enc.c

* BF_cfb64_encrypt: 

123 to  92 (2.520325e+01%)
./src/bf_cfb64.c
if else branch containing while loop / if conditions (complex region melding)

# 400.perlbench

* Perl_utf8_distance
103 to  72 (3.009709e+01%)
./src/utf8.c
if else with identical regions inside

# 447.dealII

* _ZSt22__move_median_to_firstIN9__gnu_cxx17__normal_iteratorIPSt4pairIjdESt6vectorIS3_SaIS3_EEEENS0_5__ops15_Iter_less_iterEEvT_SB_SB_SB_T0_: 
* 99 to  49 (5.050505e+01%)



