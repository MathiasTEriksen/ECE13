#include <stdint.h>
#include "Negotiation.h"
#include "BOARD.h"

NegotiationData NegotiationHash(NegotiationData secret){
    return (secret*secret) % PUBLIC_KEY;
}

int NegotiationVerify(NegotiationData secret, NegotiationData commitment){
    if(NegotiationHash(secret) == commitment){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B){
    NegotiationData C = (A ^ B);
    //C ^= C >> 16;
    C ^= C >> 8;
    C ^= C >> 4;
    C ^= C >> 2;
    C ^= C >> 1;
    return (~C) & 1;
    if(C == 1){
        return HEADS;
    }
    else{
        return TAILS;
    }
}

NegotiationData NegotiateGenerateBGivenHash(NegotiationData hash_a){
    return 0;
}
NegotiationData NegotiateGenerateAGivenB(NegotiationData B){
    return 0;
}
