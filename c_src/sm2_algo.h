
#ifndef _SM2_ALGO_H_
#define _SM2_ALGO_H_

#ifdef __cplusplus
extern "C" {
#endif

int sm2_sign(UINT8* hash, UINT8* random, UINT8* sk, UINT8* sign_r, UINT8* sign_s, ECGROUP* group);
int sm2_verify (UINT8* hash, UINT8* pk, UINT8* sign_r, UINT8* sign_s, ECGROUP* group);

#ifdef __cplusplus
}
#endif

#endif
