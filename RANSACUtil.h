
void* fit_model(void** mat, int m);
float get_error(void* x, void* y);
void* ransac_fit(void* (*fit_model)(void**, int), int minnpts, void (*delete_model)(void*), float (*get_error)(void*, void*), float errthresh, void* pts[], int npts, int niters, float* quality);
void delete_model(void*);