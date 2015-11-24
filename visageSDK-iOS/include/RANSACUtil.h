///////////////////////////////////////////////////////////////////////////////
// 
// (c) Visage Technologies AB 2002 - 2015  All rights reserved. 
// 
// This file is part of visage|SDK(tm). 
// Unauthorized copying of this file, via any medium is strictly prohibited. 
// 
// No warranty, explicit or implicit, provided. 
// 
// This is proprietary software. No part of this software may be used or 
// reproduced in any form or by any means otherwise than in accordance with
// any written license granted by Visage Technologies AB. 
// 
/////////////////////////////////////////////////////////////////////////////


void* fit_model(void** mat, int m);
float get_error(void* x, void* y);
void* ransac_fit(void* (*fit_model)(void**, int), int minnpts, void (*delete_model)(void*), float (*get_error)(void*, void*), float errthresh, void* pts[], int npts, int niters, float* quality);
void delete_model(void*);