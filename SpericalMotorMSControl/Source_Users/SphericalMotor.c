/*
 * SphericalMotor.c
 */
#include "stdlib.h"
#include "math.h"
#include "SphericalMotor.h"

void CurCauculate(float* angle,float* Td,float* u)
{
    int i,j,k,m,n;
    float alpha = angle[0] * 0.017453;
    float beta = angle[1] * 0.017453;
    float gamma = angle[2] * 0.017453;
    float Sa = sin(alpha);
    float Ca = cos(alpha);
    float Sb = sin(beta);
    float Cb = cos(beta);
    float Sg = sin(gamma);
    float Cg = cos(gamma);
    // EM，PM方位矩阵的计算--------------------------------------------------------------------------------------------------------------------------
    float R[3][3] = {{Cg * Cb,-Sg * Cb,Sb},
                     {Sg * Ca + Cg * Sb * Sa,Cg * Ca - Sg * Sb * Sa,-Cb * Sa},
                     {Sg * Sa - Cg * Sb * Ca,Cg * Sa + Sg * Sb * Ca,Cb * Ca}};
    float a[3][3] = {{0,0,0},
                     {-Sg * Sa + Cg * Sb * Ca,-Cg * Sa - Sg * Sb * Ca,-Cb * Ca},
                     {Sg * Ca + Cg * Sb * Sa,Cg * Ca - Sg * Sb * Sa,-Cb * Sa}};
    float b[3][3] = {{-Cg * Sb,Sg * Sb,Cb},
                     {Cg * Cb * Sa,-Sg * Cb * Sa,Sb * Sa},
                     {-Cg * Cb * Ca,Sg * Cb * Ca,-Sb * Ca}};
    float c[3][3] = {{-Sg * Cb,-Cg * Cb,0},
                     {Cg * Ca - Sg * Sb * Sa,-Sg * Ca - Cg * Sb * Sa,0},
                     {Cg * Sa + Sg * Sb * Ca,-Sg * Sa + Cg * Sb * Ca,0}};
    float epi[3][24] = {{0.9330,0.6830,0.2500,-0.2500,-0.6830,-0.9330,-0.9330,-0.6830,-0.2500,0.2500,0.6830,0.9330,0.9330,0.6830,0.2500,-0.2500,-0.6830,-0.9330,-0.9330,-0.6830,-0.2500,0.2500,0.6830,0.9330},
                        {0.2500,0.6830,0.9330,0.9330,0.6830,0.2500,-0.2500,-0.6830,-0.9330,-0.9330,-0.6830,-0.2500,0.2500,0.6830,0.9330,0.9330,0.6830,0.2500,-0.2500,-0.6830,-0.9330,-0.9330,-0.6830,-0.2500},
                        {-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,-0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588,0.2588}};
    float esj[3][12] = {{0.9063,0.6408,0,-0.6408,-0.9063,-0.6408,0,0.6408,0.9238,0.3826,-0.3826,-0.9238},
                        {0,0.6408,0.9063,0.6408,0,-0.6408,-0.9063,-0.6408,0.3826,0.9238,0.9238,0.3826},
                        {-0.4226,-0.4226,-0.4226,-0.4226,-0.4226,-0.4226,-0.4226,-0.4226,0,0,0,0}};
    float eri[3][24];
    for(i = 0;i < 3;i++){
        for(j = 0;j < 24;j++){
            eri[i][j] = 0;
            for(k = 0;k < 3;k++){
                eri[i][j] += R[i][k] * epi[k][j];
            }
        }
    }
    // K的计算--------------------------------------------------------------------------------------------------------------------------
    float K[3][12];
    float lambda[24] = {-1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465,1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465,1.465,-1.465};
    float dotij;
    float sigmaij;
    float Thetaij;
    for (j = 0; j < 12; j++) {
        K[0][j] = 0;
        K[1][j] = 0;
        K[2][j] = 0;
        for (i = 0; i < 24; i++) {
            dotij = 0;
            for(k = 0;k < 3;k++){
                dotij += eri[k][i] * esj[k][j];
            }
            sigmaij = 57.296 * acos(dotij);
            if(sigmaij < 60.0){
                Thetaij = lambda[i] * dfLambdaFit(sigmaij) / sqrt(1 - dotij * dotij);
            }
            else{
                continue;
            }
            float tmpma = 0;
            float tmpmb = 0;
            float tmpmc = 0;
            for (m = 0; m < 3; m++) {
                float tmpna = 0;
                float tmpnb = 0;
                float tmpnc = 0;
                for (n = 0; n < 3; n++) {
                    tmpna += a[m][n] * epi[n][i];
                    tmpnb += b[m][n] * epi[n][i];
                    tmpnc += c[m][n] * epi[n][i];
                }
                tmpma += tmpna * esj[m][j];
                tmpmb += tmpnb * esj[m][j];
                tmpmc += tmpnc * esj[m][j];
            }
            K[0][j] += Thetaij * tmpma;
            K[1][j] += Thetaij * tmpmb;
            K[2][j] += Thetaij * tmpmc;
        }
    }
    // 电流的计算--------------------------------------------------------------------------------------------------------------------------
    float KK[3][3],KKinv[3][3],KKd,x[3];
    for(i = 0;i < 3;i++){
        for(j = 0;j < 3;j++){
            KK[i][j] = 0;
            for(k = 0;k < 12;k++){
                KK[i][j] += K[i][k] * K[j][k];
            }
        }
    }
    KKd = KK[0][0] * KK[1][1] * KK[2][2] +
          KK[0][1] * KK[1][2] * KK[2][0] +
          KK[0][2] * KK[1][0] * KK[2][1] -
          KK[2][0] * KK[1][1] * KK[0][2] -
          KK[2][1] * KK[1][2] * KK[0][0] -
          KK[2][2] * KK[1][0] * KK[0][1];
    KKd = 1.0 / KKd;
    KKinv[0][0] = (KK[1][1] * KK[2][2] - KK[1][2] * KK[2][1]) * KKd;
    KKinv[0][1] = -(KK[1][0] * KK[2][2] - KK[1][2] * KK[2][0]) * KKd;
    KKinv[0][2] = (KK[1][0] * KK[2][1] - KK[1][1] * KK[2][0]) * KKd;
    KKinv[1][0] = -(KK[0][1] * KK[2][2] - KK[0][2] * KK[2][1]) * KKd;
    KKinv[1][1] = (KK[0][0] * KK[2][2] - KK[0][2] * KK[2][0]) * KKd;
    KKinv[1][2] = -(KK[0][0] * KK[2][1] - KK[0][1] * KK[2][0]) * KKd;
    KKinv[2][0] = (KK[0][1] * KK[1][2] - KK[0][2] * KK[1][1]) * KKd;
    KKinv[2][1] = -(KK[0][0] * KK[1][2] - KK[0][2] * KK[1][0]) * KKd;
    KKinv[2][2] = (KK[0][0] * KK[1][1] - KK[0][1] * KK[1][0]) * KKd;
    x[0] = 0;
    x[1] = 0;
    x[2] = 0;
    for(i = 0;i < 3;i++){
        x[i] = 0;
        for(j = 0;j < 3;j++){
            x[i] += KKinv[i][j] * Td[j];
        }
    }
    for(i = 0;i < 12;i++){
        u[i] = 0;
        for(j = 0;j < 3;j++){
            u[i] += K[j][i] * x[j];
        }
        u[i] /= 2;
    }
}

float dfLambdaFit(float sigma)
{
    float dfLambda = 0;
    int i;
    float A[7] = {0.0021,0.0117,0.0128,0.0065,0.0019,0.0011,0.0003};
    float phi[7] = {-1.5708,-2.9704,2.9036,2.7659,-2.9911,-2.2712,-2.3537};
    float w = 0.07273;
    for(i = 0;i < 7;i++)
    {
        dfLambda += A[i] * sin(i * w * sigma + phi[i]);
    }
    return dfLambda;
}

void CurBias(float* u){
    u[0] *= 1.3525;//1.2799;
    u[1] *= 1.3384;//1.2666;
    u[2] *= 1.4382;//1.3610;
    u[3] *= 1.4733;//1.3942;
    u[4] *= 1.3292;//1.2579;
    u[5] *= 1.4201;//1.3439;
    u[6] *= 1.4632;//1.3847;
    u[7] *= 1.4392;//1.3620;
    u[8] *= 1.4699;//1.3910;
    u[9] *= 1.4424;//1.3650;
    u[10] *= 1.4349;//1.3579;
    u[11] *= 1.4222;//1.3459;
}
