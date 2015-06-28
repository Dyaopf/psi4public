/*
 *@BEGIN LICENSE
 *
 * PSI4: an ab initio quantum chemistry software package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *@END LICENSE
 */

#include <libqt/qt.h>
#include "defines.h"
#include "dfocc.h"

using namespace psi;
using namespace std;


namespace psi{ namespace dfoccwave{
  
void DFOCC::mp3_WmnijT2()
{
    // defs
    SharedTensor2d K, T, Tnew, U, Tau, W, X;
    SharedTensor2d M, L, I, Y, S, A;
    SharedTensor2d V, Vs, Va, Ts, Ta;

    timer_on("WmnijT2");

    // W_mnij = <mn|ij>
    W = SharedTensor2d(new Tensor2d("W <MN|IJ>", naoccA, naoccA, naoccA, naoccA));
    K = SharedTensor2d(new Tensor2d("DF_BASIS_CC MO Ints (IJ|KL)", naoccA, naoccA, naoccA, naoccA));
    K->gemm(true, false, bQijA, bQijA, 1.0, 0.0);
    W->sort(1324, K, 1.0, 0.0);
    K.reset();

    // t_ij^ab <= \sum_{m,n} T_mn^ab Wmnij
    // (+)T(ij, ab) = 1/2 (T_ij^ab + T_ji^ab) * (2 - \delta_{ij})
    // (-)T(ij, ab) = 1/2 (T_ij^ab - T_ji^ab) * (2 - \delta_{ij}) 
    U = SharedTensor2d(new Tensor2d("T2 <IJ|AB>", naoccA, naoccA, navirA, navirA));
    U->sort(1324, t2, 1.0, 0.0);
    Ts = SharedTensor2d(new Tensor2d("(+)tT [I>=J|A>=B]", ntri_ijAA, ntri_abAA));
    Ta = SharedTensor2d(new Tensor2d("(-)tT [I>=J|A>=B]", ntri_ijAA, ntri_abAA));
    Ts->symm_row_packed4(U);
    Ta->antisymm_row_packed4(U);
    U.reset();

    // Form (+/-)W(m>=n, i>=j)
    Vs = SharedTensor2d(new Tensor2d("(+)W [M>=N|I>=J]", ntri_ijAA, ntri_ijAA));
    Va = SharedTensor2d(new Tensor2d("(-)W [M>=N|I>=J]", ntri_ijAA, ntri_ijAA));
    Vs->symm4(W);
    Va->antisymm4(W);
    W.reset();

    // Symmetric & Anti-symmetric contributions
    S = SharedTensor2d(new Tensor2d("S (I>=J, A>=B)", ntri_ijAA, ntri_abAA));
    A = SharedTensor2d(new Tensor2d("A (I>=J, A>=B)", ntri_ijAA, ntri_abAA));
    S->gemm(true, false, Vs, Ts, 1.0, 0.0); 
    A->gemm(true, false, Va, Ta, 1.0, 0.0); 
    Ts.reset();
    Ta.reset();
    Vs.reset();
    Va.reset();

    // T(ia,jb) <-- S(a>=b,i>=j) + A(a>=b,i>=j)
    Tnew = SharedTensor2d(new Tensor2d("New T2_2 (IA|JB)", naoccA, navirA, naoccA, navirA));
    //Tnew->read_symm(psio_, PSIF_DFOCC_AMPS);
    #pragma omp parallel for
    for(int a = 0 ; a < navirA; ++a){
        for(int b = 0 ; b < navirA; ++b){
            int ab = index2(a,b); 
            for(int i = 0 ; i < naoccA; ++i){
                int ia = ia_idxAA->get(i,a);
                for(int j = 0 ; j < naoccA; ++j){
                    int jb = ia_idxAA->get(j,b);
                    int ij = index2(i,j); 
                    int perm1 = ( i > j ) ? 1 : -1;
                    int perm2 = ( a > b ) ? 1 : -1;
                    double value = S->get(ij,ab) + (perm1 * perm2 * A->get(ij,ab));
                    Tnew->add(ia, jb, value);
                }
            }
        }
    }
    S.reset();
    A.reset();
    Tnew->write_symm(psio_, PSIF_DFOCC_AMPS);
    Tnew.reset();
    
    timer_off("WmnijT2");

}// end mp3_WmnijT2

//======================================================================
//    WmbejT2
//======================================================================             
void DFOCC::mp3_WmbejT2()
{
    // defs
    SharedTensor2d K, L, T, T1, Tnew, U, Tau, W, W2, X, Y;

    timer_on("WmbejT2");

    // W_mbej = W(me,jb)
    // W(me,jb) <= (me|jb)
    W = SharedTensor2d(new Tensor2d("W (ME|JB)", naoccA, navirA, naoccA, navirA));
    W->gemm(true, false, bQiaA, bQiaA, 1.0, 0.0);
    W->write(psio_, PSIF_DFOCC_AMPS);
    W.reset();

    // W_mbje = W'(me,jb)
    // W'(me,jb) <= <me|jb>
    W = SharedTensor2d(new Tensor2d("Wp (ME|JB)", naoccA, navirA, naoccA, navirA));
    L = SharedTensor2d(new Tensor2d("DF_BASIS_CC MO Ints (IJ|AB)", naoccA, naoccA, navirA, navirA));
    L->gemm(true, false, bQijA, bQabA, 1.0, 0.0);
    W->sort(1324, L, 1.0, 0.0);
    L.reset();

    // t_ij^ab <= 1/2*C(ia,jb) + 1/2*C(jb,ia) + C(ja,ib) + C(ib,ja)
    // t_ij^ab <= Ct(ia,jb) + 2*Ct(ib,ja)
    // C(ia,jb) = -\sum_{me} t_mi^ae W'(me,jb) = -\sum_{me} T'(ia,me) W'(me,jb)
    U = SharedTensor2d(new Tensor2d("T2p (IA|JB)", naoccA, navirA, naoccA, navirA));
    ccsd_t2_prime_amps(U,t2);
    Y = SharedTensor2d(new Tensor2d("C2 (IA|JB)", naoccA, navirA, naoccA, navirA));
    Y->gemm(false, false, U, W, -1.0, 0.0);
    U.reset();
    X = SharedTensor2d(new Tensor2d("C2+D2 (IA|JB)", naoccA, navirA, naoccA, navirA));
    X->sort(1432, Y, 1.0, 0.0);
    X->axpy(Y, 0.5);
    Y.reset();

    // t_ij^ab <= D(ia,jb) + D(jb,ia)
    // D_ij^ab = 1/2 \sum_{me} u_im^ae [2*W(me,jb) - W'(me,jb)]
    Y = SharedTensor2d(new Tensor2d("2*W-W' (ME|JB)", naoccA, navirA, naoccA, navirA));
    Y->axpy(W, -1.0);
    W.reset();
    W2 = SharedTensor2d(new Tensor2d("W (ME|JB)", naoccA, navirA, naoccA, navirA));
    W2->read(psio_, PSIF_DFOCC_AMPS);
    Y->axpy(W2, 2.0);
    W2.reset();
    U = SharedTensor2d(new Tensor2d("U2 (IA|JB)", naoccA, navirA, naoccA, navirA));
    ccsd_u2_amps(U,t2);
    X->gemm(false, false, U, Y, 0.5, 1.0);
    U.reset();
    Y.reset();
    X->symmetrize();
    Tnew = SharedTensor2d(new Tensor2d("New T2_2 (IA|JB)", naoccA, navirA, naoccA, navirA));
    Tnew->read_symm(psio_, PSIF_DFOCC_AMPS);
    Tnew->axpy(X, 2.0);
    X.reset();
    Tnew->write_symm(psio_, PSIF_DFOCC_AMPS);
    Tnew.reset();
    
    timer_off("WmbejT2");

}// end mp3_WmbejT2

//======================================================================
//    WabefT2  
//======================================================================             
void DFOCC::mp3_WabefT2()
{
    // defs
    SharedTensor2d K, M, L, I, T, Tnew, U, Tau, W, X, Y, S, A;
    SharedTensor2d V, Vs, Ts, Va, Ta;

    timer_on("WabefT2");

    // t_ij^ab <= \sum_{ef} T_ij^ef <ab|ef>
    // (+)T(ij, ab) = 1/2 (T_ij^ab + T_ji^ab) * (2 - \delta_{ab})
    // (-)T(ij, ab) = 1/2 (T_ij^ab - T_ji^ab) * (2 - \delta_{ab}) 
    U = SharedTensor2d(new Tensor2d("(+)T [I>=J|A>=B]", ntri_ijAA, ntri_abAA));
    T = SharedTensor2d(new Tensor2d("(-)T [I>=J|A>=B]", ntri_ijAA, ntri_abAA));
    #pragma omp parallel for
    for(int i = 0 ; i < naoccA; ++i){
        for(int j = 0 ; j <= i; ++j){
            int ij = index2(i,j); 
            for(int a = 0 ; a < navirA; ++a){
                int ia = ia_idxAA->get(i,a);
                int ja = ia_idxAA->get(j,a);
                for(int b = 0 ; b <= a; ++b){
                    double perm = (a == b ? 1.0 : 2.0);
                    int ab = index2(a,b); 
                    int jb = ia_idxAA->get(j,b);
                    int ib = ia_idxAA->get(i,b);
                    double value1 = 0.5 * perm * ( t2->get(ia,jb) + t2->get(ja,ib) ); 
                    double value2 = 0.5 * perm * ( t2->get(ia,jb) - t2->get(ja,ib) ); 
                    U->set(ij,ab,value1);
                    T->set(ij,ab,value2);
                }
            }
        }
    }

    // Read B(Q,ab)
    K = SharedTensor2d(new Tensor2d("DF_BASIS_CC B (AB|Q)", navirA * navirA, nQ));
    K = bQabA->transpose();

    // malloc
    I = SharedTensor2d(new Tensor2d("I[A] <BF|E>", navirA * navirA, navirA));
    Vs = SharedTensor2d(new Tensor2d("(+)V[A] (B, E>=F)", navirA, ntri_abAA));
    Va = SharedTensor2d(new Tensor2d("(-)V[A] (B, E>=F)", navirA, ntri_abAA));
    Ts = SharedTensor2d(new Tensor2d("(+)T[A] (B, I>=J)", navirA, ntri_ijAA));
    Ta = SharedTensor2d(new Tensor2d("(-)T[B] (B, I>=J)", navirA, ntri_ijAA));

    // Symmetric & Anti-symmetric contributions
    S = SharedTensor2d(new Tensor2d("S (A>=B, I>=J)", ntri_abAA, ntri_ijAA));
    A = SharedTensor2d(new Tensor2d("A (A>=B, I>=J)", ntri_abAA, ntri_ijAA));
    // Main loop
    for(int a = 0 ; a < navirA; ++a){
            int nb = a+1;

            // Form V[a](bf,e) = \sum_{Q} B(bfQ)*B(aeQ) cost = V^4N/2
            I->contract(false, true, navirA*nb, navirA, nQ, K, K, 0, a*navirA*nQ, 1.0, 0.0);

            // Form (+)V[a](b, e>=f) 
            #pragma omp parallel for
            for(int b = 0 ; b <= a; ++b){
                for(int e = 0 ; e < navirA; ++e){
                    int be = e + (b * navirA);
                    for(int f = 0 ; f <= e; ++f){
                        int ef = index2(e,f); 
                        int bf = f + (b * navirA);
                        double value1 = 0.5 * ( I->get(bf, e) + I->get(be, f) );
                        double value2 = 0.5 * ( I->get(bf, e) - I->get(be, f) );
                        Vs->set(b, ef, value1);
                        Va->set(b, ef, value2);
                    }
                }
            }

            // Form T[a](b, i>=j) = \sum_{e>=f} Tau(i>=j,e>=f) V[a](b, e>=f) 
            Ts->contract(false, true, nb, ntri_ijAA, ntri_abAA, Vs, U, 1.0, 0.0);
            Ta->contract(false, true, nb, ntri_ijAA, ntri_abAA, Va, T, 1.0, 0.0);

            // Form S(ij,ab) & A(ij,ab)
            #pragma omp parallel for
            for(int b = 0 ; b <=a; ++b){
                int ab = index2(a,b); 
                for(int i = 0 ; i < naoccA; ++i){
                    for(int j = 0 ; j <= i; ++j){
                        int ij = index2(i,j); 
                        S->add(ab, ij, Ts->get(b,ij));
                        A->add(ab, ij, Ta->get(b,ij));
                    }
                }
            } 

    }
    K.reset();
    I.reset();
    Vs.reset();
    Va.reset();
    Ts.reset();
    Ta.reset();
    U.reset();
    T.reset();

    // T(ia,jb) <-- S(a>=b,i>=j) + A(a>=b,i>=j)
    Tnew = SharedTensor2d(new Tensor2d("New T2_2 (IA|JB)", naoccA, navirA, naoccA, navirA));
    Tnew->read_symm(psio_, PSIF_DFOCC_AMPS);
    #pragma omp parallel for
    for(int a = 0 ; a < navirA; ++a){
        for(int b = 0 ; b < navirA; ++b){
            int ab = index2(a,b); 
            for(int i = 0 ; i < naoccA; ++i){
                int ia = ia_idxAA->get(i,a);
                for(int j = 0 ; j < naoccA; ++j){
                    int jb = ia_idxAA->get(j,b);
                    int ij = index2(i,j); 
                    int perm1 = ( i > j ) ? 1 : -1;
                    int perm2 = ( a > b ) ? 1 : -1;
                    double value = S->get(ab,ij) + (perm1 * perm2 * A->get(ab,ij));
                    Tnew->add(ia, jb, value);
                }
            }
        }
    }
    S.reset();
    A.reset();
    Tnew->write_symm(psio_, PSIF_DFOCC_AMPS);
    Tnew.reset();

    timer_off("WabefT2");

}// end mp3_WabefT2

}} // End Namespaces



